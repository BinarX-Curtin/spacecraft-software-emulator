/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator header file
 *
 */

#pragma once
#include <stdint.h>

#include "abstraction_layer/gpio_interface.h"
#include "abstraction_layer/serial_communication_interface.h"

namespace binarx_emulator {

constexpr uint16_t kMaxPayloadDataLength =
    500; /**> Max payload data size allowed */
constexpr uint32_t kDefaultCommunicationDelay =
    200; /**> The time to wait for a communication transaction*/

/**
 * @brief Binar X Emulator to implement the Emulator functions
 *
 */
class BinarXEmulator {
 public:
  /**
   * @brief Construct a new Binar X Emulator object with dependency injection
   *
   * @param spi_communication Pointer to the SPI implementation object
   * @param uart_communication Pointer to the UART implementation object
   * @param gpio_object Pointer to the GPIO implementation object
   */
  BinarXEmulator(
      binarx_serial_interface::SerialCommunicationInterface* spi_communication,
      binarx_serial_interface::SerialCommunicationInterface* uart_communication,
      binarx_gpio_interface::GpioInterface* gpio_object)
      : spi_comunication_(spi_communication),
        uart_comunication_(uart_communication),
        gpio_controller_(gpio_object){};
  /**
   * @brief Function for the SPI sequence
   *
   */
  void SpiRun();
  /**
   * @brief A function to toggle the Yellow LED to communicate that the Emulator
   * is switched on
   *
   */
  void ToggleYellowLed();
  /**
   * @brief Function sequence for when the Button is pressed
   *
   */
  void ButtonPressed();

 private:
  binarx_serial_interface::SerialCommunicationInterface* spi_comunication_;
  binarx_serial_interface::SerialCommunicationInterface* uart_comunication_;
  binarx_gpio_interface::GpioInterface* gpio_controller_;
};

}  // namespace binarx_emulator