import crypto_context
import random

n = 10
keys = [random.getrandbits(32) for i in range(n)]
ccs = [crypto_context.CryptoContext(keys[i], keys[(i+1) % n]) for i in range(n)]

vals = [random.getrandbits(16) for i in range(n)]
cfrs = [ccs[i].encode(vals[i], 0) for i in range(n)]
clear_sum = sum(vals)
cipher_sum = sum(cfrs)
print('\n'.join(["{}\t{}".format(vals[i], cfrs[i]) for i in range(n)]))
print()
print("{}\t{}".format(clear_sum, cipher_sum))
