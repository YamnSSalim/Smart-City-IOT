#include <WiFi.h>
#include <PubSubClient.h>

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

///////////////////////
///// PubSubClient ////
///////////////////////

/*------ PubSubClient-Variables ------*/
const char *broker_server = "192.168.137.208";
const char *outTopic = "joystick/data";

WiFiClient espClient;
PubSubClient client(espClient);
/*-----------------------------------*/

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

void callback(char *topic, byte *message, unsigned int length)
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();
}

void setup()
{
    Serial.begin(115200);
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
}