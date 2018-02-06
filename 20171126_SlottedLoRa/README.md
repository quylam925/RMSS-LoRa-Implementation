# ETRI Private LoRaWAN Communication Module Program (STM32F/L)

## Introduction
----------------
The aim of this project is to develop ETRI Private LoRaWAN stack implementation.

This Private LoRaWAN Communication Module runs on STM32F/L embeded system.

This project provides both LoRaWAN End device module and LoRaWAN GateWay module.

Supporting Device - ubimicro LoRa module (http://www.ubimicro.co.kr/)


## Archtecture
---------------
```
 +------------+   +--------+           +--------+   +---------+
 | LoRa       |   |  LoRa  |    LoRa   |  LoRa  |   | LoRaWAN |
 | End Device |<->| Module |<- - - - ->| Module |<->| GateWay |
 +------------+   +--------+           +--------+   +---------+
```


## Directory 
-------------
 * Src      : Target source files (including headers)
 * Inc      : Include files
 * Debug    : Debug mode objects and programs
 * Release  : Release mode objects and programs
 * Document : README and BugReporting files
 * Deivers / startup / Includes / Middlewares


## How to use?
---------------
 * Compile on STM32WORK
 * Download Program using ST-Link or other JTAG device
 

## Acknowledgments
-------------------
 * Copyright (c) 2017, ETRI (Electronics and Telecommunications Research Institute) All rights reserved.
 * This program uses the SX1276 chipset control codes by Semtech (Authors: Miguel Luis, Gregory Cristian and Wael Guibene)


## Changelog
-------------
** 2017-09-12 v0.2.1 **
 * Minor bug fix

** 2017-09-01 V0.2 **
 * LoRa Packet Size
   1. Default maximum size of LoRa has been increased to 256 bytes from 64 bytes (sx1276-ubiboard.h)
   2. VCP Tx/Rx Bug fix. - Support more than 64 bytes (main.c)
 * End Device LoRa module
   1. Join Process added
   2. Encryption
   3. Key Generation (Network Shared Key, Application Shared Key)

** 2017-08-03 V0.1 **
 * Gateway LoRa module
   1. LoRa Frame Encapsulation
   2. LoRa Communication via SX1276 chipset (SX1301 will be supported soon)
   3. LoRa Enddevice Rx Information (RSSI, SNR)
 * End Device LoRa module
   1. LoRa Communication via SX1276 chipset
   2. Rx LoRaWAN frame handling
   3. Tx LoRaWAN frame Generation
   4. Encryption - AES128
   5. MIC Check - AES128-CMAC
   6. Class A support
 * End device applications
   1. RWSMS (Real-time Works' Safety Monitoring System)
   2. Gass Detection Device
   3. User Program
 * Source File
   1. main.c         : Main routine
   2. lora_mac.c     : LoRaWAN Frame Handling
   3. sx1276.c       : Semtech shipset control
   4. crypto.c       : Encryption
   5. aes.c          : AES 128bit (EBC)
   6. aes-cmac.c     : AES 128bit base CMAC (RFC4493)
   7. application.c  : Application handling


