# xiaoxiang2ninebot_bms_protocol_converter
Xiaoxiang BMS &lt;=> Ninebot Z protocol converter
---
This project allows the use of a third-party [smartBMS](https://www.ebay.com/itm/14S-Lithium-Battery-Smart-Protective-Board-With-Bluetooth-Interface-48V-BMS-PCB/153488364010?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649) with Ninebot Z10 EUC. It works on Teensy 3.x/LC board, and probably can be adopted to other boards (e.g. STM based), but Atmega boards are not suitable, because they aren't able to support UART 115200 correctly. I use [teensy 3.6](https://www.pjrc.com/store/teensy36.html) in this project. 
I tested such build during two weeks, now I fixed all issues and ready to share it for all.

Detailed assembling guide will be added soon. 
