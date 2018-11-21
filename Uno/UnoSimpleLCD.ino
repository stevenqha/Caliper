#include <LiquidCrystal.h>

#define POT   A0
#define BUTTON 2

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

constexpr byte CALIBRATE_COMMAND = B11111111;
constexpr byte DATA_RECEIVE_COMMAND = B11111110;

double currentReading = 0.0;
double oldReading = 0;
int zeroValue;

void setup() {
    Serial.begin(9600);
    pinMode(POT, INPUT);
    pinMode(BUTTON, INPUT);

    lcd.begin(16, 2);
    
    lcd.setCursor(8, 0);
    lcd.print ("mm");
}

void loop() {
    //int potValue = analogRead(POT);

    lcd.setCursor(0,0);
    if (digitalRead(BUTTON) == LOW)
        calibrate();
        //zeroValue = potValue;

    //currentReading = 0.03488 * (potValue - zeroValue);
        
    if (currentReading != oldReading){
        //Pad with appropriate number of spaces depending on the size of the number
        if (currentReading >= 100)
            lcd.print (" ");
        else if (currentReading < 100 && currentReading >= 10)
            lcd.print ("  ");
        else if (currentReading < 10 && currentReading >= 0)
            lcd.print ("   ");
        else{
            currentReading = currentReading*-1;
            lcd.print ("-  ");  
        }
        lcd.print (currentReading, 2);
    }

    oldReading = currentReading;
    //delay(100);
}

void serialEvent(){
    if (Serial.available()>=5 && Serial.read() == DATA_RECEIVE_COMMAND){
        byte byteArr[4];
        for(int i = 0; i<4; i++){
            byteArr[i]=Serial.read();
        }
        currentReading = byteArrayToDouble(byteArr);
    }
}

double byteArrayToDouble(byte* byteArr){
    long* tempLongPtr = reinterpret_cast<long*> (byteArr);
    double decodedValue = reinterpret_cast<double&>(*tempLongPtr);
    return decodedValue;
}

void calibrate(){
    Serial.write(CALIBRATE_COMMAND);
}
