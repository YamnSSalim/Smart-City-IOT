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
const int Zumo32U4Address = 0x08;   // I2C address of the Zumo32U4
volatile bool dataReceived = false; // Flag to indicate whether data has been received
char receivedData[32];              // Stores received data
int dataLength = 0;                 // Length of the received data
/*---------------------------------------*/

// Function called when data is received via I2C
void receiveEvent(int howMany)
{
    dataLength = 0; // Reset the length of the received data

    // Read all available bytes from the I2C buffer
    while (Wire.available() && dataLength < sizeof(receivedData) - 1)
    {
        receivedData[dataLength++] = Wire.read(); // Stores the byte in the buffer
    }
    receivedData[dataLength] = '\0'; // Null-terminate the string
    dataReceived = true;             // Set the flag to indicate data has been received
    lastDirectionTime = millis();    // Update the time of the last direction update
}

//////////////////////
////// Encoder ///////
//////////////////////

/*--- Encoder-Variables ---*/
Zumo32U4Encoders encoders;

int32_t absCountLeft = 0;  // Variable for storing counts on left side
int32_t absCountRight = 0; // Variable for storing counts on right side
/*-------------------------*/

// Function for initialise Encoders
void setupEncoders()
{
    encoders.init();
}
// Function for reading and storing absolutte encoder values
void updateAbsEncoders()
{
    // Storing encoder values in variables
    long Left = encoders.getCountsAndResetLeft();
    long Right = encoders.getCountsAndResetRight();

    absCountLeft += abs(Left);
    absCountRight += abs(Right);

}

//////////////////////////
/// Calculate Distance ///
//////////////////////////

/*---- Distance-Variables -----*/
const float wheelDiameter = 0.035;                       // 35 mm converted to meters
const float wheelCircumference = 2 * PI * wheelDiameter; // Zumo32U4 wheel circumference
const float encoderCPR = 909.7;                          // Counts per revolution

float distance = 0; // Stores distance calculated
/*-----------------------------*/

// Function for updating distance
void updateDistance()
{
    // Calculating distanse
    float distanceLeft = (absCountLeft * wheelCircumference) / encoderCPR;
    float distanceRight = (absCountRight * wheelCircumference) / encoderCPR;

    distance = (distanceLeft + distanceRight) / 2; // Updating distance
}


/////////////////////////
//// Calculate Speed ////
/////////////////////////

/*--- Speed -Variables ---*/
float speed = 0; // Stores speed calculated 
unsigned long lastSpeedTime = 0; // Time of the last speed calculation 
float previousDistance = 0; // Stores previous distance
/*-------------------------*/

// Function for updating speed
void updateSpeed(){
    unsigned long currentTime = millis(); // Get the current time
    float timeInterval = (currentTime - lastSpeedTime) / 1000.0; // Calculate the time interval 

    if (timeInterval > 0) {
        speed = (distance - previousDistance) / timeInterval;
    }
    previousDistance = distance;
    lastSpeedTime = currentTime;
}





////////////////////////////
//// Battery Monitoring ////
////////////////////////////

/*---- Battery-Variables ----*/
uint16_t batteryMillivolts = 0;    // Stores battery voltage in millivolts
/*---------------------------*/

// Function for updating battery voltage
void updateBattery(){
    batteryMillivolts = readBatteryMillivolts();
}




/////////////////////
/////// OLED ////////
/////////////////////

/*----- OLED - Variables -----*/
Zumo32U4OLED oled; // Stores OLED-library info in variable
/*---------------------------*/

// Function for initialise OLED-screen
void setupOLED()
{
    oled.init();
    oled.setLayout21x8();
    oled.clear();
}

// Function to update the OLED display with directions
void updateOLED()
{
    oled.clear();

    oled.gotoXY(2, 0);
    oled.print("Input: ");
    oled.print(receivedData); // Update the OLED display with the joystick direction data

    oled.gotoXY(2, 2);
    oled.print("Meter: ");
    oled.print(distance); // Update OLED display with distance values
    oled.print(" m");

    oled.gotoXY(2, 4);
    oled.print("Speed: ");
    oled.print(speed); // Update OLED display with distance values
    oled.print(" m/s");

    oled.gotoXY(2, 6);
    oled.print("Battery: ");
    oled.print(speed); // Update OLED display with battery values
    oled.print(" mV");


}



/////////////////////
/////// Motor ///////
/////////////////////

/*----- Motor - Variable -----*/
Zumo32U4Motors motors; // Stores library info in variable
/*----------------------------*/

// Function to handle the movement based on received data
void handleMovement(const char *data)
{
    // Check if the received data indicates "Forward"
    if (strcmp(receivedData, "Forward") == 0)
    {
        motors.setSpeeds(200, 200); // Set both motors to move forward
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
        motors.setSpeeds(-100, 100); // Set right motor backward, left motor forward
        lastDirectionTime = millis();
    }

    // Check if the received data indicates "Backward"
    if (strcmp(receivedData, "Backward") == 0)
    {
        motors.setSpeeds(-200, -200); // Set both motors to move backward
        lastDirectionTime = millis();
    }
}

////////////////////
//// I2C-Sender ////
////////////////////

// Function called when data is requested via I2C
void requestEvent(){
    Wire.write((uint8_t *)&distance, sizeof(distance)); // Send the distance as raw bytes
    Wire.write((uint8_t*)&speed,sizeof(speed)); // Send the speed as raw bytes 
}

void setup()
{
    Serial.begin(115200);
    setupOLED();     // Calling OLED-setup
    setupEncoders(); // Calling Encoders-setup

    Wire.begin(Zumo32U4Address);  // Initialize I2C communication
    Wire.onReceive(receiveEvent); // Register the receive event handler
    Wire.onRequest(requestEvent); // Register the request event handler

    lastDirectionTime = millis(); // Initialize the last direction time
    lastSpeedTime = millis();     // Initialze the last speed time
}

void loop()
{
    // Check if data has been received
    if (dataReceived)
    {
        handleMovement(receivedData); // Hanlde movement based on the received data

        updateAbsEncoders(); // Update encoder values

        updateDistance(); // Update distance

        updateSpeed(); // Update speed

        updateBattery(); // Update battery

        updateOLED(); // Displaying values on OLED screen        

        dataReceived = false; // Reset the flag
    }

    // Check if any data has arrived if not the motor stops
    if (millis() - lastDirectionTime > directionTimeout)
    {
        motors.setSpeeds(0, 0); // Set both motors to 0
    }
}
