import sys
import os
from setuptools import setup, find_packages, Extension


os.chdir(os.path.dirname(sys.argv[0]) or ".")

setup(
    name='poly1305_aes',
    version='0.1',
    description='poly1305_aes',
    long_description=open('README.rst').read(),
    url='',
    author='Sundar Nagarajan',
    author_email='sun.nagarajan@gmail.com',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: Implementation :: PyPy',
        # 'License :: OSI Approved :: BSD License',
        'License :: Public Domain',
    ],
    install_requires=['cffi>=1.0.0'],
    packages=find_packages(),
    zip_safe=False,
    ext_modules=[
        Extension(
            'poly1305_aes/libpoly1305aes', [
                'c/poly1305aes-20050218/aes_big.c',
                'c/poly1305aes-20050218/aes_big_constants.c',
                'c/poly1305aes-20050218/poly1305_53.c',
                'c/poly1305aes-20050218/poly1305_53_constants.c',
                'c/poly1305aes-20050218/poly1305aes_53_authenticate.c',
                'c/poly1305aes-20050218/poly1305aes_53_clamp.c',
                'c/poly1305aes-20050218/poly1305aes_53_isequal.c',
                'c/poly1305aes-20050218/poly1305aes_53_verify.c',
            ],
            include_dirs=['c/poly1305aes-20050218']
        )],
)
