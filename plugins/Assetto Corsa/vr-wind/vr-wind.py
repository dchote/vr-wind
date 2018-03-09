#
#  vr-wind
#  By Daniel Chote 2018
#  https://github.com/dchote/vr-wind
#

import sys
import os
import platform
import time

if platform.architecture()[0] == "64bit":
    sysdir = "stdlib64"
else:
    sysdir = "stdlib"

sys.path.insert(0, os.path.join(os.path.dirname(__file__), sysdir))
os.environ['PATH'] = os.environ['PATH'] + ";."

import ac

ac.log("python platform.architecture=" + str(platform.architecture()) + "; sys.version=" + str(sys.version))

import acsys
import serial

ac.log("modules imported")

# EDIT THIS
# update serialPortName to reflect the COM port on your system
serialPortName = 'COM9'


# DO NOT EDIT PAST THIS POINT
appWindow = 0
statusLabel = 0

serPort = 0
tick = 0


def acMain(acVerison):
    global serPort, tick, appWindow, statusLabel
    
    try:
        ac.log("opening serial port {}".format(serialPortName))
        
        tick = ticker() # class below: keeps track of game tick rate and can be used to update over serial less frequently
        
        serPort = serial.Serial(port = serialPortName, baudrate = 9600, timeout = 0)
        time.sleep(3)
        serPort.write("0 0 \n".encode())
        
        appWindow = ac.newApp("vr-wind")
        ac.setSize(appWindow, 200, 200)
        
        statusLabel = ac.addLabel(appWindow, "Output: none yet");
        ac.setPosition(statusLabel, 5, 32)
        
        ac.log("vr-wind ready")
        
        return "vr-wind"
        
    except Exception as e:
        ac.log("startup error: {}".format(e))
 

def acUpdate(deltaT):  
    global serPort, tick, appWindow, statusLabel
    
    if tick.tack(deltaT):
        return
    
    try:        
        speed = str(round(ac.getCarState(0, acsys.CS.SpeedMPH)))
        # TODO implement acsys.CS.Steer to adjust left vs right speeds
        
        # speed in MPH, we handle the speed states on the controller
        command = speed + " " + speed + "\n"
        serPort.write(command.encode())
        
        ac.setText(statusLabel, "Output: {}".format(command))
        
    except Exception as e:
        ac.log("update error: {}".format(e))


def acShutdown(deltaT):  
    global serPort, tick, appWindow, statusLabel
    
    ac.log("shutdown called")
    ac.setText(statusLabel, "Shutting down")
    
    command = "0 0\n"
    serPort.write(command.encode())
    
    if (serPort == 0): return
    
    try:
        serPort.close()

    except Exception as e:
        ac.log("shutdown error: {}".format(e))
        
        
        
#-----------------------
# ticker function, to determine update rate
#--------------------
class ticker:
    def __init__(self):
        self.ticktimer = 0.0
        self.ticktime = 0.0
        self.tickrate = 0.5 # update rate in seconds
   
    def tack(self,deltaT):
        self.ticktime += deltaT
        self.ticktimer += deltaT
        if self.ticktime >= self.tickrate:
            self.ticktime = self.ticktime % self.tickrate
            return False
        else:
            return True
   
    def debuginfo(self):
        return "ticktimer: %s ticktime: %s tickrate: %s" % (self.ticktimer, self.ticktime, self.tickrate)