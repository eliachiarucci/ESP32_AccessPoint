# NodeMCU_AP_Configuration

This library is a template for connecting you Arduino NodeMCU to the wi-fi network without having to hard-code it.

When reset, the EEPROM will be empty and the NodeMCU will go into Access Point mode, from there, an interface is available where the user can choose their wi-fi network, after the password is inserted, the wi-fi credentials are stored in the EEPROM and the NodeMCU will reboot itself into working mode connected to the network.

ESP32 Drivers:
For MacOS (arm): https://github.com/WCHSoftGroup/ch34xser_macos
For Windows: [CP210x_Universal_Windows_Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads)
