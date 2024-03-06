/**
 * @file gpi.h
 * @author Stuart Buchan, Binar Space Program
 * @brief This file contains the declaration for the implementation of
 * the general purpose input class.
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "abstraction_layer/gpio/public/gpi_interface.h"

namespace bsf::hal::gpio {

/**
 * @brief Enum class representing the possible input pin options for general
 * purpose input.
 *
 */
enum class GpiPin;

/**
 * @brief Object for reading general purpose input pins.
 *
 * @tparam GpiPin An enumerated type representing an input pin.
 */
template <GpiPin>
class Gpi : public GpiInterface {
 public:
  /**
   * @brief Query if the state of the GPI pin is high.
   */
  bool IsHigh() const override;
};

}  // namespace bsf::hal::gpio