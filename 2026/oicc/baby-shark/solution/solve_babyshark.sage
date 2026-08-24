from pwn import *
from hashlib import sha256
import ast

p = (3 << 256) - (3 << 128) + 1
E  = EllipticCurve(GF(p), [0, 22])
G  = E(3, 7)
z1, z2 = [int(sha256(m).hexdigest(),16) for m in (b"Baby shark", b"Doo doo doo doo doo doo")]

def getQ(sig, z):
    r, s = map(GF(p), sig)
    for R in E.lift_x(r, all=True):
        yield (s * R - z * G) / r

def smart_dlog(P, Q):
    E2 = EllipticCurve(Zmod(p^2), [p, 22])
    P2 = E2.lift_x(ZZ(P[0]))
    Q2 = E2.lift_x(ZZ(Q[0]))
    dlog = mod(ZZ((p*Q2)[0])/ZZ((p*P2)[0]), p)
    return [-dlog,dlog][dlog*P==Q]

with remote('challenges.oceaniacc.com', 6004) as io:
    io.readuntil(b': '); sig1 = ast.literal_eval(io.readline().decode())
    io.readuntil(b': '); sig2 = ast.literal_eval(io.readline().decode())

    Q = (set(getQ(sig1, z1)) & set(getQ(sig2, z2))).pop()
    d = smart_dlog(G, Q)
    r = -(z1 + z2) / (2 * d)
    k = smart_dlog(G, E.lift_x(r))
    s = (z1 + r * d) / k

    io.sendline(f'{r,s}'.encode())
    print(io.readall())
