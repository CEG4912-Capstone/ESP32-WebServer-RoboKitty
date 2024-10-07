from machine import Pin
import utime

led = Pin(16, Pin.OUT)
sensor = Pin(15, Pin.IN)

while True:
    print(sensor.value())
    if sensor.value()==1:
        led.value(0)
    else:
        led.value(1)