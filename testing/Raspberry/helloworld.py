import numpy as np
from gpiozero import LED

from time import sleep

if __name__=="__main__":
  print("Creating files")
  with open("Test.txt", 'w') as f:
    with open("Test2.txt", 'w') as f2:
      f.write("Hello There!")
      f2.write("General Kenobi!")
  
  print("Configuring pins")
  led = LED(17)

  print("Starting loop")
  try:
    while True:
      print("Turning led on")
      led.on()
      sleep(1)
      print("Turning led off")
      led.off()
      sleep(1)
  except KeyboardInterrupt as e:
    print("Exiting")