/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator source file
 *
 */

#include "emulator/public/emulator.h"

#include <string.h>

namespace binarx_emulator {

void BinarXEmulator::Run() {
  BinarXEmulator::RunStartInfo();

  button_pressed_ = true;

  // Need to start a timer
  uint32_t emulator_timer =
      time_controller_->GetTicks() + kWaitForPayloadMaxTime;

  while (emulator_timer > time_controller_->GetTicks() &&
         waiting_for_payload_) {
    // do nothing
  }

  // Reset states
  waiting_for_payload_ = true;
  button_pressed_ = false;

  // This needs to be called even if the timer has stopped. Watchdog with a
  // callback to RunEndInfo?
  BinarXEmulator::RunEndInfo();
}

void BinarXEmulator::Init() {
  // turn on Yellow LED
  gpio_controller_->SetHigh(binarx_gpio_interface::GpioSelector::YellowLed);
}

void BinarXEmulator::PayloadCommunication() {
  if (!button_pressed_) {
    // payload communication should only be active when the button has been
    // pressed
    return;
  }

  // Reset the button pressed to false
  button_pressed_ = false;

  char error_msg[] =
      "ERROR: Sorry the message was not received correctly by the Binar "
      "Emulator \n ";

  // // Trigger the Spi Receive message
  uint8_t receive_buffer[kMaxPayloadDataLength];
  binarx_serial_interface::SerialStatus serial_status =
      payload_communication_->Receive(receive_buffer, kMaxPayloadDataLength,
                                      kDefaultCommunicationDelay);

  // Check the status of the SPI transaction
  if (serial_status == binarx_serial_interface::SerialStatus::Success) {
    // Turn on the LED to demostrate SPI success
    gpio_controller_->SetHigh(binarx_gpio_interface::GpioSelector::GreenLed);
    // Send the data over UART if SPI data was received succesfully
    computer_communication_->Transmit(receive_buffer, sizeof(receive_buffer),
                                      kDefaultCommunicationDelay);
  } else {
    // Otherwise, copy error message to receive buffer
    computer_communication_->Transmit((uint8_t *)error_msg, strlen(error_msg),
                                      kDefaultCommunicationDelay);
  }

  // // Make sure the Green LED is off
  gpio_controller_->SetLow(binarx_gpio_interface::GpioSelector::GreenLed);

  // // Let the main runner know that the payload has sent the information
  waiting_for_payload_ = false;
  // return;
}

void BinarXEmulator::RunStartInfo() {
  // turn on red LED
  gpio_controller_->SetHigh(binarx_gpio_interface::GpioSelector::RedLed);
  // Print a message to the Serial Monitor to inform the students
  uint8_t info_msg[] =
      "INFO: Button pressed and waiting for SPI transmission \n";
  computer_communication_->Transmit(info_msg, sizeof(info_msg),
                                    kDefaultCommunicationDelay);
}

void BinarXEmulator::RunEndInfo() {
  // Print a message to the Serial Monitor to inform the students
  uint8_t info_msg[] = "INFO: Turning emulator off \n";
  computer_communication_->Transmit(info_msg, sizeof(info_msg),
                                    kDefaultCommunicationDelay);

  // turn on red LED
  gpio_controller_->SetLow(binarx_gpio_interface::GpioSelector::RedLed);
}

}  // namespace binarx_emulator
