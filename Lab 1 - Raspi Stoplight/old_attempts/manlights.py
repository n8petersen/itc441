import RPi.GPIO as GPIO


def read_input():
    key = input()
    return key


def light_led(color):
    if color == 'r':
        print("red")
        GPIO.output(17, GPIO.HIGH)
        GPIO.output(27, GPIO.LOW)
        GPIO.output(22, GPIO.LOW)
        return

    if color == 'y':
        print("yellow")
        GPIO.output(17, GPIO.LOW)
        GPIO.output(27, GPIO.HIGH)
        GPIO.output(22, GPIO.LOW)
        return

    if color == 'g':
        print("green")
        GPIO.output(17, GPIO.LOW)
        GPIO.output(27, GPIO.LOW)
        GPIO.output(22, GPIO.HIGH)
        return

    if color == 'q':
        GPIO.output(17, GPIO.LOW)
        GPIO.output(27, GPIO.LOW)
        GPIO.output(22, GPIO.LOW)
        quit()

    return

def main():
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(17, GPIO.OUT)
    GPIO.setup(27, GPIO.OUT)
    GPIO.setup(22, GPIO.OUT)

    print("Press letter of color to illuminate:")
    print("  ( r / y / g )")
    print("  ( press 'q' to quit )")

    while True:
        color = read_input()
        light_led(color)


if __name__ == "__main__":
    main()
