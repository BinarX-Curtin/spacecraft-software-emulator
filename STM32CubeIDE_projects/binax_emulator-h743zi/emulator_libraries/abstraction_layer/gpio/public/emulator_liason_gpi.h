/**
 * @file emulator_liason_gpo.h
 * @author Tristan Ward, Binar Space Program
 * @brief This file contains the definition for the enumerated type
 * 'GpoPin', which contains an exhaustive list of all the output pins for the
 * emulator_liason library.
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "abstraction_layer/gpio/public/internal/gpi.h"

namespace bsf::hal::gpio {

/**
 * @brief Enum class representing the possible pinout options for general
 * purpose output.
 *
 */
enum class GpiPin {
  kPayloadChipSelect, /**< Payload ready pin to tell the emulator*/
};

}  // namespace bsf::hal::gpio