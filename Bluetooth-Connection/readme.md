# Configuring the BeagleBone Black for Bluetooth Operation

Follow these steps before getting started:
-Disconnect the USB cable and/or power from the device.
-Plug the USB Bluetooth Dongle into the device.
-Connect an Ethernet cable to the device.
-Power the device using an external 5 volt supply.
-"ssh" into the device from NUC by typing the following:

  ssh 192.168.7.2

Make sure the Dongle is recognized by entering the following command:

  root@beaglebone:~# lsusb 
  Bus 001 Device 003: ID 0a5c:21e8 Broadcom Corp. 
  Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub 
  Bus 002 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub

You should see an entry that looks something like the following for the Dongle:

  Bus 001 Device 003: ID 0a5c:21e8 Broadcom Corp.

Run the following command to install bluez package

  root@beaglebone:~# apt-get update
  root@beaglebone:~# apt-get install bluez

After installation, the bluetooth service should have started and you will see a screen that looks like:

  root@beaglebone:~# service bluetooth status 
  bluetooth.service - Bluetooth service 
  Loaded: loaded (/lib/systemd/system/bluetooth.service; enabled) 
  ctive: active (running) since Sun, 01 Mar 2015 14:46:09 -0600; 3 weeks and 4 days ago 
  Main PID: 1169 (bluetoothd) 
    CGroup: name=systemd:/system/bluetooth.service 
          └ 1169 /usr/sbin/bluetoothd -n

Make sure that the Bluetooth device is powered up. Its LED should be blinking. To find the device MAC address, type the following (this may take a few moments):

  root@beaglebone:~# hcitool scan 
  Scanning ... 
    78:CA:39:BF:9B:0F	Fahad's iMac 
    EC:FE:7E:10:E0:96	BlueRadios10E096 
    E0:06:E6:B7:E7:F3	ZHIYUCHEN-PC 
    F4:B7:E2:CD:1F:A8	CAMPUS 
    5C:F3:70:64:1D:B1	LIYANQING-PC 
    28:B2:BD:BC:45:48	ALI-PC 

Write down the mac address. You will need it for the next step.
Modify rfcomm.conf: (there is a sample here)

  root@beaglebone:~# vi /etc/bluetooth/rfcomm.conf

If you need to connect more than one device you have to add the same configuration lines under the last line for rfcomm1 and so on.
Pair the Dongle with the device. This command is being issued in the background by the "&" appended at the end of the command. 1234 is the default pairing address used by the device.

  root@beaglebone:~# bluetooth-agent 1234 & 
  [1] 1776 

Finally connect your device to rfcomm, using the mac address you wrote down above. Notice that this command is run in the background, as was the previous command:

  root@beaglebone:~# rfcomm connect hci0 20:FA:BB:01:62:53 & 
  [2] 2142

You are now ready to use Bluetooth. 
From this point forward, to start Bluetooth, you only need to type the # rfcomm connect rfcomm0 command.
For more information of how to use rfcomm you use the following command:

  root@beaglebone:~# rfcomm –help

This is the result of the UTD binary decision MotionNet sensors.
  root@beaglebone:~# rfcomm -a 
  rfcomm0: 5C:F3:70:64:F8:1E -> 20:FA:BB:01:62:53 channel 1 connected [reuse-dlc release-on-hup tty-attached]  
  root@beaglebone:~# cat /dev/rfcomm0 
  OCOCOCOCOCOCOCOCOCOCOC 

References:
http://www.instructables.com/id/Wireless-Arduino-Control-Using-the-BeagleBone-Blac/step2/Configuring-the-BBB-or-RPi-for-Bluetooth-Operation/ 
