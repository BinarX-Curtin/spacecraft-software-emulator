/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator header file
 *
 */

#pragma once
#include <stdint.h>

#include "gpio_interface.h"
#include "serial_communication_interface.h"

namespace binarx_emulator {

constexpr uint16_t kMaxPayloadDataLength = 500;
const uint16_t kDefaultCommunicationDelay = 200;

class BinarXEmulator {
 public:
  BinarXEmulator(
      binarx_serial_interface::SerialCommunicationInterface* spi_communication,
      binarx_gpio_interface::GpioInterface* gpio_object,
      binarx_serial_interface::SerialCommunicationInterface* uart_communication)
      : spi_comunication_(spi_communication),
        gpio_controller_(gpio_object),
        uart_comunication_(uart_communication){};

  void SpiRun();
  void ToggleYellowLed();
  void ButtonPressed();

 private:
  binarx_serial_interface::SerialCommunicationInterface* spi_comunication_;
  binarx_gpio_interface::GpioInterface* gpio_controller_;
  binarx_serial_interface::SerialCommunicationInterface* uart_comunication_;
};

}  // namespace binarx_emulator