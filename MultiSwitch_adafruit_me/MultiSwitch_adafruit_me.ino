// Spanish Moss Headboard
// Multi-switch

// ref: https://blog.adafruit.com/2009/10/20/example-code-for-multi-button-checker-with-debouncing/

#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>  // debounce library

//#define DEBOUNCE  10

#define PIXEL_PIN   11
#define PIXEL_COUNT 40

//#define BRAINPOWER 2
//#define NEOPOWER 3

#define SWITCHER    12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Instantiate a Bounce object
//Bounce debouncer = Bounce();

int nightLightsKB = 4;
int nightLightsLU = 5;
int white = 6;
int color = 7;
int rain = 8;
int chaser = 9;
//int seasonal = 10;

int count;
bool power;
int interval = 1000;
int lastButton, c;
int currentButton = 0;
long lastCheck;
bool lastValue, value;
static long lasttime = 0;

byte buttons[] = {nightLightsKB, nightLightsLU, white, color, rain, chaser};

#define NUMBUTTONS sizeof(buttons)
#define PIXEL_COUNT 40

// track if a button is just pressed, just releassed, or currently pressed
byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];


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
  //debouncer.attach(SWITCHER);
  //debouncer.interval(10);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  check_switches();
  /*
    for (int j = 0; j < NUMBUTTONS; j++) {
      Serial.print("Button  "); Serial.print(j); Serial.print(" is: ");
      Serial.println(digitalRead(buttons[j]));
      Serial.print("Pressed is: "); Serial.println(pressed[j]);
      delay(500);
    }
  */
  // Update the Bounce instance :
  //debouncer.update();

  // Get the updated value :
  //int value = debouncer.read();

  // do your light stuff
  c = switcher();
  for (byte i = 0; i < NUMBUTTONS; i++) {
    /*
    if (justpressed[i]) {

      Serial.print(i, DEC);
      Serial.println(" Just pressed");
      // remember, check_switches() will clear the "just pressed' flag
    }
    if (justreleased[i]) {
      Serial.print(i, DEC);
      Serial.println(" Just released");
    }
    */
    if (pressed[i] == 1) {
      Serial.print(i, DEC);
      Serial.println(" pressed");

      switch (i) {

        case  0  : // nightlightsKB
          c = switcher();
          colorWipe(strip.Color(0, 0, 0), 50);
          for (int j = 0; j < 11; j++) {
            strip.setPixelColor(j, 255, 255, 255);
            strip.show();
            delay(100);
          }
          c = switcher();

          if ( c == 1) {
            for (int j = 0; j < 11; j++) {    // softer light
              strip.setPixelColor(j, 80, 80, 80);
              strip.show();
              delay(100);

            }

          }
          break;  // nightLightsKB


        case 1:  //nightlightsLU
          c = switcher();
          colorWipe(strip.Color(0, 0, 0), 50);

          for (int j = PIXEL_COUNT; j > (PIXEL_COUNT - 11); j--) {
            strip.setPixelColor(j, 255, 255, 255);
            strip.show();
            delay(100);
          }
          c = switcher();
          if ( c == 1) {
            for (int j = PIXEL_COUNT; j > (PIXEL_COUNT - 11); j--) {
              strip.setPixelColor(j, 80, 80, 80);
              strip.show();
              delay(100);

            }
            //count = 0;
          }
          break;  // nightLights LU


        case 2:  // white
          c = switcher();

          colorWipe(strip.Color(255, 255, 255), 50);  // Super Bright White
          c = switcher();
          if (c == 1) {
            colorWipe(strip.Color(80, 80, 80), 50);  // White
          }
          c = switcher();
          if (c == 2) {
            colorWipe(strip.Color(20, 20, 20), 50);  // Dim white

          }
          break;  // white


        case 3:  // color

          colorWipe(strip.Color(126, 21, 179), 50);  // Purple
          c = switcher();
          if (c == 1) {
            colorWipe(strip.Color(0, 0, 127), 50);  // Blue
          }
          c = switcher();
          if (c == 2) {
            colorWipe(strip.Color(0, 0, 50), 50);  // Blue

          }
          break; // color

        case 4:  // rain
          c = switcher();
          rainbowCycle(20); //theaterChaseRainbow(50);
          // Update the Bounce instance :
          c = switcher();
          if (c == 1) {
            rainbow(80);
          }
          c = switcher();
          if (c == 2) {
            theaterChaseRainbow(50);

          }
          break;  // rainbow

        case 5:  // Chasesr
          c = switcher();
          theaterChase(strip.Color(127, 127, 127), 50); // White
          c = switcher();
          if (c == 1) {
            theaterChase(strip.Color(  0,   0, 127), 50); // Blue

          }

          break;  // chaser
          /*
          if( i == 6){  // Seasonal
          count = 0;
          }
           */

      } // switch
    } // if
  } //for



} // loop


void check_switches() {
  //static byte previousstate[NUMBUTTONS];
  //static byte currentstate[NUMBUTTONS];
  //static long lasttime;
  // byte index;

  int currentTime = millis();

  if (currentTime - lasttime > interval) {
    Serial.println("enough time has passed.");


    Serial.println("checking the light switches and storing presses");
    for (int i = 0; i < NUMBUTTONS; i++) {

      // Serial.print("Button number: ");Serial.println(buttons[i]);
      // Serial.print("The value of that pin is: "); Serial.println(digitalRead(buttons[i]));
      //switcher();
      if  ( (digitalRead(buttons[i])) == 0) { // pressed
        //count = 0;
        // switcher();
        if (i != lastButton) { // if we are a different button

          // reset last button pressed as 1
          pressed[lastButton] = 0; // not pressed
          Serial.print("the last button pressed now =  "); Serial.println(pressed[i]);
          // store the button with the current pressed
          pressed[i] = 1; // pressed
          Serial.print(i); Serial.println(" is pressed");
          //Serial.print("pressed "); Serial.print(i); Serial.print(" is "); Serial.println(pressed[i]);
          lastButton = i;
          // take a time stamp
          lasttime = millis();
        }


      }
      else if ( (digitalRead(buttons[i])) == 1) {
        pressed[i] = 0;
        Serial.print(i); Serial.println(" is not pressed");
      }
      delay(500);

    } // for

    /*
      if (currentTime - lasttime > interval) {
        lasttime = millis();
        // chug code


        /*
          if ( (lasttime + DEBOUNCE) < currentTime) { // switch the sign?
            // not enough time has passed to debounce
            return;
          }

          // ok we have debounced. reset the timer
          lasttime = millis();
          */
    /*
        for (index = 0; index < 3; index++) {
          justreleased[index] = 0;

          currentstate[index] = digitalRead(buttons[index]);

          if (currentstate[index] == previousstate[index]) {

            if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
              justpressed[index] = 1;
            }
            else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
              justreleased[index] = 1;
            }

            pressed[index] = !currentstate[index]; // digital HIGH means not pressed

          }
          Serial.println(pressed[index], DEC);
          previousstate[index] = currentstate[index];  // keep a running tally of the buttons
        }
      } // if we are ready

      */
  } // if interval
}

int switcher() {
  if (lastCheck - millis() > 1000) {
    value = digitalRead(SWITCHER);
    Serial.print("value is: "); Serial.println(value);
    if (value == 0 && lastValue == 1)  {
      count++;
      lastValue = value;
      lastCheck = millis();
    }
    else if (value == 1) {
      lastValue = value;
    }
    if (count > 2) {
      count = 0;
    }
  }
  Serial.print("count is: "); Serial.println(count);
  return count;
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
