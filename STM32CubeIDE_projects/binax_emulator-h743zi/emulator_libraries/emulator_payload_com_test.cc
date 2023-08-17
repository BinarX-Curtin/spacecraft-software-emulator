/**
 * @file emulator_payload_com_test.cc
 * @author Tristan Ward
 * @brief testing the coupling of the Binar X emulator and the emulator_liason
 * library that goes on the payload
 *
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sys/time.h>

#include "abstraction_layer/gpio_interface.h"
#include "abstraction_layer/serial_communication_interface.h"
#include "abstraction_layer/time_interface.h"
#include "emulator/public/emulator.h"
#include "emulator_liason/public/emulator_liason.h"

using namespace ::testing;

// ------------------ MOCK CLASES  ---------------------
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

 private:
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
  MOCK_METHOD(binarx_gpio_interface::GpioStatus, WaitForInterrupt,
              (binarx_gpio_interface::GpioSelector, uint32_t timeout),
              (override));
};
class TimeMock : public binarx_time_interface::TimeInterface {
 public:
  MOCK_METHOD(void, Delay, (uint32_t milliseconds), (override));
  MOCK_METHOD(uint32_t, GetTicks, (), (override));
};

MATCHER_P2(ArraysAreEqual, array, size,
           "elements are equal [%(array)s, %(size)s]") {
  for (uint16_t i = 0; i < size; i++) {
    if (arg[i] != array[i]) {
      return false;
    }
  }
  return true;
}

class EmulatorMockTesting : public binarx::emulator::BinarXEmulator {
 public:
  EmulatorMockTesting(binarx_serial_interface::SerialCommunicationInterface&
                          payload_communication,
                      binarx_serial_interface::SerialCommunicationInterface&
                          computer_communication,
                      binarx_gpio_interface::GpioInterface& gpio_object,
                      binarx_time_interface::TimeInterface& time_object)
      : binarx::emulator::BinarXEmulator(payload_communication,
                                        computer_communication, gpio_object,
                                        time_object){};

  void SetPayloadStatus_TestOnly(PayloadDataStatus value) {
    payload_status_ = value;
  }
  void SetButtonPressed_TestOnly(bool value) { button_pressed_ = value; }

  using binarx::emulator::BinarXEmulator::PayloadDataStatus;
};

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

class EmulatorPayloadComTest : public testing::Test {
 protected:
  // emulator
  NiceMock<SerialCommunicationMock> e_payload_com_mock;
  NiceMock<SerialCommunicationMock> e_computer_com_mock;
  NiceMock<GpioMock> e_gpio_mock;
  NiceMock<TimeMock> e_time_mock;
  EmulatorMockTesting emulator = EmulatorMockTesting(
      e_payload_com_mock, e_computer_com_mock, e_gpio_mock, e_time_mock);

  // emulator laison
  NiceMock<SerialCommunicationMock> p_emulator_com_mock;
  NiceMock<GpioMock> p_gpio_mock;
  EmulatorLiasonMockTesting emulator_liason =
      EmulatorLiasonMockTesting(p_emulator_com_mock, p_gpio_mock);

  //   uint8_t kSynkByte = 5;
};

// ------------------TESTS --------------------

TEST_F(EmulatorPayloadComTest, PayloadToEmulator_PacketTransferSuccess) {
  // if we create data to send from the payload
  uint8_t data_buffer[binarx::emulator::kMaxPayloadDataLength];
  for (uint16_t i = 0; i < sizeof(data_buffer) - sizeof(data_buffer) + 1; i++) {
    data_buffer[i] = static_cast<uint8_t>(i);
  };

  // We should expect that the payload can send the bufer to the emulator
}
