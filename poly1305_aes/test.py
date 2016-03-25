from . import get_key, authenticate, verify


if __name__ == '__main__':

    msg = 'Hello world'
    kr = get_key()
    auth = authenticate(kr, msg)

    bad_kr = get_key()
    bad_auth = authenticate(kr, msg + '1')
    bad_msg = msg + '1'

    print('Good: %s\nBad auth: %s\nBad kr: %s\nBad msg: %s' % (
        str(verify(auth, kr, msg)),
        str(verify(bad_auth, kr, msg)),
        str(verify(auth, bad_kr, msg)),
        str(verify(auth, kr, bad_msg))
    ))
