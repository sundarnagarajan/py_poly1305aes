# poly1305_aes
Python extension module wrapping poly1305aes by Daniel Bernstein

This module is a simple wrapper around the poly1305aes HMAC algorithm by Daniel J. Bernstein (http://cr.yp.to/mac.html)

## What is a HMAC (Hash-based Message Authentication Code)?
From wikipedia (https://en.wikipedia.org/wiki/Hash-based_message_authentication_code):

In cryptography, a keyed-hash message authentication code (HMAC) is a specific construction for calculating a message authentication code (MAC) involving a cryptographic hash function in combination with a secret cryptographic key. As with any MAC, it may be used to simultaneously verify both the data integrity and the authentication of a message. Any cryptographic hash function, such as MD5 or SHA-1, may be used in the calculation of an HMAC; the resulting MAC algorithm is termed HMAC-MD5 or HMAC-SHA1 accordingly. The cryptographic strength of the HMAC depends upon the cryptographic strength of the underlying hash function, the size of its hash output, and on the size and quality of the key.

## What is poly1305?
From http://cr.yp.to/mac.html:

Poly1305-AES is a state-of-the-art secret-key message-authentication code suitable for a wide variety of applications. Poly1305-AES computes a 16-byte authenticator of a message of any length, using a 16-byte nonce (unique message number) and a 32-byte secret key. Attackers can't modify or forge messages if the message sender transmits an authenticator along with each message and the message receiver checks each authenticator.

Poly1305-AES has several useful features:

-  Guaranteed security if AES is secure. There's a theorem guaranteeing that the security gap is extremely small (n/2^(102) per forgery attempt for 16n-byte messages) even for long-term keys (2^64 messages). The only way for an attacker to break Poly1305-AES is to break AES.
-  Cipher replaceability. If anything does go wrong with AES, users can switch from Poly1305-AES to Poly1305-AnotherFunction, with an identical security guarantee.
-  Extremely high speed. My Poly1305-AES software takes just 3843 Athlon cycles, 5361 Pentium III cycles, 5464 Pentium 4 cycles, 4611 Pentium M cycles, 8464 PowerPC 7410 cycles, 5905 PowerPC RS64 IV cycles, 5118 UltraSPARC II cycles, or 5601 UltraSPARC III cycles to verify an authenticator on a 1024-byte message. Poly1305-AES offers consistent high speed, not just high speed for one favored CPU.
-  Low per-message overhead. My Poly1305-AES software takes just 1232 Pentium 4 cycles, 1264 PowerPC 7410 cycles, or 1077 UltraSPARC III cycles to verify an authenticator on a 64-byte message. Poly1305-AES offers consistent high speed, not just high speed for long messages.  Most competing functions are designed for long messages and don't pay attention to short-packet performance.
-  Key agility. Poly1305-AES can fit thousands of simultaneous keys into cache, and remains fast even when keys are out of cache. Poly1305-AES offers consistent high speed, not just high speed for single-key benchmarks. Almost all competing functions use a large table for each key; as the number of keys grows, those functions miss the cache and slow down dramatically.
-  Parallelizability and incrementality. Poly1305-AES can take advantage of additional hardware to reduce the latency for long messages, and can be recomputed at low cost for a small modification of a long message.
-  No intellectual-property claims. I am not aware of any patents or patent applications relevant to Poly1305-AES.
-  Guaranteed security, cipher replaceability, and parallelizability are provided by the standard polynomial-evaluation Wegman-Carter-MAC framework. Within that framework, hash127-AES achieved extremely high speed at the expense of a large table for each key.
-  The big advantage of Poly1305-AES is key agility: extremely high speed without any key expansion. Other standard MACs are slower and less secure than Poly1305-AES. Specifically, HMAC-MD5 is slower and doesn't have a comparable security guarantee; CBC-MAC-AES is much slower and has a weaker security guarantee. Both HMAC-MD5 and CBC-MAC-AES are breakable within 2^64 messages.

# LICENSE:
Following D.J. Bernstein's code license, this code is also released into the public domain. See LICENSE file.

The 'c' directory contains the unmodified source code for poly1305aes from https://cr.yp.to/mac/poly1305aes-20050218.tar.gz.

# EXAMPLES:
~~~~ {.sourceCode .python}
    from poly1305_aes import (
        get_key, authenticate, verify
    )
    msg = 'Hello world'
    kr = get_key()
    auth = authenticate(kr, msg)

    bad_kr = get_key()
    bad_auth = authenticate(kr, msg + '1')
    bad_msg = msg + '1'

    print('Good: %s\nBad auth: %s\nBad kr: %s\nBad msg: %s' % (
        str(verify(auth, kr, msg)),
        str(verify(bad_auth, kr, msg)),
        str(verify(auth, bad_kr, msg)),
        str(verify(auth, kr, bad_msg))
    ))
~~~~

Alternately, run the test script that is shipped: 

    python -m poly1305_aes.test

To run a simple benchmark:

    python -m poly1305_aes.benchmark

# INSTALLATION:
Using pip:

   pip install 'git+https://github.com/sundarnagarajan/py_poly1305aes.git'

Using setup.py: 

   python setup.py install

# BUILD / INSTALL REQUIREMENTS:
*GNU/Linux:* 
- Python: Tested on 2.7.6, 3.4.3, pypy 2.7.10 (pypy 4.0.1)
- cffi >= 1.0.0 
- six - Python.h (libpython-dev on Debian-like systems)
- gcc (build-essential on Debian-like systems)

# TODO:
Dan Bernsteins code contains optimizations for:
-  x86 (Intel)
-  x86 (Pentium Pro)
-  x86 (Athlon)
-  UltraSparc II and III
-  PowerPC, PPC64

My code only supports generic x86. To support the additional platforms will require:
-  Detect CPU type accurately
-  Change c_src_files in setup.py based on CPU type
-  Change c_hdr in poly1305_aes.poly1305.py based on CPU type
-  Change get_key(), authenticate() and verify() to call different functions in shared
-  Test on the different CPUs (I only have access to x86!)
