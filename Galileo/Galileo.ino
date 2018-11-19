double offset = 0;
constexpr int GEAR_RATIO = 9;
constexpr int PITCH = 24; 
constexpr byte CALIBRATE_COMMAND = B11111111;
constexpr byte DATA_RECEIVE_COMMAND = B11111110;

void setup() {
    Serial.begin(9600);
    pinMode(A0, INPUT);
}

void loop() {
    double currentPosition = getCalibratedPosition();
    byte byteArr[4];
    doubleToByteArray(currentPosition, byteArr);
    sendDataToDisplay(byteArr);
}

void serialEvent(){
    while (Serial.available()){
        if (Serial.read() == CALIBRATE_COMMAND){
            calibrate();    
        }
    }
}

double getCalibratedPosition(){
    return getRawPosition() + offset;
}

double getRawPosition(){
    double potReading = analogRead(A0);
    double mappedReading = map(potReading, 0, 4095, 0, 10);
    double rawCaliperReading = mappedReading * GEAR_RATIO / PITCH * 25.4;
    return rawCaliperReading;
}

void calibrate(){
    offset = -1 * getRawPosition();
}

void doubleToByteArray(double d, byte* byteArr){
    long* tempLongPtr = reinterpret_cast<long*>(byteArr);
    *tempLongPtr = reinterpret_cast<long&>(d);
}

void sendDataToDisplay(byte data[]){
    Serial.write(DATA_RECEIVE_COMMAND);
    for(int i = 0; i < 4; i++){
        Serial.write(data[i]);
    }
}
