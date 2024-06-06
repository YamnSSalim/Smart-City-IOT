////////////////////
////  JoyStick  ////
////////////////////

/*--- JoyStick Variabel ---*/
const int xPin = 34; // Joystick X-axis pin
const int yPin = 35; // Joystick Y-axis pin

int valueX = 0; // X-axis analog storage variabel
int valueY = 0; // Y-axis analog storage variabel

/*-------------------------*/

void readJoyStickValue(int &x, int &y)
{ // Reads analog values from joystick and adds changes to the parameters x and y
    x = analogRead(xPin);
    y = analogRead(yPin);

    Serial.print(" X-value = ");
    Serial.println(x);

    Serial.println("-----------");

    Serial.print(" Y-Value = ");
    Serial.println(y);
}

void getDirection(int x, int y)
{
    const int maxValue = 4090;
    const int minValue = 0;
    const int centerValue = 1810;
    const int tol = 500;
    
    if (x > centerValue - tol && x < centerValue + tol && y <= minValue)
    {
        Serial.print("Direction: ");
        Serial.println("UP");
    }
    else if (x >= maxValue && y > centerValue - tol && y < centerValue + tol)
    {
        Serial.print("Direction: ");
        Serial.println("RIGHT");
    }
    else if (x > centerValue - tol && x < centerValue + tol && y >= maxValue)
    {
        Serial.print("Direction: ");
        Serial.println("DOWN");
    }
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
    readJoyStickValue(valueX, valueY); // calling function in loop
    getDirection(valueX,valueY);
    delay(500);
}