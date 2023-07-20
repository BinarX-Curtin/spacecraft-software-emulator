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
  if (!button_pressed_) {
    return;
  }
  BinarXEmulator::RunStartInfo();

  // Need to start a timer
  uint32_t emulator_timeout =
      time_controller_->GetTicks() + kWaitForPayloadMaxTime;

  while (emulator_timeout > time_controller_->GetTicks() &&
         payload_status_ != PayloadDataStatus::kTrasferCompleted) {
    PayloadCommunicationHandler();
  }

  button_pressed_ = false;
  payload_status_ = PayloadDataStatus::kWaitingForPayload;

  BinarXEmulator::RunEndInfo();
}

void BinarXEmulator::PayloadCommunicationHandler() {
  // Only run once the payload is ready
  if (payload_status_ != PayloadDataStatus::kPayloadReady) {
    return;
  }

  // Turn on the LED to demostrate SPI success
  gpio_controller_->SetHigh(binarx_gpio_interface::GpioSelector::GreenLed);

  char error_msg[] =
      "ERROR: Sorry the message was not received correctly by the Binar "
      "Emulator \n ";

  // Trigger the Spi Receive message
  uint8_t receive_buffer[kMaxPayloadDataLength];
  binarx_serial_interface::SerialStatus serial_status =
      payload_communication_->Receive(receive_buffer, kMaxPayloadDataLength,
                                      kDefaultCommunicationDelay);

  // Check the status of the SPI transaction
  if (serial_status == binarx_serial_interface::SerialStatus::Success) {
    // Send the data over UART if SPI data was received succesfully
    computer_communication_->Transmit(receive_buffer, sizeof(receive_buffer),
                                      kDefaultCommunicationDelay);
  } else {
    // Otherwise, copy error message to receive buffer
    computer_communication_->Transmit((uint8_t *)error_msg, strlen(error_msg),
                                      kDefaultCommunicationDelay);
  }
  // // Let the main runner know that the payload has sent the information
  payload_status_ = PayloadDataStatus::kTrasferCompleted;

  // Make sure the Green LED is off
  gpio_controller_->SetLow(binarx_gpio_interface::GpioSelector::GreenLed);
}

void BinarXEmulator::Init() {
  // turn on Yellow LED
  gpio_controller_->SetHigh(binarx_gpio_interface::GpioSelector::YellowLed);
}

void BinarXEmulator::PayloadCommunicationCallback() {
  if (payload_status_ == PayloadDataStatus::kWaitingForPayload) {
    payload_status_ = PayloadDataStatus::kPayloadReady;
  }
}
void BinarXEmulator::ButtonPressCallback() { button_pressed_ = true; }

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
