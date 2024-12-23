#include <lvgl.h>

//if you want to add the UI file to this project, please remember include ui.h
#include "ui.h"

//if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. if not, please do not include it. It will waste your Flash space.
//#include <demos/lv_demos.h>
//#include <examples/lv_examples.h>

#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12
#define SD_CS 10


/*******************************************************************************
 ******************************************************************************/
#define TFT_BL 2
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
class LGFX : public lgfx::LGFX_Device {
public:

  lgfx::Bus_RGB _bus_instance;
  lgfx::Panel_RGB _panel_instance;

  LGFX(void) {


    {
      auto cfg = _bus_instance.config();
      cfg.panel = &_panel_instance;

      //Please modify the pins according to the circuit diagram. The pins are not the same for the 4.3-inch, 5-inch, and 7-inch screens.
      cfg.pin_d0 = GPIO_NUM_15;  // B0
      cfg.pin_d1 = GPIO_NUM_7;   // B1
      cfg.pin_d2 = GPIO_NUM_6;   // B2
      cfg.pin_d3 = GPIO_NUM_5;   // B3
      cfg.pin_d4 = GPIO_NUM_4;   // B4

      cfg.pin_d5 = GPIO_NUM_9;   // G0
      cfg.pin_d6 = GPIO_NUM_46;  // G1
      cfg.pin_d7 = GPIO_NUM_3;   // G2
      cfg.pin_d8 = GPIO_NUM_8;   // G3
      cfg.pin_d9 = GPIO_NUM_16;  // G4
      cfg.pin_d10 = GPIO_NUM_1;  // G5

      cfg.pin_d11 = GPIO_NUM_14;  // R0
      cfg.pin_d12 = GPIO_NUM_21;  // R1
      cfg.pin_d13 = GPIO_NUM_47;  // R2
      cfg.pin_d14 = GPIO_NUM_48;  // R3
      cfg.pin_d15 = GPIO_NUM_45;  // R4

      cfg.pin_henable = GPIO_NUM_41;
      cfg.pin_vsync = GPIO_NUM_40;
      cfg.pin_hsync = GPIO_NUM_39;
      cfg.pin_pclk = GPIO_NUM_0;
      //------------Pin Config END--------------------

      cfg.freq_write = 16000000;

      cfg.hsync_polarity = 0;
      cfg.hsync_front_porch = 40;
      cfg.hsync_pulse_width = 48;
      cfg.hsync_back_porch = 40;

      cfg.vsync_polarity = 0;
      cfg.vsync_front_porch = 1;
      cfg.vsync_pulse_width = 31;
      cfg.vsync_back_porch = 13;

      cfg.pclk_active_neg = 1;
      cfg.de_idle_high = 0;
      cfg.pclk_idle_high = 0;

      _bus_instance.config(cfg);
    }
    {
      auto cfg = _panel_instance.config();
      //5inch and 7inch: 800*480
      cfg.memory_width = 800;
      cfg.memory_height = 480;
      cfg.panel_width = 800;
      cfg.panel_height = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      _panel_instance.config(cfg);
    }
    _panel_instance.setBus(&_bus_instance);
    setPanel(&_panel_instance);
  }
};


LGFX lcd;
/*******************************************************************************
   End of Arduino_GFX setting
 ******************************************************************************/


/*******************************************************************************
   Please config the touch panel in touch.h
 ******************************************************************************/
#include "touch.h"

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;

char currentSet = '\0';

SPIClass SD_SPI;

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

//5inch and 7inch: 800*480
static lv_color_t disp_draw_buf1[800 * 480 / 8];
static lv_color_t disp_draw_buf2[800 * 480 / 8];

//static lv_color_t disp_draw_buf;
static lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  //lcd.startWrite();
  //lcd.setAddrWindow( area->x1, area->y1, w, h );
  //lcd.pushColors( ( uint16_t * )&color_p->full, w * h, true );
  //lcd.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  //lcd.endWrite();

  lcd.pushImageDMA(area->x1, area->y1, w, h, (lgfx::rgb565_t *)&color_p->full);

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  if (touch_has_signal()) {
    if (touch_touched()) {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
      Serial.print("Data x :");
      Serial.println(touch_last_x);

      Serial.print("Data y :");
      Serial.println(touch_last_y);
    } else if (touch_released()) {
      data->state = LV_INDEV_STATE_REL;
    }
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
  delay(15);
}


void setup() {
  Serial.begin(9600);
  Serial.println("LVGL Widgets Demo");

  //IO口引脚
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);
  pinMode(42, OUTPUT);
  digitalWrite(42, LOW);


  // Init Display
  lcd.begin();
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextSize(2);
  delay(200);

  //  lcd.fillScreen(TFT_REDRED);
  //  delay(500);
  //  lcd.fillScreen(TFT_REDGREEN);
  //  delay(500);
  //  lcd.fillScreen(TFT_REDBLUE);
  //  delay(500);
  //  lcd.fillScreen(TFT_BLACK);
  //  delay(500);

  lv_init();

  delay(100);
  touch_init();

  screenWidth = lcd.width();
  screenHeight = lcd.height();

  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2, screenWidth * screenHeight / 8);
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
#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif


  lcd.fillScreen(TFT_BLACK);

  //please do not use LVGL Demo and UI export from Squareline Studio in the same time.
  //lv_demo_widgets();    // LVGL demo
  ui_init();  //UI from Squareline Studio

  if (sd_init() == 0) {
    Serial.println("TF init success");
  } else {
    Serial.println("TF init fail");
  }


  Serial.println("Setup done");
}

void onKedarnathPressed(lv_event_t *e) {
  currentSet = 'a';
  showPictures();
};
void onFamilyPressed(lv_event_t *e) {
  currentSet = 'b';
  showPictures();
};
void onFourPeoplePressed(lv_event_t *e) {
  currentSet = 'c';
  showPictures();
};
void onRishikeshPressed(lv_event_t *e) {
  currentSet = 'd';
  showPictures();
};
void onHomePressed(lv_event_t *e) {
  currentSet = 'e';
  showPictures();
};
void onOtherFriendsPressed(lv_event_t *e) {
  currentSet = 'f';
  showPictures();
};

void onGalleryPressed(lv_event_t *e) {
  char currentSet = 'a';  // Starting set (could be changed dynamically)
  int photoCount = 1;

  _ui_screen_delete(&ui_Screen1);
Serial.println(currentSet);
  // Loop through sets a to f
  while (currentSet <= 'f') {
    // Loop through photoCount 1 to 10
    while (photoCount <= 10) {
      // Construct the filename
      String fileName = "/" + String(currentSet) + "-" + String(photoCount) + ".bmp";

      // Display the image
      lcd.drawBmpFile(SD, fileName.c_str(), 0, 0);

      // Wait before showing the next image
      delay(3000);  // Show each image for 3 seconds

      // Exit the loop if we're on 'f' and photoCount is 10
      if (currentSet == 'f' && photoCount == 10) {
        Serial.println("Reached the 10th image of set f, exiting gallery.");
        break;  // Exit the inner loop and stop the gallery
      }

      // Increment the photoCount to show the next image
      photoCount++;
    }

    // Exit the outer loop if we already broke from the inner loop (i.e., when currentSet == 'f' and photoCount == 10)
    if (currentSet == 'f' && photoCount == 10) {
      break;
    }

    // Otherwise, move to the next set
    currentSet++;
    photoCount = 1;  // Reset photoCount for the next set
  }

  // Reinitialize the UI after the loop is finished
  ui_init();
}

void onHomeOptionPressed(lv_event_t *e) {
  resetUi();
}

void resetUi() {
  _ui_screen_delete(&ui_Screen1);
  ui_init();
}

void loop() {
  lv_timer_handler();
  delay(5);
}

void showPictures() {
  _ui_screen_delete(&ui_Screen1);
  for (int i = 1; i <= 10; i++) {
    String fileName = "/" + String(currentSet) + "-" + String(i) + ".bmp";
    lcd.drawBmpFile(SD, fileName.c_str(), 0, 0);
    delay(2500);  // Delay between pictures
  }
  ui_init();
}