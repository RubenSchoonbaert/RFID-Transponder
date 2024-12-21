# RFID-Transponder

![IMG_20240116_194437709](https://github.com/user-attachments/assets/b20db4b7-7c62-4658-ac37-d2e379cdeea6)

Overview

This project is a compact and versatile PCB design featuring an RFID/NFC tag IC. It includes components for writing and managing data on the RFID tag, modern power input options, and flexible microcontroller integration.
Key Features

    RFID/NFC Tag Functionality
        Built-in RFID coil, designed using eDesignSuite by ST-Engineering.
        Integrated with the ST25DV04KC RFID/NFC Tag IC, offering:
            16 KB EEPROM for data storage.
            Communication via the I2C bus.
        Data can be written to the tag using external devices, such as smartphones with RFID capabilities.

    Data Storage Support
        Includes a Micro SD card slot for additional data storage.
        Utilizes a 74LVC125 IC for safe voltage level shifting between the 5V logic of the microcontroller and the 3.3V logic of the Micro SD card.

    Power Input
        Powered via a USB-C port delivering 5V.
        Configured with 1 kΩ resistors on the CC pins to ensure USB-C standard compliance.

    Flexible Microcontroller Integration
        Designed around the AtmegaXX08 series of microcontrollers, specifically the Atmega3208.
        Compatible with other ICs in the same series and package.

    Connectivity
        Provides connectors for UPDI and UART for programming and debugging.
        Optional GPIO breakout for additional interfacing needs.

Applications

    Use as an RFID/NFC tag in systems requiring dynamic, writable RFID data storage.
    Embedded designs requiring compact, integrated EEPROM and I2C functionality.
    Systems needing programmable and customizable tag IC solutions.
