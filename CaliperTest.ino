#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

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
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#define MINPRESSURE 10
#define MAXPRESSURE 1000

//Zero Button
#define ZERO_L  100
#define ZERO_W  60
#define ZERO_X  110
#define ZERO_Y  180

//Length Text
#define TEXT_X 20
#define TEXT_Y 60

#define POT   A5
int potValue = 0;
int zeroValue = 0;
double newLength = 0;
double oldLength = 0;

void setup() {
  pinMode(POT, INPUT);
  Serial.begin(9600);
  tft.reset();
  
  uint16_t identifier = tft.readID();
  if(identifier != 0x9325 || identifier != 0x9328 || identifier != 0x4535 || identifier != 0x7575 || identifier != 0x9341 || identifier != 0x8357 || identifier==0x0101)
  {    
      identifier=0x9341;
  }
  tft.begin(identifier);
  tft.setRotation(1);

  tft.fillScreen(BLACK);

  tft.setTextColor(BLACK); 
  tft.setTextSize(4);
  tft.fillRect(ZERO_X, ZERO_Y, ZERO_L, ZERO_W, YELLOW);
  tft.setCursor(114, 196);
  tft.print ("ZERO");
  tft.setTextSize(5);
  tft.setTextColor(WHITE);
  tft.setCursor(240, TEXT_Y);
  tft.print ("mm");
}

void loop() {
  potValue = 4*analogRead (POT); 

  TSPoint p = ts.getPoint();

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) { 
    int temp_py = p.y;
    
    // scale touch screen coordinates to actual coordinates
    p.y = map(p.x, TS_MINX, TS_MAXX, tft.height(), 0);
    p.x = map(temp_py, TS_MINY, TS_MAXY, tft.width(), 0);

    //Check if button has been pressed
    if (p.x < (ZERO_X + ZERO_L) && p.x > ZERO_X && p.y < (ZERO_Y+ZERO_W) && p.y > ZERO_Y) {
      zeroValue = potValue;
      Serial.println (zeroValue);
    }
  }
  
  newLength = 0.03488 * (potValue - zeroValue);

  if (newLength != oldLength){
    tft.setCursor(TEXT_X, TEXT_Y);
    tft.fillRect(TEXT_X, TEXT_Y, 210, 35, BLACK); //Clear old text with a black rectangle
  
    //Pad with appropriate number of spaces depending on the size of the number
    if (newLength >= 100)
      tft.print (" ");
    else if (newLength < 100 && newLength >= 10)
      tft.print ("  ");
    else if (newLength < 10 && newLength >= 0)
      tft.print ("   ");
    else{
      newLength = newLength*-1;
      tft.print ("-  ");  
    }
  }
  tft.print (newLength);

  oldLength = newLength;
}
