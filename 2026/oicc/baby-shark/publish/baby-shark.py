from fastecdsa import curve, ecdsa, keys
import ast, os

FLAG = os.getenv('FLAG', 'oiccflag{?????????????????????????????????????}')

p = (3<<256) - (3<<128) + 1
EC = curve.Curve("BabyShark258", p, 0, 22, p, 3, 7)
priv, pub = keys.gen_keypair(EC)

msgs = ['Baby shark', 'Doo doo doo doo doo doo']
for msg in msgs:
    print(f'{msg}: {ecdsa.sign(msg, priv, EC)}')
sig = ast.literal_eval(input('Combined signature: '))
print(all(ecdsa.verify(sig, msg, pub, EC) for msg in msgs) and FLAG)
