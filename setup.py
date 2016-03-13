import sys
import os
from setuptools import (
    setup,
    find_packages, Extension,
)


os.chdir(os.path.dirname(sys.argv[0]) or ".")

module_name = 'poly1305_aes'
author = 'Sundar Nagarajan'
author_email = 'sun.nagarajan@gmail.com'

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
libpath = os.path.join(module_name, libname)
c_src_list = [os.path.join(c_dir, x) for x in c_src_files]


setup(
    name=module_name,
    version='0.1',
    description=module_name,
    long_description=open('README.rst').read(),
    url='',
    author=author,
    author_email=author_email,
    classifiers=[
        'Development Status :: 4 - Beta',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: Implementation :: PyPy',
        'License :: Public Domain',
    ],
    install_requires=['cffi>=1.0.0', 'six>=1.9.0'],
    packages=find_packages(),
    zip_safe=False,
    ext_modules=[
        Extension(
            libpath,
            c_src_list,
            include_dirs=[c_dir]
        )],
)
