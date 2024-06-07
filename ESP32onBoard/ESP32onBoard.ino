#include <WiFi.h>

////////////////
///// WiFi /////
////////////////

/*------ WiFi-Variables ------*/
const char* ssid = "Studysession";
const char* pass = "Network1";
/*----------------------------*/

// Function to connect ESP32 onboard to WiFi
void setupWiFi(){
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

void setup(){
    Serial.begin(115200);
    setupWiFi();
}

void loop(){

}