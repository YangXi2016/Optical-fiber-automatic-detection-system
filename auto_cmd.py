# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

#!/usr/bin/env python
# -*- coding: utf-8 -*

import serial
import winsound
import time
 
# 创建serial实例
detectport = serial.Serial()
detectport.port = 'COM10'
detectport.baudrate = 115200
detectport.parity = 'N'
detectport.bytesize = 8
detectport.stopbits = 1
detectport.timeout = 0.6

detectport.open()
detectport.setDTR(True)
detectport.setRTS(True)

while(1):
    if detectport.inWaiting()>0:
        data = detectport.readall()
        data = str(data,encoding = "utf-8")
        print(data)
        if data == 'detect ready\n':
            time.sleep(1)
            cmd = 'ST1E'
            print(cmd)
            detectport.write(bytes(cmd,encoding = "utf-8"))
        if((data.find('SBQE')!=-1) or (data.find('SDE')!=-1)):
            #time.sleep(1)
            cmd = 'SR1E'
            print(cmd)
            detectport.write(bytes(cmd,encoding = "utf-8"))
        if(data.find('SXE')!=-1):
            winsound.Beep(3000,1000)
            cmd = input("error! please input cmd:")
            detectport.write(bytes(cmd,encoding = "utf-8"))
            
            
        