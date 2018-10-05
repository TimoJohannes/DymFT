#include <LiquidCrystal.h>
#include <Servo.h>
#include <AccelStepper.h>
#include "PS2Keyboard.h"
#include <elapsedMillis.h>
#include "gfxfont.h"
#include "fonts/font5x7.h"
#include "fonts/font8x8_serif.h"
#include "fonts/unscii_default.h"
#include "fonts/unscii_alt.h"
#include "fonts/unscii_fantasy.h"
#include "fonts/unscii_mcr.h"
#include "fonts/unscii_thin.h"
#include "fonts/times_new_roman.h"

GFXfont *gfxFont;       ///< Pointer to special font

//speeds
#define HEAD_HOMINGSPEED 4000
#define HEAD_TRAVELSPEED 4000
#define LABEL_ADJUSTSPEED 500
#define LABEL_TRAVELSPEED 1000
#define LABEL_FEEDRATE 100


//distances and position
#define LABEL_RETRACTION_AFTER_ADJUSTMENT 210
#define LABEL_FEED_AFTER_PRINT 2700
#define LABEL_FEEDRATE 100
#define HEAD_START_POS 450
#define LABEL_HEIGHT 960 // ca.
#define STEPSIZE_Y 120
#define STEPSIZE_X 27


// timing and delays
#define STAMP_DELAY 78


// PINS
#define HEAD_STEP 8
#define HEAD_DIR 9
#define HEAD_ENABLE 7

#define LABEL_STEP 5
#define LABEL_DIR 6
#define LABEL_ENABLE 4

#define HEAD_STOP 14
#define LABEL_DETECT 2

#define STAMP 3

#define CUT 10
#define CUT_ENABLE 15

#define KEYBOARD_DATA 12
#define KEYBOARD_CLOCK 11

#define LCD_REGSEL 16
#define LCD_ENABLE 17
#define LCD_D1 18
#define LCD_D2 19
#define LCD_D3 20
#define LCD_D4 21

#define SOUND A14


//objects

PS2Keyboard keyboard;
LiquidCrystal lcd(LCD_REGSEL, LCD_ENABLE, LCD_D1, LCD_D2, LCD_D3, LCD_D4);

AccelStepper headMotor(AccelStepper::DRIVER, HEAD_STEP, HEAD_DIR);
AccelStepper labelMotor(AccelStepper::DRIVER, LABEL_STEP, LABEL_DIR);

Servo cut;

elapsedMillis displayTimer;
int displayInterval = 1000;

char* fontNames[] = {
  "5x7", "standard", "alternative" , "thin", "times" , "MCR" , "fantasy"
};

String fontNameString = String();

bool resetDisplay = false;
bool showFontName = false;
// statemachine
#define MODE_INPUT 0
#define MODE_PRINT 1
int MODE = 0;

char c;
byte x, y;  // track lcd position
byte maxChar = 128;
int charIndex = 0;
bool enableKeySound = false;

char masterBuffer[129];
char displayBuffer[32];
String printString = String();
void (*printFunction)(char c);
uint8_t* font = *font8x8_serif;


void setup() {

  delay(2000);
  keyboard.begin(KEYBOARD_DATA, KEYBOARD_CLOCK, PS2Keymap_German);
  Serial.begin(19200);

  headMotor.setMaxSpeed(10000);
  headMotor.setAcceleration(100000);
  labelMotor.setAcceleration(100000);
  labelMotor.setMaxSpeed(1000);

  pinMode(HEAD_STOP, INPUT_PULLUP);
  pinMode(LABEL_DETECT, INPUT_PULLUP);

  pinMode(LABEL_ENABLE, OUTPUT);
  pinMode(HEAD_ENABLE, OUTPUT);
  pinMode(STAMP, OUTPUT);
  digitalWrite(STAMP, LOW);

  pinMode(CUT_ENABLE, OUTPUT);
  digitalWrite(CUT_ENABLE,LOW);
  digitalWrite(HEAD_ENABLE,LOW);

  pinMode(SOUND,OUTPUT);
  digitalWrite(SOUND,LOW);

  lcd.begin(16, 2);
  delay(250);
  lcd.clear();
  lcd.cursor();   // Enable Cursor
  lcd.blink();  // Blinking cursor
  lcd.clear();



  cut.attach(CUT);
  enableCut(true);
  cut.writeMicroseconds(800);
  delay(15);
  enableCut(false);


  feedLabel(1,5);
  analogWriteResolution(8);
  startSound();

  homeHead();

  setFont(&unscii_default);
  printFunction = drawChar5x7;
  resetMasterBuffer();

}



void loop() {
  if(MODE == MODE_INPUT){
    updateIO();
  } else if(MODE == MODE_PRINT){
    adjustLabel();
    homeHead();
    moveHeadToStart();
    printIt();

    feedLabel(0,LABEL_FEED_AFTER_PRINT);
    finishSound();
    MODE = MODE_INPUT;
  }

}


void printIt(){

  for(int i= 0; i < printString.length(); i++){
    char curChar = printString.charAt(i);
    printFunction(curChar);
  }
}

void doTheCut(){
  enableCut(true);
  cut.writeMicroseconds(2100);
  delay(800);
  cut.writeMicroseconds(800);
  delay(800);
  enableCut(false);
}


void homeHead() {
  enableHead(true);
  headMotor.setSpeed(-HEAD_HOMINGSPEED);
  while (headStopSwitch()) {
    headMotor.runSpeed();
  }
  headMotor.stop();
  enableHead(false);
  headMotor.setCurrentPosition(0);
}

void moveHeadToStart() {
  enableHead(true);
  headMotor.setSpeed(HEAD_HOMINGSPEED);
  headMotor.moveTo(HEAD_START_POS);
  while (headMotor.currentPosition() != HEAD_START_POS) {
    headMotor.run();
  }
  headMotor.stop();
  enableHead(false);
  headMotor.setCurrentPosition(0);
}


void moveHeadTo( long pos) {
  headMotor.setSpeed(HEAD_TRAVELSPEED);
  headMotor.moveTo(pos);
  while (headMotor.currentPosition() != pos) {
    headMotor.run();
  }
  headMotor.stop();
}


void advanceLabel() {
  enableLabel(true);
  long target = labelMotor.currentPosition() + STEPSIZE_X;
  labelMotor.setSpeed(LABEL_TRAVELSPEED);
  labelMotor.moveTo(target);
  while (labelMotor.currentPosition() != target) {
    labelMotor.run();
  }
  labelMotor.stop();
  enableLabel(false);

}

void feedLabel(int dir, long dist){
  enableLabel(true);
  long target = labelMotor.currentPosition();
  if(dir == 0){
    target += dist;
  } else{
    target -= dist;
  }
  labelMotor.moveTo(target);
  while (labelMotor.currentPosition() != target) {
    labelMotor.run();
  }
  labelMotor.stop();
  enableLabel(false);

}



void adjustLabel() {
  // wenn noch kein label zu sehen ist
  enableLabel(true);
  if (!labelDetected()) {
    labelMotor.setSpeed(LABEL_ADJUSTSPEED);
    while (!labelDetected()) {
      labelMotor.runSpeed();
    }
  } else { // oder wenn des label schon im Sensor hängt
    labelMotor.setSpeed(-LABEL_ADJUSTSPEED);
    while (labelDetected()) {
      labelMotor.runSpeed();
    }
  }
  labelMotor.stop();
  labelMotor.setCurrentPosition(0);
  delay(100);
  labelMotor.setSpeed(LABEL_ADJUSTSPEED);
  labelMotor.moveTo(-LABEL_RETRACTION_AFTER_ADJUSTMENT);
  while (labelMotor.currentPosition() != -LABEL_RETRACTION_AFTER_ADJUSTMENT) {
    labelMotor.run();
  }
  labelMotor.stop();
  enableLabel(false);
}


void stamp() {
  digitalWrite(STAMP, HIGH);
  delay(STAMP_DELAY);
  digitalWrite(STAMP, LOW);

}


bool headStopSwitch() {
  return digitalRead(HEAD_STOP);
}

bool labelDetected() {
  return digitalRead(LABEL_DETECT);
}

void enableHead(bool e) {
  digitalWrite(HEAD_ENABLE, !e);
}

void enableLabel(bool e) {
  digitalWrite(LABEL_ENABLE, !e);
}

void enableCut(bool e){
  digitalWrite(CUT_ENABLE, e);
}



void setFont(const GFXfont *f) {

  gfxFont = (GFXfont *)f;
}


void startSound() {

  for(int i = 0; i < 16384; i++){

    long out = i*(42&i>>10);

    analogWrite(A14, byte(out));
    delayMicroseconds(70);
  }
  analogWrite(SOUND,0);

}

void finishSound() {
  for(int i = 0; i <16384; i++){

    long out = i*(54&i>>10)|(i>>4);

    analogWrite(A14, byte(out));
    delayMicroseconds(70);
  }
  analogWrite(SOUND,0);
}

void keySound(uint8_t var) {
  for(int i = 1024; i < 2048; i++){
    long out = (i&(i<<(i%(var%12)))^((i<<4)&63));
    analogWrite(A14, byte(out));
    delayMicroseconds(60);
  }
  analogWrite(SOUND,0);
}



void drawChar8x8(char c){
  uint16_t headStep = LABEL_HEIGHT/9;
  uint16_t idx = c;
  advanceLabel();
  enableHead(true);
  for(int xx = 0; xx < 8; xx++){
    advanceLabel();
    delay(100);

    for(int yy = 0; yy < 8; yy++){
      byte mByte = font[idx*8+(7-yy)];//*(font)[idx][7-yy]; //*font+(idx*8+(7-yy));///[idx][7-yy];

      mByte = mByte << xx;
      moveHeadTo(yy * headStep);
      if ((mByte & 0x80) == 0x80) {
        stamp();
        delay(150);
      }
      delay(100);

    }

  }
  enableHead(false);
}


void drawChar5x7(char c) {
  uint16_t idx = c * 5;
  uint16_t headStep = LABEL_HEIGHT / 8;
  advanceLabel();
  advanceLabel();
  enableHead(true);
  for (int xx = 0; xx < 5; xx++) {
    advanceLabel();
    delay(100);
    byte mByte = font5x7[idx+xx];//pgm_read_byte(&font[idx + x]);
    if(mByte != 0x00){
      for (int yy = 0; yy < 8; yy++) {

        if ((mByte & 0x80) == 0x80) {
          stamp();
          delay(150);
        }

        mByte = mByte << 1;

        moveHeadTo(yy * headStep);
        delay(100);

      }
    }
  }
  enableHead(false);
}



void drawChar( char c) {

  // funky shizzle aus der adafruit gfx library
  c -= (uint8_t)pgm_read_byte(&gfxFont->first);
  GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
  uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);
  //  uint8_t ya = pgm_read_byte(&gfxFont->yAdvance);  // wird nicht gebraucht?
  uint8_t xa = pgm_read_byte(&glyph->xAdvance);
  uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
  uint8_t  w  = pgm_read_byte(&glyph->width),
  h  = pgm_read_byte(&glyph->height);
  int8_t   xo = pgm_read_byte(&glyph->xOffset),
  yo = pgm_read_byte(&glyph->yOffset);
  uint8_t  myByte;
  uint16_t byteAddr, bit = 0;


  uint16_t headStep = LABEL_HEIGHT/9;

  for(int i = 0; i < (xa-w-1); i++){
    advanceLabel();

  }
  advanceLabel();

  enableHead(true);

  for (int xx = 0; xx < w; xx++) {
      // band weiter schieben
    advanceLabel();
    delay(100);

    for ( int yy = 0; yy <h ; yy++) {

      // bit addresse in dem gesamtem buchstaben finden
      bit = (xx) + ((h-yy-1) * w);
      // die adresse vom byte finden in dem jenes bit steckt
      byteAddr = bo + (bit/8);
      // das bit im byte finden

      bit = bit % 8;
      // maske machen
      bit = 128 >> bit;

      myByte = pgm_read_byte(&bitmap[byteAddr]);

      int headTarget = (yy)*headStep;
      moveHeadTo(headTarget);
      delay(100);
      if ((myByte & bit) != 0) {

        stamp();
        delay(200);

      }

    }

  }

  enableHead(false);

}
