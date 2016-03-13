from .ffi import FFIExt


def get_lib_ffi_shared(libpath, c_hdr):
    '''
    libpath-->str: shared library filename with optional path
    c_hdr-->str: C-style header definitions for functions to wrap
    Returns-->(ffi, lib)
    '''
    lib = SharedLibWrapper(libpath, c_hdr)
    ffi = lib.ffi
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
