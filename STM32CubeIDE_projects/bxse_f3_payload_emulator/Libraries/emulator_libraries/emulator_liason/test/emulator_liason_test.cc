#include "emulator_liason/public/emulator_liason.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sys/time.h>

#include "abstraction_layer/gpio_interface.h"
#include "abstraction_layer/serial_communication_interface.h"

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

  uint8_t kSyncByte = 5;
  uint8_t kNumberOfBytesInHeader = 2;
  uint16_t kPacketDataLength = 250;
};

// -- --Parametised Tests-- --

class EmulatorLiasonParameterizedTestFixture1
    : public EmulatorLiasonTest,
      public ::testing::WithParamInterface<uint8_t> {};

TEST_P(EmulatorLiasonParameterizedTestFixture1,
       Transmit_MultiplePacketTransferSuccess) {
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
  emulator_liason.Transmit(data_buffer, sizeof(data_buffer),
                           binarx::emulator_liason::kDefaultCommunicationDelay);
}

INSTANTIATE_TEST_CASE_P(SuccessPacketNumberTest,
                        EmulatorLiasonParameterizedTestFixture1,
                        ::testing::Values(1, 2, 6, 10));
