# DynamixelExample

Everything you need to build an Arduino shield that can communicate with Dynamixel servos.


## Hardware

DynamixelShield.fzz - Shield design files, built with [Fritzing](http://fritzing.org). It's essentially a MAX485 chip that converts the output of the Arduino's Tx/Rx pins to RS-485.


## Software
Examples of two ways to send commands to the servos:

* DynamixelExample_Write - Send one command to one servo. 

* DynamixelExample_SyncWrite - Send commands to a group of servos with one large packet instead of a bunch of individual ones.