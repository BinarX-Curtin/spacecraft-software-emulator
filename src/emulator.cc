/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator source file
 *
 */

#include "emulator.h"

#include <math.h>

namespace binarx_emulator {

void BinarXEmulator::SpiRun() {
  uint8_t receive_buffer[kMaxPayloadDataLength];

  // Wait for the payload to be ready to send data
  binarx_gpio_interface::GpioStatus payload_ready_status =
      gpio_controller_->WaitForInterrupt(
          binarx_gpio_interface::GpioSelector::PayloadReady,
          kDefaultCommunicationDelay);
  // if the payload ready pin was not set high print a message and exit
  if (payload_ready_status == binarx_gpio_interface::GpioStatus::Timeout) {
    uint8_t error_msg_gpio[] =
        "ERROR: The payload ready pin was never set high and the code timed "
        "out \n ";
    uart_comunication_->Transmit(error_msg_gpio, sizeof(error_msg_gpio),
                                 kDefaultCommunicationDelay);
    return;
  }
  // Trigger the Spi Receive message
  binarx_serial_interface::SerialStatus serial_status =
      spi_comunication_->Receive(receive_buffer, kMaxPayloadDataLength,
                                 kDefaultCommunicationDelay);
  // Check the status of the SPI transaction
  if (serial_status == binarx_serial_interface::SerialStatus::Success) {
    // Turn on the LED to demostrate SPI success
    gpio_controller_->SetHigh(binarx_gpio_interface::GpioSelector::GreenLed);
    // Send the data over UART if SPI data was received succesfully
    serial_status = uart_comunication_->Transmit(
        receive_buffer, kMaxPayloadDataLength, kDefaultCommunicationDelay);
  } else {
    // Otherwise, send an error message
    uint8_t error_msg_spi[] =
        "ERROR: Sorry the message was not received correctly by the Binar "
        "Emulator \n ";
    uart_comunication_->Transmit(error_msg_spi, sizeof(error_msg_spi),
                                 kDefaultCommunicationDelay);
  }
  // Make sure the Green LED is off
  gpio_controller_->SetLow(binarx_gpio_interface::GpioSelector::GreenLed);
}

void BinarXEmulator::ToggleYellowLed() {
  gpio_controller_->TogglePin(binarx_gpio_interface::GpioSelector::YellowLed);
}

void BinarXEmulator::ButtonPressed() {
  // Set Red LED on to aknowledge the button has been succesfully pressed.
  // Use the same pin to power the payload
  gpio_controller_->SetHigh(binarx_gpio_interface::GpioSelector::RedLed);

  // Print a message to the Serial Monitor to inform the students
  uint8_t info_msg[] =
      "INFO: Button pressed and waiting for SPI transmission \n";
  binarx_serial_interface::SerialStatus serial_status =
      uart_comunication_->Transmit(info_msg, sizeof(info_msg),
                                   kDefaultCommunicationDelay);

  // Run the SPI Receive Sequence
  SpiRun();

  // Turn Red Led and payoad off as the payload request has finished
  gpio_controller_->SetLow(binarx_gpio_interface::GpioSelector::RedLed);
}

}  // namespace binarx_emulator
