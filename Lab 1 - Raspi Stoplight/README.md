Nathan Petersen  
25 Jan 2024  
IT&C 441  

---

### State Diagram
*See Diagram in Appendix*
### Analysis of two different views
#### System View
- There is a user-facing web page, with an interactive stop light that the user can click on colors to enable them, and an off button, and a timer button to toggle those states. The input is the user clicking on buttons, the output is API calls to the light controller.
- The component view includes the Raspberry Pi connected to a breadboard using jumper cables, on the board are resistors and LED's. The raspberry pi receives HTTP requests/API calls from the web page to turn on and off the LED's using the GPIO pins.
#### Component View
### Thought Questions
1. What language did you choose for implementing this project? Why?
   - Python, because it comes on Raspian, I know it fairly well already, and there's tons of online resources.
2. What is the purpose of the resistor in this simple circuit? What would happen if you omitted it?
   - To control the flow of current in the circuit. Without it two things could happen: One, you could burn out the LED. Two, you could open the circuit to interference from the air.
3. What are practical applications of this device? What enhancements or modifications would you make?
   - The API aspect of this project means it is highly applicable. With some simple changes to either the back end, the front end, or both, this can be applied in a number of circumstances.
   - A specific application could be at a car wash, where it tells the driver when they can pull the car onto of off of the tracks to move the car through the car wash. In this instance, this would need to be placed in a water-proof container, and the lights should be bigger for the drivers to see. The computer itself should also be separated from the lights, using longer wires to connect the lights to the mainboard.
4. Please estimate the total time you spent on this lab and report.
   - About 6 hours.
### Completion Statement
This lab is my own work, performed without any intervention or guidance from others, including from AI or online forums. Some general concepts and ideas came from online resources, such as FastAPI documentation, but the ideas have been adapted to my specific use case.

---

## Appendix

### Picture of Wiring
<img src='./_images/Lab 1 - image.png' width=600></img>    

### State Diagram
<img src='./_images/Lab 1 - Flowchart.png' width=700></img>    

### main.py - Main python code to drive the LED's
Uses API from web page

``` Python
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

```

### HTML Page
``` html
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="X-UA-Compatible" content="ie=edge">
  <title>Traffic Light</title>
  <link rel="icon" type="image/x-icon" href="./favicon.ico">
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.3.1/dist/css/bootstrap.min.css"
    integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
  <link rel="stylesheet" href="./style.css">
</head>

<body onload="offLight()">
  <main>
    <h1>Web Traffic Light</h1>
    <h3>Nathan Petersen</h3>

    <div id="traffic-light">
      <input type="checkbox" class="light" id="color1" value="color1" onclick="redLight()"/>
      <input type="checkbox" class="light" name="traffic-light-color" id="color2" value="color2" onclick="yellowLight()"/>
      <input type="checkbox" class="light" name="traffic-light-color" id="color3" value="color3" onclick="greenLight()"/>
    </div>
    <div class="container">
      <button type="button" class="btn btn-info" id="timer_btn" onclick="timerLight()">Timer</button>
      <button type="button" class="btn btn-info" id="off_btn"  onclick="offLight()">Off</button>
    </div>

  </main>
  <script src="https://code.jquery.com/jquery-3.3.1.slim.min.js"
    integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo"
    crossorigin="anonymous"></script>
  <script src="https://cdn.jsdelivr.net/npm/popper.js@1.14.7/dist/umd/popper.min.js"
    integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1"
    crossorigin="anonymous"></script>
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@4.3.1/dist/js/bootstrap.min.js"
    integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM"
    crossorigin="anonymous"></script>
  <script src="./script.js"></script>

</body>

</html>
```

### CSS for page
``` css
h1 {
  text-align: center;
  margin-top: 18vh;
  color: #15151a;
}

h3 {
  text-align: center;
  margin-top: 10px;
  color: #15151a;
}

.container {
  display: grid;
}

#timer_btn {
  margin: auto;
  display: block;
  position: absolute;
  left: 50%;
  top: 60%;
  margin-left: -33px;
  margin-top: 90px;
}

#off_btn {
  margin: auto;
  display: block;
  position: absolute;
  left: 50%;
  top: 60%;
  margin-left: -24px;
  margin-top: 135px;
}

.btn-success {
  box-shadow: 0 0 10px 1px #33d457;
}

body {
  background-color: #525467;
}

#traffic-light {
  position: absolute;
  display: block;
  left: 50%;
  top: 50%;
  margin-left: -60px;
  margin-top: -160px;
  background-color: #15151a;
  width: 120px;
  height: 320px;
  border-radius: 30px;
}

.light {
  appearance: none;
  position: relative;
  left: 50%;
  width: 80px;
  height: 80px;
  margin-top: 20px;
  margin-left: -40px;
  background-color: grey;
  /* vertical-align: middle; */
  border-radius: 100%;
  display: block;

  &#color1 {
    background-color: darken(#FF0000, 15%);

    &:hover {
      background-color: #FF0000;
    }

    &:checked {
      background-color: #FF0000;
      box-shadow: 0 0 50px 5px #FF0000;
    }
  }

  &#color2 {
    background-color: darken(#FFFF00, 15%);

    &:hover {
      background-color: #FFFF00;
    }

    &:checked {
      background-color: #FFFF00;
      box-shadow: 0 0 50px 5px #FFFF00;
    }
  }

  &#color3 {
    background-color: darken(#00FF00, 15%);

    &:hover {
      background-color: #00FF00;
    }

    &:checked {
      background-color: #00FF00;
      box-shadow: 0 0 50px 5px #00FF00;
    }
  }
}

* {
  -webkit-user-select: none;
  user-select: none;
  outline: none;
}
```

### JavaScript for page
``` javascript
function redLight() {
    fetch('/api/light/red')
        .then(response => response.json())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error:', error);
        });

    document.getElementById("color1").checked = true;
    document.getElementById("color2").checked = false;
    document.getElementById("color3").checked = false;

    document.getElementById("off_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('onclick', 'timerLight()')
}

function yellowLight() {
    fetch('/api/light/yellow')
        .then(response => response.json())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error:', error);
        });

    document.getElementById("color1").checked = false;
    document.getElementById("color2").checked = true;
    document.getElementById("color3").checked = false;

    document.getElementById("off_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('onclick', 'timerLight()')
}

function greenLight() {
    fetch('/api/light/green')
        .then(response => response.json())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error:', error);
        });

    document.getElementById("color1").checked = false;
    document.getElementById("color2").checked = false;
    document.getElementById("color3").checked = true;

    document.getElementById("off_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('onclick', 'timerLight()')
}

function offLight() {
    fetch('/api/light/off')
        .then(response => response.json())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error:', error);
        });

    document.getElementById("color1").checked = false;
    document.getElementById("color2").checked = false;
    document.getElementById("color3").checked = false;

    document.getElementById("off_btn").setAttribute('class', ' btn btn-success')
    document.getElementById("timer_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('onclick', 'timerLight()')

}

function timerLight() {
    fetch('/api/light/timer')
        .then(response => response.json())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error:', error);
        });

    document.getElementById("color1").checked = true;
    document.getElementById("color2").checked = true;
    document.getElementById("color3").checked = true;

    document.getElementById("off_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('class', ' btn btn-success')
    document.getElementById("timer_btn").setAttribute('onclick', '')
}
```