# Dymopixelprinter
Firmware for the super funky dymFT, a needleprinter for dymo Labels. It's made from printer and CD-drive junk, an old PS2 Keyboard and a 16x2 character Display. 



## special keys
| Key			|Function			|
|------------	|---------------	|
|	F1			|	5x7 Font		|
|	F2			|USCII regular	|
|	F3			|USCII alterntive|
|	F4			|USCII thin		|
|	F5			|Times New Roman	|
|	F6			|UNSCII MCR		|
|	F7			|UNSCII fantasy	|
|	F8			|enabel key sound|
|	F9			|	cut				|
|	PRNT SCRN	| start printing	|
|	PG_UP		|	move Label	|
|	PG_DOWN	|	move Label	|
|arrow keys	|	move cursor	|

![the printer] (https://github.com/TimoJohannes/Dymopixelprinter/blob/master/img/dymFT_01.jpg)



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
