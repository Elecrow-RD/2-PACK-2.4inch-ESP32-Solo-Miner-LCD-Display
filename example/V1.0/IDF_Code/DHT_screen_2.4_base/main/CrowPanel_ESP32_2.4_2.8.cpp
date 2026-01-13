#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <DHT20.h>
#include "ui.h"  // UI interface creation function

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

uint16_t calData[5] = {189, 3416, 359, 3439, 1};

TFT_eSPI lcd = TFT_eSPI(); 
DHT20 dht20;

// Screen buffer (suggested to be 1/10 to 1/4 of the screen size)
#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10)

lv_color_t draw_buf[DRAW_BUF_SIZE];

// LVGL display and input devices
static lv_display_t *disp;
static lv_indev_t *indev;

// Real-time temperature and humidity label objects
lv_obj_t *temp_label;
lv_obj_t *humi_label;

// Sensor update
void update_sensor_values() {
  int temperature = dht20.getTemperature();
  int humidity = dht20.getHumidity();

  // If you have corresponding labels in ui.h, you can uncomment the following lines to update the UI:
  lv_label_set_text_fmt(temp_label, "%d", temperature);
  lv_label_set_text_fmt(humi_label, "%d", humidity);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}

// Touch reading
uint16_t touchX, touchY;
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
  if (lcd.getTouch(&touchX, &touchY)) {
    data->state = LV_INDEV_STATE_PRESSED;
    // data->point.x = SCREEN_WIDTH - touchX;  // 2.4 inch screen use this line instead
    data->point.x = touchX;              // 2.8 inch screen use this line instead
    data->point.y = touchY;
    Serial.printf("Touch X:%d  Y:%d\n", data->point.x, data->point.y);
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

// Refresh function
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  lcd.pushImage(area->x1, area->y1, w, h, (uint16_t *)px_map);
  lv_display_flush_ready(disp);
}

// Get tick time
uint32_t my_tick() {
  return millis();
}

void setup() {
  Serial.begin(115200);

  pinMode(4, INPUT);

  // IO pin setup
  pinMode(25, OUTPUT);
  digitalWrite(25, LOW);

  // Initialize I2C
  Wire.setPins(22, 21);
  Wire.begin();
  dht20.begin();

  // Initialize LCD
  lcd.begin();
  lcd.setRotation(1);        // Adjust screen orientation as needed
  lcd.setSwapBytes(true);    // Swap RGB and BGR data
  lcd.fillScreen(TFT_BLACK);
  delay(300);

  // Backlight control
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);

  SPI.begin(14, 4 , 13, 33);
  lcd.setTouch(calData);

  // Initialize LVGL
  lv_init();
  lv_tick_set_cb(my_tick);

  // Create display driver
  disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

  // Register touch input device
  indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);

  // Initialize UI
  ui_init();

  // Create temperature label
  temp_label = lv_label_create(lv_screen_active());
  lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_14, 0);  // Can be replaced with a larger font
  lv_obj_align(temp_label, LV_ALIGN_CENTER, -20, -50);  // Centered and offset upward
  lv_label_set_text(temp_label, "--");

  // Create humidity label
  humi_label = lv_label_create(lv_screen_active());
  lv_obj_set_style_text_font(humi_label, &lv_font_montserrat_14, 0);
  lv_obj_align(humi_label, LV_ALIGN_CENTER, -20, 25);   // Centered and offset downward
  lv_label_set_text(humi_label, "--");

}

void loop() {
  static unsigned long last_update = 0;

  if (millis() - last_update > 2000) {
    update_sensor_values();
    last_update = millis();
  }

  lv_timer_handler();  // Handle LVGL tasks
  delay(5);
}

// Entry point for ESP-IDF
extern "C" void app_main() {
  setup();
  while (true) {
    loop();
  }
}
 