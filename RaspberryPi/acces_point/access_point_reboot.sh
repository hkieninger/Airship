#!/bin/bash

#script based on https://www.raspberrypi.org/documentation/configuration/wireless/access-point.md
#reboots the raspberry pi and configures that it is an access point on reboot
#in further reboots it will remain an access point to undo this script run the counter script "defaut_reboot.sh"
#dnsmasq and hostapd must be installed for the script to work
#script must be run as root to work

systemctl stop dnsmasq
systemctl stop hostapd

mv -f /etc/dhcpcd.conf dhcpcd.conf.default
cp dhcpcd.conf.ap /etc/dhcpcd.conf

service dhcpcd restart

mv -f /etc/dnsmasq.conf dnsmasq.conf.default
cp dnsmasq.conf.ap /etc/dnsmasq.conf

mv -f /etc/hostapd/hostapd.conf hostapd.conf.default
cp hostapd.conf.ap /etc/hostapd/hostapd.conf

mv -f /etc/default/hostapd hostapd.default
cp hostapd.ap /etc/default/hostapd

reboot