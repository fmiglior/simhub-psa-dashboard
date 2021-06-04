# simhub-psa-dashboard
Conect a psa dashboard to the simhub, using canbus commands.
-

[![](https://img.youtube.com/vi/HzO3Sn_I2l4/0.jpg)](https://www.youtube.com/watch?v=HzO3Sn_I2l4)

## About
Using a arduino and a MCP2515, conect a canbus dashboard to any game using the software SIMHUB.

# What you need:

- Arduino Uno or NANO;
- MCP2515 and [MCP2515 Library](https://github.com/autowp/arduino-mcp2515);
- SIMHUB (https://www.simhubdash.com/)
- PSA can 2004 Dashboard (you can use any other CANBUS dashboard, need to adjust the code on the scketh)

# Arduino pinout:
![arduino pinout](https://github.com/fmiglior/simhub-psa-dashboard/blob/main/images/Pinagem%20Arduino.jpg)

## How to Connect and Install:
- Upload the DisplayClientV2.ino Sketch to the arduino. 
- on the simhub, selec "multiple arduinos"
![simhub](https://github.com/fmiglior/simhub-psa-dashboard/blob/main/images/simhub.jpg)
- Select the custom protocol option

![custom protocol](https://github.com/fmiglior/simhub-psa-dashboard/blob/main/images/Custom%20protocol.jpg)
- Use this protocol:
format([SpeedKmh],0)+';'+format([Rpms],0)+';'+format([DataCorePlugin.Computed.Fuel_Percent],0)+';'+ format([WaterTemperature],0)+';'+ [Handbrake] +';' + [TurnIndicatorLeft] +';'+ [TurnIndicatorRight]  +';'+ [TCActive] +';' + [ABSActive] + '/n'

- More infos about custom protocol here: (https://github.com/SHWotever/SimHub/wiki/Custom-Arduino-Hardware-Support)


# Changing the code to use other canbus dashboard
Find the can.h tab on the sketch, there was the can IDs i use to sen information to the PSA Dashboar.




