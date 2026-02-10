import serial
import sounddevice as sd
import soundfile as sf
import re
import time


dry_thresh = 400


#receives string input from serial connection and extracts and returns digits only
def extract_number(str):
   numbers = re.findall(r'-*\d+', str) # \d means any digit character, + means one or more of preceding
   #print(numbers)
   try:
       return int(''.join(numbers))
   except:
       return -1 #no soil moisture reading was registered


#prints the string input from the serial connection
def print_serial(str):
   print(str)


#Establish a serial connection to the arduino port
try:
   arduino = serial.Serial("/dev/cu.usbmodem1421", timeout=1) #from arduino ide bottom right
except:
   print("check port")


#define sounds
peaceful_sound, fs1 = sf.read('mp3/peace.wav')
static_sound, fs2 = sf.read('mp3/random_distortion.wav')
first4Lines = 0 #first 4 lines from serial monitor deal with calibration


while True:
   if first4Lines < 4:
       print_serial(str(arduino.readline()))
       if first4Lines == 1:
           time.sleep(5) #allow time to calibrate
       first4Lines += 1
       continue


   val = extract_number(str(arduino.readline()))
   if val == -1:
       print("no readings")
   elif val == -2:
       print("led turned OFF")
       sd.stop()
   elif val > dry_thresh:
       print("led turned ON, soil is NOT dry")
       #print(val)
       sd.play(peaceful_sound,fs1)
   else:
       print("led turned ON, soil is dry")
       #print(val)
       sd.play(static_sound,fs2)
