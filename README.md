# hardware

Hardware abstractions for [PROS](https://pros.cs.purdue.edu/)

This PROS template aims to simplify interactions with devices, and implement a common interface so virtually any custom sensors can be used in projects which depend on this template.

## Features

 - [X] **[Unitized](https://github.com/LemLib/units)**
 - [X] **Uncompromising error handling**
 - [ ] 5ms sensor updates
 - [ ] Device disconnect/reconnect callbacks

 - [X] **Motor Class**
    - [X] Changing encoder units don't affect reported angle
    - [ ] Current limit
    - [X] Differentiate 11W and 5.5W motors
    - [X] -1.0 to +1.0 power levels, adjusts automatically to motor type
    - [X] Type Safe enums
    - [ ] Custom Velocity controller
    - [ ] Custom Position controller

 - [ ] **Motor Groups**
    - [X] Motor disconnects/reconnects don't affect measured angle
    - [X] Removing motors doesn't affect the measured angle
    - [X] Automatic per-motor gear ratio calculations
    - [ ] Custom Velocity controller
    - [ ] Custom Position controller

 - [X] **Abstract Encoders**
    - [X] Generic interface for any encoder
    - [X] Support for all VEX encoders
        - [X] All custom encoders
        - [X] Motors / Motor Groups
        - [X] V5 Rotation Sensor
        - [X] Optical Shaft encoder

 - [X] **Abstract Gyro**
    - [X] Generic interface for any gyro
    - [X] Support for VEX gyros, as well as any custom gyros for use in VEX AI or VEX U
        - [X] All custom gyros
        - [X] V5 Inertial Sensor


## Who Should Use This?

Anyone who uses PROS. This API is simpler, safer, and more powerful than that of PROS. Library developers should use this in order to support custom sensors which may be used by VURC and VAIRC teams.