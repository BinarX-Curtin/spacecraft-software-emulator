/**
 * @file bcm_cm4_gpo.cc
 * @author Stuart Buchan, Binar Space Program
 * @brief This file contains the definitions for the bcm_cm4 implementation of
 * the general purpose output class.
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "abstraction_layer/gpio/public/emulator_gpo.h"

#include "main.h"

namespace bsf::hal::gpio {
template <>
void Gpo<GpoPin::kRedLed>::SetHigh() {
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
}

template <>
void Gpo<GpoPin::kRedLed>::SetLow() {
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}

template <>
void Gpo<GpoPin::kRedLed>::Toggle() {
  HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
}

template <>
void Gpo<GpoPin::kYellowLed>::SetHigh() {
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}

template <>
void Gpo<GpoPin::kYellowLed>::SetLow() {
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

template <>
void Gpo<GpoPin::kYellowLed>::Toggle() {
  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

template <>
void Gpo<GpoPin::kGreenLed>::SetHigh() {
  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
}

template <>
void Gpo<GpoPin::kGreenLed>::SetLow() {
  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
}

template <>
void Gpo<GpoPin::kGreenLed>::Toggle() {
  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
}

template <>
void Gpo<GpoPin::kPayloadSwitch>::SetHigh() {
  HAL_GPIO_WritePin(Payload_Switch_GPIO_Port, Payload_Switch_Pin, GPIO_PIN_SET);
}

template <>
void Gpo<GpoPin::kPayloadSwitch>::SetLow() {
  HAL_GPIO_WritePin(Payload_Switch_GPIO_Port, Payload_Switch_Pin,
                    GPIO_PIN_RESET);
}

template <>
void Gpo<GpoPin::kPayloadSwitch>::Toggle() {
  HAL_GPIO_TogglePin(Payload_Switch_GPIO_Port, Payload_Switch_Pin);
}

template <>
void Gpo<GpoPin::kPayloadChipSelect>::SetHigh() {
  HAL_GPIO_WritePin(Payload_Chip_Select_GPIO_Port, Payload_Chip_Select_Pin,
                    GPIO_PIN_SET);
}

template <>
void Gpo<GpoPin::kPayloadChipSelect>::SetLow() {
  HAL_GPIO_WritePin(Payload_Chip_Select_GPIO_Port, Payload_Chip_Select_Pin,
                    GPIO_PIN_RESET);
}

template <>
void Gpo<GpoPin::kPayloadChipSelect>::Toggle() {
  HAL_GPIO_TogglePin(Payload_Chip_Select_GPIO_Port, Payload_Chip_Select_Pin);
}

}  // namespace bsf::hal::gpio