////////////////////
////  JoyStick  ////
////////////////////

/*--- JoyStick Variabel ---*/
const int xPin = 34;     //Joystick X-axis pin
const int yPin = 35;     //Joystick Y-axis pin

int valueX = 0;          //X-axis analog storage variabel
int valueY = 0;          //Y-axis analog storage variabel


/*-------------------------*/


void readJoyStickValue(int &x, int &y){  //Reads analog values from joystick and adds changes to the parameters x and y 
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
    int tol = 500;
    if (valueX > 1810-tol && valueX < 1810+tol && valueY <= 0){
        Serial.print("Direction: ");
        Serial.println("UP");
    }else if (valueX>=4090 && valueY > 1810-tol && valueY < 1810+tol){
        Serial.print("Direction: ");
        Serial.println("RIGHT");  
    }else if (valueX > 1810-tol && valueX < 1810+tol && valueY >= 4090){
        Serial.print("Direction: ");
        Serial.println("DOWN");
    }else if (valueX <=0 && valueY > 1810-tol && valueY <1810+tol){
        Serial.print("Direction: ");
        Serial.println("LEFT");
    }
    delay(500);
}