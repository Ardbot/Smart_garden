#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

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
    if (WIFI_connect() == 1) {
      post_temp();
    }
    else {
      WIFI_connect();
    }
  }
}

byte WIFI_connect() {   // Подключаемся к WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connection to ");
    Serial.println(ssid);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 5) {
      WiFi.begin(ssid, password);
      i++;
      Serial.print("Connection attempt:");
      Serial.println(i);
      delay(5000);
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Status: disconnect");
      return 0;
    }
    else {
      Serial.println("Status: connect");
      return 1;
    }
  }
  else return 1;

}

byte post_temp() {  // Отправка данных
  // Пока выставляем рандом
  temperatureC = random(-40, 40);
  Serial.print("Temperature (ºC): ");
  Serial.println(temperatureC);

  int x = ThingSpeak.writeField(myChannelNumber, 1, temperatureC, myWriteAPIKey);

  if (x == 200) { // Если ответ с сервера 200
    Serial.println("Channel update successful.");
    return 1;
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
    return 0;
  }
  lastTime = millis();
}
