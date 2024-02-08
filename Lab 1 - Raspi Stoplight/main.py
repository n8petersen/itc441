# same as Try 1 but broken the lights into a function and code cleaned up

from typing import Union
from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
import uvicorn
import RPi.GPIO as GPIO
from time import sleep

# constants so we don't have to remember pin numbers
RED = 17
YELLOW = 27
GREEN = 22

# set up the GPIO pins/LED's
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(RED, GPIO.OUT) # red
GPIO.setup(YELLOW, GPIO.OUT) # yellow
GPIO.setup(GREEN, GPIO.OUT) # green

# global variables used to start/stop auto timer
run = False
loop_done = True

# general function to turn LED's on/off
def setLight(light):
    # turn everything off, then only turn on what we need
    GPIO.output(RED, GPIO.LOW) # red
    GPIO.output(YELLOW, GPIO.LOW) # yellow
    GPIO.output(GREEN, GPIO.LOW) # green
    if light == 'red':
        GPIO.output(RED, GPIO.HIGH)
        return
    if light == 'yellow':
        GPIO.output(YELLOW, GPIO.HIGH)
        return
    if light == 'green':
        GPIO.output(GREEN, GPIO.HIGH)
        return   
    if light == 'timer':
        timer()
        return   
    return

## NOTE:
## I recognize this isn't the way we'd ideally do this, like on Arudino
## I spent a long time trying to convert this program to use non-blocking loops
## but I just couldn't figure it out with FastAPI and didn't want to COMPLETELY restart the lab.
## so, i did my best to setup functions that let us break out of this loop as soon as possible
## using the "wait()" function I wrote, the max delay is 62.5ms, which I think is good enough for this project.

# cycles through like a stop light
# we can exit out of the loop by setting global 'run' to False
def timer():
    global run
    global loop_done
    run = True
    loop_done = False

    setLight('off')

    while run == True: # only run if the timer is enabled 
            setLight('red')
            wait(3) # wait checks for the current 'run' value, so we don't need to check in the loop
            setLight('green')
            wait(5)
            setLight('yellow')
            wait(2)

    setLight('off')
    loop_done = True # tell the rest of the program that the loop is done, and we're ready to move on
    return

# function used to terminate the auto timer
def stop_loop():
    global run
    run = False
    while not loop_done:
        pass # pass to wait for loop to finish before moving on
    return

# function to make sleep work at longer increments while reducing delay when terminating loop
## to reduce the delay, we multiply the time by 16, then sleep 0.0625s/62.5ms, and repeat that 16 times for each second.
## the max delay will therefore be 0.0625s or 62.5ms
def wait(sec):
    for i in range((sec*16) + 1):
        if run == True: # only sleep if the timer is enabled, 
            sleep(0.0625)
        else: # otherwise break out
            return
    return

app = FastAPI(title="traffic API")

# main (and only) endpoint, passing in a color to set the light to
@app.get("/api/light/{color}")
def read_item(color: Union[str, None] = None):
    stop_loop()
    setLight(color)    
    return {"color": color}

# serve the static page in the 'ui' directory
app.mount("/", StaticFiles(directory="ui", html=True), name="ui")

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
