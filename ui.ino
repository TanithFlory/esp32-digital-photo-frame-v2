/**************************CrowPanel ESP32 HMI Display Example Code************************
Version     :	1.1
Suitable for:	CrowPanel ESP32 HMI Display
Product link:	https://www.elecrow.com/esp32-display-series-hmi-touch-screen.html
Code	  link:	https://github.com/Elecrow-RD/CrowPanel-ESP32-Display-Course-File
Lesson	link:	https://www.youtube.com/watch?v=WHfPH-Kr9XU
Description	:	The code is currently available based on the course on YouTube, 
				        if you have any questions, please refer to the course video: Introduction 
				        to ask questions or feedback.
**************************************************************/


#include <Wire.h>
#include <SPI.h>
#include <SD.h>

/**************************LVGL and UI************************
if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
if not, please do not include it. It will waste your Flash space.
**************************************************************/
#include <lvgl.h>
#include "ui.h"
#include "ui_events.c"
// #include <demos/lv_demos.h>
// #include <examples/lv_examples.h>
/**************************LVGL and UI END************************/

/*******************************************************************************
   Config the display panel and touch panel in gfx_conf.h
 ******************************************************************************/
#include "gfx_conf.h"

#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12
#define SD_CS 10

SPIClass SD_SPI;

char currentGroup = '\0';
bool isLooping = false;
int photoCount = 1;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf1[screenWidth * screenHeight / 10];
static lv_color_t disp_draw_buf2[screenWidth * screenHeight / 10];
static lv_disp_drv_t disp_drv;


/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.pushImageDMA(area->x1, area->y1, w, h, (lgfx::rgb565_t *)&color_p->full);

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  uint16_t touchX, touchY;
  bool touched = tft.getTouch(&touchX, &touchY);
  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;

    Serial.print("Data x ");
    Serial.println(touchX);

    Serial.print("Data y ");
    Serial.println(touchY);
  }
}

int sd_init() {
  //SD_SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
  SD_SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, SD_SPI, 80000000)) {
    Serial.println("Card Mount Failed");
    return 1;
  } else {
    Serial.println("Card Mount Successed");
  }
  return 0;
}

void setup() {
  Serial.begin(9600);
  Serial.println("LVGL Widgets Demo");

  //Display Prepare
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  delay(200);

  lv_init();

  delay(100);

  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2, screenWidth * screenHeight / 10);
  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.full_refresh = 1;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  tft.fillScreen(TFT_BLACK);

  if (sd_init() == 0) {
    Serial.println("TF init success");
  } else {
    Serial.println("TF init fail");
  }

  //please do not use LVGL Demo and UI export from Squareline Studio in the same time.
  // lv_demo_widgets();    // LVGL demo
  ui_init();

  Serial.println("Setup done");
}

void displayPhoto(char fileGroup, int16_t x, int16_t y) {
  String filePath = "/" + String(fileGroup + "-" + String(photoCount) + ".bmp");
  photoCount++;
  File file = SD.open(filePath);

  if (!file) {
    Serial.println("Error opening file!");
    isLooping = false;
    return;
  }

  read32(file);
  read32(file);
  uint32_t offset = read32(file);

  read32(file);
  uint32_t width = read32(file);
  uint32_t height = read32(file);

  if (read16(file) != 1) {
    Serial.println("File is not a 24-bit bitmap!");
    isLooping = false;
    return;
  }

  file.seek(offset);

  uint16_t rowSize = (width * 3 + 3) & ~3;
  uint8_t row[rowSize];

  for (uint16_t rowNumber = height; rowNumber > 0; rowNumber--) {
    file.read(row, rowSize);

    for (uint16_t colNumber = 0; colNumber < width; colNumber++) {
      uint8_t b = row[3 * colNumber + 0];
      uint8_t g = row[3 * colNumber + 1];
      uint8_t r = row[3 * colNumber + 2];

      tft.drawPixel(colNumber + x, rowNumber - 1 + y, tft.color565(r, g, b));
    }
  }

  redrawHomeButton();
}

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read();
  return result;
}

void redrawHomeButton(){
  ui_home1 = lv_btn_create(lv_scr_act());
    lv_obj_set_width(ui_home1, 50);
    lv_obj_set_height(ui_home1, 50);
    lv_obj_set_x(ui_home1, -361);
    lv_obj_set_y(ui_home1, -204);
    lv_obj_set_align(ui_home1, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_img_src(ui_home1, &ui_img_home2_png, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void loop() {
  lv_timer_handler();
  delay(5);

  if (photoCount > 10) {
    photoCount = 1;
  }
  
  if (isLooping) {
    displayPhoto(currentGroup, 0, 0);
  }
}