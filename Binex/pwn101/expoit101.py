import pwn

io = pwn.remote("10.6.1.80",9001)
io.interactive()
