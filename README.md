# Dymopixelprinter
Firmware for the super funky dymFT, a needleprinter for dymo labels. It's made from printer and CD-drive junk, an old PS2 keyboard and a 16x2 character display. 


![the printer](https://github.com/TimoJohannes/Dymopixelprinter/blob/master/img/dymFT_01.jpg?raw=true)

## fonts

As fonts we are using bitmap fonts. The code is inspired by the AdafruitGFX library. Most fonts in use, the so called UNSCII fonts are taken from [this source](http://pelulamu.net/unscii/) and were transcoded to compatible bitmap fonts using the Adafruit font converter. Due to the low resolution of the printers y-Axis (maximum 10 pixel per column) only few fonts remain still readable when convertet. Nevertheless all the UNSCII fonts are looking nice because they were made for low resolution display.

## rough BOM

| qty | part         |
|-----|--------------|
| 1   | Teensy 3.2   |
| 2   | DRV8825 stepepr driver |
| 1   | CD-Rom Drive with stepper |
| 1   | stepper from old printer with gear reduction |
| 1   | servo motor |
| some| rubber rollers from old printer|
| 1   | beefy power supply |
| 1   | pair of scissors | 
| 1   | PS2 Keyboard |
| 1   | 16x2 LCD display |

and some screws, metal junk, springs and resistros n‘ stuff.



## special keys
| Key			|Function			|
|------------	|---------------	|
|	F1			|	5x7 Font		|
|	F2			|UNSCII regular	|
|	F3			|UNSCII alterntive|
|	F4			|UNSCII thin		|
|	F5			|Times New Roman	|
|	F6			|UNSCII MCR		|
|	F7			|UNSCII fantasy	|
|	F8			|enabel key sound|
|	F9			|	cut				|
|	PRNT SCRN	| start printing	|
|	PG_UP		|	move Label	|
|	PG_DOWN	|	move Label	|
|arrow keys	|	move cursor	|




## Teensy Pins

|Pin   |Function            |
|------|--------------------|
|0     | /                  |
|1     | /                  |
|2     | Label detect       |
|3     | Stamping Solenoid  |
|4     | Label Enable       |
|5     | Label Step         |
|6     | Label Dir          |
|7     | Head Enable        |
|8     | Head Step          |
|9     | Head Dir           |
|10    | Cut servo pwm      |
|11    | Keyboard CLK       |
|12    | Keyboard DATA      |
|13    | Status LED         |
|A14   | Sound              |
|14    | Head stop          |
|15    | cut servo enable   | 
|16    | Display REG SEL    |
|17    | Display Enable     |
|18    | Display D1         |
|19    | Display D2         |
|20    | Display D3         |
|21    | Display D4         |
