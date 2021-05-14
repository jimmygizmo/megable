# megable
Experimental - Arduino and Bluetooth Low Energy - Mega2560 and HM10 ('megable' = 'mega' + 'ble')

You can easily adapt this project to use any Arduino and/or any Bluetooth module you like.
Different boards and modules may require using different 3rd-party libraries.
I am using the following hardware:

Arduino:
Mega 2560 R3 Plus, made by OSEPP

Bluetooth Low Energy (BLE/BTLE) module:
HM-10, made by DSD Tech.

The functionality of this experimental project will evolve, beginning with making a simple connection.

One of the motivating factors is to change from experimenting with an HC-05 Bluetooth 2.0 module to
using BLE and the HM-10. Since I use an iPhone, and since iOS no longer supports old BT 2.0 like it
used to, connecting to an iPhone with an HC-05 or an HC-06 is no longer possible except for Apple
MFi program-certified vendors, which I doubt applies to any HC-05 or HC-06 modules out there.

So to test/develop with my iPhone I need to use Bluetooth Low Energy, aka Bluetooth 4.0 (and later)
aka BLE (sometimes even referred to as BTLE.)

I believe there is an HM-11 and others available, but the typical thing to start with for BLE is
the HM-10. Perhaps this project will evolve to cover other modules. Also, the 'mega' in the title does
not exclude supporting other Arduino boards, but UNO and many others should require no code changes.

