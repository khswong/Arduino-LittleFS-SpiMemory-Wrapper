# Arduino-LittleFS-SpiMemory-Wrapper

A simple LittleFS wrapper and example implementation using the SpiMemory library. 
I haven't seen anyone post an example implementation of this working, so I decided to give it a crack.  

This depends on ARMmbed's [LittleFS](https://github.com/ARMmbed/littlefs) and Marzogh's [SPIMemory](https://github.com/Marzogh/SPIMemory) Arduino library. Pull both of them into your Arduino library path.

I tested this on an Adesto AT25SF041 flash IC and an ATSAMD21G18-based Arduino Feather clone (Cortex-M0+ based). 
