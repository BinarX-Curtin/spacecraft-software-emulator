/**
 * @file gpi_interface.h
 * @author Stuart Buchan, Binar Space Program
 * @brief This file contains the declaration for the GPI interface
 * objects.
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

namespace bsf::hal::gpio {

/**
 * @brief Abstract base class for the general purpose input object.
 *
 */
class GpiInterface {
 public:
  /**
   * @brief Get the state of the input pin.
   *
   * @return true The pin is at logic level high.
   * @return false The pin is at logic level low.
   */
  virtual bool IsHigh() const = 0;
};

}  // namespace bsf::hal::gpio