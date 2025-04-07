/*!
 * An Easy TM1637 Arduino Library.
 *  Simple, functional, optimal, and all in a class!
 *
 * The TM1637 is an (up to) 6-Digit 7-Segment (+dps) LED display driver.
 *
 * Written for the Arduino Uno/Nano/Mega.
 * (c) Ian Neill 2025
 * GPL(v3) Licence
 *
 * Built on my TM1651 library, which was built on work by Derek Cooper.
 * ... with some developments backported into this library from my TM1638 and MAX7219 libraries.
 *
 ***************************************************************
 * LED Segments:         a
 *                     -----
 *                   f|     |b
 *                    |  g  |
 *                     -----
 *                   e|     |c
 *                    |     |
 *                     -----  o dp
 *                       d
 *   Register bits:
 *      bit:  7   6  5  4  3  2  1  0
 *            dp  g  f  e  d  c  b  a
 ***************************************************************
 *
 * ****************************
 * *  easiTM1637 Header File  *
 * ****************************
 */

#ifndef __TM1637_H
  #define __TM1637_H
  #include <Arduino.h>
  
  #define ON              HIGH
  #define OFF             LOW

  // Compile time control for the TM1637 addressing mode.
  //#define USEADDRAUTOMODE37
  
  // Command and address definitions for the TM1637.
  #define ADDR_AUTO37     0x40
  #define ADDR_FIXED37    0x44
  #define DISP_OFF37      0x80
  #define DISP_ON37       0x88
  #define STARTADDR37     0xc0 

  // The decimal points are controlled via bit 7 of each digit.
  #define DP_CTRL37       0x80

  // Definitions for the 7 segment display brightness.
  #define INTENSITY_MIN37 0x00
  #define INTENSITY_TYP37 0x02
  #define INTENSITY_MAX37 0x07

  // Hardware related definitions.
  #define DEF_TM_CLK37    2
  #define DEF_TM_DIN37    3
  #define DEF_DIGITS37    6
  #define MAX_DIGITS37    6
  
  class TM1637 {
    public:
      // TM1637 Class instantiation.
      TM1637(uint8_t = DEF_TM_CLK37, uint8_t = DEF_TM_DIN37);
      uint8_t cmdDispCtrl;                                // The current display control command.
      uint8_t charTableSize;                              // The size of the defined character code table.
      static uint8_t tmCharTable[];                       // This is a class variable, shared across all class instances.
      // Set up the display and initialise it with defaults values - with the default or no digit map.
      void begin(uint8_t = DEF_DIGITS37, uint8_t = INTENSITY_TYP37);
      #ifndef USEADDRAUTOMODE37
        // Set up the display and initialise it with defaults values - with a supplied digit map.
        void begin(uint8_t*, uint8_t = DEF_DIGITS37, uint8_t = INTENSITY_TYP37);
      #endif
      void displayOff(void);                              // Turn the TM1637 display OFF.
      void displayClear(void);                            // Clear all the digits in the display
      void displayBrightness(uint8_t = INTENSITY_TYP37);  // Set the brightness and turn the TM1637 display ON.
      void displayTest(bool = false);                     // Test the display - all the display digit segments (+dps).
      void displayChar(uint8_t, uint8_t, bool = false);   // Display a character in a specific digit.
      void displayInt8(uint8_t, uint8_t, bool = true);    // Display a decimal integer between 0 - 99, or a hex integer between 0x00 - 0xff, starting at a specific digit.
      void displayInt12(uint8_t, uint16_t, bool = true);  // Display a decimal integer between 0 - 999, or a hex integer between 0x000 - 0xfff, starting at a specific digit.
      void displayInt16(uint8_t, uint16_t, bool = true);  // Display a decimal integer between 0 - 9999, or a hex integer between 0x0000 - 0xffff, starting at a specific digit.
      void displayDP(uint8_t, bool = OFF);                // Turn ON/OFF the decimal point in a specific digit.
    private:
      uint8_t _clkPin;                                    // The current TM1637 clock pin.
      uint8_t _dataPin;                                   // The current TM1637 data pin.
      uint8_t _numDigits;                                 // The number of TM1637 module digits.
      uint8_t _brightness;                                // The current TM1637 display brightness.
      uint8_t _registers[MAX_DIGITS37] = {0};             // An array used to hold the TM1638 display digit values.
      #ifndef USEADDRAUTOMODE37
        uint8_t* _tmDigitMap;                             // A pointer to the physical to logical digit mapping.
        static uint8_t tmDigitMapDefault[];               // An array to hold the default physical to logical digit mapping.
      #endif
      void writeCommand(uint8_t);                         // Write a command to the TM1637.
      #ifndef USEADDRAUTOMODE37
        void writeDigit(uint8_t);                         // Write the given logical digit value to the correct physical digit.
      #else
        void writeDigit(uint8_t, uint8_t = 1);            // Write the given logical digit values to the correct physical digits.
      #endif
      bool writeByte(uint8_t);                            // Write a byte of data to the TM1637.
      void start(void);                                   // Send start signal to the TM1637.
      void stop(void);                                    // Send stop signal to the TM1637.
      void bitDelay(void);                                // Wait a bit...
  };
#endif

// EOF