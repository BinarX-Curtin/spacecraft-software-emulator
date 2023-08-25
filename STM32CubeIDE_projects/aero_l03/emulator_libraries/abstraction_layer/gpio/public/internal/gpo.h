/**
 * @file gpo.h
 * @author Stuart Buchan, Binar Space Program
 * @brief This file contains the declaration for the implementation of
 * the general purpose output class.
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "abstraction_layer/gpio/public/gpo_interface.h"

namespace bsf::hal::gpio {

/**
 * @brief Enum class representing the possible output pin options for general
 * purpose output.
 *
 */
enum class GpoPin;

/**
 * @brief Object for controlling general purpose output pins.
 *
 * @tparam GpoPin An enumerated type representing an output pin.
 */
template <GpoPin>
class Gpo : public GpoInterface {
 public:
  /**
   * @brief Set the output pin high.
   */
  void SetHigh() override;

  /**
   * @brief Set the output pin low.
   */
  void SetLow() override;

  /**
   * @brief Toggle the output pin.
   */
  void Toggle() override;
};

}  // namespace bsf::hal::gpio