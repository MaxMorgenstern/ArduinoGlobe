#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// --------- PINs
int ledPin = 2;
int rumblePin = 7;

// --------- STATUS
bool enableLEDAttract = false;
bool enableLEDAction = false;
bool enableBuzzer = false;

// --------- CONST
int buzzerDuration = 500;
int buzzerDurationRunner = 0;

// --------- DIV
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(12, ledPin, NEO_GRB + NEO_KHZ800);


void setup() {


  pinMode(rumblePin, OUTPUT);
    
  // Init LEDs
  ledStrip.begin();
  ledStrip.setBrightness(50);
  ledStrip.show();

  // Init Serial Communication
  Serial.begin(115200);
}

void loop() {
  ReadSerialData();
  
  if(enableLEDAttract) {
    theaterChase(ledStrip.Color(127, 127, 127), 50);
    colorWipe(ledStrip.Color(0, 0, 0), 0);
    enableLEDAttract = false;
  }
  if(enableLEDAction) {
    rainbow(20);
    colorWipe(ledStrip.Color(0, 0, 0), 0);
    enableLEDAction = false;
  }
  
  if(enableBuzzer) {
    buzzerDurationRunner = millis();
    enableBuzzer = false;
  }
  if(buzzerDurationRunner+buzzerDuration <= millis()) {
    digitalWrite(rumblePin, LOW);
  } else {
    digitalWrite(rumblePin, HIGH);
  }
}



void ReadSerialData() {
  String data;
  if (Serial.available() > 0) {
    data = Serial.readStringUntil(';');
    Serial.println(data);
    
    if (data.equals("attract")) {
      enableLEDAttract = true;
    }
    if (data.equals("action")) {
      enableLEDAction = true;
    }
    if (data.equals("buzz")) {
      enableBuzzer = true;
    }
  }
}





// --------- NEO Pixel Default

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<ledStrip.numPixels(); i++) {
    ledStrip.setPixelColor(i, c);
    ledStrip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<ledStrip.numPixels(); i++) {
      ledStrip.setPixelColor(i, Wheel((i+j) & 255));
    }
    ledStrip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< ledStrip.numPixels(); i++) {
      ledStrip.setPixelColor(i, Wheel(((i * 256 / ledStrip.numPixels()) + j) & 255));
    }
    ledStrip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < ledStrip.numPixels(); i=i+3) {
        ledStrip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      ledStrip.show();

      delay(wait);

      for (uint16_t i=0; i < ledStrip.numPixels(); i=i+3) {
        ledStrip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < ledStrip.numPixels(); i=i+3) {
        ledStrip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      ledStrip.show();

      delay(wait);

      for (uint16_t i=0; i < ledStrip.numPixels(); i=i+3) {
        ledStrip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return ledStrip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return ledStrip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ledStrip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
