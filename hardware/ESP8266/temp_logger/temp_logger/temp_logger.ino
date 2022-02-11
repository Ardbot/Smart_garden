#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <Adafruit_Sensor.h>

const char* ssid = "Xiaomi_2CC8";   // Имя WI-FI сети (SSID)
const char* password = "2styKHDQ";   // Пароль от WI-FI

WiFiClient  client;

unsigned long myChannelNumber = 1384395;  // Номер канала
const char * myWriteAPIKey = "ETSGVZ600T5AWTH7";  // Ключ канала

// Время между отправкой сообщения
unsigned long lastTime = 0;
unsigned long timerDelay = 20000;

// Переменная для храниния температуры
float temperatureC;


void setup() {
  Serial.begin(115200);  //Инициализируем последовательный порт
  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);  // Инициализируем ThingSpeak
}

void loop() {
  if ((millis() - lastTime) > timerDelay) { // Если время пауза прошла

    Serial.print(WIFI_connect());
    if ( WIFI_connect() == 1) {

    // Пока выставляем рандом
    temperatureC = random(-40, 40);
      Serial.print("Temperature (ºC): ");
      Serial.println(temperatureC);

      int x = ThingSpeak.writeField(myChannelNumber, 1, temperatureC, myWriteAPIKey);

      if (x == 200) { // Если ответ с сервера 200
        Serial.println("Channel update successful.");
      }
      else {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
      }
      lastTime = millis();
    }
  }
}


byte WIFI_connect() {   // Подключаемся к WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Error connect 0");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
    return 1;
  }

}
