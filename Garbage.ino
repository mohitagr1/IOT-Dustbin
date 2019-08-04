#define BLYNK_MAX_SENDBYTES 256
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define TRIG_PIN 2    //D4
#define ECHO_PIN 0    //D3
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 5

DHT dht(dht_dpin, DHTTYPE); 

char auth[] = "736b9f3f048f4d23a9b4634571031996";
char ssid[] = "mohit";
char pass[] = "mohit1234";

float distance;
long duration;
float h;
float hprev=0;

BlynkTimer timer;
void myTimerEvent()
{
  h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }  
  if(h-hprev>=2){
    Blynk.notify("You have put wet garbage in Dry Bin, pls make sure it wont be repeated. Swatch Bharat Abhiyan!");
    Blynk.email("mohitagr00001@gmail.com","You are warned!!","You have put wet garbage in Dry Bin, Swatch Bharat Abhiyan!");
    }
  hprev=h;
  Blynk.virtualWrite(V2, h);
}

void setup()
{
  dht.begin();
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration*0.017;  
  if(distance>=400 || distance<=2)
  distance=0;
  else if(distance>14)
  distance=14;
  distance=(distance/14)*100;
  Blynk.virtualWrite(V1,distance);
  Blynk.run();
  timer.run();
  delay(1000);
}
