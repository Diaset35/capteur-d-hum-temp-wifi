#include <uTimerLib.h>

#include <WiFiServer.h>
#include <WiFiClient.h>
#include <WiFi.h>

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>

DHT dht(13, DHT11);

WiFiClient wiFiClient;
Adafruit_MQTT_Client mqttClient(&wiFiClient, "192.168.1.103", 1883);
Adafruit_MQTT_Publish tempPublisher(&mqttClient, "/temperature");
Adafruit_MQTT_Publish humPublisher(&mqttClient, "/humidity");

void sendData(){
  int temperature = dht.readTemperature();
  tempPublisher.publish(temperature);
  int humidity = dht.readHumidity();
  humPublisher.publish(humidity);
}

void setup() {
  dht.begin();
  Serial.begin(115200);
  WiFi.begin("createch", "createch");
  delay(5000);
  Serial.print(WiFi.localIP());
  TimerLib.setInterval_s(sendData, 2);
  
}

void loop() {
  if (mqttClient.connected()) {
    mqttClient.processPackets(10000);
    mqttClient.ping();
  } else {
    mqttClient.disconnect();
    mqttClient.connect();
  }



}
