from typing import Union
from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
import uvicorn

import RPi.GPIO as GPIO
from time import sleep

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT) # red
GPIO.setup(27, GPIO.OUT) # yellow
GPIO.setup(22, GPIO.OUT) # green

global run
global loop_done
run = False
loop_done = True

def stop_loop():
    global run
    run = False
    while loop_done == False:
        sleep(0) # pass to wait for loop to finish before moving on
    return

def wait(sec):
    for i in range(sec + 1):
        if run == True:
            sleep(1)
        else:
            return
    return

app = FastAPI(title="traffic API")

@app.get("/api/light/{color}")
def read_item(color: Union[str, None] = None):
    if color == "red":
        print("red light")
        stop_loop()
        GPIO.output(17, GPIO.HIGH) # red
        GPIO.output(27, GPIO.LOW) # yellow
        GPIO.output(22, GPIO.LOW) # green
    
    if color == "yellow":
        print("yellow light")
        stop_loop()
        GPIO.output(17, GPIO.LOW) # red
        GPIO.output(27, GPIO.HIGH) # yellow
        GPIO.output(22, GPIO.LOW) # green
    
    if color == "green":
        print("green light")
        stop_loop()
        GPIO.output(17, GPIO.LOW) # red
        GPIO.output(27, GPIO.LOW) # yellow
        GPIO.output(22, GPIO.HIGH) # green
    
    if color == "off":
        print("turn lights off")
        stop_loop()
        GPIO.output(17, GPIO.LOW) # red
        GPIO.output(27, GPIO.LOW) # yellow
        GPIO.output(22, GPIO.LOW) # green
    
    if color == "timer":
        print("automatic traffic light")
        global run
        global loop_done
        run = True
        loop_done = False

        GPIO.output(17, GPIO.LOW) # red
        GPIO.output(27, GPIO.LOW) # yellow
        GPIO.output(22, GPIO.LOW) # green

        try:
            while run == True:
                
                if run == True:
                    GPIO.output(27, GPIO.LOW) # yellow
                    GPIO.output(17, GPIO.HIGH) # red
                    wait(3)

                if run == True:
                    GPIO.output(17, GPIO.LOW) # red
                    GPIO.output(22, GPIO.HIGH) # green
                    wait(5)

                if run == True:
                    GPIO.output(22, GPIO.LOW) # green
                    GPIO.output(27, GPIO.HIGH) # yellow
                    wait(2)

        except KeyboardInterrupt:
            pass
        
        GPIO.output(17, GPIO.LOW) # red
        GPIO.output(27, GPIO.LOW) # yellow
        GPIO.output(22, GPIO.LOW) # green

        loop_done = True
    
    return {"color": color}

app.mount("/", StaticFiles(directory="ui", html=True), name="ui")

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)



# recommendations:
    # break the LED controls into functions
    # see if you can use a non-blocking loop
    # use a timer to change the colors rather than a sleep/wait.