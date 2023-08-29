#include "emulator_liason/public/emulator_liason.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sys/time.h>

#include "abstraction_layer/gpio/public/gpi_interface.h"
#include "abstraction_layer/serial_communication_interface.h"
#include "emulator_definitions/emulator_definitions.h"
#include "external_libraries/ArduinoJson-v6.21.3.h"

using namespace ::testing;
using namespace binarx::emulator_definitions;

class SerialCommunicationMock
    : public binarx_serial_interface::SerialCommunicationInterface {
 public:
  MOCK_METHOD(binarx_serial_interface::SerialStatus, Transmit,
              (uint8_t * buffer, uint16_t size, uint32_t timeout), (override));
  MOCK_METHOD(binarx_serial_interface::SerialStatus, Receive,
              (uint8_t * receive_buffer, uint16_t size, uint32_t timeout),
              (override));
  MOCK_METHOD(binarx_serial_interface::SerialStatus, TransmitIt,
              (uint8_t * buffer, uint16_t size), (override));
};

class GpoMock : public bsf::hal::gpio::GpoInterface {
 public:
  MOCK_METHOD(void, SetHigh, (), (override));
  MOCK_METHOD(void, SetLow, (), (override));
  MOCK_METHOD(void, Toggle, (), (override));
};
class GpiMock : public bsf::hal::gpio::GpiInterface {
 public:
  MOCK_METHOD(bool, IsHigh, (), (const));
};

MATCHER_P2(ArraysAreEqual, array, size, "") {
  for (uint16_t i = 0; i < size; i++) {
    if (arg[i] != array[i]) {
      *result_listener << "the expected value is " << PrintToString(array[i])
                       << " and we got " << PrintToString(arg[i])
                       << " for value in index " << PrintToString(i);
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
      bsf::hal::gpio::GpoInterface& gpo_payload_ready)
      : binarx::emulator_liason::EmulatorLiason(emulator_communication,
                                                gpo_payload_ready){};

  void SetPayloadStatus_TestOnly(PayloadDataStatus value) {
    payload_status_ = value;
  }

  using binarx::emulator_liason::EmulatorLiason::PayloadDataStatus;
};

class EmulatorLiasonTest : public testing::Test {
 protected:
  // void SetUp() override{};
  NiceMock<SerialCommunicationMock> emulator_com_mock;
  NiceMock<GpoMock> gpo_payload_ready_mock;

  EmulatorLiasonMockTesting emulator_liason =
      EmulatorLiasonMockTesting(emulator_com_mock, gpo_payload_ready_mock);
};

template <size_t SIZE>
void FillHeader(std::array<uint8_t, SIZE>& buffer, uint16_t data_size) {
  buffer[0] = kSyncByte;
  buffer[1] = static_cast<uint8_t>(data_size >> 8);
  buffer[2] = static_cast<uint8_t>(data_size);
}

TEST_F(EmulatorLiasonTest, SimpleSuccessTest) {
  constexpr uint16_t kDataSize = kPacketLength;
  constexpr uint16_t kBufferSize = kPacketLength + kNumberOfBytesInHeader;
  std::array<uint8_t, kBufferSize> buffer = {0};
  uint8_t count = 0;
  for (auto& item : buffer) {
    item = count++;
  }

  FillHeader(buffer, kDataSize);

  std::array<uint8_t, kDataSize> data_buffer;
  std::copy(buffer.begin() + kNumberOfBytesInHeader, buffer.end(),
            data_buffer.begin());

  EXPECT_CALL(
      emulator_com_mock,
      TransmitIt(ArraysAreEqual(buffer.data(), kBufferSize), kBufferSize))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  // When Payload Comunication is called
  emulator_liason.Transmit(data_buffer.data(), data_buffer.size());
  emulator_liason.ChipSelectInterrupt();
  emulator_liason.TransmitCallBackInterrupt();
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

  std::array<uint8_t, kMaxPayloadDataLength> buffer;

  FillHeader(buffer, kDataSize);

  std::copy(data_buffer.begin(), data_buffer.begin() + kDataSize,
            buffer.begin() + kNumberOfBytesInHeader);

  EXPECT_CALL(
      emulator_com_mock,
      TransmitIt(
          ArraysAreEqual(buffer.data(), kDataSize + kNumberOfBytesInHeader), _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  // When Payload Comunication is called
  emulator_liason.Transmit(data_buffer.data(), kDataSize);
  emulator_liason.ChipSelectInterrupt();
  emulator_liason.TransmitCallBackInterrupt();
}

class EmulatorLiasonParameterizedTestFixture2
    : public EmulatorLiasonTest,
      public ::testing::WithParamInterface<uint16_t> {};

TEST_P(EmulatorLiasonParameterizedTestFixture2,
       Transmit_MultiplePacketTransferSuccess_DiferentDataSize) {
  uint16_t data_buffer_size = GetParam();

  if (data_buffer_size > kMaxPayloadDataLength) {
    FAIL();
  }

  uint8_t expected_packet_num = data_buffer_size / kPacketLength;
  if (data_buffer_size % kPacketLength != 0) {
    expected_packet_num++;
  }

  // Given a buffer with data to send to the emulator
  std::array<uint8_t, kMaxPayloadDataLength> data_buffer;

  for (uint16_t i = 0; i < data_buffer_size; i++) {
    data_buffer[i] = static_cast<uint8_t>(i);
  };

  // create a buffer with
  uint16_t buffer_size =
      expected_packet_num * kPacketLength + kNumberOfBytesInHeader;

  std::array<uint8_t, kMaxPayloadDataLength + kNumberOfBytesInHeader> buffer;
  FillHeader(buffer, data_buffer_size);

  for (uint16_t i = 0; i < data_buffer_size; i++) {
    buffer[i + kNumberOfBytesInHeader] = data_buffer[i];
  };

  // transmit all this bytes
  EXPECT_CALL(emulator_com_mock,
              TransmitIt(ArraysAreEqual(buffer, data_buffer_size), buffer_size))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  // Expect for the GPIO line to be set high
  EXPECT_CALL(gpo_payload_ready_mock, SetHigh()).Times(1);

  EXPECT_CALL(gpo_payload_ready_mock, SetLow()).Times(1);
  // When Payload Comunication is called
  emulator_liason.Transmit(data_buffer.data(), data_buffer_size);
  emulator_liason.ChipSelectInterrupt();
  emulator_liason.TransmitCallBackInterrupt();
}

INSTANTIATE_TEST_CASE_P(
    SuccessDataSizeTest, EmulatorLiasonParameterizedTestFixture2,
    ::testing::Values(1, 131, kPacketLength, kPacketLength + 1,
                      2 * kPacketLength, 2 * kPacketLength + 1, 600,
                      3 * kPacketLength, 2 * kPacketLength + 1,
                      kMaxPayloadDataLength));
