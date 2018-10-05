# Dymopixelprinter
Firmware for the super funky dymFT, a needleprinter for dymo Labels. It's made from printer and CD-drive junk, an old PS2 Keyboard and a 16x2 character Display. 


![the printer](https://github.com/TimoJohannes/Dymopixelprinter/blob/master/img/dymFT_01.jpg?raw=true)

## fonts

As font we are using bitmap fonts. The code is inspired by the adafruitGFX library. Most fonts in use, the so called UNSCII fonts are taken from [this source](http://pelulamu.net/unscii/) and were transcoded to compatible bitmap fonts using the Adafruit font converter. Due to the low resolution of the y-Axis (maximum 10 pixel per row) only few fonts are still readable. 


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

|Pin   |Function            |Done|
|------|--------------------|----|
|0     | /                  |/   |
|1     | /                  |/   |
|2     | Label detect       |y   |
|3     | Stamping Solenoid  |y   |
|4     | Label Enable       |y   |
|5     | Label Step         |y   |
|6     | Label Dir          |y   |
|7     | Head Enable        |y   |
|8     | Head Step          |y   |
|9     | Head Dir           |y   |
|10    | Cut servo pwm      |y   |
|11    | Keyboard CLK       |y   |
|12    | Keyboard DATA      |y   |
|13    | Status LED         |/   |
|a14   | Sound              |y   |
|14    | Head stop          |y   |
|15    | cut servo enable   |y   | 
|16    | Display REG SEL    |y   |
|17    | Display Enable     |y   |
|18    | Display D1         |y   |
|19    | Display D2         |y   |
|20    | Display D3         |y   |
|21    | Display D4         |y   |
