double offset = 0;
constexpr int GEAR_RATIO = 9;
constexpr int PITCH = 16;
constexpr byte CALIBRATE_COMMAND = B11111111;
constexpr byte DATA_RECEIVE_COMMAND = B11111110;

void setup() {
    Serial.begin(9600);
    pinMode(A0, INPUT);
    analogReadResolution(12);
}

void loop() {
    double currentPosition = getCalibratedPosition();
    byte byteArr[4];
    doubleToByteArray(currentPosition, byteArr);
//    sendDataToDisplay(byteArr);
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
    long rawPotReading = analogRead(A0);
    double potReading = runningAverage(rawPotReading);
    double mappedReading = 0.00299615382989385 * potReading;
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

double fmap(long x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double runningAverage(long M)
{
// #define LMSIZE 50
#define LMSIZE 255
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
