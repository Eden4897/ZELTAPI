/**
 * \file module/llemu.cpp
 *
 * all llemu commands and functions
 *
 * Copyright (c) 2019-2020, by Eden Cheung.
 * All rights reserved
 */
#include "_header.h"

bool isBlueSide = false;
int autoSelect = 1;
int noOfAutos = 5;
lv_obj_t * lines[8];


typedef  FILE * pc_file_t;


lv_res_t autoMod(lv_obj_t * btn){
  uint8_t id = lv_obj_get_free_num(btn);
  switch (id)
  {
    case 1:
      if (autoSelect >= noOfAutos)
      {
        autoSelect = 1;
      }else
      {
        autoSelect++;
      }
    break;

    case 2:
      isBlueSide = !isBlueSide;
    break;

    case 3:
    if (autoSelect <= 1)
    {
      autoSelect = noOfAutos;
    }else
    {
      autoSelect--;
    }
    break;
  }
  return LV_RES_OK;
}


static lv_fs_res_t pcfs_open( void * file_p, const char * fn, lv_fs_mode_t mode)
{
    errno = 0;
    const char * flags = "";
    if(mode == LV_FS_MODE_WR) flags = "wb";
    else if(mode == LV_FS_MODE_RD) flags = "rb";
    else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) flags = "a+";

    char buf[256];
    sprintf(buf, "/%s", fn);
    pc_file_t f = fopen(buf, flags);

    if(f == NULL)
      return LV_FS_RES_UNKNOWN;
    else {
      fseek(f, 0, SEEK_SET);
      pc_file_t * fp = (pc_file_t *)file_p;
      *fp = f;
    }

    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_close( void * file_p)
{
    pc_file_t * fp = (pc_file_t *)file_p;
    fclose(*fp);
    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_read( void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    pc_file_t * fp =  (pc_file_t *)file_p;
    *br = fread(buf, 1, btr, *fp);
    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_seek( void * file_p, uint32_t pos)
{
    pc_file_t * fp = (pc_file_t *)file_p;
    fseek(*fp, pos, SEEK_SET);
    return LV_FS_RES_OK;
}

static lv_fs_res_t pcfs_tell( void * file_p, uint32_t * pos_p)
{
    pc_file_t * fp =  (pc_file_t *)file_p;
    *pos_p = ftell(*fp);
    return LV_FS_RES_OK;
}
namespace llemu{

  lv_obj_t * createBtn(lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, int id, const char * lableText, lv_action_t actionFunction){

    lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, width, height);
    lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, actionFunction);
    lv_obj_set_free_num(btn, id);

    lv_obj_t * label = lv_label_create(btn, NULL);
    lv_label_set_text(label, lableText);
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

    return btn;
  }



  lv_style_t * createBtnStyle(lv_obj_t * btn, lv_color_t relMColor, lv_color_t relGColor, lv_color_t prMColor, lv_color_t prGColor,lv_color_t textColor, lv_coord_t radius){

    lv_style_t * btnStyle = (lv_style_t *)malloc(sizeof(lv_style_t) * 2);

    for(int i = 0; i < 2; i++){
      lv_style_copy(&btnStyle[i], &lv_style_plain);
    }

    btnStyle[0].body.main_color = relMColor;
    btnStyle[0].body.grad_color = relGColor;
    btnStyle[0].body.radius = radius;
    btnStyle[0].text.color = textColor;

    btnStyle[1].body.main_color = prMColor;
    btnStyle[1].body.grad_color = prGColor;
    btnStyle[1].body.radius = radius;
    btnStyle[1].text.color = textColor;

    lv_btn_set_style(btn, LV_BTN_STYLE_REL, &btnStyle[0]);
    lv_btn_set_style(btn, LV_BTN_STYLE_PR, &btnStyle[1]);

    return btnStyle;
  }


  lv_obj_t * createLabel(lv_coord_t x, lv_coord_t y, int id,const char * lableText){
    lv_obj_t * label = lv_label_create(lv_scr_act(), NULL);
    lv_style_t * labelStyle = &lv_style_plain;
    labelStyle->text.color = LV_COLOR_BLACK;
    lv_label_set_style(label, labelStyle);
    lv_label_set_text(label, lableText);
    lv_obj_set_pos(label, x, y);
    lv_obj_set_free_num(label, id);
    return label;
  }
  lv_obj_t * editLabel(lv_obj_t * label,const char * lableText){
    lv_label_set_text(label, lableText);
    return label;
  }

  lv_obj_t * createImage(lv_coord_t x, lv_coord_t y,const void *src_img){
    lv_obj_t * image = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(image,src_img);
    lv_obj_set_pos(image, x, y);
    return image;
  }
  void init(){
    /********************
     *      DRIVER
     ********************/
    lv_fs_drv_t pcfs_drv;
    memset(&pcfs_drv, 0, sizeof(lv_fs_drv_t));

    pcfs_drv.file_size = sizeof(pc_file_t);
    pcfs_drv.letter = 'S';
    pcfs_drv.open = pcfs_open;
    pcfs_drv.close = pcfs_close;
    pcfs_drv.read = pcfs_read;
    pcfs_drv.seek = pcfs_seek;
    pcfs_drv.tell = pcfs_tell;
    lv_fs_add_drv(&pcfs_drv);

    lv_obj_t * canvas = llemu::createBtn(0,0,477,239,4,"", NULL);
    llemu::createBtnStyle(canvas, LV_COLOR_MAKE(255, 87, 87),LV_COLOR_MAKE(212, 0, 0),LV_COLOR_MAKE(255, 87, 87),LV_COLOR_MAKE(212, 0, 0), LV_COLOR_BLACK,0);

    lv_obj_t * canvas2 = llemu::createBtn(410,0,87,239,4,"", NULL);
    llemu::createBtnStyle(canvas2, LV_COLOR_WHITE, LV_COLOR_WHITE, LV_COLOR_WHITE, LV_COLOR_WHITE, LV_COLOR_BLACK,0);

    lv_obj_t * autoAdd = llemu::createBtn(420, 20, 50, 50, 1, "", autoMod);
    llemu::createBtnStyle(autoAdd, LV_COLOR_MAKE(255, 255, 255), LV_COLOR_MAKE(50, 50, 50),
    LV_COLOR_MAKE(200, 200, 200) , LV_COLOR_MAKE(20, 20, 20), LV_COLOR_MAKE(0, 0, 0), 50);

    lv_obj_t * autoMid = llemu::createBtn(420, 80, 50, 50, 2, "", autoMod);
    llemu::createBtnStyle(autoMid, LV_COLOR_MAKE(255, 255, 255) , LV_COLOR_MAKE(50, 50, 50),
    LV_COLOR_MAKE(200, 200, 200) , LV_COLOR_MAKE(20, 20, 20), LV_COLOR_MAKE(0, 0, 0), 50);

    lv_obj_t * autoSub = llemu::createBtn(420, 140, 50, 50, 3, "", autoMod);
    llemu::createBtnStyle(autoSub, LV_COLOR_MAKE(255, 255, 255) , LV_COLOR_MAKE(50, 50, 50),
    LV_COLOR_MAKE(200, 200, 200) , LV_COLOR_MAKE(20, 20, 20), LV_COLOR_MAKE(0, 0, 0), 50);

    createImage(0,0,"S:usd/hk.bin");

    lines[0] = llemu::createLabel(20, 10,  0, "");
    lines[1] = llemu::createLabel(20, 35,  0, "");
    lines[2] = llemu::createLabel(20, 60,  0, "");
    lines[3] = llemu::createLabel(20, 85,  0, "");
    lines[4] = llemu::createLabel(20, 110, 0, "");
    lines[5] = llemu::createLabel(20, 135, 0, "");
    lines[6] = llemu::createLabel(20, 160, 0, "");
    lines[7] = llemu::createLabel(20, 185, 0, "");
  }
}
