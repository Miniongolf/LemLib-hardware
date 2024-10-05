# hardware

Hardware abstractions for [PROS](https://pros.cs.purdue.edu/) and the VEX V5

This PROS template aims to simplify interactions with devices, and implement a common interface so virtually any custom sensors can be used in projects which depend on this template.


## Features

 - [X] **[Units](https://github.com/LemLib/units)**
 - [X] **Uncompromising error handling**
 - [X] **Motor Class**
    - [X] Changing encoder units don't affect reported angle
    - [X] Differentiation between 11W and 5.5W motors
    - [X] -1.0 to +1.0 power levels, adjusts automatically to motor type
    - [X] Type Safe enums

 - [X] **Motor Groups**
    - [X] Motor disconnects/reconnects don't affect reported angle
    - [X] Adding motors automatically sets their brake mode and measured angle
    - [X] Removing motors doesn't affect the measured angle
    - [X] Automatic per-motor gear ratio calculations. Just input port and cartridge. Supports different cartridges in the same group

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
        - [ ] V5 GPS Sensor (pending viability tests, gyro only)


## Who Should Use This?

This template is geared towards developers of PROS templates such as [LemLib](https://github.com/LemLib/LemLib) and its users, especially VURC and VAIRC teams.
