import serial
import argparse
import queue
import threading

queue01 = queue.Queue() # main-> newthread
queue02 = queue.Queue() # newThread -> main

def proc(q1, q2):
	while True:
		c = input()
		print("input=[{}]".format(c))
		q2.put(c)

parser = argparse.ArgumentParser()
parser.add_argument('--device', required=True, help="uart device file name")
args = parser.parse_args()
print("device={}".format(args.device))
ser = serial.Serial(args.device, 115200)

th1 = threading.Thread(target=proc, args=(queue01, queue02,))
th1.start()

while True:
	c = ser.read()
	print("c={}".format(c))
	if not queue02.empty():
		send_data = queue02.get()
		print("send_data=[{}] {}".format(send_data, type(send_data)))
		ser.write(send_data.encode(encoding='utf-8'))

ser.close()

