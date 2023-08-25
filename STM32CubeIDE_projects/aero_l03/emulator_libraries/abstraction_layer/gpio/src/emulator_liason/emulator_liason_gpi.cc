/**
 * @file emulator_liason_gpi.cc
 * @author Tristan Ward, Binar Space Program
 * @brief This file contains the definitions for the bcm_cm4 implementation of
 * the general purpose output class.
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "abstraction_layer/gpio/public/emulator_liason_gpi.h"

#include "main.h"

namespace bsf::hal::gpio {
template <>
bool Gpi<GpiPin::kPayloadChipSelect>::IsHigh() const {
  return HAL_GPIO_ReadPin(Payload_Chip_Select_GPIO_Port,
                          Payload_Chip_Select_Pin);
}

}  // namespace bsf::hal::gpio
