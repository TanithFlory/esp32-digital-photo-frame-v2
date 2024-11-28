// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: new_proj

#ifndef _NEW_PROJ_UI_H
#define _NEW_PROJ_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
extern lv_obj_t * ui_Screen1;
void ui_event_Button1(lv_event_t * e);
extern lv_obj_t * ui_Button1;
extern lv_obj_t * ui_Panel1;
void ui_event_kedarnath(lv_event_t * e);
extern lv_obj_t * ui_kedarnath;
void ui_event_family(lv_event_t * e);
extern lv_obj_t * ui_family;
void ui_event_fourPeople(lv_event_t * e);
extern lv_obj_t * ui_fourPeople;
void ui_event_rishikesh(lv_event_t * e);
extern lv_obj_t * ui_rishikesh;
void ui_event_home(lv_event_t * e);
extern lv_obj_t * ui_home;
void ui_event_otherFriends(lv_event_t * e);
extern lv_obj_t * ui_otherFriends;
// CUSTOM VARIABLES

// EVENTS

extern lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS
LV_IMG_DECLARE(ui_img_background_png);    // assets/background.png
LV_IMG_DECLARE(ui_img_685164422);    // assets/home-icon.png
LV_IMG_DECLARE(ui_img_kedarnath_png);    // assets/kedarnath.png
LV_IMG_DECLARE(ui_img_family_png);    // assets/family.png
LV_IMG_DECLARE(ui_img_fourpeople_png);    // assets/fourPeople.png
LV_IMG_DECLARE(ui_img_rishikesh_png);    // assets/rishikesh.png
LV_IMG_DECLARE(ui_img_home_png);    // assets/home.png
LV_IMG_DECLARE(ui_img_otherfriends_png);    // assets/otherFriends.png

// UI INIT
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
