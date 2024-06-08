#include <Wire.h>
#include <Zumo32U4.h>

/////////////////////
////// Millis ///////
/////////////////////

/*------------Millis-Variables-----------*/
unsigned long lastDirectionTime;            // Time of the last direction update
const unsigned long directionTimeout = 350; // Timeout period in milliseconds
/*---------------------------------------*/

///////////////////////////////
////// I2C-Communcation ///////
///////////////////////////////

/*------------ I2C-Variables ------------*/
const int Zumo32U4Address = 0x08;           // I2C address of the Zumo32U4
volatile bool dataReceived = false;         // Flag to indicate whether data has been received
char receivedData[32];                      // Stores received data
int dataLength = 0;                         // Length of the received data
/*---------------------------------------*/

// Function called when data is received via I2C
void receiveEvent(int howMany)
{
    dataLength = 0;                                // Reset the length of the received data

    // Read all available bytes from the I2C buffer
    while (Wire.available() && dataLength < sizeof(receivedData) - 1)
    {
        receivedData[dataLength++] = Wire.read(); // Stores the byte in the buffer
    }
    receivedData[dataLength] = '\0';              // Null-terminate the string
    dataReceived = true;                          // Set the flag to indicate data has been received
    lastDirectionTime = millis();                 // Update the time of the last direction update
}

/////////////////////
/////// OLED ////////
/////////////////////

/*----- OLED - Variable -----*/
Zumo32U4OLED oled; // Stores library info in variable
/*---------------------------*/

// Function for initialise OLED-screen
void setupOLED()
{
    oled.init();
    oled.setLayout21x8();
    oled.clear();
}

// Function to update the OLED display with a given message
void updateOLEDDisplay(const char *label, const char *message)
{
    oled.clear();

    oled.gotoXY(2, 0);
    oled.print(label);

    oled.gotoXY(2, 2);
    oled.print(message);
}

/////////////////////
/////// Motor ///////
/////////////////////

/*----- Motor - Variable -----*/
Zumo32U4Motors motors;            // Stores library info in variable
/*----------------------------*/





void setup()
{
    Serial.begin(115200);
    setupOLED();
    Wire.begin(Zumo32U4Address);  // Initialize I2C communication
    Wire.onReceive(receiveEvent); // Register the receive event handler
    lastDirectionTime = millis(); // Initialize the last direction time
}

void loop()
{
    // Check if data has been received
    if (dataReceived)
    {
        Serial.print("Received data: ");
        Serial.println(receivedData);    // Print the received data to the serial monitor

        // Update the OLED display with the received
        updateOLEDDisplay("Received:", receivedData);

        // Check if the received data indicates "Forward"
        if (strcmp(receivedData, "Forward") == 0)
        {
            motors.setSpeeds(200, 200);  // Set both motors to move forward
            lastDirectionTime = millis();
        }

        // Check if the received data indicates "RIGHT"
        if (strcmp(receivedData, "RIGHT") == 0)
        {
            motors.setSpeeds(100, -100); // Set right motor forward, left  motor backward
            lastDirectionTime = millis();
        }

        // Check if the received data indicates "LEFT"
        if (strcmp(receivedData, "LEFT") == 0)
        {
            motors.setSpeeds(-100,100);  // Set right motor backward, left motor forward 
            lastDirectionTime = millis();
        }

        // Check if the received data indicates "Backward"
        if (strcmp(receivedData, "Backward") == 0)
        {
            motors.setSpeeds(-200,-200); // Set both motors to move backward
            lastDirectionTime = millis();
        }

        dataReceived = false;            // Reset the flag
    }
    if (millis() - lastDirectionTime > directionTimeout){
        motors.setSpeeds(0,0);           // Set both motors to 0
    }
}
