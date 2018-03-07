__author__ = 'Daniel Chote'
#
#  vr-wind
#  By Daniel Chote 2018
#  https://github.com/dchote/vr-wind
#

import sys
import os
import platform

if platform.architecture()[0] == "64bit":
  sysdir='apps/python/vr-wind/stdlib64'
else:
  sysdir='apps/python/vr-wind/stdlib'

sys.path.insert(0, sysdir)

os.environ['PATH'] = os.environ['PATH'] + ";."

import ac
import acsys
import serial


# EDIT THIS
# update serialPortName to reflect the COM port on your system
serialPortName = 'COM9'


# DO NOT EDIT PAST THIS POINT

serialPort = false
sequence = 0

def acMain(acVerison):
    global serialPort

    try:
        serialPort = serial.Serial(port = serialPortName, baudrate = 9600, timeout = 0)
        
        time.sleep(3)
        
        serialPort.write("0 0 \n".encode())
        
        return "vr-wind"
        
    except Exception as e:
        Log.error("startup error: %s" % e)
 

def acUpdate(deltaT):  
    global serialPort
    
    if (serialPort == false): return
    
    try:
        sequence = sequence + 1
        
        if (sequence >= 15):
            speed = str(round(ac.getCarState(0, acsys.CS.SpeedMPH)))
            # TODO implement acsys.CS.Steer to adjust left vs right speeds
            
            # speed in MPH roughly matches what we want as a PWM value anyway, so just using that. 
            command = speed + " " + speed + "\n"
            serialPort.write(command.encode())
            
            sequence = 0
        
        
    except Exception as e:
        Log.error("update error: %s" % e)


def acShutdown(deltaT):  
    global serialPort
    
    if (serialPort == false): return
    
    try:
        serialPort.close()

    except Exception as e:
        Log.error("shutdown error: %s" % e)