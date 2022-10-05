import crypto_context
import random
import time

total = 0
for i in range(1000):
    cc = crypto_context.CryptoContext(random.getrandbits(32), random.getrandbits(32))
    val = random.getrandbits(16)

    t0 = time.ticks_us()
    cfr = cc.encode(val, 0)
    t1 = time.ticks_us()
    total += (time.ticks_diff(t1, t0))

print(total)
