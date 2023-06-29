/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator header file
 *
 */

#pragma once
#include <stdint.h>

#include <map>
#include <string>

#include "gpio_interface.h"
#include "serial_communication_interface.h"

namespace binarx_emulator {

constexpr uint32_t kMaxDataLength = 17;

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

 private:
  std::map<std::string, std::string> error_strings_;
  binarx_serial_interface::SerialCommunicationInterface* spi_comunication_;
  binarx_gpio_interface::GpioInterface* gpio_controller_;
  binarx_serial_interface::SerialCommunicationInterface* uart_comunication_;
  uint8_t receive_buffer_[kMaxDataLength];
};

}  // namespace binarx_emulator 