#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library
#include <TouchScreen.h>   // Touchscreen library

#include <FreeDefaultFonts.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

//Touch For New ILI9341 TP
#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
//TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define MINPRESSURE 10
#define MAXPRESSURE 1000

//Zero Button
#define ZERO_L  120
#define ZERO_W  80
#define ZERO_X  180
#define ZERO_Y  250
#define BUTTON_TEXT_X 195
#define BUTTON_TEXT_Y 270

//Length Text
#define TEXT_X 90
#define TEXT_Y 75

MCUFRIEND_kbv tft;

constexpr byte CALIBRATE_COMMAND = B11111111;
constexpr byte DATA_RECEIVE_COMMAND = B11111110;

bool isNewNumber = false;
char number[9];
String oldNum = "";

void setup() {
    Serial.begin(115200);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    
    //setup screen
    uint16_t ID = tft.readID();
    if (ID == 0xD3) ID = 0x9481;
    tft.begin(ID);
    tft.setRotation(1);

    //Draw zero button
    tft.fillScreen(BLACK);
    tft.setTextColor(BLACK); 
    tft.setTextSize(4);
    tft.fillRect(ZERO_X, ZERO_Y, ZERO_L, ZERO_W, YELLOW);
    tft.setCursor(BUTTON_TEXT_X, BUTTON_TEXT_Y);
    tft.print ("ZERO");
    
    //set screen and font
    tft.setTextColor(WHITE);
    tft.setTextSize(6);
    tft.setCursor(TEXT_X,TEXT_Y);
    
}

void loop() {
    
//    TSPoint p = ts.getPoint();
//
//    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) { 
//        int temp_py = p.y;
//
//        // scale touch screen coordinates to actual coordinates
//        p.y = map(p.x, TS_MINX, TS_MAXX, tft.height(), 0);
//        p.x = map(temp_py, TS_MINY, TS_MAXY, tft.width(), 0);
//
//        //Check if button has been pressed
//        if (p.x < (ZERO_X + ZERO_L) && p.x > ZERO_X && p.y < (ZERO_Y+ZERO_W) && p.y > ZERO_Y) {
//            calibrate();
//        }
//    }

    if (isNewNumber){
      tft.fillRect(TEXT_X, TEXT_Y, 300, 50, BLACK); //Clear old text with a black rectangle
      tft.setCursor(TEXT_X,TEXT_Y);
      tft.setTextColor(WHITE);
      tft.setTextSize(6);
      Serial.println(number);
      tft.print(number);
      isNewNumber = false;
    }

    
}

void serialEvent(){
    if(Serial.available() >= 10 && Serial.read() == DATA_RECEIVE_COMMAND){
      for(int i = 0; i < 8; i++){
        number[i] = Serial.read();
      }
      Serial.read();
      String newNum(number);
      isNewNumber = !newNum.equals(oldNum);
      oldNum = newNum;
    }
}

void calibrate(){
    Serial.write(CALIBRATE_COMMAND);
}
