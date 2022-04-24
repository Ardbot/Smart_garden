//Avtor: Ardbot https://github.com/Ardbot
//Date: 24.04.2022

#include <ESP8266WiFi.h>
#include <WiFiServer.h> wifiServer(80);


#include "config.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros


char ssid[] = SECRET_SSID;   // Имя WI-FI
char pass[] = SECRET_PASS;   // Пароль WI-FI
//int keyIndex = 0;            // your network key Index number (needed only for WEP)

uint32_t timer = 0;   // переменная таймера
int counter = 0;

WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int ledPin = 1; // Пин светодиода

void setup() {
  pinMode(ledPin, OUTPUT);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
//  if (WiFi.status() != WL_CONNECTED) {
//    Serial.begin(115200);  // Initialize serial
//    delay(1000);
//    Serial.print("Connect to SSID: ");
//    Serial.println(SECRET_SSID);
//
//    while (WiFi.status() != WL_CONNECTED) {
//      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
//      Serial.print(".");
//      //Serial.println(WiFi.status());
//
//      // Мигаем светодиодом во время подключения
//      for (int i = 0; i < 5; i++) {
//        blink(1000, 1000);
//        Serial.println(i);
//        delay(1000);
//      }
//    }
//    Serial.println("Подключено. Закрываю порт");
//    delay(500);
//    Serial.end(); // Закрываем порт
//    ThingSpeak.setStatus("\nConnected.");
//    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
//  }
  blink(50, 1000,3); // Пингую
}

// Время свечения, время паузы. Кол-во бликов
void blink(int on_time, int off_time, int blinks) {
  byte k = 0; // Ключ
  if (millis() - timer >= on_time & k == 0) { // таймер на millis()
    digitalWrite(ledPin, LOW);   // Включает светодиод (инвертировано)
    counter++;  // прибавляем счётчик
    timer = millis(); // сброс
    else if (millis() - timer >= off_time) & k == 1 { // таймер на millis()
      digitalWrite(ledPin, HIGH);    // Выключает светодиод (инвертировано)
      timer = millis(); // сброс
    }
    if (counter > blinks) counter = 0;  // закольцовываем изменение
  }
}
