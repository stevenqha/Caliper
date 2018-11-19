constexpr byte CALIBRATE_COMMAND = B11111111;
constexpr byte DATA_RECEIVE_COMMAND = B11111110;

double currentReading = 0.0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    //DisplaycodeHere

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
    double decodedValue= reinterpret_cast<double&>(*tempLongPtr);
    return decodedValue;
}

void calibrate(){
    Serial.write(CALIBRATE_COMMAND);
}
