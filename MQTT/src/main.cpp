#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "";
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_user = "";
const char* mqtt_pass = "";

const char* device_id = "ESP32_Pro_Unit_01";

const char* topic_telemetry = "esp32/unit01/data";   
const char* topic_command   = "esp32/unit01/cmd";    
const char* topic_status    = "esp32/unit01/status";

// GLOBAL NESNE VE DEĞİŞKENLER
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsgTime = 0;
const long interval = 5000;

#define LED_PIN_BLUE 2
#define LED_PIN_GREEN 5
#define LED_PIN_RED 4


void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("WiFi'ye bağlanılıyor...");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi bağlandı!");
  Serial.print("IP Adres: ");
  Serial.println(WiFi.localIP());
}



int LastLight = -1;

void callback(char* topic, byte* payload, unsigned int lenght){
  Serial.print("Mesaj gönderildi [");
  Serial.print(topic);
  Serial.print("] ");

  String message;
  for(int i = 0; i < lenght; i++){
    message += (char)payload[i];
  } 
  Serial.println(message);

  

  if(String(topic) == topic_command){
    if(message == "RED"){
      if(LastLight != -1) digitalWrite(LastLight, LOW);
      digitalWrite(LED_PIN_RED, HIGH);
      client.publish(topic_telemetry, "{\"led\":\"ON\",\"color\":\"RED lighting...\"}");
      LastLight = LED_PIN_RED;
    }
    else if (message == "BLUE") {
      if(LastLight !=  -1) digitalWrite(LastLight, LOW);
      digitalWrite(LED_PIN_BLUE, HIGH);
      client.publish(topic_telemetry, "{\"led\":\"ON\",\"color\":\"BLUE lighting...\"}");
      LastLight = LED_PIN_BLUE;
    }
    else if (message == "GREEN") {
      if(LastLight !=  -1) digitalWrite(LastLight, LOW);
      digitalWrite(LED_PIN_GREEN, HIGH);
      client.publish(topic_telemetry, "{\"led\":\"ON\",\"color\":\"GREEN lighting...\"}");
      LastLight = LED_PIN_GREEN;
    }
    else if (message == "OFF") {
      if(LastLight != -1) digitalWrite(LastLight, LOW);
      client.publish(topic_telemetry, "{\"led\": \"OFF\"}");
      LastLight = -1;   
    }
  }
}


void reconnect(){
  while(!client.connected()){
    Serial.print("MQTT bağlantısı deneniyor...");

    if(client.connect(device_id, mqtt_user, mqtt_pass, topic_status,1,true,"offline")){
      Serial.println("Bağlandı");

      client.publish(topic_status, "online", true);

      client.subscribe(topic_command);
    }else{
      Serial.print("hata, rc=");
      Serial.print(client.state());
      Serial.println("5 saniye içerisinde yeniden denenecek");
      delay(5000);
    }
  }
}


void setup(){
  Serial.begin(115200);
  pinMode(LED_PIN_BLUE, OUTPUT);
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);



  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop(){
  if(!client.connected()){
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if(now - lastMsgTime > interval){
    lastMsgTime = now;

    JsonDocument doc;
    doc["device"] = device_id;
    doc["uptime"] = millis() / 1000;
    doc["wifi_rssi"] = WiFi.RSSI();

    doc["temp"] = random(20,30);

    char buffer[256];
    serializeJson(doc, buffer);

    Serial.print("Yayınlanan veri: ");
    Serial.println(buffer);
    client.publish(topic_telemetry, buffer);
  }
}



