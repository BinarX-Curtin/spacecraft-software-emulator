/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator source file
 *
 */

#include "emulator.h"

#include <math.h>

#include <chrono>
#include <thread>

namespace binarx_emulator {

void BinarXEmulator::SpiRun() {
  uint8_t receive_buffer[kMaxPayloadDataLength];
  binarx_serial_interface::SerialStatus serial_status =
      spi_comunication_->Receive(receive_buffer, kMaxPayloadDataLength,
                                 kDefaultCommunicationDelay);

  serial_status = uart_comunication_->Transmit(
      receive_buffer, kMaxPayloadDataLength, kDefaultCommunicationDelay);
}

void BinarXEmulator::ToggleYellowLed() {
  gpio_controller_->TogglePin();
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void BinarXEmulator::ButtonPressed() {
  uint8_t info_msg[] =
      "INFO: Button pressed and waiting for SPI transmission \n";
  binarx_serial_interface::SerialStatus serial_status =
      uart_comunication_->Transmit(info_msg, sizeof(info_msg),
                                   kDefaultCommunicationDelay);
  SpiRun();
}

}  // namespace binarx_emulator