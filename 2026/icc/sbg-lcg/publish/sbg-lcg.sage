FLAG = b'ICC2026{??????????????????????????????????????????????????????????}'

class SublinearBicongruentialGenerator:
    def __init__(self):
        F = GF(2^128)
        self.seed, self.a, self.c = (F^3).random_element()

    # Square roots are sublinear by definition, hence so are iterated square roots
    @staticmethod
    def sublinear(x):
        for _ in range(256):
            x = sqrt(x)
        return x

    # Output is converted from GF(2^128) -> ZZ -> Zmod(256), so it's bicongruential
    @staticmethod
    def bicongruential(x):
        return x.to_integer() % 256

    def clock(self):
        self.seed = self.sublinear(self.a * self.seed + self.c)
        return self.bicongruential(self.seed)

    def encrypt(self, msg):
        return bytes(b ^^ self.clock() for b in msg)

SBG = SublinearBicongruentialGenerator()
for _ in "SBG":
    print(SBG.encrypt(FLAG).hex())

'''
cba85c4725f821d37f1d0b103fa76757f8ae9ef685829a20a139936b4a784115effbf870c5c854730013d25713e2c91c02526392e6efce86fef7d7ef026511bbd33a59
2f605e691f6a5d83060d85d737c499cc517a2f16356d487fe35dabfbed5c0cd185c1686dcc877029f8790044693d087b2772ada304a9ede224ee76a0dcdb76d0552556
da1179c3862c1054c04bf2048aa2b652853f783e24adb731d3a2e79c7859f562877c0215e8e8ba5b5fe5abe9f8fc54c6624db242d831abe8fa7063621077df35280673
'''
