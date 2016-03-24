import os
from cffi_utils.utils2to3 import toBytes
from cffi_utils.sowrapper import get_lib_ffi_resource


c_hdr = '''
extern void poly1305aes_53_clamp(unsigned char kr[32]);
extern void poly1305aes_53_authenticate(unsigned char out[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l);
extern int poly1305aes_53_verify(const unsigned char a[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l);
'''


module_name = 'poly1305_aes'
libpath = 'libpoly1305aes.so'


(ffi, lib) = get_lib_ffi_resource(
    module_name=module_name, libpath=libpath, c_hdr=c_hdr)


def get_key(random_fn=None):
    '''
    random_fn-->callable: takes one int param: number of random bytes
                Preferably leave unset; os.urandom() will be used
    Returns-->bytes: Secure key
    '''
    if random_fn is None:
        random_fn = os.urandom
    kr = ffi.new('unsigned char[32]', random_fn(32))
    lib.poly1305aes_53_clamp(kr)
    return ffi.get_bytes(kr)


def authenticate(kr, msg, random_fn=None):
    '''
    kr-->bytes: Secure key - returned by get_key()
    msg-->str or bytes: Message to be authenticated
    random_fn-->callable: takes one int param: number of random bytes
                Preferably leave unset; os.urandom() will be used
    Returns-->bytes: auth + nonce
    '''
    if random_fn is None:
        random_fn = os.urandom
    nonce = ffi.new('unsigned char[16]', random_fn(16))
    auth = ffi.new('unsigned char[16]')
    msg = toBytes(msg)

    lib.poly1305aes_53_authenticate(auth, kr, nonce, msg, len(msg))
    return ffi.get_bytes(auth) + ffi.get_bytes(nonce)


def verify(auth_nonce, kr, msg):
    (auth_nonce, kr) = (toBytes(auth_nonce), toBytes(kr))
    (auth, nonce) = (auth_nonce[:16], auth_nonce[16:])
    msg = toBytes(msg)
    ret = lib.poly1305aes_53_verify(auth, kr, nonce, msg, len(msg))
    return ret != 0
