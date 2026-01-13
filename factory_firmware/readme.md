# How to install the factory demo firmware with flash download tool?

#### 1.Preparation:

- USB C cable
- Laptop/PC
- ESP32 Solo Miner LCD Display 
- Flash download tool

Check whether the materials on the PCBA board are defective. Such as less material, false welding, tin and other bad phenomena.

#### 2.Flash the firmware 

##### Step1 Connect the ESP32 P4 display to the laptop/PC with USB-C cable. The power indicator will light up.

##### Step2 Open the flash download tool. 

![flash02](./flash02.jpg)



For the  Crowpanel Advance HMI ESP32 P4  AI Display, the chip select ESP32:

![firmware1](https://github.com/user-attachments/assets/cd178e01-45b8-46bb-8434-bd1ea500733c)



##### Step3

①Choose the firmware files;

②Enter the address: 

boot_app0.bin------0xe000

xxxx.ino.bin--------0x10000

xxxx.ino.bootloader.bin------0x1000

xxxx.ino.partitions.bin-------0x8000



③Check the firmware files;

④Select the correct port number;

⑤Click “Start” to download

![firmware2](https://github.com/user-attachments/assets/684665a0-6671-4369-8269-7e0bf19a2824)




##### Step4 Press the reset button to show the demo.



