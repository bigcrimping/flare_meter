# Xray Flare Meter
An Arduino Nano 33 IoT based gadget which inteorgates the latest xray flare class information from NOAA GOES Satelite and shows the result visually.

Inspired by a burst of transmission errors picked up in my day job we linked back to a coronal mass ejection I wanted something to show me what the current state of solar flares is.

The data is proviided by NOAA in JSON via the website here: https://services.swpc.noaa.gov/json/

![image](https://user-images.githubusercontent.com/74270551/180198689-856a7521-7ceb-471e-8b40-b6d9c847180a.png)


Using (the excelent) X.27 Stepper motor interface from snafu: https://www.tindie.com/products/boffin/serial-x27-stepper-motor-gauge/


## Project Code

The code is here: https://github.com/bigcrimping/flare_meter/tree/main/arduino_code/flare_meter_0.1

You will need to add your WiFi details to the arduino_secrets.h file for the network you wish to connect to.

## Wiring

The wiring is per the code
![image](https://user-images.githubusercontent.com/74270551/180201465-8e81591a-bcb6-4230-ba54-b1fda24936fd.png)


![image](https://user-images.githubusercontent.com/74270551/180200741-4fa06252-f080-486e-9b4a-64a945a73adb.png)


## Mechanical Files

![image](https://user-images.githubusercontent.com/74270551/180155837-8c465e86-1bfe-498d-afaa-9dbd630525f7.png)

The design comprises (from left to right) a 
Front Plate
Spacer
Back Plate (which mounts the motor)
Chassis
Rear Cover

The needle passes through the spacer to connect to the motor, the Arudino mounts to the Rear Cover with a dab of glue 

Fixing required 

10 x M3 nuts
10 x M3x12mm cap head bolts
