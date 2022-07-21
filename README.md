# Xray Flare Meter
An Arduino Nano 33 IoT based gadget which inteorgates the latest xray flare class information from NOAA GOES Satelite and shows the result visually.

Inspired by a burst of transmission errors picked up in my day job we linked back to a coronal mass ejection I wanted something to show me what the current state of solar flares is.

![image](https://user-images.githubusercontent.com/74270551/180198689-856a7521-7ceb-471e-8b40-b6d9c847180a.png)

The data is proviided by NOAA in JSON via the website here: https://services.swpc.noaa.gov/json/

![image](https://user-images.githubusercontent.com/74270551/180202599-106be046-0b00-4df6-8d66-06493c564c13.png)

The data presented is from the GOES 16 Satelite. 

Using (the excelent) X.27 Stepper motor interface from snafu: https://www.tindie.com/products/boffin/serial-x27-stepper-motor-gauge/

The code is an almagamation from the following sources

WiFi https://www.arduino.cc/reference/en/libraries/wifinina/wifi.beginap/

SSL https://www.arduino.cc/reference/en/libraries/wifinina/client.connectssl/

JSON Handling https://arduinojson.org/v6/api/json/deserializejson/

## Project Code

The code is here: https://github.com/bigcrimping/flare_meter/tree/main/arduino_code/flare_meter_0.1

You will need to add your WiFi details to the arduino_secrets.h file for the network you wish to connect to.

## Wiring

The wiring is as below, hook up for power/gnd/TX

![image](https://user-images.githubusercontent.com/74270551/180201866-a9b74970-905d-4301-a8f7-3036af4c13be.png)

Inside the chassis it looks like the below

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

The M3 nuts should be a press fit into the mounting holes but may require a dab of super glue.

Fixing required 

10 x M3 nuts

10 x M3x12mm cap head bolts

STLs for printing available here: https://github.com/bigcrimping/flare_meter/tree/main/mechanics

## Dial Artwork

Artwork can be found in PDF format here: https://github.com/bigcrimping/flare_meter/tree/main/dial_artwork

![image](https://user-images.githubusercontent.com/74270551/180205061-77064ea2-c936-4e92-8c4f-a9e9ee8dcf58.png)

There are a few variations to choose from
