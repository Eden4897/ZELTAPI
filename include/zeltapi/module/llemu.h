#ifndef _ZELTAPI_LLEMU_H_
#define _ZELTAPI_LLEMU_H_
#include "main.h"
#include <iostream>
#include <string>

extern lv_obj_t * lines[8];

namespace llemu{
  lv_obj_t * createBtn(lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, int id, const char * lableText, lv_action_t actionFunction);
  lv_style_t * createBtnStyle(lv_obj_t * btn, lv_color_t relColor, lv_color_t prColor,lv_color_t tglRelColor, lv_color_t tglPrColor, lv_color_t tglBorderColor,lv_color_t textColor, lv_coord_t radius);
  lv_obj_t * createLabel(lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, const char * lableText);
  lv_obj_t * editLabel(lv_obj_t * label,const char * lableText);
  lv_obj_t * createImage(lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, int id,const void *src_img);
  void init();
}
#endif
