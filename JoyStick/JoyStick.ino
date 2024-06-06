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
    const int tol = 500;        // Tolerance value 

    // Check if the joystick is pushed up
    if (x > centerValue - tol && x < centerValue + tol && y <= minValue)  
    {
        Serial.print("Direction: ");  
        Serial.println("UP");
    }

    // Check if the joystick is pushed right
    else if (x >= maxValue && y > centerValue - tol && y < centerValue + tol) 
    {
        Serial.print("Direction: "); 
        Serial.println("RIGHT");
    }

    // Check if the joystick is pushed down
    else if (x > centerValue - tol && x < centerValue + tol && y >= maxValue)
    {
        Serial.print("Direction: ");
        Serial.println("DOWN");
    }

    // Check if the joystick is pushed left
    else if (x <= minValue && y > centerValue - tol && y < centerValue + tol)
    {
        Serial.print("Direction: ");
        Serial.println("LEFT");
    }
}






void setup()
{
    Serial.begin(115200);
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
}

void loop()
{
    readJoyStickValue(valueX, valueY); // Call analog read function
    getDirection(valueX,valueY);    // Call direction function
    delay(500);
}