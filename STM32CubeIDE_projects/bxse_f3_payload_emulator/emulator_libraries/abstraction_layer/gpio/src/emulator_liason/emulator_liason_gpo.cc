/**
 * @file emulator_liason_gpo.cc
 * @author Tristan Ward, Binar Space Program
 * @brief This file contains the definitions for the bcm_cm4 implementation of
 * the general purpose output class.
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "abstraction_layer/gpio/public/emulator_liason_gpo.h"

#include "main.h"

namespace bsf::hal::gpio {
template <>
void Gpo<GpoPin::kPayloadReady>::SetHigh() {
  HAL_GPIO_WritePin(Data_Ready_GPIO_Port, Data_Ready_Pin, GPIO_PIN_SET);
}

template <>
void Gpo<GpoPin::kPayloadReady>::SetLow() {
  HAL_GPIO_WritePin(Data_Ready_GPIO_Port, Data_Ready_Pin, GPIO_PIN_RESET);
}

template <>
void Gpo<GpoPin::kPayloadReady>::Toggle() {
  HAL_GPIO_TogglePin(Data_Ready_GPIO_Port, Data_Ready_Pin);
}

}  // namespace bsf::hal::gpio