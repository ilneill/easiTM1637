/*!
 * TM1637 Example with a TM1637 based 6-digit (+dps) LED display module.
 *
 * Written for the Arduino Uno/Nano/Mega.
 * (c) Ian Neill 2025
 * GPL(v3) Licence
 *
 * Built on my TM1651 library demo for the LEDC68 LED module.
 * ... with some developments backported into this TM1637 demo from my TM1638 and MAX7219 demos.
 *
 * ****************************
 * *  easiTM1637 Demo Sketch  *
 * ****************************
 */

#include "easiTM1637.h"

#define ON          HIGH
#define OFF         LOW

// Pin definitions for the TM1637 - the interface might look like I2C, but it is not!
#define CLKPIN      2                                     // Clock.
#define DIOPIN      3                                     // Data In/Out.
#define LEDPIN      13                                    // The builtin LED.

// The number of digits in the TM1637 based LED display.
#define NUMDIGITS   6

// A table to describe the physical to logical digit numbering.
// This can be determined the when this demo first runs with the default map.
uint8_t tmDigitMap[NUMDIGITS] = {2, 1, 0, 5, 4, 3};

// Instantiate a TM1637 display.
TM1637 myDisplay(CLKPIN, DIOPIN);                         // Set clock and data pins.

void setup() {
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, OFF);
  Serial.begin(9600);
  // TM1637 auto addressing mode, or fixed addressing mode and the default digit map.
  // Digits = 6, Brightness = 2, Display cleared (all segments OFF and decimal points OFF).
  //myDisplay.begin(NUMDIGITS, INTENSITY_TYP37);
  // TM1637 fixed addressing mode using a supplied digit map.
  // Digits = 6, Brightness = 2, Display cleared (all segments OFF and decimal points OFF).
  myDisplay.begin(tmDigitMap, NUMDIGITS, INTENSITY_TYP37);
  Serial.println("\nDisplay physical to logical mapping test.");
  findDigitMap();
  Serial.println("\nDisplay brightness and digit tests.");
  testDisplay();
  blinkLED(100);
  delay(1000);
}

void loop() {
  unsigned long timeNow;                                  // Used to time the 10 and 20 min delay demos.
  
  // 8-bit hex number count up, 0x00 - 0xFF, 1 count/250ms.
  Serial.println("Demo 1: 8-bit hex count up.");
  countHex8(250);
  blinkLED(100);
  delay(1000);
  
  // 12-bit hex number count up, 0x000 - 0xFFF, 1 count/125ms.
  Serial.println("Demo 2: 12-bit hex count up.");
  countHex12(125);
  blinkLED(100);
  delay(1000);
  
  // 16-bit hex number count up, 0x0000 - 0xFFFF, 1 count/125ms.
  Serial.println("Demo 3: 16-bit hex count up.");
  countHex16(125);
  blinkLED(100);
  delay(1000);
  
  // Decimal number count up, 0 - 9999, 1 count/100ms.
  Serial.println("Demo 4: 9999 decimal count up.");
  countUp(9999, 100);
  blinkLED(100);
  delay(1000);
  
  // Decimal number count down, 9999 - 0, 1 count/500ms.
  Serial.println("Demo 5: 9999 decimal count down.");
  countDown(9999, 500);
  blinkLED(100);
  delay(1000);
  
  // A 10 minute timer.
  Serial.print("Demo 6: A 10 minute delay() timer: ");
  timeNow = millis();
  countXMins(10);
  Serial.print(millis() - timeNow);
  Serial.println("ms");
  blinkLED(100);
  delay(1000);
  
  // A 20 minute timer with flashing decimal point.
  Serial.print("Demo 7: A 20 minute millis() timer & flashing decimal point: ");
  timeNow = millis();
  countXMinsDP(20);
  Serial.print(millis() - timeNow);
  Serial.println("ms");
  blinkLED(100);
  delay(1000);
}

// Flash the builtin LED to signal the end of a demo stage.
void blinkLED(uint32_t interval) {
  digitalWrite(LEDPIN, ON);
  delay(interval);
  digitalWrite(LEDPIN, OFF);
}

void findDigitMap() {
  byte digit, counter;
  // Map the digits, physical to logical.
  for(digit = 0; digit < NUMDIGITS; digit++) {
    myDisplay.displayChar(digit, digit);
    delay(1000);
  }
  // Pulse the decimal points as a bar, giving some time to note down the number on the TM1637 based LED display.
  for(counter = 0; counter < NUMDIGITS; counter++) {
    for(digit = 0; digit < NUMDIGITS; digit++) {
      myDisplay.displayDP(digit, ON);
      delay(100);
    }
    for(digit = 0; digit < NUMDIGITS; digit++) {
      myDisplay.displayDP((NUMDIGITS - digit - 1), OFF);
      delay(100);
    }
  }
  // Ensure we clear the display as we leave the digit mapping function.
  myDisplay.displayClear();
}

void testDisplay() {
  byte counter, brightness, character;
  // Display brightness test.
  for(brightness = INTENSITY_MIN37; brightness <= INTENSITY_MAX37; brightness++) {
    myDisplay.displayBrightness(brightness);
    for(counter = 0; counter < NUMDIGITS; counter++) {
      myDisplay.displayChar(counter, brightness);
    }
    delay(1000);
  }
  // Clear the display and set the brightness to the typical value.
  myDisplay.displayClear();
  myDisplay.displayBrightness(INTENSITY_TYP37);
  // Cycle through each code in the character table, deliberately exceeding the table size by 1 to finish on a default space (0x00).
  for(character = 0; character <= myDisplay.charTableSize; character++) {
    // Display all characters on all display digits.
    for(counter = 0; counter < NUMDIGITS; counter++) {
      myDisplay.displayChar(counter, character);
    }
    delay(500);
  }
  // Decimal point ON/OFF test for each display digit.
  myDisplay.displayClear();
  for(counter = 0; counter < NUMDIGITS; counter++) {
    myDisplay.displayDP(counter, ON);
    delay(250);
    myDisplay.displayDP(counter, OFF);
    delay(250);
  }
  // All segments and decimal points ON, then restored.
  myDisplay.displayTest(true);
  delay(2000);
  myDisplay.displayTest(false);
  // Ensure we clear the display (+dps) as we leave the display test function.
  myDisplay.displayClear();
}

void countHex8(uint32_t interval) {
  byte counter = 0;
  myDisplay.displayChar(0, 0x12);                         // Print an "h" in the 1st digit.
  myDisplay.displayChar(3, 0x12);                         // Print an "h" in the 4th digit.
  do {
    myDisplay.displayInt8(1, counter, false);             // Print the 8-bit count in the 2nd and 3rd digits.
    myDisplay.displayInt8(4, counter, false);             // Print the 8-bit count in the 5th and 6th digits.
    delay(interval);
  } while(++counter != 0);                                // The counter is an 8-bit number that will wrap to zero.
  // Ensure we clear the display (+dps) as we leave the 8-bit counter function.
  myDisplay.displayClear();
}

void countHex12(uint32_t interval) {
  uint16_t counter = 0;
  do {
    myDisplay.displayInt12(0, counter, false);            // Print the 12-bit count in the 1st, 2nd and 3rd digits.
    myDisplay.displayInt12(3, counter, false);            // Print the 12-bit count in the 4th, 5th and 6th digits.
    delay(interval);
  } while(++counter != 0x1000);                           // The counter is a 16-bit number, so watch for it crossing the 12-bit boundry.
  // Ensure we clear the display (+dps) as we leave the 12-bit counter function.
  myDisplay.displayClear();
}

void countHex16(uint32_t interval) {
  uint16_t counter = 0;
  myDisplay.displayChar(0, 0x26);                         // Print a left-border character in the 1st digit.
  myDisplay.displayChar(5, 0x27);                         // Print a right-border character in the 6th digit.
  do {
    myDisplay.displayInt16(1, counter, false);            // Print the 16-bit count in the 1st, 2nd, 3rd and 4th digits.
    delay(interval);
  } while(++counter != 0);                                // The counter is a 16-bit number that will wrap to zero.
  // Ensure we clear the display (+dps) as we leave the 16-bit counter function.
  myDisplay.displayClear();
}

void countUp(uint16_t number, uint32_t interval) {
  int16_t counter;
  myDisplay.displayChar(0, 0x26);                         // Print a left-border character in the 1st digit.
  myDisplay.displayChar(5, 0x27);                         // Print a right-border character in the 6th digit.
  for(counter = 0; counter <= number; counter++) {
    myDisplay.displayInt16(1, counter);                   // Print the 0 - 9999 count in the 1st, 2nd, 3rd and 4th digits.
    delay(interval);
  }
  // Ensure we clear the display (+dps) as we leave the count up function.
  myDisplay.displayClear();
}

void countDown(uint16_t number, uint32_t interval) {
  int16_t counter;
  myDisplay.displayChar(0, 0x26);                         // Print a left-border character in the 1st digit.
  myDisplay.displayChar(5, 0x27);                         // Print a right-border character in the 6th digit.
  for(counter = number; counter >= 0; counter--) {
    myDisplay.displayInt16(1, counter);                   // Print the 9999 - 0 count in the 1st, 2nd, 3rd and 4th digits.
    delay(interval);
  }
  // Ensure we clear the display (+dps) as we leave the count down function.
  myDisplay.displayClear();
}

void countXMins(byte minutesMax) {
  byte minutes, seconds;
  if(minutesMax > 100) {                                  // Clip the number at the maximum for a 4-digit MM.SS timer.
    minutesMax = 100;
  }
  myDisplay.displayChar(0, 0x26);                         // Print a left-border character in the 1st digit.
  myDisplay.displayChar(5, 0x27);                         // Print a right-border character in the 6th digit.
  for(minutes = 0; minutes < minutesMax; minutes++) {
    for(seconds = 0; seconds < 60; seconds++) {
      myDisplay.displayChar(1, minutes / 10);             // Print the minutes (x10) in the 2nd digit.
      myDisplay.displayChar(2, minutes % 10);             // Print the minutes (units) in the 3rd digit.
      myDisplay.displayChar(3, seconds / 10);             // Print the seconds (x10) in the 4th digit.
      myDisplay.displayChar(4, seconds % 10);             // Print the seconds (units) in the 5th digit.
      delay(1000);
    }
  }
  // Ensure we clear the display (+dps) as we leave the timer function.
  myDisplay.displayClear();
}

void countXMinsDP(byte minutesMax) {
  bool dPoint = ON;
  byte minutes = 0, seconds = 0;
  unsigned long timeNow, timeMark;
  timeMark = millis();
  if(minutesMax > 100) {                                  // Clip the number at the maximum for a 4-digit MM.SS timer.
    minutesMax = 100;
  }
  myDisplay.displayChar(0, 0x26);                         // Print a left-border character in the 1st digit.
  myDisplay.displayChar(5, 0x27);                         // Print a right-border character in the 6th digit.
  while (minutes != minutesMax || !dPoint) {
    timeNow = millis();
    if(timeNow - timeMark >= 500) {
      timeMark = timeNow;
      // Update the display decimal point.
      myDisplay.displayDP(2, dPoint);
      // Update the display and increment the time.
      if(dPoint) {
        myDisplay.displayChar(1, minutes / 10);           // Print the minutes (x10) in the 2nd digit.
        myDisplay.displayChar(2, minutes % 10);           // Print the minutes (units) in the 3rd digit.
        myDisplay.displayChar(3, seconds / 10);           // Print the seconds (x10) in the 4th digit.
        myDisplay.displayChar(4, seconds % 10);           // Print the seconds (units) in the 5th digit.
        if(++seconds == 60) {
          seconds = 0;
          minutes++;
        }
      }
      // Toggle the decimal point flag.
      dPoint = !dPoint;
    }
  };
  // Ensure we clear the display (+dps) as we leave the timer function.
  myDisplay.displayClear();
}

// EOF