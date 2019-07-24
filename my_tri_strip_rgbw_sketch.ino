#include <Adafruit_NeoPixel.h>

#define SENSOR_PIN 2
#define STRIP1_PIN 4
#define STRIP2_PIN 5
#define STRIP3_PIN 6
#define LED_PIN 13
#define LDR_PIN A0
#define LONG_TOUCH_DURATION 1200

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(5, STRIP1_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(5, STRIP2_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(5, STRIP3_PIN, NEO_GRBW + NEO_KHZ800);

// IMPORTANT: Avoid connecting on a live circuit...if you must, connect GND first.

char rxChar= 0;

volatile byte touched = LOW;
volatile byte strip_on = LOW;

long last_touch_time = 0;

int showType = 4;
int processInt = 0;
int showSwitched = 0;

void touch_ISR() {
  touched = !touched;
  digitalWrite(LED_PIN, touched);

  if (touched) {
    last_touch_time = millis();
  } else {
    if (millis() - last_touch_time > LONG_TOUCH_DURATION ) {
       // switch to next showType
       showType++;
       //if (showType > 10) showType=1;
       processInt = 1;
    } else { 
        if (!showSwitched) {
          strip_on = !strip_on;
          processInt = 1;
        }
        showSwitched = 0;
    }
  }

}

void printHelp(void){
  Serial.println("--- Command list: ---");
  Serial.println("? -> Print this HELP");  
  Serial.println("0 -> LEDs OFF");
  Serial.println("1-9,A-F -> Effects");
  Serial.println("Z -> Custom: NStrip,NLED,R,G,B,W ");
  }

void setup() {

//  analogReference(INTERNAL);

  pinMode(LED_PIN, OUTPUT);
//  pinMode(SENSOR_PIN, INPUT);

//  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), touch_ISR, CHANGE);
  
  strip1.begin();
  strip1.setBrightness(100); //adjust brightness here
  strip1.show(); // Initialize all pixels to 'off'

  strip2.begin();
  strip2.setBrightness(100); //adjust brightness here
  strip2.show(); // Initialize all pixels to 'off'

  strip3.begin();
  strip3.setBrightness(100); //adjust brightness here
  strip3.show(); // Initialize all pixels to 'off'

  Serial.begin(115200);
  Serial.flush();
  printHelp();

  strip_on = HIGH;

}

void loop() {
  if (strip_on) {
    startShow(showType);
  } else {
    colorWipe(strip1, strip1.Color(0, 0, 0, 0), 20);
    colorWipe(strip2, strip2.Color(0, 0, 0, 0), 20);
    colorWipe(strip3, strip3.Color(0, 0, 0, 0), 20);
  }

  processInt = 0;
  if (digitalRead(SENSOR_PIN) != touched) {
    touched = digitalRead(SENSOR_PIN);
    digitalWrite(LED_PIN, touched);
  }

  if (touched && millis() - last_touch_time > LONG_TOUCH_DURATION) {
    last_touch_time = millis();
    showType++;
    processInt = 1;
    showSwitched = 1;
  }

  if (Serial.available()>0 ) {
    rxChar = Serial.read();
    switch (rxChar) {
      case '0':
          showType = 0;
          Serial.println("Got command: 0");
          break;
      case '1':
          showType = 1;
          Serial.println("Got command: 1");
          break;
      case '2':
          showType = 2;
          Serial.println("Got command: 2");
          break;
      case '3':
          showType = 3;
          Serial.println("Got command: 3");
          break;
      case '4':
          showType = 4;
          Serial.println("Got command: 4");
          break;
      case '5':
          showType = 5;
          Serial.println("Got command: 5");
          break;
      case '6':
          showType = 6;
          Serial.println("Got command: 6");
          break;
      case '7':
          showType = 7;
          Serial.println("Got command: 7");
          break;
      case '8':
          showType = 8;
          Serial.println("Got command: 8");
          break;
      case '9':
          showType = 9;
          Serial.println("Got command: 9");
          break;
      case 'a':
      case 'A':
          showType = 10;
          Serial.println("Got command: A");
          break;
      case 'b':
      case 'B':
          showType = 11;
          Serial.println("Got command: B");
          break;
      case 'c':
      case 'C':
          showType = 12;
          Serial.println("Got command: C");
          break;
      case 'd':
      case 'D':
          showType = 13;
          Serial.println("Got command: D");
          break;
      case 'e':
      case 'E':
          showType = 14;
          Serial.println("Got command: E");
          break;
      case 'f':
      case 'F':
          showType = 15;
          Serial.println("Got command: F");
          break;
      case 'g':
      case 'G':
          showType = 16;
          Serial.println("Got command: G");
          break;
      case 'z':
      case 'Z':
          showType = 99;
          Serial.println("Got command: Z");
          Serial.println("Input format: NStrip[1-3],NLED[1-5],R[0-255],G[0-255],B[0-255],W[0-255] ");
          Serial.println(" --- Not available yet. WorkInProgress ---");
          break;
      case '+':
          Serial.println("Got command: +");
          if (showType < 16) {
            showType = showType + 1;
          }
          Serial.print("Show: ");
          Serial.println(showType);
          break;
      case '-':
          Serial.println("Got command: -");
          if (showType > 0) {
            showType = showType - 1;
          }
          Serial.print("Show: ");
          Serial.println(showType);
          break;
      case '?':
          Serial.println("Got command: ?");
          printHelp();
          break;
      default:                           
          Serial.print("'");
          Serial.print((char)rxChar);
          Serial.println("' is not a command!");
    }
    processInt = 1;
  }
}

void startShow(int i) {
  switch(i){
    case 0: colorWipe(strip1, strip1.Color(0, 0, 0, 0), 10);    // Black/off
            colorWipe(strip2, strip2.Color(0, 0, 0, 0), 10);    // Black/off
            colorWipe(strip3, strip3.Color(0, 0, 0, 0), 10);    // Black/off
            break;
    case 1: colorWipe(strip1, strip1.Color(255, 0, 0, 0), 50);  // Red
            colorWipe(strip2, strip2.Color(255, 0, 0, 0), 50);  // Red
            colorWipe(strip3, strip3.Color(255, 0, 0, 0), 50);  // Red
            break;
    case 2: colorWipe(strip1, strip1.Color(0, 255, 0, 0), 50);  // Green
            colorWipe(strip2, strip2.Color(0, 255, 0, 0), 50);  // Green
            colorWipe(strip3, strip3.Color(0, 255, 0, 0), 50);  // Green
            break;
    case 3: colorWipe(strip1, strip1.Color(0, 0, 255, 0), 50);  // Blue
            colorWipe(strip2, strip2.Color(0, 0, 255, 0), 50);  // Blue
            colorWipe(strip3, strip3.Color(0, 0, 255, 0), 50);  // Blue
            break;
    case 4: colorWipe(strip1, strip1.Color(0, 0, 0, 255), 50);  // White
            colorWipe(strip2, strip2.Color(0, 0, 0, 255), 50);  // White
            colorWipe(strip3, strip3.Color(0, 0, 0, 255), 50);  // White
            break;
    case 5: colorWipe(strip1, strip1.Color(255, 255, 0, 0), 50); //Yellow
            colorWipe(strip2, strip2.Color(0, 0, 255, 0), 50); //Blue
            colorWipe(strip3, strip3.Color(0, 0, 255, 0), 50); //Blue
            break;
    case 6: colorWipe(strip1, strip1.Color(255, 255, 0, 0), 50);  // R + G (Yellow)
            colorWipe(strip2, strip2.Color(255, 255, 0, 0), 50);  // R + G
            colorWipe(strip3, strip3.Color(255, 255, 0, 0), 50);  // R + G
            break;
    case 7: colorWipe(strip1, strip1.Color(0, 255, 255, 0), 50);  // G + B
            colorWipe(strip2, strip2.Color(0, 255, 255, 0), 50);  // G + B
            colorWipe(strip3, strip3.Color(0, 255, 255, 0), 50);  // G + B
            break;
    case 8: colorWipe(strip1, strip1.Color(255, 0, 255, 0), 50);  // R + B
            colorWipe(strip2, strip2.Color(255, 0, 255, 0), 50);  // R + B
            colorWipe(strip3, strip3.Color(255, 0, 255, 0), 50);  // R + B
            break;
    case 9: theaterChase(strip1.Color(255,   0,   0,   0), 50); // Red
            theaterChase(strip2.Color(255,   0,   0,   0), 50); // Red
            theaterChase(strip3.Color(255,   0,   0,   0), 50); // Red
            break;
    case 10: theaterChase(strip1.Color(  0, 255,   0,   0), 50); // Green
            theaterChase(strip2.Color(  0, 255,   0,   0), 50); // Green
            theaterChase(strip3.Color(  0, 255,   0,   0), 50); // Green
            break;
    case 11: theaterChase(strip1.Color(  0,   0, 255,   0), 50); // Blue
            theaterChase(strip2.Color(  0,   0, 255,   0), 50); // Blue
            theaterChase(strip3.Color(  0,   0, 255,   0), 50); // Blue
            break;
    case 12: theaterChase(strip1.Color(0, 0, 0, 255), 50); // White
            theaterChase(strip2.Color(0, 0, 0, 255), 50); // White
            theaterChase(strip3.Color(0, 0, 0, 255), 50); // White
            break;
    case 13: rainbow(20);
            break;
    case 14: rainbowCycle(20);
            break;
    case 15: theaterChaseRainbow(50);
            break;
    case 16: colorWipe(strip1, strip1.Color(255, 255, 255, 255), 50);  // White
            colorWipe(strip2, strip2.Color(255, 255, 255, 255), 50);  // White
            colorWipe(strip3, strip3.Color(255, 255, 255, 255), 50);  // White
            break;
    case 17: showType = 1;
            break;
    case 99: showType = 99;
            break;
  }
}



// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel &theStrip,uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<theStrip.numPixels(); i++) {
    if (processInt) return;
    theStrip.setPixelColor(i, c);
    theStrip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    if (processInt) return;
    for(i=0; i<strip1.numPixels(); i++) {
      if (processInt) return;
      strip1.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip1.show();

    for(i=0; i<strip2.numPixels(); i++) {
      if (processInt) return;
      strip2.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip2.show();
    
    for(i=0; i<strip3.numPixels(); i++) {
      if (processInt) return;
      strip3.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip3.show();

    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    if (processInt) return;
    for(i=0; i< strip1.numPixels(); i++) {
      if (processInt) return;
      strip1.setPixelColor(i, Wheel(((i * 256 / strip1.numPixels()) + j) & 255));
    }
    strip1.show();
    for(i=0; i< strip2.numPixels(); i++) {
      if (processInt) return;
      strip2.setPixelColor(i, Wheel(((i * 256 / strip2.numPixels()) + j) & 255));
    }
    strip2.show();
    for(i=0; i< strip3.numPixels(); i++) {
      if (processInt) return;
      strip3.setPixelColor(i, Wheel(((i * 256 / strip3.numPixels()) + j) & 255));
    }
    strip3.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    if (processInt) return;
    for (int q=0; q < 3; q++) {
      if (processInt) return;
      for (int i=0; i < strip1.numPixels(); i=i+3) {
        if (processInt) return;
        strip1.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip1.show();

      for (int i=0; i < strip2.numPixels(); i=i+3) {
        if (processInt) return;
        strip2.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip2.show();

      for (int i=0; i < strip3.numPixels(); i=i+3) {
        if (processInt) return;
        strip3.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip3.show();

      delay(wait);

      for (int i=0; i < strip1.numPixels(); i=i+3) {
        if (processInt) return;
        strip1.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      for (int i=0; i < strip2.numPixels(); i=i+3) {
        if (processInt) return;
        strip2.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      for (int i=0; i < strip3.numPixels(); i=i+3) {
        if (processInt) return;
        strip3.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

/*
//Theatre-style crawling lights.
void theaterChaseDual(uint32_t c1, uint32_t c2, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    if (processInt) return;
    for (int q=0; q < 3; q++) {
      if (processInt) return;
      for (int i=0; i < strip1.numPixels(); i=i+3) {
        if (processInt) return;
        strip1.setPixelColor(i+q, c1);    //turn every third pixel on
      }
      strip1.show();

      for (int i=0; i < strip2.numPixels(); i=i+3) {
        if (processInt) return;
        strip2.setPixelColor(i+q, c2);    //turn every third pixel on
      }
      strip2.show();

      delay(wait);

      for (int i=0; i < strip1.numPixels(); i=i+3) {
        if (processInt) return;
        strip1.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      for (int i=0; i < strip2.numPixels(); i=i+3) {
        if (processInt) return;
        strip2.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
*/

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    if (processInt) return;
    for (int q=0; q < 3; q++) {
      if (processInt) return;
      for (int i=0; i < strip1.numPixels(); i=i+3) {
        if (processInt) return;
        strip1.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip1.show();

      for (int i=0; i < strip2.numPixels(); i=i+3) {
        if (processInt) return;
        strip2.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip2.show();

      for (int i=0; i < strip3.numPixels(); i=i+3) {
        if (processInt) return;
        strip3.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip3.show();

      delay(wait);

      for (int i=0; i < strip1.numPixels(); i=i+3) {
        if (processInt) return;
        strip1.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      for (int i=0; i < strip2.numPixels(); i=i+3) {
        if (processInt) return;
        strip2.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      for (int i=0; i < strip3.numPixels(); i=i+3) {
        if (processInt) return;
        strip3.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
