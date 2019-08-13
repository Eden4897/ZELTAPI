/**
 * \file zeltapi/module/vision.hpp
 *
 * vision header
 *
 * Copyright (c) 2019-2020, by Eden Cheung.
 * All rights reserved
 */
#ifndef _ZELTAPI_VISION_H_
#define _ZELTAPI_VISION_H_
#include "main.h"
#include "core.hpp"
#include "lcd.hpp"
namespace vision{
  int getWidthBySig(std::uint32_t size_id,std::uint8_t sig_id);
  int getHeightBySig(std::uint32_t size_id,std::uint8_t sig_id);
  int getXBySig(std::uint32_t size_id,std::uint8_t sig_id);
  int getYBySig(std::uint32_t size_id,std::uint8_t sig_id);
  int getWidthByCode(uint32_t size_id,pros::vision_color_code_t color_code);
  int getHeightByCode(std::uint32_t size_id,pros::vision_color_code_t color_code);
  int getAngleByCode(std::uint32_t size_id,pros::vision_color_code_t color_code);
  int getXByCode(std::uint32_t size_id,pros::vision_color_code_t color_code);
  int getYByCode(std::uint32_t size_id,pros::vision_color_code_t color_code);
}
#define stack1 vision_sensor.create_color_code(1, 2, 3);
#define SIG_1 1
#define SIG_2 2
#define SIG_3 3
#endif
