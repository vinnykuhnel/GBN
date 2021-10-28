import random

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
    

linearBack(5)