#include "emulator_liason/public/emulator_liason.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sys/time.h>

#include "abstraction_layer/gpio_interface.h"
#include "abstraction_layer/serial_communication_interface.h"
#include "external_libraries/ArduinoJson-v6.21.3.h"

using namespace ::testing;

class FakeSerialComunication
    : public binarx_serial_interface::SerialCommunicationInterface {
 public:
  binarx_serial_interface::SerialStatus Receive(uint8_t* receive_buffer,
                                                uint16_t size,
                                                uint32_t timeout) override {
    for (uint16_t i = 0; i < size; i++) {
      receive_buffer[i] = uint8_t(static_cast<uint8_t>(i));
    };
    return binarx_serial_interface::SerialStatus::Success;
  };
  binarx_serial_interface::SerialStatus Transmit(uint8_t* transmit_buffer,
                                                 uint16_t size,
                                                 uint32_t timeout) override {
    return binarx_serial_interface::SerialStatus::Success;
  };
};

class SerialCommunicationMock
    : public binarx_serial_interface::SerialCommunicationInterface {
 public:
  MOCK_METHOD(binarx_serial_interface::SerialStatus, Transmit,
              (uint8_t * buffer, uint16_t size, uint32_t timeout), (override));
  MOCK_METHOD(binarx_serial_interface::SerialStatus, Receive,
              (uint8_t * receive_buffer, uint16_t size, uint32_t timeout),
              (override));

  void DelegateToSuccessfulFake() {
    ON_CALL(*this, Receive)
        .WillByDefault(
            [this](uint8_t* buffer, uint16_t size, uint32_t timeout) {
              return fake_.Receive(buffer, size, timeout);
            });
  }

 private:
  FakeSerialComunication fake_;
};

class GpioMock : public binarx_gpio_interface::GpioInterface {
 public:
  MOCK_METHOD(void, SetHigh, (binarx_gpio_interface::GpioSelector), (override));
  MOCK_METHOD(void, SetLow, (binarx_gpio_interface::GpioSelector), (override));
  MOCK_METHOD(void, TogglePin, (binarx_gpio_interface::GpioSelector),
              (override));
};

MATCHER_P2(ArraysAreEqual, array, size, "") {
  for (uint16_t i = 0; i < size; i++) {
    if (arg[i] != array[i]) {
      *result_listener << "the expected value is " << PrintToString(array[i])
                       << " and we got " << PrintToString(arg[i])
                       << " for value in possition " << PrintToString(i);
      return false;
    }
  }
  return true;
}

class EmulatorLiasonMockTesting
    : public binarx::emulator_liason::EmulatorLiason {
 public:
  EmulatorLiasonMockTesting(
      binarx_serial_interface::SerialCommunicationInterface&
          emulator_communication,
      binarx_gpio_interface::GpioInterface& gpio_object)
      : binarx::emulator_liason::EmulatorLiason(emulator_communication,
                                                gpio_object){};

  void SetPayloadStatus_TestOnly(PayloadDataStatus value) {
    payload_status_ = value;
  }

  using binarx::emulator_liason::EmulatorLiason::PayloadDataStatus;
};

class EmulatorLiasonTest : public testing::Test {
 protected:
  // void SetUp() override{};
  NiceMock<SerialCommunicationMock> emulator_com_mock;
  NiceMock<GpioMock> gpio_mock;

  EmulatorLiasonMockTesting emulator_liason =
      EmulatorLiasonMockTesting(emulator_com_mock, gpio_mock);

  constexpr static uint8_t kSyncByte = 5;
  constexpr static uint8_t kNumberOfBytesInHeader = 2;
  constexpr static uint16_t kPacketDataLength = 250;
  constexpr static uint16_t kMaxDataLength = 1000;
};

TEST_F(EmulatorLiasonTest, SuccessWithStdArray) {
  constexpr uint16_t kDataSize = 252;
  std::array<uint8_t, kDataSize> buffer = {0};
  uint8_t count = 0;
  for (auto& item : buffer) {
    item = count++;
  }

  buffer[0] = kSyncByte;
  buffer[1] = 1;

  std::array<uint8_t, kDataSize - 2> data_buffer;
  std::copy(buffer.begin() + 2, buffer.end(), data_buffer.begin());

  EXPECT_CALL(emulator_com_mock,
              Transmit(ArraysAreEqual(buffer.data(), kDataSize), kDataSize, _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  // When Payload Comunication is called
  emulator_liason.Transmit(data_buffer.data(), data_buffer.size());
}

TEST_F(EmulatorLiasonTest, SuccessWithJsonLibrary) {
  // create a JsonDocument
  StaticJsonDocument<1000> doc;
  // Add values in the document
  doc["sensor"] = "Test";
  doc["time"] = 0;
  doc["memory_doc"] = doc.memoryUsage();
  doc["memory_buffer"] = 0;
  int extra_numbers = 10;
  JsonArray data = doc.createNestedArray("data");
  for (int i = 0; i < extra_numbers; i++) {
    if (!data.add(doc.memoryUsage())) {
      data[0] = i;
      break;
    }
  }

  // copy the data to the buffer
  uint16_t kDataSize = static_cast<uint16_t>(measureJsonPretty(doc));
  // Create a buffer to use to send
  std::array<uint8_t, 1000> data_buffer;
  serializeJsonPretty(doc, data_buffer.data(), kDataSize);

  for (const auto& data : data_buffer) {
    printf("%c", data);
  }
  printf("\n");

  std::array<uint8_t, kMaxDataLength> buffer;

  buffer[0] = kSyncByte;
  buffer[1] = 1;
  std::copy(data_buffer.begin(), data_buffer.begin() + kDataSize,
            buffer.begin() + 2);

  EXPECT_CALL(emulator_com_mock,
              Transmit(ArraysAreEqual(buffer.data(), kDataSize + 2), _, _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  // When Payload Comunication is called
  emulator_liason.Transmit(data_buffer.data(), kDataSize);
}

// -- --Parametised Tests-- --

class EmulatorLiasonParameterizedTestFixture1
    : public EmulatorLiasonTest,
      public ::testing::WithParamInterface<uint8_t> {};

TEST_P(EmulatorLiasonParameterizedTestFixture1,
       Transmit_MultiplePacketTransferSuccess_FullPackets) {
  uint8_t expected_packet_num = GetParam();

  // Given a buffer with data to send to the emulator
  uint16_t data_buffer_size =
      binarx::emulator_liason::kPacketDataLength * expected_packet_num;
  uint8_t data_buffer[data_buffer_size];
  for (uint16_t i = 0; i < data_buffer_size; i++) {
    data_buffer[i] = static_cast<uint8_t>(i);
  };

  // create a buffer with
  uint16_t buffer_size =
      expected_packet_num * kPacketDataLength + kNumberOfBytesInHeader;
  uint8_t buffer[buffer_size];
  buffer[0] = kSyncByte;
  buffer[1] = expected_packet_num;

  for (uint16_t i = 0; i < buffer_size; i++) {
    buffer[i + 2] = data_buffer[i];
  };

  // transmit all this bytes
  EXPECT_CALL(
      emulator_com_mock,
      Transmit(ArraysAreEqual(buffer, sizeof(buffer)), sizeof(buffer), _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  // Expect for the GPIO line to be set high
  EXPECT_CALL(gpio_mock,
              SetHigh(binarx_gpio_interface::GpioSelector::PayloadReady))
      .Times(1);

  EXPECT_CALL(gpio_mock,
              SetLow(binarx_gpio_interface::GpioSelector::PayloadReady))
      .Times(1);

  // When Payload Comunication is called
  emulator_liason.Transmit(data_buffer, data_buffer_size);
}

INSTANTIATE_TEST_CASE_P(SuccessPacketNumberTest,
                        EmulatorLiasonParameterizedTestFixture1,
                        ::testing::Values(1, 2, 6, 10));

class EmulatorLiasonParameterizedTestFixture2
    : public EmulatorLiasonTest,
      public ::testing::WithParamInterface<uint16_t> {};

TEST_P(EmulatorLiasonParameterizedTestFixture2,
       Transmit_MultiplePacketTransferSuccess_DiferentDataSize) {
  uint16_t data_buffer_size = GetParam();

  uint8_t expected_packet_num =
      data_buffer_size / binarx::emulator_liason::kPacketDataLength;
  if (data_buffer_size % binarx::emulator_liason::kPacketDataLength != 0) {
    expected_packet_num++;
  }

  // Given a buffer with data to send to the emulator
  uint8_t data_buffer[data_buffer_size];
  for (uint16_t i = 0; i < data_buffer_size; i++) {
    data_buffer[i] = static_cast<uint8_t>(i);
  };

  // create a buffer with
  uint16_t buffer_size =
      expected_packet_num * kPacketDataLength + kNumberOfBytesInHeader;

  uint8_t buffer[buffer_size];
  buffer[0] = kSyncByte;
  buffer[1] = expected_packet_num;

  for (uint16_t i = 0; i < data_buffer_size; i++) {
    buffer[i + 2] = data_buffer[i];
  };

  // Expect for the GPIO line to be set high
  EXPECT_CALL(gpio_mock,
              SetHigh(binarx_gpio_interface::GpioSelector::PayloadReady))
      .Times(1);

  EXPECT_CALL(gpio_mock,
              SetLow(binarx_gpio_interface::GpioSelector::PayloadReady))
      .Times(1);

  // When Payload Comunication is called
  emulator_liason.Transmit(data_buffer, data_buffer_size);
}

INSTANTIATE_TEST_CASE_P(SuccessDataSizeTest,
                        EmulatorLiasonParameterizedTestFixture2,
                        ::testing::Values(1, 131, 250, 251, 500, 501, 600, 750,
                                          751, 3000));
