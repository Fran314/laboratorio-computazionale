from hashlib import sha256 as _sha256

def itb(val):
    return val.to_bytes(4, "big")
    
def bti(val):
    return int.from_bytes(val[0:4], "big")
    
def sha256(val):
    return _sha256(val).digest()

def hmac_sha256(key, val):
    return sha256(
        bytes([b ^ 0x5c for b in key]) +
        sha256(
            bytes([b ^ 0x36 for b in key]) +
            val
        )
    )

class CryptoContext:
    def __init__(self, r_prev, r_succ):
        self.r_prev = sha256(itb(r_prev))
        self.r_succ = sha256(itb(r_succ))
    
    def encode(self, val, time):
        r_t_prev = sha256(hmac_sha256(self.r_prev, itb(time)))
        r_t_succ = sha256(hmac_sha256(self.r_succ, itb(time)))
        r_t = bti(r_t_succ) - bti(r_t_prev)
        
        return val + r_t
    
