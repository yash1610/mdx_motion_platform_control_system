import serial
ser = serial.Serial('/dev/ttyACM0')
ser.baudrate = 57600
while True:
	print(ser.readline())