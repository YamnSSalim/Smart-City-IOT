#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

////////////////
///// WiFi /////
////////////////

/*------ WiFi-Variables ------*/
const char *ssid = "Studysession";
const char *pass = "Network1";
/*----------------------------*/

// Function to connect ESP32 onboard to WiFi
void setupWiFi()
{
    Serial.printf("\nConnecting to %s\n\n", ssid);

    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }
    Serial.println("\nSuccessfully connected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

//////////////////////////////
///// PubSubClient + I2C  ////
//////////////////////////////

/*------ PubSubClient-Variables ------*/
const char *broker_server = "192.168.137.208";
const char *outTopic = "joystick/data"; // MQTT topic for joystick data
const char *inTopic = "zumo32u4/data"; // MQTT topic for zumo32u4 data


WiFiClient espClient;
PubSubClient client(espClient);

const int zumoAddress = 0x08; // I2C address of the Zumo32U4


float distance;     // Variable to store received distance data

unsigned long lastRequestTime = 0; // Time of the last I2C request
const unsigned long requestInterval = 1000; // Interval between I2C request 
/*-----------------------------------*/

// Function to reconnect to the MQTT broker
void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");

        if (client.connect("ESP32ClientSubscriber"))
        {
            Serial.println("connected");
            client.subscribe(outTopic);
        }
        else
        {
            Serial.print("failed, reconnect= ");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

// Callback function to handle messages received from the subscribed topic
void callback(char *topic, byte *message, unsigned int length)
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    // Saving topic data on messageTemp variabel 
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();

    // Send message to Zumo32U4 via I2C
    Wire.beginTransmission(zumoAddress);
    Wire.write((const uint8_t*)messageTemp.c_str(), messageTemp.length());
    byte error = Wire.endTransmission();

    // Check if message was send to Zumo32U4
    if (error == 0)
    {
        Serial.print("Message sent to Zumo32U4");
    }
    else
    {
        Serial.print("Error sending message to Zumo32U4");
        Serial.println(error);
    }
}

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    setupWiFi();
    client.setServer(broker_server, 1883);
    client.setCallback(callback);
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    unsigned long currentTime = millis();

    // Check if its time to request data from the Zumo32U4
    if (currentTime - lastRequestTime >= requestInterval){

        Wire.requestFrom(zumoAddress, sizeof(distance));

        if (Wire.available() == sizeof(distance)){
            Wire.readBytes((char*)&distance, sizeof(distance));

            // Convert the distance to a string

            char distanceStr[10];
            dtostrf(distance, 5, 2, distanceStr);
            // Print the received distance
             Serial.print("Received distance: ");
            Serial.println(distanceStr);

            // Publish the distance to the MQTT broker
            client.publish(inTopic,distanceStr);
        }

        // Update the last request time
        lastRequestTime = currentTime;
    }
    
}