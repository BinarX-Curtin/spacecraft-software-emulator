#include "emulator/public/emulator.h"

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

class EmulatorMockTesting : public binarx_emulator::BinarXEmulator {
 public:
  EmulatorMockTesting(binarx_serial_interface::SerialCommunicationInterface*
                          payload_communication,
                      binarx_serial_interface::SerialCommunicationInterface*
                          computer_communication,
                      binarx_gpio_interface::GpioInterface* gpio_object,
                      binarx_time_interface::TimeInterface* time_object)
      : binarx_emulator::BinarXEmulator(payload_communication,
                                        computer_communication, gpio_object,
                                        time_object){};

  void SetPayloadStatus_TestOnly(binarx_emulator::PayloadDataStatus value) {
    payload_status_ = value;
  }
  void SetButtonPressed_TestOnly(bool value) { button_pressed_ = value; }
};

class EmulatorTest : public testing::Test {
 protected:
  // void SetUp() override{};
  NiceMock<SerialCommunicationMock> payload_com_mock;
  NiceMock<SerialCommunicationMock> computer_com_mock;
  NiceMock<GpioMock> gpio_mock;
  NiceMock<TimeMock> time_mock;

  EmulatorMockTesting emulator = EmulatorMockTesting(
      &payload_com_mock, &computer_com_mock, &gpio_mock, &time_mock);
};

TEST_F(EmulatorTest, Run_DataTransferSuccess) {
  // Given a buffer with data that is sent succesfully from the payload
  uint8_t data_buffer[binarx_emulator::kMaxPayloadDataLength];
  for (uint16_t i = 0; i < sizeof(data_buffer) - sizeof(data_buffer) + 1; i++) {
    data_buffer[i] = static_cast<uint8_t>(i);
  };

  // Then the data received by the emulator from the payload should be equal to
  // the data sent to by the emulator to the Users Computer
  EXPECT_CALL(payload_com_mock, Receive(_, _, _))
      .WillOnce(DoAll(
          SetArrayArgument<0>(data_buffer, data_buffer + sizeof(data_buffer)),
          Return(binarx_serial_interface::SerialStatus::Success)));

  // There can be any number of transmisions to the computer.
  EXPECT_CALL(computer_com_mock, Transmit(_, _, _))
      .Times(AnyNumber())
      .WillRepeatedly(Return(binarx_serial_interface::SerialStatus::Success));

  // But at least one must match the data receive from the payload
  EXPECT_CALL(computer_com_mock,
              Transmit(ArraysAreEqual(data_buffer, sizeof(data_buffer)),
                       sizeof(data_buffer), _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  emulator.SetButtonPressed_TestOnly(true);
  emulator.SetPayloadStatus_TestOnly(
      binarx_emulator::PayloadDataStatus::kPayloadReady);
  // When Payload Comunication is called
  emulator.Run();
}

TEST_F(EmulatorTest, Run_IsRedLedOn) {
  EXPECT_CALL(gpio_mock, SetHigh(_)).Times(AnyNumber());
  // Has the Red Led turn on when the emulator starts running
  EXPECT_CALL(gpio_mock, SetHigh(binarx_gpio_interface::GpioSelector::RedLed))
      .Times(1);

  EXPECT_CALL(gpio_mock, SetLow(_)).Times(AnyNumber());
  // And is it turned off
  EXPECT_CALL(gpio_mock, SetLow(binarx_gpio_interface::GpioSelector::RedLed))
      .Times(1);

  emulator.SetButtonPressed_TestOnly(true);
  emulator.SetPayloadStatus_TestOnly(
      binarx_emulator::PayloadDataStatus::kTrasferCompleted);
  // When
  emulator.Run();
}

TEST_F(EmulatorTest, PayloadTimeOut) {
  // Given a buffer with data
  uint8_t data_buffer[binarx_emulator::kMaxPayloadDataLength];
  for (uint16_t i = 0; i < sizeof(data_buffer); i++) {
    data_buffer[i] = static_cast<char>(i);
  };

  // when data is copied to the received data buffer from the payload, but
  // the function returns a timeout. the buffer will not be transmited as an
  // error message gets sent
  EXPECT_CALL(payload_com_mock, Receive(_, _, _))
      .WillOnce(DoAll(
          SetArrayArgument<0>(data_buffer, data_buffer + sizeof(data_buffer)),
          Return(binarx_serial_interface::SerialStatus::Timeout)));

  // There can be any number of transmisions to the computer and no
  // transmission should be equal to the data buffer received from the payload
  // as it returned timeout
  EXPECT_CALL(
      computer_com_mock,
      Transmit(Not(ArraysAreEqual(data_buffer, sizeof(data_buffer))), _, _))
      .Times(AnyNumber())
      .WillRepeatedly(Return(binarx_serial_interface::SerialStatus::Success));

  emulator.SetButtonPressed_TestOnly(true);
  emulator.SetPayloadStatus_TestOnly(
      binarx_emulator::PayloadDataStatus::kPayloadReady);
  // When
  emulator.Run();
}

TEST_F(EmulatorTest, PayloadReturnsError) {
  // Given a buffer with data
  uint8_t data_buffer[binarx_emulator::kMaxPayloadDataLength];
  for (uint16_t i = 0; i < sizeof(data_buffer); i++) {
    data_buffer[i] = static_cast<char>(i);
  };

  // when data is copied to the received data buffer from the payload, but
  // the function returns an Error. then the buffer will not be transmited as
  // an error message gets sent
  EXPECT_CALL(payload_com_mock, Receive(_, _, _))
      .WillOnce(DoAll(
          SetArrayArgument<0>(data_buffer, data_buffer + sizeof(data_buffer)),
          Return(binarx_serial_interface::SerialStatus::Error)));

  // There can be any number of transmisions to the computer and no transmission
  // should be equal to the data buffer received from the payload as it returned
  // timeout
  EXPECT_CALL(
      computer_com_mock,
      Transmit(Not(ArraysAreEqual(data_buffer, sizeof(data_buffer))), _, _))
      .Times(AnyNumber())
      .WillRepeatedly(Return(binarx_serial_interface::SerialStatus::Success));

  emulator.SetButtonPressed_TestOnly(true);
  emulator.SetPayloadStatus_TestOnly(
      binarx_emulator::PayloadDataStatus::kPayloadReady);
  // When
  emulator.Run();
}

TEST_F(EmulatorTest, EmulatorTimeout) {
  // GTEST_SKIP()
  //     << "Skipping single test as it is not completed but should be
  // completed ";
  uint32_t now = 0;
  uint32_t emulator_before_timeout =
      binarx_emulator::kWaitForPayloadMaxTime - 1;
  uint32_t emulator_timeout = binarx_emulator::kWaitForPayloadMaxTime + 1;

  EXPECT_CALL(time_mock, GetTicks())
      .Times(3)
      .WillOnce(Return(now))
      .WillOnce(Return(emulator_before_timeout))
      .WillOnce(Return(emulator_timeout));

  emulator.SetButtonPressed_TestOnly(true);
  // When
  emulator.Run();
}
