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
    print(arr)
    print(slotCount)

    return remainingDevices, slotCount


def linearBack(devices: int):
    windowSize = 2
    slotCount = 0
    while devices > 0:
        devices, slotCount = testWindow(windowSize, devices, slotCount)
        windowSize += 1
    
def binaryBack(devices: int):
    windowSize = 2
    slotCount = 0
    while devices > 0:
        devices, slotCount = testWindow(windowSize, devices, slotCount)
        windowSize *= 2

def logBack(devices: int):
    windowSize = 4
    slotCount = 0
    while devices > 0:
        devices, slotCount = testWindow(windowSize, devices, slotCount)
        windowSize = m.floor(((1 + (1/(m.log(m.log(windowSize, 2), 2)))) * windowSize))   

logBack(6)