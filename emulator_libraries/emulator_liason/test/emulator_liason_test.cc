#include "emulator_liason/public/emulator_liason.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sys/time.h>

#include "abstraction_layer/gpio_interface.h"
#include "abstraction_layer/serial_communication_interface.h"
#include "abstraction_layer/time_interface.h"

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

  uint8_t kSynkByte = 5;
};

TEST_F(EmulatorLiasonTest, Transmit_PacketTransferSuccess) {
  // Given a buffer with data to send to the emulator
  uint8_t data_buffer[binarx::emulator_liason::kPacketDataLength];
  for (uint16_t i = 0; i < sizeof(data_buffer) - sizeof(data_buffer) + 1; i++) {
    data_buffer[i] = static_cast<uint8_t>(i);
  };

  // The packet buffer to receive
  uint8_t expected_packet_num = 1;
  uint8_t packet_num_buffer[2] = {kSynkByte, expected_packet_num};

  // We expect a fisrt paquet to say that one packet will be transmitted
  EXPECT_CALL(
      emulator_com_mock,
      Transmit(ArraysAreEqual(packet_num_buffer, sizeof(packet_num_buffer)),
               sizeof(packet_num_buffer), _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  // And another transmission request with the data
  EXPECT_CALL(emulator_com_mock,
              Transmit(ArraysAreEqual(data_buffer, sizeof(data_buffer)),
                       sizeof(data_buffer), _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  // When Payload Comunication is called
  emulator_liason.Transmit(data_buffer, sizeof(data_buffer),
                           binarx::emulator_liason::kDefaultCommunicationDelay);
}

TEST_F(EmulatorLiasonTest, Transmit_MultiplePacketTransferSuccess) {
  uint8_t expected_packet_num = 6;

  // Given a buffer with data to send to the emulator
  uint8_t data_buffer[binarx::emulator_liason::kPacketDataLength *
                      expected_packet_num];
  for (uint16_t i = 0; i < sizeof(data_buffer) - sizeof(data_buffer) + 1; i++) {
    data_buffer[i] = static_cast<uint8_t>(i);
  };

  // The packet buffer to receive
  uint8_t packet_num_buffer[2] = {kSynkByte, expected_packet_num};

  // We expect a fisrt paquet to say that one packet will be transmitted
  EXPECT_CALL(
      emulator_com_mock,
      Transmit(ArraysAreEqual(packet_num_buffer, sizeof(packet_num_buffer)),
               sizeof(packet_num_buffer), _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  // And another transmission request with the data
  EXPECT_CALL(emulator_com_mock,
              Transmit(ArraysAreEqual(data_buffer, sizeof(data_buffer)),
                       sizeof(data_buffer), _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  // When Payload Comunication is called
  emulator_liason.Transmit(data_buffer, sizeof(data_buffer),
                           binarx::emulator_liason::kDefaultCommunicationDelay);
}
