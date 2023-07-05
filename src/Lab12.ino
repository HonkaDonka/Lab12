#include "MQTT.h"
#include "oled-wing-adafruit.h"

#define LEDPIN D7
#define TOPIC "VinsonLab12"

SYSTEM_THREAD(ENABLED);

void callback(char *topic, byte *payload, unsigned int length);

MQTT client("lab.thewcl.com", 1883, callback);
OledWingAdafruit display;

void setup()
{
  display.setup();
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();

  pinMode(LEDPIN, OUTPUT);
}

void loop()
{
  if (client.isConnected())
  {
    client.loop();
    display.loop();

    if (display.pressedA())
    {
      client.publish(TOPIC, "Button A on the OLED was pressed!");
    }
    else if (display.pressedB())
    {
      client.publish(TOPIC, "Button B on the OLED was pressed!");
    }
    else if (display.pressedC())
    {
      client.publish(TOPIC, "Button C on the OLED was pressed!");
    }
  }
  else
  {
    client.connect(System.deviceID());
    client.subscribe(TOPIC);
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;

  if (String(p).equals("1"))
  {
    digitalWrite(LEDPIN, HIGH);
  }
  else if (String(p).equals("0"))
  {
    digitalWrite(LEDPIN, LOW);
  }
  else
  {
    resetDisplay();
    display.println(p);
    display.display();
  }
}

void resetDisplay()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.display();
}