#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);   // I2C adresi, sütun sayısı, satır sayısı

SemaphoreHandle_t lcdMutex;   // LCD'yi aynı anda iki görevin kullanmasını engelleyen kilit

// Görev 1: üst satırda (satır 0) yazıyı kaydırır
void TaskScrollText(void *parameter) {
  String message = "Merhaba FreeRTOS ";
  int len = message.length();

  while (true) {
    for (int i = 0; i < len; i++) {
      String rotated = message.substring(i) + message.substring(0, i);
      String visible = rotated.substring(0, 16);

      if (xSemaphoreTake(lcdMutex, portMAX_DELAY) == pdTRUE) {
        lcd.setCursor(0, 0);
        lcd.print(visible);
        xSemaphoreGive(lcdMutex);
      }
      vTaskDelay(300 / portTICK_PERIOD_MS);
    }
  }
}

// Görev 2: alt satırda (satır 1) yazı yanıp söner
void TaskBlinkText(void *parameter) {
  bool visible = true;
  while (true) {
    if (xSemaphoreTake(lcdMutex, portMAX_DELAY) == pdTRUE) {
      lcd.setCursor(0, 1);
      lcd.print(visible ? "Durum: AKTIF    " : "                ");
      xSemaphoreGive(lcdMutex);
    }
    visible = !visible;
    vTaskDelay(600 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);   // SDA, SCL

  lcd.init();
  lcd.backlight();

  lcdMutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(TaskScrollText, "ScrollText", 2000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskBlinkText, "BlinkText", 2000, NULL, 1, NULL, 0);
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}