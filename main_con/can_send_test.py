# -*- coding: utf-8 -*-
import can
import sys
import time

Forward  = 1
Reverse  = 0
SOFTSTOP = 0
HARDSTOP = 1
SOFTHIZ  = 2
HARDHIZ  = 3

L6470_Run  = 0x50
L6470_Move = 0x40
L6470_Stop = 0x00

def main():
	# バスの初期化
	bus = can.interface.Bus(channel = 'can0', bustype='socketcan_native', bitrate=1000000, canfilters=None)

	msg = can.Message(arbitration_id = 0x123,
		data = [0,0,0,0,0,0,0,0],
		extended_id = False)	

	while(1):
		i=int(input("servo(1) or stepping(2)? >>"))
		msg.data[0] = i
		if i == 1:
			i=int(input("angle? >>"))
			msg.data[1] = i
			try:
  				bus.send(msg)
  				print("Message sent on {}".format(bus.channel_info))
			except can .CanError:
				print("Message NOT sent")

		elif i == 2:
			dt = cmd_change(500,3)
			msg = can.Message(arbitration_id = 0x123,
				data = [0x10,L6470_Move,Forward,dt[2],dt[1],dt[0],0,0],
				extended_id = False)
			try:
  				bus.send(msg)
  				print("Message sent on {}".format(bus.channel_info))
			except can .CanError:
				print("Message NOT sent")
			time.sleep(1)

			'''	
			msg = can.Message(arbitration_id = 0x123,
				data = [0x10,L6470_Stop,HARDSTOP,0,0,0,0,0],
				extended_id = False)
			try:
  				bus.send(msg)
  				print("Message sent on {}".format(bus.channel_info))
			except can .CanError:
				print("Message NOT sent")
			time.sleep(1)
			'''
			msg = can.Message(arbitration_id = 0x123,
				data = [0x10,L6470_Move,Reverse,dt[2],dt[1],dt[0],0,0],
				extended_id = False)
			try:
  				bus.send(msg)
  				print("Message sent on {}".format(bus.channel_info))
			except can .CanError:
				print("Message NOT sent")
			time.sleep(1)
			'''
			msg = can.Message(arbitration_id = 0x123,
				data = [0x10,L6470_Stop,HARDSTOP,0,0,0,0,0],
				extended_id = False)
			try:
  				bus.send(msg)
  				print("Message sent on {}".format(bus.channel_info))
			except can .CanError:
				print("Message NOT sent")
			time.sleep(1)
			'''
def cmd_change(val, num):
	i = 0
	dt = []
	while i < num:
		dt.append(val & 0xff)
		val = val >> 8
		i += 1
	print(dt)
	return dt
		
		
if __name__ == '__main__':
	main()