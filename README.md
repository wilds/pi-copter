πCopter
==========

A quadcopter based on Raspberry PI
------------------------------------------

This project is the development of a code to control a quadcopter
with a Raspberry Pi.

The main idea is to use the RPI as much as possible. The only external
devices needed are a 6 Axis IMU (Accelerometer+Gyro) for attitude measurment
and a Wifi dongle.
The quadcopter is remotely controlled via wifi and an Android App.


The Raspberry PI hosts:
- PID controller
- ESC signal generator (ServoBlaster)
- UDP server to get command from an android device
- Communication with the IMU through I2C (RTIMULib)
- CLI to test over ssh connection


This project is based on source code from:
- https://github.com/big5824/Picopter

It uses ServoBlaster to control the ESCs:
- https://github.com/richardghirst/PiBits

It uses RTIMULib to handle the sensors:
- https://github.com/richards-tech/RTIMULib

Many thanks to these people.

Hardware
--------

This projects includes:
- 1 Raspberry Pi model B
- 1 QuadCopter frame
- 4 Electronic Speed Controllers (Q Brain 4 x 20A Brushless Quadcopter)
- 4 brushless motors (Turnigy Multistar 2213-980Kv)
- 2 props 9047 + 2 props 9047R (Right Hand Rotation) 
- 1 6 axis IMU supported by RTIMULib (MPU6050 + optional HMC5883L digital compass)
- 1 LiPo 3s battery (Turnigy 3000mAh 3S 20C Lipo Pack)
- 1 Android device for remote

Other useful hardware:
- 1 TURNIGY BESC Programming Card
- 1 TURNIGY 3~8S Voltage Detector
- 1 Hobbyking DC-4S Balance Charger & Cell Checker 30w 2s~4s 

Wiring
------

MPU6050 :
-VDD -> 3.3V
-GND -> GND
-SDA -> SDA
-SCL -> SCL
-VIO -> 3.3V


ESCs:
-GND -> GND
-Signal Servo 0 -> pin 15
-Signal Servo 1 -> pin 16
-Signal Servo 2 -> pin 18
-Signal Servo 3 -> pin 22



Interfacing Raspberry Pi and MPU-6050
------
(from: http://blog.bitify.co.uk/2013/11/interfacing-raspberry-pi-and-mpu-6050.html)

First you need to install the relevant Linux drivers, here's how.  Open the file for editing (needs sudo)
```
sudo vi /etc/modules
```
add the following lines to the bottom of the file, save it and reboot the Pi
```
i2c-bcm2708
i2c-dev
```
Now check the blacklists file
```
sudo vi /etc/modprobe.d/raspi-blacklist.conf
```
and make sure that the following lines start with a # (a comment) if they are present, if not don't worry
```
#blacklist spi-bcm2708
#blacklist i2c-bcm2708
```

To connect the sensor you need to use the GPIO pins on the Pi, the important pins are
•	Pin 1 - 3.3V connect to VCC
•	Pin 3 - SDA connect to SDA
•	Pin 5 - SCL connect to SCL
•	Pin 6 - Ground connect to GND
these need to be connect as shown in the image.
![alt text](http://3.bp.blogspot.com/-2604CaZam3k/UnlQY-50ejI/AAAAAAAAA3A/-UKwowAWSmA/s1600/IIC-to-6050.png "Logo Title Text 1")


Install & running ServoBlaster
------
```
git clone https://github.com/richardghirst/PiBits
cd PiBits/ServoBlaster
make servod
```
If you want to start ServoBlaster from command line:
```
sudo ./servod --p1pins=15,16,18,22,0,0,0,0 --min=1000us --max=2000us --cycle-time=20000
```
If you want to start ServoBlaster as daemon:
```
sudo make install
```
edit /etc/init.d/servoblaster to change the parameters


Install wirePi for Ultrasonic sensor
------
```
git clone git://git.drogon.net/wiringPi
cd wiringPi
./build
```