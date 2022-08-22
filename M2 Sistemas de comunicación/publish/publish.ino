/******************************************
 *
 * This example works for both Industrial and STEM users.
 *
 * Developed by Jose Garcia, https://github.com/jotathebest/
 *
 * ****************************************/

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsEsp32Mqtt.h"

/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBFF-FnYXOGW2q25Zd5YsRHsbpyBIi1Scig";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "Tec-IoT";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "spotless.magnetic.bridge";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "guayabitos_nodemcu";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "Potter"; // Put here your Variable label to which data  will be published
const char *SUBSCRIBE_DEVICE_LABEL = "guayabitos_nodemcu";   // Replace with the device label to subscribe to
const char *SUBSCRIBE_VARIABLE_LABEL = "led_control"; // Replace with the variable label to subscribe to

const int PUBLISH_FREQUENCY = 5000; // Update rate in millisecondsx

unsigned long timer;
uint8_t analogPin = 34; // Pin used to read data from GPIO34 ADC_CH6.
uint8_t led=23;

Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String messageTemp;
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();
  
  if(messageTemp == "1.0"){
    digitalWrite(led,HIGH);
  }else if(messageTemp == "0.0"){
    digitalWrite(led,LOW);
  }
}

/****************************************
 * Main Functions
 ****************************************/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL, SUBSCRIBE_VARIABLE_LABEL); // Insert the device and variable's Labels, respectively

  timer = millis();
  pinMode(led,OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!ubidots.connected())
  {
    ubidots.reconnect();
    ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL, SUBSCRIBE_VARIABLE_LABEL); // Insert the device and variable's Labels, respectively
  }
  if (millis() - timer > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    float value = analogRead(analogPin);
    ubidots.add(VARIABLE_LABEL, value); // Insert your variable Labels and the value to be sent
    ubidots.publish(DEVICE_LABEL);
    timer = millis();
  }

  ubidots.loop();
}
