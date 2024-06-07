#include <Wire.h>

const int Zumo32U4Address = 4;
String receivedData = "";


void receiveEvent(int howMany)
{
    receivedData = "";

    while (Wire.available())
    {
        char c = Wire.read();
        receivedData += c;
    }
}

void setup()
{
    Serial.begin(115200);

    Wire.begin(Zumo32U4Address);
    Wire.onReceive(receiveEvent);
}

void loop()
{
    if (receivedData.length() > 0)
    {
        Serial.print("Received joystick data: ");
        Serial.println(receivedData);
        receivedData = "";
    }
}



