#ifndef	__LIB_SM2__
#define __LIB_SM2__

//大数运算相关的数据结构定义与运算函数
//#define LITTLE_ENDIAN
#define SM2_MAX_MESSAGE_LEN		128
#define ECC_BIGINT32_MAXLEN 	32
#define SM3_DIGEST_LEN			32
#define HASH_IN_SIZE	(ECC_BIGINT32_MAXLEN*2+4)
#define HASH_OUT_SIZE	SM2_MAX_MESSAGE_LEN

typedef unsigned char   UINT8;
typedef signed   char   SINT8;
typedef unsigned short  UINT16;
typedef signed   short  SINT16;
typedef unsigned int    UINT32;
typedef signed 	 int    SINT32;

typedef UINT32 BIG_INT;
typedef UINT16 BIG_HALF_DIGIT;

#if !defined(NULL)
#define NULL 0
#endif

//定义大数的32位数组结构
//auValue数组的低32位地址存放大数的低32位有效数，高32位地址存放大数的高32位有效数
typedef struct
{                         //32位数组的实际长度
	UINT8 auValue[ECC_BIGINT32_MAXLEN]; //32位数组的值
}ECC_STU_BIGINT32;

typedef struct
{
	UINT8 auValue[48];
}ECC_STU_BIGINT48;

//定义ECC参数的格式
typedef struct
{
	UINT32 uField;
	UINT32 uBytes;                               // 模数P的字节数
	ECC_STU_BIGINT32  stuPrimeP;				// 模数P
	ECC_STU_BIGINT32  stuCoefficientA;			// 椭圆系数A
	ECC_STU_BIGINT32  stuCoefficientB;			// 椭圆系数B
	ECC_STU_BIGINT32  stuGx;				    // 椭圆基点坐标Gx
	ECC_STU_BIGINT32  stuGy;				    // 椭圆基点坐标Gy
	ECC_STU_BIGINT32  stuPrimeN;				// 椭圆基点坐标G的阶
	ECC_STU_BIGINT32  stuCoefficientH;			// 椭圆系数H
} ECC_STU_PRAMS;


//定义ECC公钥的格式
typedef struct
{
	ECC_STU_BIGINT32  stuQx;			        // 公钥坐标Qx
	ECC_STU_BIGINT32  stuQy;				    // 公钥坐标Qy
} ECC_STU_PUBKEY;

typedef struct
{
	ECC_STU_BIGINT32  stuQx;			        // 公钥坐标Qx
	ECC_STU_BIGINT32  stuQy;				    // 公钥坐标Qy
	ECC_STU_BIGINT32  stuQz;
}ECC_STU_JACOBIAN;

//void SM2Init(ECC_STU_PRAMS *pstuEccPrams);
UINT8  EccGenerateKeyPair(ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_PUBKEY *pstuPubKey);
void SM2Encrypt(UINT8 *mes,
		UINT16 klen,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_PRAMS *pstuEccPrams,
		UINT8 *cipher0,
		UINT8 *cipher1,
		UINT8 *cipher2);
UINT8 SM2Decrypt(UINT8 *cipher0,
		UINT8 *cipher1,
		UINT8 *cipher2,
		UINT16 klen,
		ECC_STU_BIGINT32 *stuPrivKey,
		ECC_STU_PRAMS *pstuEccPrams,
		UINT8 *mes);
void EccSignature(UINT8 *mes,
		UINT16 klen,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS);
UINT8 EccVerification(UINT8 *mes,
		UINT16 klen,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS);
void SM2SignatureWithIDA(UINT8 *mes,
		UINT16 klen,
		UINT8 *IDA,
		UINT16 entla,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS);
UINT8 SM2VerificationWithIDA(UINT8 *mes,
		UINT16 klen,
		UINT8 *IDA,
		UINT16 entla,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS);

UINT8 SM2_KeyExchange_HT(ECC_STU_PRAMS *pstuEccPrams,  // ECC上下文
                       ECC_STU_BIGINT32 *pSelfPrikey,   // 己方私钥
                       ECC_STU_PUBKEY *pSelfTempPubkey, // 己方临时公钥。
                       ECC_STU_BIGINT32 *pSelfTempPrikey,   // 己方临时私钥。
                       ECC_STU_PUBKEY *pOtherPubkey,      // 对方公钥
                       ECC_STU_PUBKEY *pOtherTempPubkey,  // 对方临时公钥。
                       UINT32 *za,            // 己方Z值
                       UINT32 *zb,           // 对方Z值
                       UINT32 *pAgreedKey,        // 协商密钥。
                    UINT32 AgreedKeyLen,       // 协商密钥长度
                    UINT32 *sA,               // SA（或S2）。
                    UINT32 *sB,               // SB（或S1）。
                       UINT32 Mode);              // 0表示己方为A方，1表示己方为B方

int SM2Init(ECC_STU_PRAMS *pstuEccPrams);

int  EccGenerateKeyPairDpa(ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_PUBKEY *pstuPubKey);

int SM2EncryptDpa(UINT8 *mes,
		UINT16 klen,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_PRAMS *pstuEccPrams,
		UINT8 *cipher0,
		UINT8 *cipher1,
		UINT8 *cipher2);

int SM2DecryptDpa(UINT8 *cipher0,
		UINT8 *cipher1,
		UINT8 *cipher2,
		UINT16 klen,
		ECC_STU_BIGINT32 *stuPrivKey,
		ECC_STU_PRAMS *pstuEccPrams,
		UINT8 *mes);

int EccSignatureDpa(UINT8 *mes,
		UINT16 klen,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS);

int EccVerificationDpa(UINT8 *mes,
		UINT16 klen,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS);

int SM2SignatureWithIDADpa(UINT8 *mes,
		UINT16 klen,
		UINT8 *IDA,
		UINT16 entla,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS);
int SM2VerificationWithIDADpa(UINT8 *mes,
		UINT16 klen,
		UINT8 *IDA,
		UINT16 entla,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS);

int SM2KeyExchangeDpa(ECC_STU_PRAMS *pstuEccPrams,  // ECC上下文
                       ECC_STU_BIGINT32 *pSelfPrikey,   // 己方私钥
                       ECC_STU_PUBKEY *pSelfTempPubkey, // 己方临时公钥。
                       ECC_STU_BIGINT32 *pSelfTempPrikey,   // 己方临时私钥。
                       ECC_STU_PUBKEY *pOtherPubkey,      // 对方公钥
                       ECC_STU_PUBKEY *pOtherTempPubkey,  // 对方临时公钥。
                       UINT32 *za,            // 己方Z值
                       UINT32 *zb,           // 对方Z值
                       UINT32 *pAgreedKey,        // 协商密钥。
                    UINT32 AgreedKeyLen,       // 协商密钥长度
                    UINT32 *sA,               // SA（或S2）。
                    UINT32 *sB,               // SB（或S1）。
                       UINT32 Mode);              // 0表示己方为A方，1表示己方为B方



#define JR0_BASE	(0xeb500000+0x1000)
#define JR0_IRBARH   (JR0_BASE+0x00)
#define JR0_IRBARL   (JR0_BASE+0x04)
#define JR0_IRSR     (JR0_BASE+0x0C)
#define JR0_IRSAR    (JR0_BASE+0x14)
#define JR0_IRJAR    (JR0_BASE+0x1C)
#define JR0_ORBARH   (JR0_BASE+0x20)
#define JR0_ORBARL   (JR0_BASE+0x24)
#define JR0_ORSR     (JR0_BASE+0x2C)
#define JR0_ORJRR    (JR0_BASE+0x34)
#define JR0_ORSFR    (JR0_BASE+0x3C)
#define JR0_JRSTAR   (JR0_BASE+0x44)
#define JR0_JRINTR   (JR0_BASE+0x4C)
#define JR0_JRCFGRH  (JR0_BASE+0x50)
#define JR0_JRCFGRL  (JR0_BASE+0x54)
#define JR0_IRRIR    (JR0_BASE+0x5C)
#define JR0_ORWIR    (JR0_BASE+0x64)
#define JR0_JRCR     (JR0_BASE+0x6C)

#define AXIRAM_UPADDR	(0x00000004)
#define AXIRAM_LADDR	(0xe00c0000)

#endif






