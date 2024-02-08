from typing import Union
import signal
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

def stop_server(*args):
    global running
    running = False

async def lifespan(app: FastAPI):
    signal.signal(signal.SIGINT, stop_server)
    yield
    print("Shutting Down.")

app = FastAPI(title="traffic API", lifespan=lifespan)
running = False

# @app.get("/")
# def index():
#     while running:
#         print("test")
#         sleep(1)
#     print("end loop")

@app.get("/api/toggle")
def toggle_running():
    global running
    running = not running
    print("running: " + str(running))

@app.get("/api/light/{color}")
def read_item(color: Union[str, None] = None):
    if color == "red":
        print("red light")
    
    if color == "yellow":
        print("yellow light")
    
    if color == "green":
        print("green light")
    
    if color == "off":
        print("turn lights off")
    
    if color == "timer":
        print("start timer")

    return {"color": color}

app.mount("/", StaticFiles(directory="ui", html=True), name="ui")

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
