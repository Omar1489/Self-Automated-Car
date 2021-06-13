#include <Wire.h>
#include <Adafruit_GFX.h>        
#include <Adafruit_TFTLCD.h>  
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_liter44al) string_literal
#endif

#define YP A3  
#define XM A2  
#define YM 9   
#define XP 8   

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 60
#define PENRADIUS 3
int oldcolor;
 float f1 ;
float currentFreq = 95.4;
String freq = "";
String sender = "";

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  
  tft.reset();
  
  uint16_t identifier = tft.readID();

  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.4\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);

  tft.fillScreen(WHITE);

  tft.fillRect(90, 15, BOXSIZE, BOXSIZE, BLACK);
  tft.fillRect(90, 235, BOXSIZE, BOXSIZE, BLACK);

tft.fillRect(0, 15, BOXSIZE, BOXSIZE, GREEN);
tft.fillRect(180, 15, BOXSIZE, BOXSIZE, RED);


 // tft.fillRect(115, 20, 10, 50, WHITE);
 tft.fillRect(95, 40, 50, 10, WHITE);
  
  tft.fillRect(115, 240, 10, 50, WHITE);
  
  tft.fillRect(95, 260, 50, 10, WHITE);

  
 
     
 
 
  pinMode(13, OUTPUT);
}

void loop() {
  
  
   screen();
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
 
  freq = String(sender); 
 // Serial.print(freq);
  char buffer[32];
 freq.toCharArray(buffer, 32);

  Wire.write(buffer); // respond with message of 6 bytes
  // as expected by master
}

void screen(){
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);


  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  
 /* if(p.x >= 500 && p.x <= 512 && p.y >= 796 && p.y<=780){
  currentFreq = currentFreq-0.1;   
  }*/
  /*currentFreq = currentFreq+0.1;*/  
  if (p.z > ts.pressureThreshhold) {
     if(p.x >= 410 && p.x <= 624 && p.y >= 710 && p.y<=860){
     
  freqDOWN(); 
  delay(300);
     }
      else if(p.x >= 410 && p.x <= 624 && p.y >= 137 && p.y<=283){
   
  freqUP();  
  delay(300);
     } else if(p.x >= 727 && p.x <= 916 && p.y >= 710 && p.y<=860){
    sender = '/'; 
   
  delay(300);
     }
     else if(p.x >= 110 && p.x <= 310 && p.y >= 710 && p.y<=860){
    sender = '*'; 
   
  delay(300);
     }
     Serial.print("X = "); Serial.print(p.x);
     Serial.print("\tY = "); Serial.print(p.y);
     Serial.print("\tPressure = "); Serial.println(p.z);
     
  }else{
      sender= '0'; 
     }
     requestEvent();
  tft.fillRect(0, 80, 200, 100, WHITE);
  tft.setCursor(70, 150);
      tft.setTextSize(3);
      tft.setTextColor(BLACK);
   tft.println(currentFreq);
 delay(100);
  
}

void freqUP(){
  sender = '+'; 
  if(currentFreq == 90.90){
    currentFreq = 92.10;
  }
  else if(currentFreq == 92.10){
    currentFreq = 95.00;
  }else if(currentFreq == 95.00){
    currentFreq = 95.40;
  }else if(currentFreq == 95.40){
    currentFreq = 95.90;
  }else if(currentFreq == 95.90){
    currentFreq = 98.20;
  }else if(currentFreq == 98.20){
    currentFreq = 98.80;
  }else if(currentFreq == 98.80){
    currentFreq = 100.60;
  }else if(currentFreq == 100.60){
    currentFreq = 102.20;
  }else if(currentFreq == 102.20){
    currentFreq = 104.20;
  }else if(currentFreq == 104.20){
    currentFreq = 105.30;
  }else if(currentFreq == 105.30){
    currentFreq = 105.80;
  }else if(currentFreq == 105.80){
    currentFreq = 106.30;
  }else if(currentFreq == 106.30){
    currentFreq = 107.40;
  }
}
void freqDOWN(){
   sender = '-'; 
  if(currentFreq == 92.10){
    currentFreq = 90.90;
  }else if(currentFreq == 95.00){
    currentFreq = 92.10;
  }else if(currentFreq == 95.40){
    currentFreq = 95.00;
  }else if(currentFreq == 95.90){
    currentFreq = 95.40;
  }else if(currentFreq == 98.20){
    currentFreq = 95.90;
  }else if(currentFreq == 98.80){
    currentFreq = 98.20;
  }else if(currentFreq == 100.60){
    currentFreq = 98.80;
  }else if(currentFreq == 102.20){
    currentFreq = 100.60;
  }else if(currentFreq == 104.20){
    currentFreq = 102.20;
  }else if(currentFreq == 105.30){
    currentFreq = 104.20;
  }else if(currentFreq == 105.80){
    currentFreq = 105.30;
  }else if(currentFreq == 106.30){
    currentFreq = 105.80;
  }else if(currentFreq == 107.40){
    currentFreq = 106.30;
  }
}
