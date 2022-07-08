SYSTEM_THREAD(ENABLED)

#include "MQTT.h"
#include "oled-wing-adafruit.h"

OledWingAdafruit display;

long previousMillis = 0;
long interval = 10000;
double lon;
double lat;
bool coords = true;

void displaySetup()
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
}

void callback(char *topic, byte *payload, unsigned int length);

MQTT client("lab.thewcl.com", 1883, callback);

void callback(char *topic, byte *payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String s = p;
  double value = s.toFloat();
  
  if (coords == true) {
    //Serial.println("Longitude");
    lon = value;
    coords = false;
    
  } else if (coords == false) {
    coords = true;
    //Serial.println("Latitude");
    //Serial.println(value);
    lat = value;
  }
  








}

void setup()
{
    Serial.begin(9600);
    display.setup();
    displaySetup();
    display.setTextColor(WHITE);
    display.display();
}

void loop()
{
    unsigned long currentMillis = millis();

    display.loop();
    
    
    if (client.isConnected())
    {
        client.subscribe("longitude");
        client.loop();
        
        client.subscribe("latitude");
        client.loop();
        
        
        if (currentMillis - previousMillis > interval)
        {
            previousMillis = currentMillis;
            client.publish("main", "a.request");

            displaySetup();
            display.print("Lon: ");
            display.println(lon);
            display.print("Lat: ");
            display.println(lat);
            display.display();
        }
        
        
    } else {
    client.connect(System.deviceID());
    }

} 