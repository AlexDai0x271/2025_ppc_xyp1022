
#include "bn_lcl.h"

#define RSA_RAND_P_Q
//#define RSA_RAND_E
#define MODE_CRT

#define input_bits_random 		1024			//512	1024	2048  4096
#define e_bits_random			48			//when RSA_RAND_E	use
//#define ptext_input_bits_random	64			//when RSA_RAND_P_Q use
//#define ptext_input_bits_random	72			//when RSA_RAND_P_Q use

struct rsa_param{
//	unsigned int bits;
    BIGNUM *n;
    BIGNUM *e;
    BIGNUM *d;
    BIGNUM *p;
    BIGNUM *q;
	BIGNUM *dmp1;
	BIGNUM *dmq1;
	BIGNUM *iqmp;
};

//#define TIMER_TEST
//#define RNG_TEST

void rsa_keygen(struct rsa_param *rsa, unsigned int bits, BIGNUM *e_value);
void rsa_public_encrypt(struct rsa_param *rsa, unsigned char *from, unsigned int flen, unsigned char *to);
void rsa_private_decrypt(struct rsa_param *rsa, unsigned char *from, unsigned int flen, unsigned char *to, unsigned char tlen);
//void rsa_public_encrypt(struct rsa_param *rsa, unsigned char *from, unsigned char *to);
//void rsa_private_decrypt(struct rsa_param *rsa, unsigned char *from, unsigned char *to);
void rsa_keygen_one_desc(struct rsa_param *rsa, unsigned int bits, BIGNUM *e_value);
void rsa_keygen_with_RNG_one_desc(struct rsa_param *rsa, unsigned int bits, BIGNUM *e_value);
void rsa_private_signature(struct rsa_param *rsa, unsigned char *from, unsigned int flen, unsigned char *to);
unsigned int rsa_public_verify(struct rsa_param *rsa, unsigned char *from, unsigned int flen, unsigned char *check, unsigned char clen);








