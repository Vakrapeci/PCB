/*****************************************************************************
* | File      	:   Readme_EN.txt
* | Author      :   Waveshare team
* | Function    :   Help with use
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-12-19
* | Info        :   Here is an English version of the documentation for your quick use.
******************************************************************************/
This file is to help you use this routine.


1. Basic information:
This routine has been verified using the Separate LCD module. 
You can view the corresponding test routines in the \Examples\ of the project.
This Demo has been verified on the VisionFive2;

2. Pin connection:
Pin connection You can view it in \lib\lcdconfig.py , and repeat it here:
EPD  	=>	VisionFive2(Board)
VCC    	->    	5V
GND    	->    	GND
DIN    	->    	19(SPI0_MOSI)
CLK    	->    	23(SPI0_SCK)
CS     	->    	24(CE0)
DC     	->    	22
RST    	->    	13
BL  	->    	12


3.Installation library
     apt-get install pip
     pip install VisionFive.gpio
     apt-get install python3-numpy
     apt-get install python3-pil

4. Basic use:
Since this project is a comprehensive project, you may need to read the following for use:
You can view the test program in the examples\ directory.
Please note which LCD Module you purchased.
Chestnut 1:
     If you purchased 1.54inch LCD Module, then you should execute the command:
     python3 ./1inch54_LCD_test.py
