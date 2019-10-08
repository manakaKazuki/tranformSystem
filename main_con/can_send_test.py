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

L6470_Stop = 0x00
L6470_Run  = 0x01
L6470_Move = 0x02


def main():
	# バスの初期化
	bus = can.interface.Bus(channel = 'can0', bustype='socketcan_native', bitrate=500000, canfilters=None)

	angle = 0	

	while(1):
		msg = can.Message(arbitration_id = 0x123,
		data = [0,0,0,0,0,0,0,0],
		extended_id = False)
		
		can_id=int(input("arm_con No?(2~5) >>"))
		if can_id == 2:
			can_id = 0x102
		elif can_id == 3:
			can_id = 0x103
		elif can_id == 4:
			can_id = 0x104
		elif can_id == 5:
			can_id = 0x105
		

		i=int(input("servo(1) or stepping(2)? >>"))
		if i == 1:
			angle=int(input("angle? >>"))
			msg = can.Message(arbitration_id = can_id,
			data = [0,angle,L6470_Stop,0,0,0,0,HARDSTOP],
			extended_id = False)

		elif i == 2:
			dt = cmd_change(500,3)
			msg = can.Message(arbitration_id = can_id,
			data = [0,angle,L6470_Move,Forward,dt[2],dt[1],dt[0],0],
			extended_id = False)
			try:
  				bus.send(msg)
  				print("Message sent on {}".format(bus.channel_info))
			except can .CanError:
				print("Message NOT sent")
			time.sleep(1)

			msg = can.Message(arbitration_id = can_id,
			data = [0,angle,L6470_Move,Reverse,dt[2],dt[1],dt[0],0],
			extended_id = False)
			try:
  				bus.send(msg)
  				print("Message sent on {}".format(bus.channel_info))
			except can .CanError:
				print("Message NOT sent")
			time.sleep(1)

			msg = can.Message(arbitration_id = can_id,
			data = [0,angle,L6470_Stop,0,0,0,0,HARDSTOP],
			extended_id = False)

			
		try:
  			bus.send(msg)
  			print("Message sent on {}".format(bus.channel_info))
		except can .CanError:
			print("Message NOT sent")

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