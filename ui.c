// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
lv_obj_t * ui_Screen1;
lv_obj_t * ui_Panel1;
void ui_event_kedarnath(lv_event_t * e);
lv_obj_t * ui_kedarnath;
void ui_event_rishikesh(lv_event_t * e);
lv_obj_t * ui_rishikesh;
void ui_event_home(lv_event_t * e);
lv_obj_t * ui_home;
void ui_event_fourPeople(lv_event_t * e);
lv_obj_t * ui_fourPeople;
void ui_event_family(lv_event_t * e);
lv_obj_t * ui_family;
void ui_event_otherFriends(lv_event_t * e);
lv_obj_t * ui_otherFriends;
lv_obj_t * ui_home1;
// CUSTOM VARIABLES
lv_obj_t * uic_otherFriends;

// EVENTS
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS
const lv_img_dsc_t * ui_imgset_[6] = {&ui_img_1_png, &ui_img_2_png, &ui_img_3_png, &ui_img_4_png, &ui_img_5_png, &ui_img_6_png};
const lv_img_dsc_t * ui_imgset_home[1] = {&ui_img_home2_png};

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_kedarnath(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        onKedarnathPressed(e);
    }
}

void ui_event_rishikesh(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        onRishikeshPressed(e);
    }
}

void ui_event_home(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        onHomePressed(e);
    }
}

void ui_event_fourPeople(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        onFourPeoplePressed(e);
    }
}

void ui_event_family(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        onFamilyPressed(e);
    }
}

void ui_event_otherFriends(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        onOtherFriendsPressed(e);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Screen1);
}