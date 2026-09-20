#include <WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "";
const char* password = "";

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("WiFi'ye bağlanılıyor");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Bağlandı! IP: ");
  Serial.println(WiFi.localIP());

  ArduinoOTA.setHostname("esp32-ota-test");
  ArduinoOTA.setPassword("guclu_bir_sifre123");

  ArduinoOTA.onStart([]() {
    Serial.println("OTA başladı");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA tamamlandı");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("İlerleme: %u%%\r", (progress * 100) / total);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA hatası [%u]\n", error);
  });

  ArduinoOTA.begin();
  Serial.println("OTA hazır, komut bekleniyor...");
}

void loop() {
  ArduinoOTA.handle();

  static unsigned long last = 0;
  if (millis() - last > 2000) {
    last = millis();
    Serial.println("Çalışıyor... sürüm 4");   // testte bunu değiştireceğiz
  }
}