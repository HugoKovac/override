import os
import subprocess

max_pid = subprocess.check_output(["cat", "/proc/sys/kernel/pid_max"])

print("Max pid : {}".format(int(max_pid)))

command = input("Enter the command to execute : ")

for i in range(int(max_pid)):
	try:
		pid = os.fork()
	except BlockingIOError:
		result = subprocess.check_output(command.split())
		print(result)
	if pid == 0:
		exit()

print("{} or more are running".format(max_pid.decode("utf-8")))