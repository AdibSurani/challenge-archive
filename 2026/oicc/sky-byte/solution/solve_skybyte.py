from pwn import *

with remote('challenges.oceaniacc.com', 6005) as io:
    io.sendlines([bytes(16)] * 1337)
    for _ in range(1337):
        bs = bytes.fromhex(io.readline().split()[-1].decode())
        max(bs) > 127 or exit(bs)
