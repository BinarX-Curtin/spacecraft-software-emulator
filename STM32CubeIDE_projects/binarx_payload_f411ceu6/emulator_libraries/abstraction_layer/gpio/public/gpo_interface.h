/**
 * @file gpo_interface.h
 * @author Stuart Buchan, Binar Space Program
 * @brief This file contains the declaration for the GPO interface
 * object.
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

namespace bsf::hal::gpio {

/**
 * @brief Abstract base class for the general purpose output object.
 *
 */
class GpoInterface {
 public:
  /**
   * @brief Set the output pin high.
   */
  virtual void SetHigh() = 0;

  /**
   * @brief Set the output pin low.
   */
  virtual void SetLow() = 0;

  /**
   * @brief Toggle the output pin.
   */
  virtual void Toggle() = 0;
};

}  // namespace bsf::hal::gpio