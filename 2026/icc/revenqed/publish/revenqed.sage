from Crypto.Cipher import AES
from os import urandom
FLAG = b'ICC2026{???????????????????????????????????????????????????????}'

key = urandom(32)
iv = urandom(16)
aes = AES.new(key, AES.MODE_CBC, iv=iv)
ct = aes.encrypt(FLAG)
print(f'{ct.hex() = }')

p = 2^255 - 19
e = ZZ.from_bytes(key)
coeffs = ZZ.from_bytes(iv).digits(2^16)[::-1]
Q.<i,j,k> = QuaternionAlgebra(GF(p), -1, -1)
qs = Q(coeffs[:4]), Q(coeffs[4:])
hint = qs[0]^e * qs[1]^e
print(f'{hint = }')

'''
ct.hex() = '77497b4aed0a89ed572a7c99b52b0c5b4ec8bc33b993995e3674121839a8a59dd85502b9c0ef73cccc379366e275011f4fbde6f83b121778a2dfc2ecb2d74759'
hint = 44738988632321119145712125941732545545457765680234151484492758856987127121723 + 24151693353021226344656177052018227418995632839531043225459434096038196176908*i + 2176366404905574452931335772493323372843539740713312829357728111931522677776*j + 45660648725334310467259313522043767385820489506391497711421701627529057131261*k
'''
