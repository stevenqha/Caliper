double offset = 0;
constexpr int GEAR_RATIO = 9;
constexpr int PITCH = 16;
constexpr byte CALIBRATE_COMMAND = B11111111;
constexpr byte DATA_RECEIVE_COMMAND = B11111110;

void setup() {
    Serial1.begin(115200);
    Serial.begin(115200);
    pinMode(A0, INPUT);

    
    pinMode(2, INPUT_PULLUP);
    analogReadResolution(12);
}

void loop() {
    double currentPosition = getCalibratedPosition();
    byte byteArr[4];
    sendDataToDisplay(currentPosition);
    delay(50);
    if(digitalRead(2)== LOW)
      calibrate();
    //serialEvent();
}

//void serialEvent(){
//    while (Serial1.available()){
//        //Serial.println("SERIAL EVENT");
//        if (Serial1.read() == CALIBRATE_COMMAND){
////        if (Serial.read() == 'c') {
//            calibrate();    
//        }
//    }
//}

double getCalibratedPosition(){
    return getRawPosition() + offset;
}

double getRawPosition(){
    long rawPotReading = analogRead(A0);
    double potReading = runningAverage(rawPotReading);
    double mappedReading = 0.0029696125647186800 * potReading -0.11537717163673600;
    double rawCaliperReading = mappedReading * GEAR_RATIO / PITCH * 25.4;

    Serial.println("");
    Serial.print(rawPotReading);
    Serial.print("\t\t");
    Serial.print(potReading);
    Serial.print("\t\t");
    Serial.print(mappedReading);
    Serial.print("\t\t");
    Serial.print(rawCaliperReading);
    Serial.print("\t\t");
    Serial.print(rawCaliperReading + offset);
    Serial.print("\t"); 
            
    return rawCaliperReading;
}

void calibrate(){
    offset = -1 * getRawPosition();
}

void sendDataToDisplay(double reading ){
    Serial1.write(DATA_RECEIVE_COMMAND);
    char doubleString[9];   
    sprintf(doubleString, "%5.1f mm", reading);
    Serial1.write(doubleString);
    Serial.print("\t");
    Serial.print(doubleString);
    Serial1.flush();
}

double fmap(long x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double runningAverage(long M)
{
  #define LMSIZE 25
 static long LM[LMSIZE]; // LastMeasurements
 static byte index = 0;
 static long long sum = 0;
 static byte count = 0;

 // keep an updated sum to improve speed.
 sum -= LM[index];
 LM[index] = M;
 sum += LM[index];
 index = (++index) % LMSIZE;
 if (count < LMSIZE) count++;

 return sum / static_cast<double>(count);
}
