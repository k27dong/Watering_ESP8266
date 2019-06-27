#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PlainProtocol.h>
#include <SimpleTimer.h>

SimpleTimer timer;

char auth[] = "YourAuthToken";

// Hidden for privacy issues
char ssid[] = "";
char pass[] = "";

PlainProtocol puloadThread(Serial);

WidgetLED wateringLED(V4);
WidgetLED highTemLED(V5);

void uploadFunction() {}

BLYNK_WRITE(V3)
{
  //nt value = param.asInt();
  int x = param.asInt();
  puloadThread.write("SET", x);
}

void setup()
{
  puloadThread.begin(115200);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  //timer.run();
  if (puloadThread.available())
  {
    if (puloadThread.equals("TEM"))
    {
      int temperature = puloadThread.read();
      Blynk.virtualWrite(V0, temperature);
      if (temperature > 30)
      {
        highTemLED.setValue(255);
      }
      else
      {
        highTemLED.setValue(0);
      }
    }
    else if (puloadThread.equals("SOIL"))
    {
      Blynk.virtualWrite(V1, puloadThread.read());
    }
    else if (puloadThread.equals("HUM"))
    {
      Blynk.virtualWrite(V2, puloadThread.read());
    }
    else if (puloadThread.equals("WATERING"))
    {
      wateringLED.setValue(255);
    }
    else if (puloadThread.equals("STOP"))
    {
      wateringLED.setValue(0);
    }
  }
}
