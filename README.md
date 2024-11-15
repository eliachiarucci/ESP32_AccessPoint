# ESP32 Access point template

This repository contains a template for connecting you ESP32 Devkit to your wi-fi network via a user interface.
The board will first go into Access Point (AP) mode and print its IP address on the serial console.
Steps to connect the ESP32 board to your WiFi connection:
- Connect to the ESP32 wifi network (default password: testtesttest).
- Open your browser and navigate to the IP address printed in the serial console.
- Select SSID and input WiFi password.
- The ESP32 will reboot and connect to the WiFi, the new IP address of board will be shown in the serial console.

By clicking the BOOT button on the DevKit, the saved WiFi credentials will be cleared and the process can be started again.

More routes can be added to the routes.ino file, or by simply adding additional files.
