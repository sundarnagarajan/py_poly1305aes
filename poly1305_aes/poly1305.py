import os
from utils import toBytes
from cffi_utils.wrapper import get_lib_ffi, get_lib_ffi_shared
from c_source import c_hdr


USE_SHARED_LIB = True


if USE_SHARED_LIB:
    libpath = os.path.join(
        os.path.dirname(__file__), './libpoly1305aes.so')
    (ffi, lib) = get_lib_ffi_shared(libpath=libpath, c_hdr=c_hdr)
else:
    from c_source import c_src
    (ffi, lib) = get_lib_ffi(
        module_name='poly1305_aes', c_hdr=c_hdr, c_src=c_src)


'''
Conventions:
    - Provide methods for allocation of all C pointers and arrays
        use ffi.new()
    - Return FFI-allocated C pointers and arrays as BUFFERS
        that can be read and written by python (if required)
        use ffi.get_buffer
    - If FFI-allocated C arrays are NOT going to be passed into
        C-routines, convert them to BYTEs using ffi.get_bytes()
        Note: bytes cannot be converted back into cdata using
        ffi.get_cdata()
    - Automatically convert buffers that were originally allocated
        using ffi.new() using ffi.get_cdata()
'''


class Poly1305(object):
    @classmethod
    def get_key(cls, random_fn=None):
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

    @classmethod
    def authenticate(cls, kr, msg, random_fn=None):
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

    @classmethod
    def verify(cls, auth_nonce, kr, msg):
        (auth_nonce, kr) = (toBytes(auth_nonce), toBytes(kr))
        (auth, nonce) = (auth_nonce[:16], auth_nonce[16:])
        msg = toBytes(msg)
        ret = lib.poly1305aes_53_verify(auth, kr, nonce, msg, len(msg))
        return ret != 0


if __name__ == '__main__':
    p = Poly1305
    msg = 'Hello world'
    kr = p.get_key()
    auth = p.authenticate(kr, msg)

    bad_kr = p.get_key()
    bad_auth = p.authenticate(kr, msg + '1')
    bad_msg = msg + '1'

    print('Good: %s\nBad auth: %s\nBad kr: %s\nBad msg: %s' % (
        str(p.verify(auth, kr, msg)),
        str(p.verify(bad_auth, kr, msg)),
        str(p.verify(auth, bad_kr, msg)),
        str(p.verify(auth, kr, bad_msg))
    ))
