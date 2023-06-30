#include "src/emulator.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/serial_communication_interface.h"

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

TEST_F(EmulatorTest, SerialComunicationSuccess) {
  // Given a spi returns success
  spi_com_mock.DelegateToFake();

  EXPECT_CALL(spi_com_mock, Receive(_, _, _)).Times(1);
  EXPECT_CALL(uart_com_mock, Transmit(_, _, _)).Times(1);

  // When
  emulator.SpiRun();
}

TEST_F(EmulatorTest, DataInVsOutValueCheck) {
  // Given a buffer with data
  uint8_t data_buffer[binarx_emulator::kMaxPayloadDataLength];
  for (uint16_t i = 0; i < sizeof(data_buffer); i++) {
    data_buffer[i] = static_cast<uint8_t>(i);
  };

  EXPECT_CALL(spi_com_mock, Receive(_, _, _))
      .WillOnce(DoAll(
          SetArrayArgument<0>(data_buffer, data_buffer + sizeof(data_buffer)),
          Return(binarx_serial_interface::SerialStatus::Success)));

  // EXPECT_CALL(uart_com_mock, Transmit(ElementsAreArray(data_buffer), _, _))
  //     .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));
  // When
  emulator.SpiRun();
}

TEST_F(EmulatorTest, ToggleYellowLed) {
  EXPECT_CALL(gpio_mock, TogglePin()).Times(1);

  emulator.ToggleYellowLed();
}

TEST_F(EmulatorTest, ButtonPushedSuccess) {
  // Given a spi returns success
  spi_com_mock.DelegateToFake();

  EXPECT_CALL(spi_com_mock, Receive(_, _, _)).Times(1);
  EXPECT_CALL(uart_com_mock, Transmit(_, _, _)).Times(2);

  // When
  emulator.ButtonPressed();
}