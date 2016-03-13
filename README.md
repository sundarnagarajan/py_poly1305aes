poly1305_aes

-------------

Python extension module wrapping poly1305aes by Daniel Bernstein

This module is a simple wrapper around the poly1305aes HMAC algorithm
by Daniel J. Bernstein (http://cr.yp.to/mac.html)

LICENSE:
Following D.J. Bernstein's code license, this code is also released into the 
public domain. See LICENSE file.

The 'c' directory contains the unmodified source code for poly1305aes from
https://cr.yp.to/mac/poly1305aes-20050218.tar.gz.

**EXAMPLES:**

.. code-block:: python

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


**INSTALLATION:**

Using pip:

pip install 'git+<github_http_url.git>'

Using setup.py:
python setup.py install

**BUILD / INSTALL REQUIREMENTS:**

*GNU/Linux:*
- Python
  Tested on 2.7.6, 3.4.3, pypy 2.7.10 (pypy 4.0.1)
- cffi >= 1.0.0
- Python.h (libpython-dev on Debian-like systems)

