import sys
import os
from setuptools import setup, find_packages, Extension


os.chdir(os.path.dirname(sys.argv[0]) or ".")


'''
==============================================================================
PACKAGE DATA
==============================================================================
'''
toplevel = 'poly1305_aes'
version = '0.1'
packages = find_packages()
description = 'Python wrapper for Poly1305aes HMAC'
license = 'License :: OSI Approved :: MIT License'

long_description = open('README.rst').read()
url = 'https://github.com/sundarnagarajan/py_poly1305aes.git'
download_url = 'https://github.com/sundarnagarajan/py_poly1305aes.git'
author = 'Sundar Nagarajan'
author_email = ''
maintainer = author
maintainer_email = author_email
install_requires = [
    'cffi>=1.0.0',
    'six>=1.9.0'
],
maintainer = author
maintainer_email = author_email
classifiers = [
    'Development Status :: 4 - Beta',
    'Programming Language :: Python :: 2',
    'Programming Language :: Python :: 3',
    'Programming Language :: Python :: Implementation :: PyPy',
    'License :: OSI Approved :: MIT License',
]
zip_safe = True


'''
==============================================================================
C EXTENSION DETAILS
==============================================================================
'''
c_dir = 'c/poly1305aes-20050218'
libname = 'libpoly1305aes'
c_src_files = [
    'aes_big.c',
    'aes_big_constants.c',
    'poly1305_53.c',
    'poly1305_53_constants.c',
    'poly1305aes_53_authenticate.c',
    'poly1305aes_53_clamp.c',
    'poly1305aes_53_isequal.c',
    'poly1305aes_53_verify.c',
]
libpath = os.path.join(toplevel, libname)
c_src_list = [os.path.join(toplevel, c_dir, x) for x in c_src_files]
ext_modules = [
    Extension(
        name=libpath,
        sources=c_src_list,
        include_dirs=[c_dir],
    )
]


'''
==============================================================================
ADDITIONAL DATA FILES
==============================================================================
'''

data_dirs = [
    'doc',
    c_dir,
]


'''
==============================================================================
ADDITIONAL keyword args to setup()
==============================================================================
'''
ADDL_KWARGS = dict(
)


'''
==============================================================================
           DO NOT CHANGE ANYTHING BELOW THIS
==============================================================================
'''


def get_dirtree(toplevel, dirlist=[]):
    '''
    toplevel-->str: must be name of a dir under current working dir
    dirlist-->list of str: must all be names of dirs under toplevel
    '''
    ret = []
    curdir = os.getcwd()
    if not os.path.isdir(toplevel):
        return ret
    os.chdir(toplevel)
    try:
        for dirname in dirlist:
            if not os.path.isdir(dirname):
                continue
            for (d, ds, fs) in os.walk(dirname):
                for f in fs:
                    ret += [os.path.join(d, f)]
        return ret
    except:
        return ret
    finally:
        os.chdir(curdir)


# Required keywords
kwdict = dict(
    name=toplevel,
    version=version,
    packages=packages,
    description=description,
    license=license,
)

# Optional keywords
kwdict.update(dict(
    # requires=globals().get('requires,', []),
    install_requires=globals().get('install_requires,', []),
    long_description=globals().get('long_description', ''),
    url=globals().get('url', ''),
    download_url=globals().get('download_url', ''),
    author=globals().get('author', ''),
    author_email=globals().get('author_email', ''),
    maintainer=globals().get('maintainer', ''),
    maintainer_email=globals().get('maintainer_email', ''),
    classifiers=globals().get('classifiers', []),
    keywords=globals().get('keywords', []),
    zip_safe=globals().get('zip_safe', False),
))
kwdict.update(ADDL_KWARGS)

# More optional keywords, but which are added conditionally
ext_modules = globals().get('ext_modules', [])
if ext_modules:
    kwdict['ext_modules'] = ext_modules

dirlist = globals().get('data_dirs', None)
if isinstance(dirlist, list):
    kwdict['package_dir'] = {toplevel: toplevel}
    kwdict['package_data'] = {toplevel:
                              get_dirtree(toplevel, dirlist)}


setup(**kwdict)
