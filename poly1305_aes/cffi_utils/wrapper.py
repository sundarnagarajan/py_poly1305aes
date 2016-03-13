import sys
import binascii
import threading
from .ffi import FFIExt
from cffi.verifier import Verifier


def get_lib_ffi_shared(libpath, c_hdr):
    '''
    libpath-->str: shared library filename with optional path
    c_hdr-->str: C-style header definitions for functions to wrap
    Returns-->(ffi, lib)
    '''
    lib = SharedLibWrapper(libpath, c_hdr)
    ffi = lib.ffi
    return (ffi, lib)


def get_lib_ffi(module_name, c_hdr, c_src):
    '''
    module_name-->str
    c_hdr-->str: C-style header definitions for functions to wrap
    c_src-->str: C-source for wrapped functions

    Returns-->(ffi, lib)
    '''
    ffi = FFIExt()
    lib = CSourceWrapper(
        c_hdr=c_hdr, c_src=c_src, ffi=ffi,
        module_name=module_name,
    )
    return (ffi, lib)


class SharedLibWrapper(object):
    def __init__(self, libpath, c_hdr):
        '''
        libpath-->str: library name; can also be full path
        c_hdr-->str: C-style header definitions for functions to wrap
        ffi-->FFIExt or cffi.FFI
        '''
        self._libpath = libpath
        self._c_hdr = c_hdr
        self.ffi = FFIExt()

        self.ffi.cdef(self._c_hdr)
        self._lib = self.ffi.dlopen(self._libpath)

    def __getattr__(self, name):
        if self._lib is None:
            return self.__getattribute__(name)
        return getattr(self._lib, name)

    def get_extension(self):
        return [self.ffi.verifier.get_extension()]


'''
CWrapper Copied from and inspired by:
    https://caremad.io/2014/11/distributing-a-cffi-project/
'''


class CSourceWrapper(object):
    @classmethod
    def getClassName(cls):
        return str(cls).rsplit("'", 1)[0].rsplit("'", 1)[1]

    def __init__(self, c_hdr, c_src, ffi, module_name=None):
        '''
        c_hdr-->str: C-style header definitions for functions to wrap
        c_src-->str: C-source for wrapped functions
        ffi-->FFIExt or cffi.FFI
        module_name-->str
        '''
        self._c_hdr = c_hdr
        self._c_src = c_src
        self.ffi = ffi
        if module_name is None:
            module_name = self.getClassName()
        self._module_name = module_name
        self.ffi.cdef(self._c_hdr)
        self.ffi.verifier = Verifier(
            self.ffi,
            self._c_src,
            modulename=self._create_modulename(
                self._c_hdr,
                self._c_src,
                sys.version
            ),
        )
        self._lib = None
        self._lock = threading.Lock()

    def _create_modulename(self, c_hdr, c_src, sys_version):
        '''
        This is the same as CFFI's create modulename except we don't
        include the CFFI version.
        '''
        key = '\x00'.join([sys_version[:3], c_src, c_hdr])
        key = key.encode('utf-8')
        k1 = hex(binascii.crc32(key[0::2]) & 0xffffffff)
        k1 = k1.lstrip('0x').rstrip('L')
        k2 = hex(binascii.crc32(key[1::2]) & 0xffffffff)
        k2 = k2.lstrip('0').rstrip('L')
        return '_' + self._module_name + '_cffi_{0}{1}'.format(k1, k2)

    def _compile_module(*args, **kwargs):
        raise RuntimeError(
            "Attempted implicit compile of a cffi module."
            " All cffi modules should be pre-compiled at installation time."
        )

    def __getattr__(self, name):
        if self._lib is None:
            with self._lock:
                if self._lib is None:
                    # Patch Verifier() to prevent CFFI from compiling
                    self.ffi.verifier.compile_module = self._compile_module
                    self.ffi.verifier._compile_module = self._compile_module
                    self._lib = self.ffi.verifier.load_library()
        return getattr(self._lib, name)
