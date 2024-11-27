# ESP32 Photo Display with LVGL

## Setup Instructions

1. **Install ESP32 Board:**
   - Open Arduino IDE and go to `File` > `Preferences`.
   - In the "Additional Boards Manager URLs" field, add:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to `Tools` > `Board` > `Boards Manager`, search for "ESP32", and install the latest version.

2. **Install Libraries:**
   - In the Arduino IDE, go to `Sketch` > `Include Library` > `Manage Libraries`.
   - Install the following libraries:
     - **lovyanGfx**: Latest version (Preferred)
     - **lvgl**: Version 8.3.11
     - **tamc-g6911**: Latest version (Preferred)

3. **Select Your ESP32 Board:**
   - Go to `Tools` > `Board` and select your ESP32 board (e.g., ESP32 Dev Module).
   - Make sure you're using the latest version of the ESP32 board support.

