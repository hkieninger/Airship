#!/bin/bash

#script based on https://www.raspberrypi.org/documentation/configuration/wireless/access-point.md
#reboots the raspberry pi
#script must be run as root to work, do not run it before you have run "access_point_reboot.sh"

cp /etc/dhcpcd.conf dhcpcd.conf.bak #make a backup if something goes wrong
mv -f dhcpcd.conf.default /etc/dhcpcd.conf

cp /etc/dnsmasq.conf dnsmasq.conf.bak #make a backup if something goes wrong
rm -f /etc/dnsmasq.conf #remove the file for the case it didn't exist before
mv -f dnsmasq.conf.default /etc/dnsmasq.conf #if the file didn't exist before nothing happens

cp /etc/hostapd/hostapd.conf hostapd.conf.bak #make a backup if something goes wrong
rm -f /etc/hostapd/hostapd.conf #remove the file for the case it didn't exist before
mv -f hostapd.conf.default /etc/hostapd/hostapd.conf

cp /etc/default/hostapd hostapd.bak #make a backup if something goes wrong
mv -f hostapd.default /etc/default/hostapd 

reboot