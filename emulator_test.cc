#include "src/emulator.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
using namespace ::testing;

class SerialCommunicationMock
    : public binarx_serial_interface::SerialCommunicationInterface {
 public:
  MOCK_METHOD(void, Transmit, (uint8_t * buffer), (override));
  MOCK_METHOD(uint8_t, Receive, (uint8_t * receive_buffer), (override));
  MOCK_METHOD(uint8_t, TransmitReceive,
              (uint8_t * transmit_buffer, uint8_t *receive_buffer), (override));
};

class GpioMock : public binarx_gpio_interface::GpioInterface {
 public:
  MOCK_METHOD(void, SetHigh, (), (override));
  MOCK_METHOD(void, SetLow, (), (override));
  MOCK_METHOD(void, TogglePin, (), (override));
};

class EmulatorTest : public testing::Test {
 protected:
  // void SetUp() override{};
  SerialCommunicationMock spi_com_mock;
  SerialCommunicationMock uart_com_mock;
  GpioMock gpio_mock;
  binarx_emulator::BinarXEmulator emulator = binarx_emulator::BinarXEmulator(
      &spi_com_mock, &gpio_mock, &uart_com_mock);
};

TEST_F(EmulatorTest, SerialComunication) {
  EXPECT_CALL(spi_com_mock, Receive(_)).Times(1);

  emulator.SpiRun();
}

TEST_F(EmulatorTest, ToggleYellowLed) {
  EXPECT_CALL(gpio_mock, TogglePin()).Times(1);

  emulator.ToggleYellowLed();
}
