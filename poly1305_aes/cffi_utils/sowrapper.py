from .ffi import FFIExt
import six
import sys
from pkg_resources import resource_filename


def get_lib_ffi_resource(module_name, libpath, c_hdr):
    '''
    module_name-->str: module name to retrieve resource
    libpath-->str: shared library filename with optional path
    c_hdr-->str: C-style header definitions for functions to wrap
    Returns-->(ffi, lib)

    The 'clobbered' paths are tried FIRST, falling back to trying the
        unchanged libpath
    For generating the 'clobbered' filenames,libpath has to end in '.so'

    Use this method when you are loading a package-specific shared library
    If you want to load a system-wide shared library, use get_lib_ffi_shared
    instead
    '''
    # The lib name gets clobbered by FFI in Python3 and pypy
    # The format of the clobbered name doesn't seem to be documented anywhere
    # and is generated here by visual inspection :-(
    lib_base = libpath.rsplit('.so', 1)[0]
    if six.PY2 and sys.subversion[0].lower() == 'pypy':
        clobbered_path = '%s.%s-26-%s.so' % (
            lib_base, sys.subversion[0].lower(), sys._multiarch,
        )
    elif six.PY2:
        clobbered_path = libpath
    elif six.PY3:
        clobbered_path = lib_base + '.' + sys.implementation.cache_tag + 'm.so'

    try:
        libres = resource_filename(module_name, clobbered_path)
        return get_lib_ffi_shared(libpath=libres, c_hdr=c_hdr)
    except:
        # On Py2 we only need to try once
        if clobbered_path == libpath:
            raise
        pass
    libres = resource_filename(module_name, libpath)
    return get_lib_ffi_shared(libpath=libres, c_hdr=c_hdr)


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
