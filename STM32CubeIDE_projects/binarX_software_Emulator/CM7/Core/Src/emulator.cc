/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator source file
 *
 */

#include "emulator.h"

#include <string.h>

namespace binarx_emulator {

void BinarXEmulator::Run() {
  // Wait for button push
  if (!button_pressed_) {
    return;
  }
  BinarXEmulator::RunStartInfo();

  // Start the timeout timer by storing the now time
  uint32_t emulator_timeout =
      time_controller_.GetTicks() + kWaitForPayloadMaxTime;

  // Wait for payload data transfer or until timeout
  while (emulator_timeout > time_controller_.GetTicks() &&
         payload_status_ != PayloadDataStatus::kTrasferCompleted) {
    PayloadCommunicationHandler();
  }

  // Reset the emulator states
  button_pressed_ = false;
  payload_status_ = PayloadDataStatus::kWaitingForPayload;

  BinarXEmulator::RunEndInfo();
}

void BinarXEmulator::PayloadCommunicationHandler() {
  // return if the payload is not ready
  if (payload_status_ != PayloadDataStatus::kPayloadReady) {
    return;
  }

  // Turn on the Green LED to inform that the payload ready interrupt was
  // received
  gpio_controller_.SetHigh(binarx_gpio_interface::GpioSelector::GreenLed);

  // Trigger the Master Spi Receive message
  uint8_t receive_buffer[kMaxPayloadDataLength];
  binarx_serial_interface::SerialStatus serial_status =
      payload_communication_.Receive(receive_buffer, kMaxPayloadDataLength,
                                     kDefaultCommunicationDelay);

  // Check the status of the SPI transaction
  if (serial_status == binarx_serial_interface::SerialStatus::Success) {
    // Send the data over UART if SPI data was received succesfully
    computer_communication_.Transmit(receive_buffer, sizeof(receive_buffer),
                                     kDefaultCommunicationDelay);
  } else {
    // Otherwise send error message
    char error_msg[] =
        "ERROR: Sorry the message was not received correctly by the Binar "
        "Emulator \n ";
    computer_communication_.Transmit((uint8_t *)error_msg, strlen(error_msg),
                                     kDefaultCommunicationDelay);
  }
  // Move to the next state as the communication has been completed
  payload_status_ = PayloadDataStatus::kTrasferCompleted;

  // Turn off Green LED
  gpio_controller_.SetLow(binarx_gpio_interface::GpioSelector::GreenLed);
}

void BinarXEmulator::Init() {
  // turn on Yellow LED
  gpio_controller_.SetHigh(binarx_gpio_interface::GpioSelector::YellowLed);
}

void BinarXEmulator::PayloadReadyInterruptCallback() {
  if (payload_status_ == PayloadDataStatus::kWaitingForPayload) {
    payload_status_ = PayloadDataStatus::kPayloadReady;
  }
}
void BinarXEmulator::ButtonPressCallback() { button_pressed_ = true; }

void BinarXEmulator::RunStartInfo() {
  // turn on red LED
  gpio_controller_.SetHigh(binarx_gpio_interface::GpioSelector::RedLed);
  // Print a message to the Serial Monitor to inform the students
  uint8_t info_msg[] =
      "INFO: Button pressed and waiting for SPI transmission \n";
  computer_communication_.Transmit(info_msg, sizeof(info_msg),
                                   kDefaultCommunicationDelay);
}

void BinarXEmulator::RunEndInfo() {
  // Print a message to the Serial Monitor to inform the students
  uint8_t info_msg[] = "INFO: Turning emulator off \n";
  computer_communication_.Transmit(info_msg, sizeof(info_msg),
                                   kDefaultCommunicationDelay);

  // turn on red LED
  gpio_controller_.SetLow(binarx_gpio_interface::GpioSelector::RedLed);
}

}  // namespace binarx_emulator
