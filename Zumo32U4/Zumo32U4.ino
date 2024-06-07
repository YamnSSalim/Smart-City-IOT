#include <Wire.h>
#include <Zumo32U4.h>

///////////////////////////////
////// I2C-Communcation ///////
///////////////////////////////

/*------------ I2C-Variables ------------*/
const int Zumo32U4Address = 0x08; // I2C address of the Zumo32U4
volatile bool dataReceived = false; // Flag to indicate whether data has been received
char receivedData[32]; // Stores received data
int dataLength = 0; // Length of the received data 
/*---------------------------------------*/


// Function called when data is received via I2C
void receiveEvent(int howMany)
{
    dataLength = 0; // Reset the length of the received data 


    // Read all available bytes from the I2C buffer 
    while (Wire.available() && dataLength < sizeof(receivedData) -1){
        receivedData[dataLength++] = Wire.read(); //Stores the byte in the buffer
    }
    receivedData[dataLength] = '\0'; //Null-terminate the string
    dataReceived = true; // Set the flag to indicate data has been received
}



/////////////////////
/////// OLED ////////
/////////////////////

/*----- OLED - Variable -----*/
Zumo32U4OLED oled;  // Stores library info in variable
/*---------------------------*/

// Function for initialise OLED-screen
void setupOLED(){
    oled.init();
    oled.setLayout21x8();
    oled.clear();
}

// Function to update the OLED display with a given message
void updateOLEDDisplay(const char* label, const char* message){
    oled.clear();

    oled.gotoXY(2,0);
    oled.print(label);

    oled.gotoXY(2,2);
    oled.print(message);
}




void setup()
{
    Serial.begin(115200);
    setupOLED();
    Wire.begin(Zumo32U4Address);    // Initialize I2C communication
    Wire.onReceive(receiveEvent);   // Register the receive event handler
}

void loop()
{
    // Check if data has been received
    if (dataReceived)
    {
        Serial.print("Received data: ");
        Serial.println(receivedData);   // Print the received data to the serial monitor 

        // Update the OLED display with the received
        updateOLEDDisplay("Received:", receivedData);

        dataReceived = false;   // Reset the flag
    }
}



