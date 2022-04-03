#include <ESP8266WiFi.h>
#include "config.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

#include <microDS18B20.h>
MicroDS18B20<4> sensor;
//MicroDS18B20<5> sensor2;

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

String myStatus = "";

void setup() {
  Serial.begin(115200);  // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      Serial.println(WiFi.status());
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  sensor.requestTemp();
//  sensor2.requestTemp();
  delay(1000);
  float temp1 = sensor.getTemp();
//  int temp2 = sensor2.getTemp();

  // set the fields with the values
  ThingSpeak.setField(1, temp1);
//  ThingSpeak.setField(2, temp2);

  // figure out the status message
  if (temp1<1) {
    myStatus = String("Заморозки " + String(temp1));
  }
    else if (temp1<15) {
    myStatus = String("Прохладно "  + String(temp1));
  }
  else if (temp1>30) {
    myStatus = String("Жарко " + String(temp1));
  }
  else {
    myStatus = String("Температура норм "  + String(temp1));
  }

  // set the status
  ThingSpeak.setStatus(myStatus);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(300000); // Wait 300 seconds to update the channel again
}
