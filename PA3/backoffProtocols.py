import random
import math as m

def testWindow(windowSize: int, remainingDevices: int, slotCount: int):
    arr = [0] * windowSize
    i = 0
    while i < remainingDevices:
        randInt = random.randint(0, (windowSize - 1))
        arr[randInt] += 1        
        i += 1
    succeeded = 0
    for item in arr:
        slotCount += 1
        if item == 1:
            succeeded += 1
            remainingDevices -= 1            
            if remainingDevices == 0:
                break
    

    return remainingDevices, slotCount


def linearBack(devices: int):
    windowSize = 2
    slotCount = 0
    while devices > 0:
        devices, slotCount = testWindow(windowSize, devices, slotCount)
        windowSize += 1
    return slotCount
    
def binaryBack(devices: int):
    windowSize = 2
    slotCount = 0
    while devices > 0:
        devices, slotCount = testWindow(windowSize, devices, slotCount)
        windowSize *= 2
    return slotCount

def logBack(devices: int):
    windowSize = 4
    slotCount = 0
    while devices > 0:
        devices, slotCount = testWindow(windowSize, devices, slotCount)
        windowSize = m.floor(((1 + (1/(m.log(m.log(windowSize, 2), 2)))) * windowSize)) 
    return slotCount  

def deviceInc():
    devices = 100
    l = open("linearLatency.txt", "w")
    b = open("binaryLatency.txt", "w")
    log = open("loglogLatency.txt", "w")
    while devices <= 6000:
        i = 10
        linearSlots = 0
        binarySlots = 0
        logSlots = 0
        for j in range(i):
            linearSlots += linearBack(devices)
            binarySlots += binaryBack(devices)
            logSlots += logBack(devices)           
            j += 1
            
        l.write(str(linearSlots / 10) + '\n')
        b.write(str(binarySlots / 10) + '\n')
        log.write(str(logSlots / 10) + '\n')
        devices += 100

deviceInc()