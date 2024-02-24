# /*****************************************************************************
# * | File        :	  epdconfig.py
# * | Author      :   Waveshare team
# * | Function    :   Hardware underlying interface
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2019-06-21
# * | Info        :   
# ******************************************************************************
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

import os
import sys
import time
import logging
import numpy as np

logger = logging.getLogger(__name__)

class RaspberryPi:
    np=np
    RST_PIN= 27
    DC_PIN = 25
    BL_PIN = 18
    spi_freq  =40000000
    BL_freq=1000
    
    def __init__(self):
        import RPi.GPIO      
        import spidev

        self.GPIO = RPi.GPIO
        self.GPIO.setmode(self.GPIO.BCM)
           
        #Initialize SPI
        self.SPI = spidev.SpiDev()


    def digital_write(self, pin, value):
        self.GPIO.output(pin, value)

    def digital_read(self, pin):
        return self.GPIO.input(pin)

    def delay_ms(self, delaytime):
        time.sleep(delaytime / 1000.0)

    def spi_writebyte(self, data):
        if self.SPI!=None :
            self.SPI.writebytes(data)
    def bl_DutyCycle(self, duty):
        self._pwm.ChangeDutyCycle(duty)
        
    def bl_Frequency(self,freq):
        self._pwm.ChangeFrequency(freq)
           
    def module_init(self):
        self.GPIO.setup(self.RST_PIN, self.GPIO.OUT)
        self.GPIO.setup(self.DC_PIN, self.GPIO.OUT)
        self.GPIO.setup(self.BL_PIN, self.GPIO.OUT)
        self._pwm=self.GPIO.PWM(self.BL_PIN,self.BL_freq)
        self._pwm.start(100)
        
        self.SPI.open(0, 0)
        self.SPI.max_speed_hz = spi_freq
        self.SPI.mode = 0b00   
        return 0

    def module_exit(self):
        logging.debug("spi end")
        if self.SPI!=None :
            self.SPI.close()
        
        logging.debug("gpio cleanup...")
        self.GPIO.output(self.RST_PIN, 1)
        self.GPIO.output(self.DC_PIN, 0)        
        self._pwm.stop()
        time.sleep(0.001)
        self.GPIO.output(self.BL_PIN, 1)
        #self.GPIO.cleanup()

class VisionFive2:
    np=np
    RST_PIN= 13
    DC_PIN = 22
    BL_PIN = 12
    spi_freq  =4000000
    BL_freq=1000

    def __init__(self):
        import VisionFive.boardtype as board_t
        import VisionFive.spi as spi
        import VisionFive.gpio as gpio
        self.board_t = board_t
        self.GPIO = gpio
        self.SPI = spi

    def digital_write(self, pin, value):
        self.GPIO.output(pin, value)

    def digital_read(self, pin):
        return self.GPIO.input(pin)

    def delay_ms(self, delaytime):
        time.sleep(delaytime / 1000.0)

    def spi_writebyte(self, data):
        if len(data) == 1:
            a = data[0]
            self.SPI.transfer(a)
        else:
            self.SPI.write(data)
            
    def bl_DutyCycle(self, duty):
        self._pwm.ChangeDutyRatio(duty)
        
    def bl_Frequency(self,freq):
        self.GPIO.PWM(self.BL_PIN,freq)
           
    def module_init(self):
        self.GPIO.setup(self.RST_PIN, self.GPIO.OUT)
        self.GPIO.setup(self.DC_PIN, self.GPIO.OUT)
        self.GPIO.setup(self.BL_PIN, self.GPIO.OUT)
        self._pwm=self.GPIO.PWM(self.BL_PIN,self.BL_freq)
        self._pwm.start(100)    
        
        #Initialize SPI
        vf_t = self.board_t.boardtype()
        if vf_t == 1:
            self.SPI_DEVICE = "/dev/spidev0.0"
        elif vf_t == 2:
            self.SPI_DEVICE = "/dev/spidev1.0"
        else:
            print('This module can only be run on a VisionFive board!')
            return 0
        
        self.SPI.getdev(self.SPI_DEVICE)
        self.SPI.setmode(self.spi_freq, 0, 8)
        
        return 0

    def module_exit(self):
        logging.debug("spi end")        
        logging.debug("gpio cleanup...")
        self.GPIO.output(self.RST_PIN, 1)
        self.GPIO.output(self.DC_PIN, 0)        
        self._pwm.stop()
        time.sleep(0.001)
        self.GPIO.output(self.BL_PIN, 1)

if os.path.exists('/sys/bus/platform/drivers/gpiomem-bcm2835'):
    implementation = RaspberryPi()
else:
    implementation = VisionFive2()   
for func in [x for x in dir(implementation) if not x.startswith('_')]:
    setattr(sys.modules[__name__], func, getattr(implementation, func))




### END OF FILE ###
