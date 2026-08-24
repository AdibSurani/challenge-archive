# Sublinear Bicongruential Generator

## What does the SBG do?

The challenge constructs a stream cipher and gives us three consecutive encryptions of the same length-67 flag under this cipher:

```py
for _ in "SBG":
    print(SBG.encrypt(FLAG).hex())
```

First, let's break down this cipher, which looks like a truncated LCG. We work over the finite field $GF(2^{128})$, and the recurrence is given by $x \mapsto (ax+c)^{2^{-256}}$, with $a$ and $c$ unknown.

We know from Fermat's Little Theorem (over finite fields) that every element $x \in GF(2^{128})$ must satisfy $x = x^{2^{128}}$, and in particular $x^{2^{-256}} = x$ so that `sublinear(x)` really just returns $x$.

The output function is `seed.to_integer() % 256`. In Sage, elements of $GF(2^{128})$ are represented as polynomials over $GF(2)$. So taking the integer representation mod 256 is the same as keeping the lowest 8 coefficients.

In other words, the generator is an affine linear recurrence over a 128-dimensional vector space over $GF(2)$, i.e. $$s_{n+1} = as_n+c,$$ and we see 8 output coordinates each step.

To simplify this, we can take successive differences (or equivalently sums in characteristic 2) to remove the constant. That is, define $\delta_n = s_n + s_{n+1}$ and our recurrence collapses to $$\delta_{n+1} = a\delta_n.$$

This is not the only difference we can take. We can also take an entire flag length apart, i.e. $\Delta_n = s_n + s_{n+67},$ and it will also have the same recurrence.

## The magic of characteristic polynomials

How does this help us? Let $p = p_0 + p_1 x + \ldots + p_{128} x^{128}$ be the characteristic polynomial of $a$, which is almost definitely an irreducible polynomial of degree exactly 128. The Cayley-Hamilton theorem states that the matrix representation $M$ of $a$ also satisfies $p(M) = 0$. This means that any 129 consecutive bits in either $\delta$ (likewise $\Delta$) must satisfy

$$p_0 \delta_i + p_1 \delta_{i+1} + \ldots + p_{128} \delta_{i+128} = 0.$$

This is, of course, linear in $p$, so if we can collect 129 such equations we can determine our $p$ exactly! Well, we need a bit less, since we know from irreducibility and the degree that $p_0 = p_{128} = 1$.

## So how many equations can we construct?

Assuming the flag is ASCII, the top bits are all zero. So we know all 201 top bits. Well, all 200 successive differences $\delta$ anyway. This gives us $200 - 128 = 72$ equations.

And since the flag is encrypted three times, we can also use the $67 \times 2 = 134$ known byte differences of $\Delta$. In fact, each byte has eight different bit positions, and we can use all of them, though keep in mind the top bit is already used up from the first part. This gives us $(134 - 128) \times 7 = 42$ equations.

All in all, we have 114 such equations, so $p$ must exist in some small-enough kernel that we can just iterate through this.

We build the list of equations as such:

```py
M2 = matrix(seq[i:i+129] for seq in [(M[:-1] + M[1:]).T[0], *(M[:-67] + M[67:]).T] for i in range(len(seq)-128))
```

And we can get all candidates like this:

```py
candidates = [p for p in M2.right_kernel() if p[0] and p[-1] and F(p.list()).is_irreducible()]
```

It turns out that this leaves us with just 117 candidates, and we can quickly go through them all.

## Wait, is knowing $p$ enough?

Well, yes. From the characteristic polynomial $p$, we can take all its roots, and there should be 128 of them, and then one of these will be our $a$. Once we have $a$, we can work backwards to determine $\delta$, since we have more than 128 such equations. And this in turns produces our flag.

While the above does work, we take an alternative approach that doesn't require enumerating through the roots, and is thus faster. We basically apply the known recurrence $p$ to every bit position, and check whether it admits a solution that is consistent with our known flag format `ICC2026{`. The rest is pretty self-explanatory.

## Full solve script

```py
def bytes_to_matrix(bs):
    return matrix(GF(2), [list(f"{b:08b}") for b in bs])

def matrix_to_bytes(mat):
    return bytes(ZZ([*row][::-1], 2) for row in mat)

M = bytes_to_matrix(bytes.fromhex(
    "cba85c4725f821d37f1d0b103fa76757f8ae9ef685829a20a139936b4a784115effbf870c5c854730013d25713e2c91c02526392e6efce86fef7d7ef026511bbd33a59" +
    "2f605e691f6a5d83060d85d737c499cc517a2f16356d487fe35dabfbed5c0cd185c1686dcc877029f8790044693d087b2772ada304a9ede224ee76a0dcdb76d0552556" +
    "da1179c3862c1054c04bf2048aa2b652853f783e24adb731d3a2e79c7859f562877c0215e8e8ba5b5fe5abe9f8fc54c6624db242d831abe8fa7063621077df35280673"))

F.<x> = GF(2)[]

# A maps the 67 flag bytes to the 200 adjacent XORs, Y maps the successive differences
A = matrix(GF(2), 200, 67, lambda i,j: (j-i)%67 < 2)
Y = M[:-1] + M[1:]

# Known flag prefix
A2 = identity_matrix(GF(2), 67)[:8]
Y2 = bytes_to_matrix(b'ICC2026{')

# Build recurrence equations (72 high bits + 72 lower bits), then find all candidate charpolys
M2 = matrix(seq[i:i+129] for seq in [Y.T[0], *(M[:-67] + M[67:]).T] for i in range(len(seq)-128))
candidates = [p for p in M2.right_kernel() if p[0] and p[-1] and F(p.list()).is_irreducible()]
print(f'{len(candidates) = }')

for p in candidates:
    try:
        # Roughly speaking, the Toeplitz turns the recurrence p into a matrix form P
        P = matrix.toeplitz([*p] + [0]*71, [0]*71).T
        print(matrix_to_bytes(A2.stack(P*A).solve_right(Y2.stack(P*Y))))
    except ValueError:
        pass
```

After about five seconds, the script completes, and this is the entire output:

```
len(candidates) = 117
b'ICC2026{sublinear_is_a_no-op,_but_at_least_this_flag_has_length_67}'
```

The flag is thus `ICC2026{sublinear_is_a_no-op,_but_at_least_this_flag_has_length_67}`.

## Other notes

The bottleneck of the above script is very surprising. It's not the linear algebra or the irreducible, but actually just the boilerplate while enumerating through the kernel (sage creates and destroys lots of objects, and it probably does some evals in there).

Generating a massive matrix of all possible vectors turns out to be a lot faster:
```py
all_right_kernel_vectors = matrix((GF(2)**M2.right_nullity()).list()) * M2.right_kernel_matrix()
```

With this change, the script completes in under 2 seconds.

And most importantly, SBG loves LCG.
