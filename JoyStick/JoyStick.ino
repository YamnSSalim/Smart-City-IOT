#include <WiFi.h>
#include <PubSubClient.h>

///////////////
////  WiFi  ///
///////////////

/*---------------WiFi-Variables----------------*/
const char* ssid = "Studysession";  // WiFi name
const char* pass = "Network1";  // WiFi
/*---------------------------------------------*/

// Function to connect ESP32 to WiFi
void setupWiFi(){

    Serial.printf("\nConnecting to %s\n\n", ssid);  //Print a message indicating the WiFi connection attempt

    WiFi.begin(ssid,pass);  // Initialize the connection to WiFi network

    // Loop until the ESP32 is connected to WiFi
    while (WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }
    Serial.println("\nSuccessfully connected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP()); //Print the local IP address of the ESP32
}


///////////////////////
//// PubSubClient /////
///////////////////////

/*------ PubSubClient-Variables ------*/
const char* broker_server = "192.168.137.208";
const char* outTopic = "joystick/data";

WiFiClient espClient;
PubSubClient client(espClient);
/*------------------------------------*/

// Function to reconnect to MQTT broker
void reconnect(){
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");

        if (client.connect("ESP32ClientUser")){
            Serial.println("connected");
        }else{
            Serial.print("failed, reconnect= ");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}



////////////////////
////  JoyStick  ////
////////////////////

/*--- JoyStick Variabel ---*/
const int xPin = 34; // Joystick X-axis pin
const int yPin = 35; // Joystick Y-axis pin

int valueX = 0; // X-axis analog storage variabel
int valueY = 0; // Y-axis analog storage variabel
/*-------------------------*/


// Function to read analog value from the joystick
void readJoyStickValue(int &x, int &y)
{ 
    x = analogRead(xPin);   // Read analog value from X-axis
    y = analogRead(yPin);   // Read analog value from Y-axis


    //Debugging option
    
    Serial.print(" X-value = ");    // Print X-axis analog value
    Serial.println(x);

    Serial.println("-----------");

    Serial.print(" Y-Value = ");    // Print Y-axis analog value
    Serial.println(y);
    
}

// Function to convert analog values into directions
void getDirection(int x, int y) 
{
    const int maxValue = 4090;  // Maximal analog value
    const int minValue = 0;     // Minimal analog value
    const int centerValue = 1810;   // Center value 
    const int tol = 600;        // Tolerance value 

    String direction;

    // Check if the joystick is pushed up
    if (x > centerValue - tol && x < centerValue + tol && y <= minValue)  
    {
        Serial.print("Direction: ");  
        Serial.println("Forward");
        direction = "Forward";
    }

    // Check if the joystick is pushed right
    else if (x >= maxValue && y > centerValue - tol && y < centerValue + tol) 
    {
        Serial.print("Direction: "); 
        Serial.println("RIGHT");
        direction = "RIGHT";
    }

    // Check if the joystick is pushed down
    else if (x > centerValue - tol && x < centerValue + tol && y >= maxValue)
    {
        Serial.print("Direction: ");
        Serial.println("Backward");
        direction = "Backward";
    }

    // Check if the joystick is pushed left
    else if (x <= minValue && y > centerValue - tol && y < centerValue + tol)
    {
        Serial.print("Direction: ");
        Serial.println("LEFT");
        direction = "LEFT";
    }

    // Publish direction to MQTT topic 
    if (!direction.isEmpty()){
        client.publish(outTopic, direction.c_str());
    }
}


void setup()
{
    Serial.begin(115200);
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
    setupWiFi();    // Call setupWiFi in setup
    client.setServer(broker_server, 1883);  // Setting up server, based on broker_server and port 
}

void loop()
{
    // Loop reconnect if not connected to broker 
    if(!client.connected()){
        reconnect();
    }
    client.loop();

    // Read joystick values
    readJoyStickValue(valueX, valueY);

    // Get joystick direction and publish to MQTT
    getDirection(valueX, valueY);

    delay(100);

}