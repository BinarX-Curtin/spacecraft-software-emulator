#include "emulator/public/emulator.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "abstraction_layer/serial_communication_interface.h"

using namespace ::testing;

class FakeSerialComunication
    : public binarx_serial_interface::SerialCommunicationInterface {
 public:
  binarx_serial_interface::SerialStatus Receive(uint8_t *receive_buffer,
                                                uint16_t size,
                                                uint32_t timeout) override {
    for (uint16_t i = 0; i < size; i++) {
      receive_buffer[i] = uint8_t(static_cast<uint8_t>(i));
    };
    return binarx_serial_interface::SerialStatus::Success;
  };
  binarx_serial_interface::SerialStatus Transmit(uint8_t *transmit_buffer,
                                                 uint16_t size,
                                                 uint32_t timeout) override {
    return binarx_serial_interface::SerialStatus::Success;
  };

  // binarx_serial_interface::SerialStatus TransmitReceive(
  //     std::array<uint8_t, kMaxPayloadDataLength> transmit_buffer,
  //     std::array<uint8_t, kMaxPayloadDataLength> receive_buffer, uint16_t
  //     size, uint32_t timeout) override {
  //   return binarx_serial_interface::SerialStatus::Success;
  // };
};

class SerialCommunicationMock
    : public binarx_serial_interface::SerialCommunicationInterface {
 public:
  MOCK_METHOD(binarx_serial_interface::SerialStatus, Transmit,
              (uint8_t * buffer, uint16_t size, uint32_t timeout), (override));
  MOCK_METHOD(binarx_serial_interface::SerialStatus, Receive,
              (uint8_t * receive_buffer, uint16_t size, uint32_t timeout),
              (override));
  // MOCK_METHOD(binarx_serial_interface::SerialStatus, TransmitReceive,
  //             (std::array<uint8_t, kMaxPayloadDataLength> transmit_buffer,
  //             std::array<uint8_t, kMaxPayloadDataLength> receive_buffer,
  //              uint16_t size, uint32_t timeout),
  //             (override));

  void DelegateToFake() {
    ON_CALL(*this, Receive)
        .WillByDefault(
            [this](uint8_t *buffer, uint16_t size, uint32_t timeout) {
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

MATCHER_P2(ArraysAreEqual, array, size,
           "elements are equal [%(array)s, %(size)s]") {
  for (uint16_t i = 0; i < size; i++) {
    if (arg[i] != array[i]) {
      return false;
    }
  }
  return true;
}

class EmulatorTest : public testing::Test {
 protected:
  // void SetUp() override{};
  SerialCommunicationMock spi_com_mock;
  SerialCommunicationMock uart_com_mock;
  GpioMock gpio_mock;
  binarx_emulator::BinarXEmulator emulator = binarx_emulator::BinarXEmulator(
      &spi_com_mock, &uart_com_mock, &gpio_mock);
};

TEST_F(EmulatorTest, SPIComunicationSuccess) {
  // Given a spi returns success
  spi_com_mock.DelegateToFake();

  EXPECT_CALL(
      gpio_mock,
      WaitForInterrupt(binarx_gpio_interface::GpioSelector::PayloadReady, _))
      .Times(1);

  // When a transaction is successful the green Led will turn on
  EXPECT_CALL(gpio_mock, SetHigh(binarx_gpio_interface::GpioSelector::GreenLed))
      .Times(1);

  EXPECT_CALL(gpio_mock, SetLow(binarx_gpio_interface::GpioSelector::GreenLed))
      .Times(1);

  EXPECT_CALL(spi_com_mock, Receive(_, _, _)).Times(1);
  EXPECT_CALL(uart_com_mock, Transmit(_, _, _)).Times(1);

  // When
  emulator.SpiRun();
}

TEST_F(EmulatorTest, SpiInEqualsUartOut) {
  // Given a buffer with data
  uint8_t data_buffer[binarx_emulator::kMaxPayloadDataLength];
  for (uint16_t i = 0; i < sizeof(data_buffer); i++) {
    data_buffer[i] = static_cast<uint8_t>(i);
  };

  EXPECT_CALL(spi_com_mock, Receive(_, _, _))
      .WillOnce(DoAll(
          SetArrayArgument<0>(data_buffer, data_buffer + sizeof(data_buffer)),
          Return(binarx_serial_interface::SerialStatus::Success)));

  EXPECT_CALL(uart_com_mock,
              Transmit(ArraysAreEqual(data_buffer, sizeof(data_buffer)),
                       sizeof(data_buffer), _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));
  // When
  emulator.SpiRun();
}

TEST_F(EmulatorTest, SpiTimeOut) {
  // Given a buffer with data
  uint8_t data_buffer[binarx_emulator::kMaxPayloadDataLength];

  EXPECT_CALL(spi_com_mock, Receive(_, _, _))
      .WillOnce(DoAll(Return(binarx_serial_interface::SerialStatus::Timeout)));

  EXPECT_CALL(
      uart_com_mock,
      Transmit(Not(ArraysAreEqual(data_buffer, sizeof(data_buffer))), _, _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));
  // When
  emulator.SpiRun();
}

TEST_F(EmulatorTest, SpiReturnsError) {
  // Given a buffer with data
  uint8_t data_buffer[binarx_emulator::kMaxPayloadDataLength];

  EXPECT_CALL(spi_com_mock, Receive(_, _, _))
      .WillOnce(DoAll(Return(binarx_serial_interface::SerialStatus::Error)));

  EXPECT_CALL(
      uart_com_mock,
      Transmit(Not(ArraysAreEqual(data_buffer, sizeof(data_buffer))), _, _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));
  // When
  emulator.SpiRun();
}

TEST_F(EmulatorTest, WaitForInterruptTimeOut) {
  // Given a buffer with data
  uint8_t data_buffer[binarx_emulator::kMaxPayloadDataLength];

  EXPECT_CALL(
      gpio_mock,
      WaitForInterrupt(binarx_gpio_interface::GpioSelector::PayloadReady, _))
      .WillOnce(Return(binarx_gpio_interface::GpioStatus::Timeout));

  // An error message will be sent by Uart
  EXPECT_CALL(
      uart_com_mock,
      Transmit(Not(ArraysAreEqual(data_buffer, sizeof(data_buffer))), _, _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  // When
  emulator.SpiRun();
}

TEST_F(EmulatorTest, ToggleYellowLed) {
  EXPECT_CALL(gpio_mock,
              TogglePin(binarx_gpio_interface::GpioSelector::YellowLed))
      .Times(1);

  emulator.ToggleYellowLed();
}

TEST_F(EmulatorTest, ButtonPushedSuccess) {
  // Given spi returns success
  spi_com_mock.DelegateToFake();

  // Expect for the payload to be turned on, and then off
  EXPECT_CALL(gpio_mock, SetHigh(binarx_gpio_interface::GpioSelector::RedLed))
      .Times(1);
  EXPECT_CALL(gpio_mock, SetLow(binarx_gpio_interface::GpioSelector::RedLed))
      .Times(1);

  // Green LED is set high and low but we do not care how many times.
  EXPECT_CALL(gpio_mock,
              SetHigh(binarx_gpio_interface::GpioSelector::GreenLed));
  EXPECT_CALL(gpio_mock, SetLow(binarx_gpio_interface::GpioSelector::GreenLed));

  // Expect an SPI receive and Uart Transmit transactions
  EXPECT_CALL(spi_com_mock, Receive(_, _, _)).Times(1);
  EXPECT_CALL(uart_com_mock, Transmit(_, _, _)).Times(2);

  // When
  emulator.ButtonPressed();
}