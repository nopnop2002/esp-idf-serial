import serial
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--device', required=True, help="uart device file name")
args = parser.parse_args()
print("device={}".format(args.device))
ser = serial.Serial(args.device, 115200)
while True:
	c = ser.read()
	print("c={}".format(c))
ser.close()

