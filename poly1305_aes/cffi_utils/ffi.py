from cffi import FFI


class FFIExt(FFI):
    def get_cdata(self, *args):
        '''
        all args-->_cffi_backend.buffer
        Returns-->cdata (if a SINGLE argument was provided)
                  LIST of cdata (if a args was a tuple or list)
        '''
        res = tuple([
            self.from_buffer(x) for x in args
        ])

        if len(res) == 0:
            return None
        elif len(res) == 1:
            return res[0]
        else:
            return res

    def get_buffer(self, *args):
        '''
        all args-->_cffi_backend.CDataOwn
        Must be a pointer or an array
        Returns-->buffer (if a SINGLE argument was provided)
                  LIST of buffer (if a args was a tuple or list)
        '''
        res = tuple([
            self.buffer(x) for x in args
        ])

        if len(res) == 0:
            return None
        elif len(res) == 1:
            return res[0]
        else:
            return res

    def get_bytes(self, *args):
        '''
        all args-->_cffi_backend.CDataOwn
        Must be a pointer or an array
        Returns-->bytes (if a SINGLE argument was provided)
                  LIST of bytes (if a args was a tuple or list)
        '''
        res = tuple([
            bytes(self.buffer(x)) for x in args
        ])

        if len(res) == 0:
            return None
        elif len(res) == 1:
            return res[0]
        else:
            return res

    def get_extension(self):
        return [self.verifier.get_extension()]
