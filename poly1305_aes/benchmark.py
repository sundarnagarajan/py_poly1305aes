#!/usr/bin/env/python
'''
    authenticate(kr, msg, random_fn=None)
        kr-->bytes: Secure key - returned by get_key()
        msg-->str or bytes: Message to be authenticated
        random_fn-->callable: takes one int param: number of random bytes
                    Preferably leave unset; os.urandom() will be used
        Returns-->bytes: auth + nonce

    get_key(random_fn=None)
        random_fn-->callable: takes one int param: number of random bytes
                    Preferably leave unset; os.urandom() will be used
        Returns-->bytes: Secure key

    verify(auth_nonce, kr, msg)
'''
import sys
import os
import time
from poly1305_aes.poly1305 import get_key, authenticate, verify


def time_auth(d, msg_len=64):
    for k in d.keys():
        d[k] = [os.urandom(msg_len)]
    start = time.time()
    for (k, v) in d.items():
        v += [authenticate(k, v[0])]
    return float(len(d)) / (time.time() - start)


def time_verify(d):
    start = time.time()
    for (k, v) in d.items():
        verify(v[1], k, v[0])
    return float(len(d)) / (time.time() - start)


if __name__ == '__main__':
    try:
        NUM_ITER = int(sys.argv[1])
    except:
        NUM_ITER = 1000
    print('Iterations: ' + str(NUM_ITER))

    # get_key()
    global_d = {}
    start = time.time()
    for i in range(NUM_ITER):
        global_d[get_key()] = None
    key_persec = float(NUM_ITER) / (time.time() - start)

    # auth, verify short messages
    auth_persec_short = time_auth(global_d)
    verify_persec_short = time_verify(global_d)

    # reset keys
    global_d = {}
    for i in range(NUM_ITER):
        global_d[get_key()] = None
    # auth, verify long messages
    auth_persec_long = time_auth(global_d, msg_len=10000)
    verify_persec_long = time_verify(global_d)

    print(
        ('get_key /sec: %.0f'
         '\nauthenticate / sec (64-byte message): %.0f'
         '\nverify / sec (64-byte message): %.0f'
         '\nauthenticate / sec (10K message): %.0f'
         '\nverify / sec (10K message): %.0f'
         ) % (
             key_persec, auth_persec_short, verify_persec_short,
             auth_persec_long, verify_persec_long,))
