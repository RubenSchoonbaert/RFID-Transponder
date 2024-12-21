Application Note for RFID Transponder source code
Overview

This application demonstrates the integration of an ST25DV NFC/RFID Tag IC, an SD card module, and an Arduino-compatible microcontroller. It provides functionality for reading and writing data to the RFID tag, managing records on an SD card, and testing the system's integrated components such as LEDs and a piezo buzzer.
Features

    RFID Tag Management:
        Write a record to the ST25DV RFID Tag IC.
        Clear the last RFID entry.
        Detect new records from the RFID tag and log them on the SD card.

    SD Card Integration:
        Store and retrieve records from a text file (records.txt).
        Check for duplicate records before saving.
        Clear all records on the SD card.

    System Testing:
        Test the onboard piezo buzzer with preprogrammed melodies.
        Test the LED with blink functionality.

    Command Interface:
        Interactive commands via Serial Monitor for managing RFID and SD card operations.

Command Reference

    $printFile
    Prints all records stored on the SD card.

    $clearFile
    Clears all records in records.txt on the SD card.

    $clearRFID
    Writes a placeholder (!ignore) to the RFID tag to clear its last entry.

    $addRecord <record>
    Adds a new record to both the RFID tag and the SD card.

    $testBuzzer
    Plays a short melody on the piezo buzzer.

    $testLED
    Blinks the onboard LED to verify functionality.

System Workflow

    Setup:
        Initializes the ST25DV RFID Tag IC and the SD card module.
        Displays a splash screen with available commands.

    Serial Commands:
        Waits for user input via Serial Monitor.
        Executes the corresponding command and provides feedback through Serial output and the buzzer/LED.

    RFID Reading:
        Continuously reads the URI stored on the RFID tag.
        If a new record is detected and is not a duplicate, logs it to the SD card.

    Data Logging:
        Uses the SD card to store a persistent record of RFID tag data.
        Ensures duplicate records are not logged to prevent redundancy.
