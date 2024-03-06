/**
 * @file time_impl.h
 * @author Tristan Ward
 * @brief time implementation of the time interface within the stm32CubeIDE
 * project
 *
 */

#include "abstraction_layer/inc/time_impl.h"

#include "main.h"

namespace binarx_time_impl {

void TimeImpl::Delay(uint32_t milliseconds) { HAL_Delay(milliseconds); };

uint32_t TimeImpl::GetTicks() { return HAL_GetTick(); };

}  // namespace binarx_time_impl
