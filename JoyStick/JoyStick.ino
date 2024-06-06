////////////////////
////  JoyStick  ////
////////////////////

/*--- JoyStick Variabel ---*/
const int xPin = 34;     //Joystick X-axis pin
const int yPin = 35;     //Joystick Y-axis pin

int valueX = 0;          //X-axis analog storage variabel
int valueY = 0;          //Y-axis analog storage variabel
/*-------------------------*/


void readJoyStickValue(int x, int y){  //Reads analog values from joystick 
    x = analogRead(xPin);
    y = analogRead(yPin);

    Serial.print(" X-value = ");
    Serial.println(x);

    Serial.println("-----------");

    Serial.print(" Y-Value = ");
    Serial.println(y);
}


void setup(){
    Serial.begin(115200);
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
}


void loop(){ 
    readJoyStickValue(valueX, valueY); //calling function in loop
    delay(500);
}