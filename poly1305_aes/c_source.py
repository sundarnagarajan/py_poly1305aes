c_hdr = '''
extern void poly1305aes_53_clamp(unsigned char kr[32]);
extern void poly1305aes_53_authenticate(unsigned char out[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l);
extern int poly1305aes_53_verify(const unsigned char a[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l);
'''
