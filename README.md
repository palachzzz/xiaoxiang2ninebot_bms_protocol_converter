# xiaoxiang2ninebot_bms_protocol_converter
Xiaoxiang BMS &lt;=> Ninebot Z protocol converter
---
This project allows the use of a third-party [smartBMS](https://www.ebay.com/itm/14S-Lithium-Battery-Smart-Protective-Board-With-Bluetooth-Interface-48V-BMS-PCB/153488364010?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649) with Ninebot Z10 EUC. It works on Teensy 3.x board, and probably can be adopted to other boards (e.g. STM based), but Atmega boards are not suitable, because they aren't able to support UART 115200 correctly. I use [teensy 3.6](https://www.pjrc.com/store/teensy36.html) in this project. 
I tested such build during two weeks, now I fixed all issues and ready to share it.
UPD: Working fine for several years.

WARNING: Teensy LC does't work with this project (not enough RAM)

Tested boards:
- teensy 3.6
- teensy 3.2
- teensy 4.0
 
I can't guarantee that it will work properly on others Teensy 3, and Teensy 4 (need to test). 
WARNING: We can use only 3.3v boards! Please avoid to use any 5v boards even for 

It is recommended to power teensy from LED connector near battery (3-Pin connector with red, white and black wire, use red wire for +5v and black wire for GND). In this case it will not drain power when wheel is in shutdown state, and it is easy to reboot it if nessesary (by turn off/turn on Z10).
Don't forget to connect GND, you may use LED connector, or battery ground.

When connected and everything work properly - you should see correct battery indicator state, if not - don't try to ride - it is high probability to burn LM5576 and TDM31035. 

If you connect only teensy without connection xiaoxiang BMS, you should see 2 bars on battery indicator and 21% in application on both batteries (I don't recommend to use it without BMS)

It is mandatory to connect power to both mainboard inputs (battery A and Battery B), if you have only one battery you need connect it in parallel.

In case of any questions, you can contact me via telegram: https://t.me/z_z_z_77
