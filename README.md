# xiaoxiang2ninebot_bms_protocol_converter
Xiaoxiang BMS &lt;=> Ninebot Z protocol converter
---
This project allows the use of a third-party [smartBMS](https://www.ebay.com/itm/14S-Lithium-Battery-Smart-Protective-Board-With-Bluetooth-Interface-48V-BMS-PCB/153488364010?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649) with Ninebot Z10 EUC. It works on Teensy 3.x board, and probably can be adopted to other boards (e.g. STM based), but Atmega boards are not suitable, because they aren't able to support UART 115200 correctly. I use [teensy 3.6](https://www.pjrc.com/store/teensy36.html) in this project. 
I tested such build during two weeks, now I fixed all issues and ready to share it.
UPD: Working fine for several years.

# 2024 UPDATE:
It is recommended to use this modern [xiaoxiang BMS](https://aliexpress.ru/item/1005005648437341.html),  which should fit into battery box. Please see schemes directory to see HW changes required.<br />
Due to end of life for Teensy 3.6, the code was adopted to use Teensy 4.0 <br />
Also to decrease cost of such project, code was adopted to run on ESP32 Dev board, it is tested and working. ESP32 board is 1.5x bigger then Teensy 3.6, and 3x bigger then Teensy 4.0, but taking into account the price of ESP32, I think it will be acceptable for more people. <br />
Also, added capability to use it without Xiaoxiang BMS (and stupid BMS will work), in that case see scheme Teensy3.6_without_xiaoxiang_bms.jpg (for Teensy 4.0 it will be pin 15, for ESP32 it will be pin 35) and you need to uncommend following line in code:
```
#define ANALOG_MEASURE 
```
It is recommended to use 1M + 47K, basically you can use any resistors, but don't use too low resistors, as it will drain battery when Wheel is off. Don't forget to change following lines to your nominals:
```
#define R_LOW 470   //x100 Ohm
#define R_HIGH 1000 //x100 Ohm
```

# Hardware

WARNING: Teensy LC does't work with this project (not enough RAM)

Tested boards:
- teensy 3.6 (tested for years)
- teensy 3.2 (tested for years)
- teensy 4.0 (tested in lab)
- ESP32 Dev board (tested in lab)

Possible boards which may work:
- teensy 3.5, 4.1
- any ESP32 board with 3 UARTs (ESP32-S2 and ESP32-S3 -based boards are not applicable)
 
WARNING: We can use only 3.3v boards! Please avoid to use any 5v boards even for tests

It is recommended to power teensy from LED connector near battery (3-Pin connector with red, white and black wire, use red wire for +5v and black wire for GND). In this case it will not drain power when wheel is in shutdown state, and it is easy to reboot it if nessesary (by turn off/turn on Z10).
Don't forget to connect GND, you may use LED connector, or battery ground.

When connected and everything work properly - you should see correct battery indicator state, if not - don't try to ride - it is high probability to burn LM5576 and TDM31035. 

If you connect only teensy without connection xiaoxiang BMS, you should see 2 bars on battery indicator and 79% in application on both batteries (I don't recommend to use it without BMS)

It is mandatory to connect power to both mainboard inputs (battery A and Battery B), if you have only one battery you need connect it in parallel.

In case of any questions, you can contact me via telegram: https://t.me/z_z_z_77
