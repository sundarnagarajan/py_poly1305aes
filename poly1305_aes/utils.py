
# Import as: from common.py3 import long, bytes, chr, ord

import six

if six.PY3:
    long = int
    xrange = range
elif six.PY2:
    long = long
    xrange = xrange


def toBytes(s):
    if six.PY3:
        if isinstance(s, bytes):
            return s
        elif isinstance(s, str):
            return encode(s, 'latin-1')
    elif six.PY2:
        if isinstance(s, bytearray):
            return str(s)
        elif isinstance(s, list):
            return str(bytearray(s))
        elif isinstance(s, str):
            return s


def fromBytes(b):
    if six.PY3:
        if isinstance(b, bytes):
            return decode(b, 'latin-1')
        elif isinstance(b, list):
            return decode(bytes(b), 'latin-1')
        elif isinstance(b, str):
            return b
    elif six.PY2:
        if isinstance(b, bytearray):
            return str(b)
        elif isinstance(b, list):
            return str(bytearray(b))
        elif isinstance(b, str):
            return b


def encode(s, encoding='latin-1'):
    if six.PY3:
        import codecs
        if isinstance(s, str):
            return bytes(s, encoding)
        elif isinstance(s, bytes):
            return codecs.encode(s, encoding)
    elif six.PY2:
        if isinstance(s, str):
            return s.encode(encoding=encoding)
        elif isinstance(s, bytearray):
            return toBytes(fromBytes(s).encode(encoding=encoding))


def decode(s, encoding='latin-1'):
    if six.PY3:
        import codecs
        return codecs.decode(s, encoding)
    elif six.PY2:
        if isinstance(s, str):
            print('decode: got string')
            return s.decode(encoding=encoding)
        elif isinstance(s, bytearray):
            print('decode: got bytearray')
            return toBytes(fromBytes(s).decode(encoding=encoding))

_chr = chr
_ord = ord


def chr(x):
    global _chr
    if isinstance(x, int):
        return toBytes(_chr(x))
    else:
        return toBytes(x)


def ord(x):
    global _ord
    if isinstance(x, int):
        return x
    else:
        return _ord(x)


def _convToBytes(s):
    if isinstance(s, str):
        return toBytes(s)
    else:
        return s


def _convFromBytes(b):
    if isinstance(b, bytes) or isinstance(b, bytearray):
        return fromBytes(b)
    else:
        return b


def _convInputs(func, f, *args, **kwargs):
    def wrapper(*args, **kwargs):
        def wrapped(*args, **kwargs):
            newargs = []
            for a in args:
                newargs += [f(a)]
            for (k, v) in kwargs.items():
                kwargs[k] = f(v)
            result = func(*newargs, **kwargs)
            return result
        return wrapped
    return wrapper()


def _convResults(func, f, *args, **kwargs):
    def wrapper(*args, **kwargs):
        def wrapped(*args, **kwargs):
            result = func(*args, **kwargs)
            if isinstance(result, tuple) or isinstance(result, list):
                newres = []
                for r in result:
                    newres += [f(r)]
            else:
                newres = f(result)
            return newres
        return wrapped
    return wrapper()


def inputFromBytes(func, *args, **kwargs):
    return _convInputs(func, _convFromBytes, *args, **kwargs)


def inputToBytes(func, *args, **kwargs):
    return _convInputs(func, _convToBytes, *args, **kwargs)


def outputToBytes(func, *args, **kwargs):
    return _convResults(func, _convToBytes, *args, **kwargs)


def outputFromBytes(func, *args, **kwargs):
    return _convResults(func, _convFromBytes, *args, **kwargs)
