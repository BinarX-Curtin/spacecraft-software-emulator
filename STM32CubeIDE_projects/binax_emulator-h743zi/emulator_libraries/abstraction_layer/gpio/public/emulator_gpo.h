/**
 * @file bcm_cm4_gpo.h
 * @author Stuart Buchan, Binar Space Program
 * @brief This file contains the definition for the enumerated type
 * 'GpoPin', which contains an exhaustive list of all the output pins on the CM4
 * core of the satellite flight computer.
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "abstraction_layer/gpio/public/internal/gpo.h"

namespace bsf::hal::gpio {

/**
 * @brief Enum class representing the possible pinout options for general
 * purpose output.
 *
 * TODO(stuartbuchan): Refactor these to use kConstStyleNaming.
 *
 */
enum class GpoPin {
  kRedLed,       /**< Red Led */
  kYellowLed,    /**< Yellow Led */
  kGreenLed,     /**< Green Led */
  kPayloadSwitch /**< Turn the payload on and off */
};

}  // namespace bsf::hal::gpio