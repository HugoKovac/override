import subprocess


for i in range(322424814, 322425824):
    p = subprocess.Popen(["echo", "-ne", str(i)], stdout=subprocess.PIPE)
    result = subprocess.check_output(["/home/users/level03/level03"], stdin=p.stdout)
    p.wait()
    print(i)
    if not result.split('\n')[4] == 'Invalid Password':
        print(i, result)