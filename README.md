# hardware

Hardware abstractions for [PROS](https://pros.cs.purdue.edu/)

This PROS template aims to simplify interactions with devices, and implement a common interface so virtually any custom sensors can be used in projects which depend on this template.

## Features

 - [X] [Unitized](https://github.com/LemLib/units)
 - [X] thread-safe
 - [X] Advanced Error Handling
 - [ ] Device disconnect/reconnect callbacks
 - [X] Compile-time Port Checks

 - [ ] **Motor**
   - [X] Changing encoder units don't affect reported angle
   - [X] Current limit
   - [X] Differentiate 11W and 5.5W motors
   - [X] -1.0 to +1.0 power levels, adjusts automatically to motor type
   - [X] Type Safe enums
   - [ ] Micro-disconnect detection

 - [ ] **Motor Groups**
   - [X] Motor disconnects/reconnects don't affect measured angle
   - [X] Removing motors doesn't affect the measured angle
   - [X] Automatic per-motor gear ratio calculations
   - [ ] Micro-disconnect detection

 - [ ] **Abstract Distance Sensor**
   - [ ] Generic interface for any distance sensor
   - [ ] Support for all VEX distance sensors
     - [ ] V5 Distance Sensor
     - [ ] V5 Optical Sensor
     - [ ] ADI Ultrasonic Sensor

 - [ ] **Abstract Color Sensor**
   - [ ] Generic interface for any color sensor
   - [ ] Support for all VEX color sensors
     - [ ] V5 Optical Sensor

 - [ ] **Abstract Encoders**
   - [X] Generic interface for any encoder
   - [X] Support for all VEX encoders
     - [X] Motors / Motor Groups
     - [X] V5 Rotation Sensor
     - [X] Optical Shaft encoder
     - [ ] ADI Potentiometer V1
     - [ ] ADI Potentiometer V2

 - [ ] **Abstract Gyro**
   - [ ] Generic interface for any gyro
   - [ ] Gyro Scaling
   - [ ] Support for all VEX gyros
     - [ ] V5 Inertial Sensor
     - [ ] V5 GPS Sensor
     - [ ] ADI analog gyro

 - [ ] **Abstract Accelerometer**
   - [ ] Generic interface for any accelerometer
   - [ ] Support for all VEX accelerometers
     - [ ] V5 Inertial Sensor
     - [ ] V5 GPS Sensor
     - [ ] ADI Accelerometer

 - [ ] **Abstract IMU**
   - [X] Generic interface for any IMU
   - [X] Gyro Scaling
   - [ ] Support for all VEX IMUs
     - [X] V5 Inertial Sensor
     - [ ] V5 GPS Sensor

## Who Should Use This?

Anyone who uses PROS. This API is simpler, safer, and more powerful than that of PROS. Library developers should use this in order to support custom sensors which may be used by VURC and VAIRC teams.
