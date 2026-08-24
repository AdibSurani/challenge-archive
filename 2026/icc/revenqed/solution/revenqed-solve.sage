p = 2^255 - 19
Q.<i,j,k> = QuaternionAlgebra(GF(p), -1, -1)
hint = 44738988632321119145712125941732545545457765680234151484492758856987127121723 + 24151693353021226344656177052018227418995632839531043225459434096038196176908*i + 2176366404905574452931335772493323372843539740713312829357728111931522677776*j + 45660648725334310467259313522043767385820489506391497711421701627529057131261*k

F.<a,b,c,d,e,f> = QQ[]
g0 = vector([a,b,c])
g1 = vector([d,e,f])

def split(q):
    return q[0], vector([*q][1:])

# Part 1: Solve for u and v (the directions of the original quaternions)
print('Starting part 1')
_, *w = hint / hint[0]
wx = vector(w).cross_product_matrix()
h = g0 * (wx + wx^2).change_ring(QQ) * g1

cm = Sequence([h*h] + [h*x*y*p for x in g0 for y in g1]).coefficients_monomials(sparse=False)
lll = block_matrix([[cm[0]],[p^2]]).LLL()
rkm = lll[:-1].right_kernel_matrix()[0]

sol = ideal(list(rkm*sgn(rkm[0]) - cm[1]) + [a-1,d-1]).variety()[0]
u = matrix([sol[a],sol[b],sol[c]])._clear_denom()[0][0].change_ring(GF(p))
v = matrix([sol[d],sol[e],sol[f]])._clear_denom()[0][0].change_ring(GF(p))

print(f'{u = }')
print(f'{v = }')

# Part 2: Solve for a and b, the scalar part of the post-exponentiated quaternions
print('Starting part 2')
b, a, _ = matrix([u,v,w]).solve_left(v.cross_product(u))
print(f'{a = }')
print(f'{b = }')

q0e = Q([a,*u])
q1e = Q([b,*v])

# Part 3: DLP over small subgroup to recover the IV / original quaternions
print('Starting part 3')
F2 = GF(p^2)
n0 = F2(-u*u).sqrt()
n1 = F2(-v*v).sqrt()

print(f'{factor(p+1) = }')
q = 60824497 * 213156431 # small factors of p+1
def foo(x):
    return (x / x.conjugate()) ^ ((p+1)//q)

lg = foo(a + n0).log(foo(b + n1), order=q)
print(f'{lg = }')

gs = [foo(i+n0) for i in range(65536)]
hs = [foo(i+n1)^lg for i in range(65536)]
for t in set(gs) & set(hs) - {1}:
    q0 = Q([gs.index(t),*u])
    q1 = Q([hs.index(t),*v])
    coeffs = [*q0, *q1]
    iv = ZZ(coeffs[::-1], 2^16).to_bytes(16)
    print(f'{iv.hex() = }')
    # there should only be one solution, but we run it exhaustively anyway to be sure

# Part 4: Solve 256-bit DLP using CADO-NFS
print('Starting part 4')
ell = factor(p-1)[-1][0]
print(f'{ell = }')
g = (q0 * q1).reduced_norm() ^ ((p-1)/ell)
h = hint.reduced_norm() ^ ((p-1)/ell)
print(f'{g = }')
print(f'{h = }')
print(f'{p = }')
print('We wish to find e (mod ell) such that g^e = h (mod p).')

'''
The following part is performed outside of sage.
Run the next two commands sequentially on cado-nfs, and get the logs .

./cado-nfs.py -dlp \
  -ell 74058212732561358302231226437062788676166966415465897661863160754340907 \
  target=16129380738592216134000951979978244515875790708977871900924433187360091867796 \
  57896044618658097711785492504343953926634992332820282019728792003956564819949 \
  tasks.workdir=/home/adib/ctf/cado-revenqed

./cado-nfs.py /home/adib/ctf/cado-revenqed/p75.parameters_snapshot.0 \
  target=23352160079620251410372658347867082011994253952656912105411958931717976107207

These are the two values it returns, after ~2.5 minutes and 0.5 minutes respectively.
'''
retval0 = 51067923652493631290370286408786379160299016259993427540834494561263722
retval1 = 37140280871231156293136575906019460386414490609064057986399182008793023

e0 = retval1 / retval0 % ell
print(f'{e0 = } (mod ell)')

# Part 5: Brute force 20 bits of e to solve for flag!
print('Starting part 5')
from Crypto.Cipher import AES
from tqdm import trange

ct = bytes.fromhex('77497b4aed0a89ed572a7c99b52b0c5b4ec8bc33b993995e3674121839a8a59dd85502b9c0ef73cccc379366e275011f4fbde6f83b121778a2dfc2ecb2d74759')

for i in trange(2^256//ell):
    key = (e0 + i * ell).to_bytes(32)
    bs = AES.new(key, AES.MODE_CBC, iv=iv).decrypt(ct)
    if bs.startswith(b'ICC'):
        print(bs)
