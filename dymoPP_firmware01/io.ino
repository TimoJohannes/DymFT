
void updateIO() {

  if (keyboard.available()) {
    // read the next key
    c = keyboard.read();


    if (c == PS2_F1){
      printFunction = drawChar5x7;
      fontNameString = fontNames[0];
      displayTimer = 0;
      showFontName = true;
    } else if (c == PS2_F2){
      printFunction = drawChar;
      setFont(&unscii_default);
      fontNameString = fontNames[1];
      displayTimer = 0;
      showFontName = true;
    } else if (c == PS2_F3){
      printFunction = drawChar;
      setFont(&unscii_alt);
      fontNameString = fontNames[2];
      displayTimer = 0;
      showFontName = true;
    } else if (c == PS2_F4){
      //thin
      printFunction = drawChar;
      setFont(&unscii_thin);
      fontNameString = fontNames[3];
      displayTimer = 0;
      showFontName = true;
    } else if(c == PS2_F5){
      //times
      printFunction = drawChar;
      setFont(&Times_New_Roman);
      fontNameString = fontNames[4];
      displayTimer = 0;
      showFontName = true;
    } else if(c == PS2_F6){
      printFunction = drawChar;
      setFont(&unscii_mcr);
      fontNameString = fontNames[5];
      displayTimer = 0;
      showFontName = true;
    } else if(c== PS2_F7){
      printFunction = drawChar;
      setFont(&unscii_fantasy);
      fontNameString = fontNames[6];
      displayTimer = 0;
      showFontName = true;
    } else if(c == PS2_F8){
      enableKeySound = ! enableKeySound;
    } else if(c == PS2_F9){
      doTheCut();
    } else if(c == PS2_PRNT_SCRN){
      //printiwinti
      printString = String();
      int sLength = 0;

      for(int i = 127; i >= 0; i--){
        if(masterBuffer[i] != ' '){
          sLength = i+1;
          break;
        }
      }

      if(sLength > 0){
        for(int i = 0; i < sLength; i++){
          printString += String(masterBuffer[i]);
        }
        MODE = MODE_PRINT;
      }

    } else if (c == PS2_ESC) {
      lcd.clear();

      resetMasterBuffer();
      charIndex = 0;

    } else if (c == PS2_PAGEDOWN) {

      feedLabel(0,LABEL_FEEDRATE);

    } else if (c == PS2_PAGEUP) {
      feedLabel(1,LABEL_FEEDRATE);

    } else if (c == PS2_LEFTARROW) {
      charIndex = constrain(charIndex - 1, 0, 127);

    } else if (c == PS2_RIGHTARROW) {
      charIndex = constrain(charIndex + 1, 0, 127);

    } else if (c == PS2_UPARROW) {
      charIndex = constrain(charIndex - 16, 0, 127);

    } else if (c == PS2_DOWNARROW) {
      charIndex = constrain(charIndex + 16, 0, 127);

      //delete
    } else if (c == PS2_DELETE) {
      for (int i = charIndex-1; i < 127; i ++) {
        if(c>=0){
          if (i == 0) {
            masterBuffer[i] = ' ';
          } else masterBuffer[i] = masterBuffer[i + 1];
        }
      }
      charIndex = constrain((charIndex - 1), 0, 127);


    }
    else {
      if (charIndex < maxChar) {

        if (c >= 0x20 && c <= 0x7E) {
          for (int i = 127; i > charIndex; i--) {
            masterBuffer[i + 1] = masterBuffer[i];
          }
          masterBuffer[charIndex] = c;

          charIndex = constrain(charIndex + 1, 0, 127);

        }
      }


    }


    if(enableKeySound){
      keySound(c);
    }

    if(!showFontName){
      updateDisplay();
      if(charIndex > 127) charIndex = 127;
      else if (charIndex < 0) charIndex = 0;
      x = (charIndex % 16);
      if (charIndex < 32) {
        y = (charIndex / 16) % 2;
      } else {
        y = 1;
      }

      lcd.setCursor(x, y);
    }
  }

  if(showFontName){
    showFontName = false;
    lcd.noCursor();
    lcd.noBlink();
    lcd.clear();
    lcdprint(fontNameString);
    resetDisplay = true;
  } else if(displayTimer > displayInterval && resetDisplay){
    resetDisplay = false;
    lcd.cursor();
    lcd.blink();
    updateDisplay();
    if(charIndex > 127) charIndex = 127;
    else if (charIndex < 0) charIndex = 0;
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


  for (int i = start, count = 0; i < stop; i++, count++) {
    displayBuffer[count] = masterBuffer[i];

  }



  lcd.clear();
  x = 0;
  y = 0;
  String screen = String(displayBuffer);
  if(screen.length() != 0){
    lcdprint(screen);
  }

}




void lcdprint(String t) { // display string on lcd and Serial
  lcd.setCursor(0, 0);
  String b1 = t.substring(0, 16);
  lcd.print(b1);
  lcd.setCursor(0, 1);
  String b2 = t.substring(16, 32);
  lcd.print(b2);

}




void resetMasterBuffer(){
  for(int i = 0; i < 128; i ++){
    masterBuffer[i] = ' ';
  }
}
