/**
 * @file emulator_liason_gpo.cc
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
void Gpo<GpiPin::kPayloadChipSelect>::SetHigh() {
  HAL_GPIO_WritePin(Payload_Chip_Select_GPIO_Port, Payload_Chip_Select_Pin,
                    GPIO_PIN_SET);
}

template <>
void Gpo<GpiPin::kPayloadChipSelect>::SetLow() {
  HAL_GPIO_WritePin(Payload_Chip_Select_GPIO_Port, Payload_Chip_Select_Pin,
                    GPIO_PIN_RESET);
}

template <>
void Gpo<GpiPin::kPayloadChipSelect>::Toggle() {
  HAL_GPIO_TogglePin(Payload_Chip_Select_GPIO_Port, Payload_Chip_Select_Pin);
}

}  // namespace bsf::hal::gpio