/* TypeToDisplay - PS2Keyboard keystrokes show on a standard LCD Display

  Lcd support added 18/2/2018 D.R.Patterson
    lcd character set:
  http://forum.arduino.cc/index.php?topic=19002.0

  Tested on a mega with a Keypad Shield for Arduino

  PS2Keyboard now requries both pins specified for begin()
  keyboard.begin(data_pin, irq_pin);

  Valid irq pins:
     Arduino Uno:  2, 3
     Arduino Due:  All pins, except 13 (LED)
     Arduino Mega: 2, 3, 18, 19, 20, 21
     Teensy 2.0:   All pins, except 13 (LED)
     Teensy 2.0:   5, 6, 7, 8
     Teensy 1.0:   0, 1, 2, 3, 4, 6, 7, 16
     Teensy++ 2.0: 0, 1, 2, 3, 18, 19, 36, 37
     Teensy++ 1.0: 0, 1, 2, 3, 18, 19, 36, 37
     Sanguino:     2, 10, 11

  for more information you can read the original wiki in arduino.cc
  at http://www.arduino.cc/playground/Main/PS2Keyboard
  or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html

  Like the Original library and example this is under LGPL license.

  Modified by Cuninganreset@gmail.com on 2010-03-22
  Modified by Paul Stoffregen <paul@pjrc.com> June 2010
*/

#include <PS2Keyboard.h>
const int DataPin = 14;
const int IRQpin =  15;
PS2Keyboard keyboard;

#include <LiquidCrystal.h>
// LCD Keypad Shield for Arduino
// http://www.hobbytronics.co.uk/lcd/lcd-displays-5v/arduino-lcd-keypad-shield

//set constants for number of rows and columns to match your LCD
const int numRows = 2;
const int numCols = 16;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

byte x, y;  // track lcd position
byte maxChar = 128;
byte charIndex;
byte displayIndex;

char masterBuffer[129];
char displayBuffer[32];

// custom pound using https://omerk.github.io/lcdchargen/
byte customPound[8] = {
  0b01100,
  0b10010,
  0b01000,
  0b11100,
  0b01000,
  0b01000,
  0b11110,
  0b00000
};
const char pound = 1;

byte customkey[8] = { // ¬
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b00001,
  0b00001,
  0b00000
};
const char custom = 2;

byte customslash[8] = { // backslash
  0b00000,
  0b00000,
  0b10000,
  0b01000,
  0b00100,
  0b00010,
  0b00001,
  0b00000
};
const char backslash = 3;

char c;

// currently un-used:
#define is_printable(c) (!(c&0x80))   // don't print if top bit is set

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin, PS2Keymap_German);
  Serial.begin(115200);
  while (!Serial) yield();
  lcd.begin(numRows, numCols);
  delay(250);
  lcd.clear();
  // create a new custom character
  lcd.createChar(pound, customPound);
  lcd.createChar(custom, customkey);
  lcd.createChar(backslash, customslash);
  lcd.cursor();   // Enable Cursor
  lcd.blink();  // Blinking cursor
  lcd.clear();

  //  lcd.print(F("Keyboard Test:"));
  //  lcd.setCursor(0, 1);
  //  lcd.print(F("Esc to clear LCD"));
  Serial.println(F("Keyboard Test:"));
  unsigned long t = millis();

  charIndex = 0;

  /* do {
     if (keyboard.available()) {
       c = keyboard.read();
       if (c == PS2_ESC) break;
     }
    } while ((millis() - t) < 8000);
  */
  lcd.clear();
}

void loop() {
  if (keyboard.available()) {
    // read the next key
    c = keyboard.read();

    // check for some of the special keys
    /*
      if (c == PS2_ENTER) {
      Serial.println();
      x = 0;
      y += 1;
      if (y == numRows) {
        y = 0;
        lcd.clear();
      }
      lcd.setCursor(x, y);

      } */
    /*else*/ if (c == PS2_F1) {
      Serial.println("helloF1!");

    }
    else if (c == PS2_F2) {
      Serial.println("helloF2!");

    }
    else if (c == PS2_F3) {
      Serial.println("helloF3!");

    }
    else if (c == PS2_F4) {
      Serial.println("helloF4!");

    } else if (c == PS2_TAB) {

    } else if (c == PS2_ESC) {
      Serial.println(F("\n[ESC]\n"));
      lcd.clear();

      for (int i = 0; i < 128; i ++) {
        masterBuffer[i] = ' ';
      }
      charIndex = 0;

    } else if (c == PS2_PAGEDOWN) {
      lcdprint(F("[PgDn]")); /// feeed +

    } else if (c == PS2_PAGEUP) {
      lcdprint(F("[PgUp]")); // feeeeeed -


      //arrowkeys
      //works
    } else if (c == PS2_LEFTARROW) {
      charIndex = constrain(charIndex - 1, 0, 127);
      x = (charIndex % 16) ;
      y = (charIndex / 16) % 2;
      lcd.setCursor(x, y);

    } else if (c == PS2_RIGHTARROW) {
      charIndex = constrain(charIndex + 1, 0, 127);
      x = (charIndex % 16) ;
      y = (charIndex / 16) % 2;
      lcd.setCursor(x, y);
      
      //works
    } else if (c == PS2_UPARROW) {
      charIndex = constrain(charIndex - 16, 0, 127);
      x = (charIndex % 16) ;
      y = (charIndex / 16) % 2;
      lcd.setCursor(x, y);
      
      //works
    } else if (c == PS2_DOWNARROW) {
      charIndex = constrain(charIndex + 16, 0, 127);
      x = (charIndex % 16) ;
      y = (charIndex / 16) % 2;
      lcd.setCursor(x, y);
      
      //delete
    } else if (c == PS2_DELETE) {
      for (int i = charIndex; i < 127; i ++) {
        if (i == 0) {
          masterBuffer[i] = ' ';
        } else masterBuffer[i] = masterBuffer[i + 1];
      }
      Serial.println(charIndex);
      charIndex = constrain((charIndex - 1), 0, 127);
      x = (charIndex % 16) ;
      y = (charIndex / 16) % 2;
      lcd.setCursor(x, y);

    }
    else {
      if (charIndex < maxChar) {

        if (c >= 0x20 && c <= 0x7E) {
          for (int i = 127; i > charIndex; i--) {
            masterBuffer[i + 1] = masterBuffer[i];
          }

          masterBuffer[charIndex] = c;
          Serial.println(c, HEX);

          //        Serial.println(displayBuffer);
          charIndex = constrain(charIndex + 1, 0, 127);

          //displayBuffer = masterBuffer.substring(charIndex, charIndex + 32);
        }
      }

    }


    updateDisplay();

    if (charIndex > 127) charIndex = 127;
    x = (charIndex % 16);
    if (charIndex < 32) {
      y = (charIndex / 16) % 2;
    } else {
      y = 1;
    }
    lcd.setCursor(x, y);
  }





}

void updateDisplay() {
  int line = (charIndex / 16) - 1;
  if (line < 1) {
    line = 0;
  }

  int start = line * 16;
  int stop = start + 32;
  Serial.println(charIndex);

  Serial.print(start);
  Serial.print(",");
  Serial.print(stop);
  Serial.print(",");
  Serial.println(line);

  for (int i = start, count = 0; i < stop; i++, count++) {
    displayBuffer[count] = masterBuffer[i];
  }
  //displayBuffer[32] = '\0';


  lcd.clear();
  x = 0;
  y = 0;
  String screen = String(displayBuffer);
  lcdprint(screen);

}


void lcdprint(String t) { // display string on lcd and Serial
  byte L = t.length();
  lcd.setCursor(0, 0);
  String b1 = t.substring(0, 16);
  lcd.print(b1);
  lcd.setCursor(0, 1);
  String b2 = t.substring(16, 32);
  lcd.print(b2);

}

void lcddelete() { // display string on lcd and Serial


}
