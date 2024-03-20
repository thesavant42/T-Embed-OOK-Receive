![Alt text](images/IMG_2521.jpg?raw=true "OOKRX")

This is a sample implementation of the rtl_433_ESP library with the CC1101 Transceiver Module. This fork has been patched for the Lilygo T-Embed

## ISM Band RF Stumbler

The idea is to implement netstumbler-like discovery and mapping functionality, but for subghz radio in the ISM bands. ISM - (Industrial/Scientific/Medical) band radio traffic is found eminating from all sorts of devices. I acdidentally stumbled upon neighboring sensors while adding one to my Home Assistant lab and thought "Say, I could map that."

The Lilygo T-Embed is a combination of their T-Display S3 esp32-s3 development board and a roatary encoder w/ RGB LEDs. Also built in are an i2s DAC, built in microSD, and an expansion module via a pogo-pin backpack with a TI CC1101 radio, NFC... There's even a grove i2c header, all for about $60. Quite a handy little development board!



## CC1101 Transceiver Module

T-Embed's backpack uses a custom SPI. This has been handle automatically via the platformio.ini definitions for pin mappings, and using a fork of the rtl_433_ESP library, patched to use HSPI.

# Sample Outputs

## CC1101 Device
```
�N:  
N: ****** setup ******
rtl_433_ESP(6): CC1101 gpio receive pin: 22
rtl_433_ESP(6): CC1101 receive frequency: 433.920013
rtl_433_ESP(6): # of device(s) configured 101
rtl_433_ESP(6): ssizeof(r_device): 108
rtl_433_ESP(6): cfg->devices size: 10908
rtl_433_ESP(6): # of device(s) enabled 100
M       CC1101
rtl_433_ESP(6): CC1101 radio.begin() success!
rtl_433_ESP(6): CC1101 setOOK - success!
rtl_433_ESP(6): CC1101 setCrcFiltering - success!
rtl_433_ESP(6): CC1101 receiveDirect - success!
----- CC1101 Status -----
CC1101_MDMCFG1: 0x02
CC1101_MDMCFG2: 0x32
CC1101_MDMCFG3: 0x83
CC1101_MDMCFG4: 0xa7
-------------------------
CC1101_DEVIATN: 0x14
CC1101_AGCCTRL0: 0x91
CC1101_AGCCTRL1: 0x40
CC1101_AGCCTRL2: 0x03
-------------------------
CC1101_IOCFG0: 0x0d
CC1101_IOCFG1: 0x2e
CC1101_IOCFG2: 0x29
-------------------------
CC1101_FIFOTHR: 0x07
CC1101_SYNC0: 0xad
CC1101_SYNC1: 0x12
-------------------------
CC1101_PKTLEN: 0xff
CC1101_PKTCTRL0: 0x32
CC1101_PKTCTRL1: 0x04
-------------------------
CC1101_ADDR: 0x00
CC1101_CHANNR: 0x00
CC1101_FSCTRL0: 0x00
CC1101_FSCTRL1: 0x0f
-------------------------
CC1101_FREQ0: 0x71
CC1101_FREQ1: 0xb0
CC1101_FREQ2: 0x10
-------------------------
CC1101_MCSM0: 0x14
CC1101_MCSM1: 0x30
CC1101_MCSM2: 0x07
-------------------------
CC1101_FOCCFG: 0x76
CC1101_BSCFG: 0x6c
CC1101_WOREVT0: 0x6b
CC1101_WOREVT1: 0x87
CC1101_WORCTRL: 0xf8
CC1101_FREND0: 0x11
CC1101_FREND1: 0x56
-------------------------
CC1101_FSCAL0: 0x0d
CC1101_FSCAL1: 0x3b
CC1101_FSCAL2: 0x0c
CC1101_FSCAL3: 0xaa
-------------------------
CC1101_RCCTRL0: 0x00
CC1101_RCCTRL1: 0x41
-------------------------
CC1101_PARTNUM: 0x00
CC1101_VERSION: 0x14
CC1101_MARCSTATE: 0x0d
CC1101_PKTSTATUS: 0xe0
CC1101_RXBYTES: 0x00
----- CC1101 Status -----
N: ****** setup complete ******
rtl_433_ESP(6): Signal length: 220996, Gap length: 6248856, Signal RSSI: -57, train: 0, messageCount: 0, pulses: 169
rtl_433_ESP(6): data_output {"model":"Acurite-Tower","id":5989,"channel":"A","battery_ok":1,"temperature_C":21.5,"humidity":127,"mic":"CHECKSUM","protocol":"Acurite 592TXR Temp/Humidity, 5n1 Weather Station, 6045 Lightning, 3N1, Atlas","rssi":-57,"duration":220996}
N: Received message : {"model":"Acurite-Tower","id":5989,"channel":"A","battery_ok":1,"temperature_C":21.5,"humidity":127,"mic":"CHECKSUM","protocol":"Acurite 592TXR Temp/Humidity, 5n1 Weather Station, 6045 Lightning, 3N1, Atlas","rssi":-57,"duration":220996}
rtl_433_ESP(6): data_output {"model":"Acurite-Tower","id":5989,"channel":"A","battery_ok":1,"temperature_C":21.5,"humidity":127,"mic":"CHECKSUM","protocol":"Acurite 592TXR Temp/Humidity, 5n1 Weather Station, 6045 Lightning, 3N1, Atlas","rssi":-57,"duration":220996}
N: Received message : {"model":"Acurite-Tower","id":5989,"channel":"A","battery_ok":1,"temperature_C":21.5,"humidity":127,"mic":"CHECKSUM","protocol":"Acurite 592TXR Temp/Humidity, 5n1 Weather Station, 6045 Lightning, 3N1, Atlas","rssi":-57,"duration":220996}
rtl_433_ESP(6): # of messages decoded 2
 
rtl_433_ESP(6): Ignored Signal length: 161039, Gap length: 2207276, Signal RSSI: -79, Current RSSI: -87, pulses: 6
rtl_433_ESP(6): Signal length: 439000, Gap length: 4780726, Signal RSSI: -29, train: 1, messageCount: 1, pulses: 222
rtl_433_ESP(6): data_output {"model":"Acurite-Tower","id":2043,"channel":"B","battery_ok":1,"temperature_C":24.7,"humidity":84,"mic":"CHECKSUM","protocol":"Acurite 592TXR Temp/Humidity, 5n1 Weather Station, 6045 Lightning, 3N1, Atlas","rssi":-29,"duration":439000}
N: Received message : {"model":"Acurite-Tower","id":2043,"channel":"B","battery_ok":1,"temperature_C":24.7,"humidity":84,"mic":"CHECKSUM","protocol":"Acurite 592TXR Temp/Humidity, 5n1 Weather Station, 6045 Lightning, 3N1, Atlas","rssi":-29,"duration":439000}
rtl_433_ESP(6): data_output {"model":"Acurite-Tower","id":2043,"channel":"B","battery_ok":1,"temperature_C":24.7,"humidity":84,"mic":"CHECKSUM","protocol":"Acurite 592TXR Temp/Humidity, 5n1 Weather Station, 6045 Lightning, 3N1, Atlas","rssi":-29,"duration":439000}
N: Received message : {"model":"Acurite-Tower","id":2043,"channel":"B","battery_ok":1,"temperature_C":24.7,"humidity":84,"mic":"CHECKSUM","protocol":"Acurite 592TXR Temp/Humidity, 5n1 Weather Station, 6045 Lightning, 3N1, Atlas","rssi":-29,"duration":439000}
rtl_433_ESP(6): # of messages decoded 2
...
 
rtl_433_ESP(6): Signal length: 215000, Gap length: 8389809, Signal RSSI: -55, train: 0, messageCount: 2, pulses: 162
rtl_433_ESP(6): data_output {"model":"Acurite-Tower","id":5989,"channel":"A","battery_ok":1,"temperature_C":21.5,"humidity":127,"mic":"CHECKSUM","protocol":"Acurite 592TXR Temp/Humidity, 5n1 Weather Station, 6045 Lightning, 3N1, Atlas","rssi":-55,"duration":215000}
N: Received message : {"model":"Acurite-Tower","id":5989,"channel":"A","battery_ok":1,"temperature_C":21.5,"humidity":127,"mic":"CHECKSUM","protocol":"Acurite 592TXR Temp/Humidity, 5n1 Weather Station, 6045 Lightning, 3N1, Atlas","rssi":-55,"duration":215000}
rtl_433_ESP(6): data_output {"model":"Acurite-Tower","id":5989,"channel":"A","battery_ok":1,"temperature_C":21.5,"humidity":127,"mic":"CHECKSUM","protocol":"Acurite 592TXR Temp/Humidity, 5n1 Weather Station, 6045 Lightning, 3N1, Atlas","rssi":-55,"duration":215000}
N: Received message : {"model":"Acurite-Tower","id":5989,"channel":"A","battery_ok":1,"temperature_C":21.5,"humidity":127,"mic":"CHECKSUM","protocol":"Acurite 592TXR Temp/Humidity, 5n1 Weather Station, 6045 Lightning, 3N1, Atlas","rssi":-55,"duration":215000}
rtl_433_ESP(6): # of messages decoded 2
```
