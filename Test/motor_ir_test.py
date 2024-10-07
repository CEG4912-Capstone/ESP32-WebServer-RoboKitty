"""This micropython program makes the motor1 
move in forward and backward directions."""

from machine import Pin, PWM
from L298N_motor import L298N
import time
# import utime

lightOn = 0

led = Pin(16, Pin.OUT)
sensor1 = Pin(14, Pin.IN) #Front Sensor
sensor2 = Pin(15, Pin.IN) #Back Sensor
sensor3 = Pin(16, Pin.IN)
sensor4 = Pin(17, Pin.IN)


#Motor 1
ENA = PWM(Pin(0))
IN1 = Pin(1, Pin.OUT)         
IN2 = Pin(2, Pin.OUT)

#Motor 2 
ENB = PWM(Pin(5))
IN3 = Pin(3, Pin.OUT)         
IN4 = Pin(4, Pin.OUT)

#Motor 1 Motion 
motor1 = L298N(ENA, IN1, IN2)     #create a motor1 object
motor1.setSpeed(42500)            #set the speed of motor1. Speed value varies from 25000 to 65534
#Motor 2 Motion 
motor2 = L298N(ENB, IN3, IN4)     #create a motor2 object
motor2.setSpeed(43000)            #set the speed of motor2. Speed value varies from 25000 to 65534

forward = True

#Forward movement test
while True:
    # motor1.forward()      #run motor1 forward
    # motor2.forward()      #run motor2 forward
    # time.sleep(5)         #wait for 5 seconds
    # motor1.backward()     #run motor1 backward
    # motor2.backward()     #run motor2 backward
    # time.sleep(5)         #run motor2 backward
    print("Forward Test")
    print("Sensor 1: ", sensor1.value())
    print("Sensor 2: ", sensor2.value())

    #Front sensor detects white, back sensor detects black 
    if sensor1.value()==lightOn and sensor2.value()== (not lightOn): 
        print("Motor forward")
        forward = True
        motor1.backward()
        motor2.backward()

    #Back sensor detects white, front sensor detects black 
    elif sensor2.value()==lightOn and sensor1.value()== (not lightOn):  
        #move backwards 
        print("Motor Backward")
        forward = False
        motor1.forward()
        motor2.forward()

    #Both sensors detect white
    elif sensor1.value()==lightOn and sensor2.value()==lightOn:  
        if forward == True: 
            print("Motor Forward")
            motor1.backward()
            motor2.backward() 
        else: 
            print("Motor Backward")
            forward = False
            motor1.forward()
            motor2.forward()

    #Both sensors detect black, stop all movement
    else:
        print("Motor stop!!!!!!!!!!!!!!")
        motor1.stop()
        motor2.stop()
        


    #if sensor1.value()==1 and sensor2.value()==1 and sensor3.value()==1 and sensor4.value()==1: #if on white paper, move
    #    motor1.forward()
    #    motor2.forward()
    #else: #if not over white paper, stop
    #    motor1.stop()
    #    motor2.stop()
    
    #ForBackward movement test
    print("Backward Test")

    # motor1.forward()      #run motor1 forward
    # motor2.forward()      #run motor2 forward
    # time.sleep(5)         #wait for 5 seconds
    # motor1.backward()     #run motor1 backward
    # motor2.backward()     #run motor2 backward
    # time.sleep(5)         #run motor2 backward
    print("Sensor 1: ", sensor1.value())
    print("Sensor 2: ", sensor2.value())

    if sensor1.value()==lightOn and sensor2.value()==lightOn: #if on white paper, move
        print("Motor forward")
        motor1.forward()
        motor2.forward()
    else:
        print("Motor stop!!!!!!!!!!!!!!")
        motor1.stop()
        motor2.stop() 
        fDirect = True 



   