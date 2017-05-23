// Spanish Moss Headboard
// Multi-switch  take 7 ---> THE FINAL VERSON
#include <Adafruit_NeoPixel.h>

//#define DEBOUNCE  10

#define PIXEL_PIN   11
#define PIXEL_COUNT 40

//#define BRAINPOWER 2
//#define NEOPOWER 3

#define SWITCHER    12

int nightLightsKB = 4;
int nightLightsLU = 5;
int white = 6;
int c0lor = 7;
int rain = 8;
int chaser = 9;
//int seasonal = 10;
int deck;

int count = 0;
int c, c2;
bool power;
int interval = 5000;
int lastButton = 1;
int currentButton = 0;
long lastCheck;
bool lastValue, value;
unsigned long lasttime = 0;

byte buttons[] = {nightLightsKB, nightLightsLU, white, c0lor, rain, chaser};

#define NUMBUTTONS sizeof(buttons)
#define PIXEL_COUNT 40

byte pressed[NUMBUTTONS];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  byte i;
  Serial.begin(9600);
  Serial.print("Button checker with: ");
  Serial.print(NUMBUTTONS, DEC);
  Serial.println(" buttons");

  for (i = 0; i < NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    //digitalWrite(buttons[i], HIGH);
  }

  pinMode(SWITCHER, INPUT_PULLUP);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  //Serial.println(digitalRead(buttons[0]));

  //unsigned long currentTime = millis();
  //if (currentTime - lasttime >= interval) {
  //Serial.println(" time to check the light switches and storing presses");

  // check the button switches:
  for (int i = 0; i < NUMBUTTONS; i++) {
    if  ( (digitalRead(buttons[i])) == 0) { // pressed

       // if we are a different button than last time:
      if (i != lastButton) {
        // then the last button's slot is now low, as it's no longer pressed
        pressed[lastButton] = 0; // not pressed
        count = 0;
        // store the button with the current pressed:
        pressed[i] = 1; // is pressed
        // Serial.print(i); Serial.println(" is pressed");
        lastButton = i;
        switch_lights(i, count);
        //Serial.print(i); Serial.print(","); Serial.println(count);
        deck = 0;
      }
      // if we are the same as last time:
      else if (i == lastButton) {
        deck = 0;
        //Serial.println("deck is zero'd out. Same button");
        pressed[i] = 1; // pressed

        // then read the switcher button:
        int value_ = digitalRead(SWITCHER);
        //Serial.print("value is: "); Serial.println(value_);
        if (value_ == 0 && lastValue == 1)  {
          count++;
          lastValue = value_;
          options(count);
          //lastCheck = millis();
        }
        else if (value_ == 1) {
          lastValue = value_;
        }
        if (count > 2) {
          count = 0;
          //switch_lights(i, count);
        }
        Serial.print("count is: "); Serial.println(count);

      } // if same button
    }
    else if ( (digitalRead(buttons[i])) == 1) {
      //for(int i = 0
      //pressed[i] = 0;
      //Serial.print("deck is: "); Serial.println(deck);
      deck++;
      // nothing is pressed, so everything should be off
      if ( deck >= 6) {
        // Serial.println("dark and Deck re-started");
        colorWipe(strip.Color(0, 0, 0), 50);
        deck = 0;
      }

    }
  } // for

  // if enough time has pasted, check the switcher button
  if (lastCheck - millis() > 300) {
    int value_ = digitalRead(SWITCHER);
    //Serial.print("value is: "); Serial.println(value_);
    if (value_ == 0 && lastValue == 1)  {
      count++;
      lastValue = value_;
      options(count);
      lastCheck = millis();
    }
    else if (value_ == 1) {
      lastValue = value_;
    }
    if (count > 2) {
      count = 0;
      options(count);
    }
  }
  //Serial.print("count is: "); Serial.println(count);
  //delay(500);

}


void options(int c) {
  for (byte i = 0; i < NUMBUTTONS; i++) {
    if (pressed[i] == 1) { // which switch is thrown
      switch (i) {
        case 0:  // nightlights
          if (c == 0) {
            //switch_lights(i, c);
            for (int j = 0; j < 11; j++) {
              strip.setPixelColor(j, 255, 255, 255);
              strip.show();
              delay(40);
            }
          }

          if ( c == 1) {
            //colorWipe(strip.Color(0, 0, 0), 50);
            for (int j = 0; j < 11; j++) {    // softer light
              strip.setPixelColor(j, 80, 80, 80);
              strip.show();
              delay(40);
            } // for loop illumination
          } // if c == 1

          if ( c == 2) {
            for (int j = 0; j < 11; j++) {    // softer light
              strip.setPixelColor(j, 20, 20, 20);
              strip.show();
              delay(40);
            } // for loop illumination
          }
          break;  // nightLightsKB

        case 1:  //nightlightsLU
          if ( c == 0) {
            //switch_lights(i, c);

            for (int j = PIXEL_COUNT; j > (PIXEL_COUNT - 11); j--) {
              strip.setPixelColor(j, 255, 255, 255);
              strip.show();
              delay(40);
            }

          }
          if ( c == 1) {
            //colorWipe(strip.Color(0, 0, 0), 50);
            for (int j = PIXEL_COUNT; j > (PIXEL_COUNT - 11); j--) {
              strip.setPixelColor(j, 80, 80, 80);
              strip.show();
              delay(40);
            }
          }

          if (c == 2) {
            for (int j = PIXEL_COUNT; j > (PIXEL_COUNT - 11); j--) {
              strip.setPixelColor(j, 20, 20, 20);
              strip.show();
              delay(40);
            } // Dim white
          }
          break;  // nightLights LU


        case 2:  // white
          if (c == 0) {
            //switch_lights(i, c);
            colorWipe(strip.Color(255, 255, 255), 50); 
          }
          if (c == 1) {
            colorWipe(strip.Color(80, 80, 80), 50);  // White
          }
          if (c == 2) {
            colorWipe(strip.Color(20, 20, 20), 50);  // Dim white
          }
          break;  // white


        case 3:  // color
          if ( c == 0) {
            //switch_lights(i, c);
             colorWipe(strip.Color(126, 21, 179), 50);  // Purple

          }
          if (c == 1) {
            colorWipe(strip.Color(0, 0, 127), 50);  // Blue

          }
          if (c == 2) {
            colorWipe(strip.Color(0, 0, 50), 50);  // Blue
          }
          break; // color


        case 4:  // rain
          if (c == 0) {
            //switch_lights(i, c);
             rainbowCycle(20);
          }
          if (c == 1) {
            rainbow(80);
          }
          if (c == 2) {
            theaterChaseRainbow(50);
          }
          break;  // rainbow

        case 5:  // Chasesr
          if (c == 0) {
            //switch_lights(i, c);
            theaterChase(strip.Color(127, 127, 127), 50); // White
          }
          if (c == 1) {
            theaterChase(strip.Color(  0,   0, 127), 50); // Blue
          }
          break;  // chaser

      } // switch
    } // if pressed

  } // for loops

} // options



void switch_lights(int whichSwitch, int c) {
  //Serial.print("I'm going to switch: "); Serial.println(whichSwitch);
  //Serial.print("C is:  "); Serial.println(c);

  switch (whichSwitch) {

    case  0  : // nightlightsKB
      if (c == 0) {
        colorWipe(strip.Color(0, 0, 0), 50);
        for (int j = 0; j < 11; j++) {
          strip.setPixelColor(j, 255, 255, 255);
          strip.show();
          delay(40);
        }
      }
      break;  // nightLightsKB


    case 1:  //nightlightsLU
      if ( c == 0) {
        colorWipe(strip.Color(0, 0, 0), 50);
        for (int j = PIXEL_COUNT; j > (PIXEL_COUNT - 11); j--) {
          strip.setPixelColor(j, 255, 255, 255);
          strip.show();
          delay(40);
        }
      }
      break;  // nightLights LU


    case 2:  // white
      if (c == 0) {
        colorWipe(strip.Color(255, 255, 255), 50);  // Super Bright White
      }
      break;  // white


    case 3:  // color
      if ( c == 0) {
        colorWipe(strip.Color(126, 21, 179), 50);  // Purple

      }
      break; // color

    case 4:  // rain
      if (c == 0) {
        rainbowCycle(20); //theaterChaseRainbow(50);
      }
      break;  // rainbow

    case 5:  // Chasesr
      if (c == 0) {
        theaterChase(strip.Color(127, 127, 127), 50); // White
      }
      break;  // chaser


      /* case 6: // Seasonal
       *  if( c = 0){
       *  }

       */

  } // switch


}


// pixel animations


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
