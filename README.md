bleuart

A utility that functions similar to rfcomm, but for cheap china bluetooth low energy uart bridges, such as the HM-10

Dependencies: [gattlib](https://github.com/labapart/gattlib)

How to use this code:
```
git clone https://github.com/labapart/gattlib.git
cd gattlib
mkdir build
cd build
cmake ../
make
cpack -G DEB
sudo dpkg -i gattlib_0.2-dev_x86_64.deb 

cd ../..
git clone git@github.com:tiger12506/bleuart.git
cd bleuart
make
bluetoothctl
./bleuart -d 00:15:83:10:EA:BE -c 0000ffe1-0000-1000-8000-00805f9b34fb
```

In order to find bluetooth address and characteristic uuid, use bluetoothctl
```
jacob@js:~/bleuart$ bluetoothctl
[NEW] Device 00:15:83:10:EA:BE CC41-A

... much output withheld for brevity ...

[bluetooth]# connect 00:15:83:10:EA:BE 
Attempting to connect to 00:15:83:10:EA:BE
[CHG] Device 00:15:83:10:EA:BE Connected: yes
Connection successful
[CHG] Device 00:15:83:10:EA:BE ServicesResolved: yes

[CC41-A]# list-attributes 00:15:83:10:EA:BE 
Primary Service
	/org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023
	0000ffe0-0000-1000-8000-00805f9b34fb
	Unknown
Characteristic
	/org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024
	0000ffe1-0000-1000-8000-00805f9b34fb
	Unknown
Descriptor
	/org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0027
	00002901-0000-1000-8000-00805f9b34fb
	Characteristic User Description
Descriptor
	/org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0026
	00002902-0000-1000-8000-00805f9b34fb
	Client Characteristic Configuration

... other attributes withheld for brevity ...

[CC41-A]# select-attribute /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0027 
[CC41-A:/service0023/char0024/desc0027]# read
Attempting to read /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0027
[CHG] Attribute /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0027 Value: 0x20
[CHG] Attribute /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0027 Value: 0x54
[CHG] Attribute /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0027 Value: 0x58
[CHG] Attribute /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0027 Value: 0x20
[CHG] Attribute /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0027 Value: 0x26
[CHG] Attribute /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0027 Value: 0x20
[CHG] Attribute /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0027 Value: 0x52
[CHG] Attribute /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0027 Value: 0x58
[CHG] Attribute /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024/desc0027 Value: 0x20
  20 54 58 20 26 20 52 58 20                        TX & RX        

... Looks like char0024 is the serial characteristic, because it's "User Description" descriptor has this value ...

[CC41-A:/service0023/char0024/desc0027]# select-attribute /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024

[CC41-A:/service0023/char0024]# read
Attempting to read /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024
[CHG] Attribute /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024 Value: 0x0a
  0a                                               .               

... I'm not sure this is meant to work, notify will work, however ...

[CC41-A:/service0023/char0024]# write 0x30
Attempting to write /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024


... Write a '0' character through the bluetooth uart characteristic. Should appear on the TX pin of the HM-10 ...

[CC41-A:/service0023/char0024]# attribute-info /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023/char0024
Characteristic - Unknown
	UUID: 0000ffe1-0000-1000-8000-00805f9b34fb
	Service: /org/bluez/hci0/dev_00_15_83_10_EA_BE/service0023
	Value: 0x0a
	Notifying: no
	Flags: read
	Flags: write-without-response
	Flags: write
	Flags: notify
[CC41-A:/service0023/char0024]# 


... There's the UUID we want, that belongs to the serial characteristic we found ...

```
