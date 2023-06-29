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

void BinarXEmulator::SpiRun() { spi_comunication_->Receive(receive_buffer_); }

void BinarXEmulator::ToggleYellowLed() {
  gpio_controller_->TogglePin();
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

}  // namespace binarx_emulator