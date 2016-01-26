/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <stdio.h>
#include <stdlib.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D6
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float temp = 0.0;



const char* ssid     = "your-ssid";
const char* password = "your-wifi-password";
byte server[] = { 192, 168, 0, 41 };
WiFiClient client;
   PubSubClient mqttclient(server, 1883, 0, client);
   unsigned long keepalivetime=0;
   unsigned long MQTT_reconnect=0;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  while (mqttclient.connect("tempmon") != 1)
  {
    Serial.println("Error connecting to MQTT");
    delay(3000);
  }
  MQTT_reconnect = millis();
  Serial.println("setup complete");
    mqttclient.publish("fromArduino","hello world");
  
 sensors.begin();
}

int value = 0;

void loop() {
  delay(5000);
  ++value;
  

  sensors.requestTemperatures();
  Serial.print("Temperature for Device 1 is: ");
  Serial.println(sensors.getTempCByIndex(0));
  temp = sensors.getTempCByIndex(0);
  char message_buffa[6];
  dtostrf(temp, 4, 3, message_buffa); 
  String json = buildJson();
  char jsonStr[200];
  json.toCharArray(jsonStr,200);
  
  mqttclient.publish("/Temperature/temp1",message_buffa);
  // Use WiFiClient class to create TCP connections
  

/*  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return; */
  }
  
String buildJson() {
  
  String data = "{";
//  data+="\n";
//  data+= "\"d\": {";
//  data+="\n";
  data+="\"room\":";
//  data+="\n";
  data+="\"temperature\": ";
//  data+=(int)tempF;
  data+= ",";
  data+="\n";
  data+="\"temperature (C)\": ";
//  data+=messagebuffa;
  data+= ",";
  data+="\n";
//  data+="\"humidity\": ";
//  data+=(int)humidity;
  data+="\n";
  data+="}";
  data+="\n";
  data+="}";
  return data;
}
  


