#include "lib_sm2.h"
#include "desc.h"
#include "desc_constr.h"
//#include "as_archppc.h"
#include "e500mc_asm.h"
#include "e500mc.h"

#define PERFORMANCE_TEST

//#define SM2_TASK_NUM	128
#define SM2_TASK_NUM	1
#define RING_SIZE	16				//over 8 can ensure all DECO work
//#define RING_SIZE	1				//over 8 can ensure all DECO work

#define CIPHER_CNT	256

extern void jr_dequeue_irq(void);

UINT8 SM2Parametres[224] =
{
		0x85, 0x42, 0xD6, 0x9E, 0x4C, 0x04, 0x4F, 0x18, 0xE8, 0xB9, 0x24, 0x35,
		0xBF, 0x6F, 0xF7, 0xDE, 0x45, 0x72, 0x83, 0x91, 0x5C, 0x45, 0x51, 0x7D,
		0x72, 0x2E, 0xDB, 0x8B, 0x08, 0xF1, 0xDF, 0xC3,

		0x78, 0x79, 0x68, 0xB4, 0xFA, 0x32, 0xC3, 0xFD, 0x24, 0x17, 0x84, 0x2E,
		0x73, 0xBB, 0xFE, 0xFF, 0x2F, 0x3C, 0x84, 0x8B, 0x68, 0x31, 0xD7, 0xE0,
		0xEC, 0x65, 0x22, 0x8B, 0x39, 0x37, 0xE4, 0x98,

		0x63, 0xE4, 0xC6, 0xD3, 0xB2, 0x3B, 0x0C, 0x84, 0x9C, 0xF8, 0x42, 0x41,
		0x48, 0x4B, 0xFE, 0x48, 0xF6, 0x1D, 0x59, 0xA5, 0xB1, 0x6B, 0xA0, 0x6E,
		0x6E, 0x12, 0xD1, 0xDA, 0x27, 0xC5, 0x24, 0x9A,

		0x42, 0x1D, 0xEB, 0xD6, 0x1B, 0x62, 0xEA, 0xB6, 0x74, 0x64, 0x34, 0xEB,
		0xC3, 0xCC, 0x31, 0x5E, 0x32, 0x22, 0x0B, 0x3B, 0xAD, 0xD5, 0x0B, 0xDC,
		0x4C, 0x4E, 0x6C, 0x14, 0x7F, 0xED, 0xD4, 0x3D,

		0x06, 0x80, 0x51, 0x2B, 0xCB, 0xB4, 0x2C, 0x07, 0xD4, 0x73, 0x49, 0xD2,
		0x15, 0x3B, 0x70, 0xC4, 0xE5, 0xD7, 0xFD, 0xFC, 0xBF, 0xA3, 0x6E, 0xA1,
		0xA8, 0x58, 0x41, 0xB9, 0xE4, 0x6E, 0x09, 0xA2,

		0x85, 0x42, 0xD6, 0x9E, 0x4C, 0x04, 0x4F, 0x18, 0xE8, 0xB9, 0x24, 0x35,
		0xBF, 0x6F, 0xF7, 0xDD, 0x29, 0x77, 0x20, 0x63, 0x04, 0x85, 0x62, 0x8D,
		0x5A, 0xE7, 0x4E, 0xE7, 0xC3, 0x2E, 0x79, 0xB7,
        /* h */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};

unsigned char SM2Parametres2[224] =
{
        /* no seed */
        /* p */
        0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        /* a */
        0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC,
        /* b */
        0x28, 0xE9, 0xFA, 0x9E, 0x9D, 0x9F, 0x5E, 0x34, 0x4D, 0x5A, 0x9E, 0x4B,
        0xCF, 0x65, 0x09, 0xA7, 0xF3, 0x97, 0x89, 0xF5, 0x15, 0xAB, 0x8F, 0x92,
        0xDD, 0xBC, 0xBD, 0x41, 0x4D, 0x94, 0x0E, 0x93,
        /* x */
        0x32, 0xC4, 0xAE, 0x2C, 0x1F, 0x19, 0x81, 0x19, 0x5F, 0x99, 0x04, 0x46,
        0x6A, 0x39, 0xC9, 0x94, 0x8F, 0xE3, 0x0B, 0xBF, 0xF2, 0x66, 0x0B, 0xE1,
        0x71, 0x5A, 0x45, 0x89, 0x33, 0x4C, 0x74, 0xC7,
        /* y */
        0xBC, 0x37, 0x36, 0xA2, 0xF4, 0xF6, 0x77, 0x9C, 0x59, 0xBD, 0xCE, 0xE3,
        0x6B, 0x69, 0x21, 0x53, 0xD0, 0xA9, 0x87, 0x7C, 0xC6, 0x2A, 0x47, 0x40,
        0x02, 0xDF, 0x32, 0xE5, 0x21, 0x39, 0xF0, 0xA0,
        /* order */
        0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0x72, 0x03, 0xDF, 0x6B, 0x21, 0xC6, 0x05, 0x2B,
        0x53, 0xBB, 0xF4, 0x09, 0x39, 0xD5, 0x41, 0x23,
        /* h */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};

static UINT8 one[32] =
{
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

UINT8 SM2ParametresHash[32] = {0};

void desc_dump(UINT32 *desc, UINT32 num)
{
	UINT32 i;
	for(i=0; i<num; i++)
		PSPRINTF("0x%x, ", desc[i]);
	PSPRINTF("\r\n");
}

UINT8  EccGenerateKeyPair(ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_PUBKEY *pstuPubKey)
{
	UINT32 ret;
	UINT32 desc[MAX_CAAM_DESCSIZE];
#ifdef PERFORMANCE_TEST
	unsigned int start, finish, i;
#endif

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_E_SZ);
	//generate private key with random data
	append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc, SM3_DIGEST_LEN);
	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI);
	append_seq_fifo_store(desc, SM3_DIGEST_LEN, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGFIFO);
	append_move(desc, MOVE_WAITCOMP | MOVE_SRC_OUTFIFO | MOVE_DEST_PK_A | SM3_DIGEST_LEN);
	//private key mod order
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	if(pstuEccPrams->uBytes%16)
		append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_ALL | JUMP_COND_PK_0 | 0xf5);
	else
		append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_ALL | JUMP_COND_PK_0 | 0xf7);
	append_fifo_store(desc, virt_to_phys(pstuPrivKey), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOST_CLASS_SWAP | FIFOST_TYPE_PKHA_B);
	//load p
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeP.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	//load G
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	//load a
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A3|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3 | FIFOLDST_SGF | FIFOLD_IMM);

	//copy k to e_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//load b
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_B0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	//do ecc mod mult
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);
	//store pubkey
	append_fifo_store(desc, virt_to_phys(pstuPubKey->stuQx.auValue), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOST_CLASS_SWAP | FIFOST_TYPE_PKHA_B1);
	append_fifo_store(desc, virt_to_phys(pstuPubKey->stuQy.auValue), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOST_CLASS_SWAP | FIFOST_TYPE_PKHA_B2);

//	PSPRINTF("EccGenerateKeyPair:\r\n");
//	desc_dump(desc, MAX_CAAM_DESCSIZE);
#ifdef PERFORMANCE_TEST
	MFSPR(start, TBL);
	for(i=0; i<100; i++)
	{
#endif
	ret = run_descriptor_jr(desc, &jr_t);
#ifdef PERFORMANCE_TEST
	}
	MFSPR(finish, TBL);
	PSPRINTF("cost %d cycle\r\n", finish-start);
#endif

	if(!ret)
		return 0;//ok
	else
		return 0xff;//error
}

void SM2Encrypt(UINT8 *mes,
		UINT16 klen,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_PRAMS *pstuEccPrams,
		UINT8 *cipher0,
		UINT8 *cipher1,
		UINT8 *cipher2)
{
	UINT32 desc[MAX_CAAM_DESCSIZE];
	UINT32 ct = (klen+SM3_DIGEST_LEN-1)/SM3_DIGEST_LEN, i;
	//UINT32 desc_ext[MAX_CAAM_DESCSIZE];
	UINT8 hashin[HASH_IN_SIZE];
	UINT8 hashout[HASH_OUT_SIZE];

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_E_SZ);
	//generate k
	append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc, SM3_DIGEST_LEN);
	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI);
	append_seq_fifo_store(desc, SM3_DIGEST_LEN, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGFIFO);
	append_move(desc, MOVE_WAITCOMP | MOVE_SRC_OUTFIFO | MOVE_DEST_PK_A | SM3_DIGEST_LEN);
	//k mod order
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	if(pstuEccPrams->uBytes%16)
		append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_ALL | JUMP_COND_PK_0 | 0xf5);
	else
		append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_ALL | JUMP_COND_PK_0 | 0xf7);
	//calculate c1
	//load p
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeP.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	//load G
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	//load a
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A3|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3 | FIFOLDST_SGF | FIFOLD_IMM);
	//save a
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A3 | OP_ALG_PKMODE_DST_REG_N10 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//load k
	//copy k to e_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//load b
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_B0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	//save b
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B0 | OP_ALG_PKMODE_DST_REG_N11 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//do ecc mod mult
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);
	//store c1
	append_fifo_store(desc, virt_to_phys(cipher0+1), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOST_CLASS_SWAP | FIFOST_TYPE_PKHA_B1);
	append_fifo_store(desc, virt_to_phys(cipher0+1+pstuEccPrams->uBytes), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOST_CLASS_SWAP | FIFOST_TYPE_PKHA_B2);

	//calculate c2
	//load public key
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	//load a&b
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_A3 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N11 | OP_ALG_PKMODE_DST_REG_B0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//do ecc mod mult
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);
	//store x2,y2
	append_fifo_store(desc, virt_to_phys(hashin), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOST_CLASS_SWAP | FIFOST_TYPE_PKHA_B1);
	append_fifo_store(desc, virt_to_phys(hashin+pstuEccPrams->uBytes), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOST_CLASS_SWAP | FIFOST_TYPE_PKHA_B2);//72
	//kdf
	//calc ct
	append_seq_out_ptr(desc, virt_to_phys(hashout), ct*SM3_DIGEST_LEN, 0);
	//append_math_sub(desc, REG0, REG0, REG0, 16);
	append_load(desc, virt_to_phys(&ct), 4, LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH0);
	append_math_swap(desc, REG0, REG0, ONE, 4);
	append_math_sub(desc, REG1, REG1, REG1, 4);

	append_math_add(desc, REG1, REG1, ONE, 4);
#ifdef LITTLE_ENDIAN
	append_math_swap(desc, REG2, REG1, ONE, 4);
	append_store(desc, virt_to_phys(hashin+2*pstuEccPrams->uBytes), 4, LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH2);
#else
	append_store(desc, virt_to_phys(hashin+2*pstuEccPrams->uBytes), 4, LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH1);
#endif
	append_load(desc, virt_to_phys(hashin+2*pstuEccPrams->uBytes), 4, LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);
	append_move(desc, MOVE_SRC_CLASS1CTX | MOVE_DEST_MATH3 | 4);
	append_math_swap(desc, REG3, REG3, ONE, 4);
	append_math_sub(desc, REG3, REG1, REG3, 4);
	append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_INVALL | JUMP_COND_MATH_Z | 0xfa);
	//do sm3
	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x40000);
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc, virt_to_phys(hashin), 2*pstuEccPrams->uBytes+4, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_seq_store(desc, SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);
	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x10000);
	append_math_sub(desc, REG3, REG0, REG1, 4);
#ifdef LITTLE_ENDIAN
	append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_INVALL | JUMP_COND_MATH_Z | 0xf1);
#else
	append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_INVALL | JUMP_COND_MATH_Z | 0xeb);
#endif
/*
	append_math_add(desc, REG0, ZERO, ONE, 4);
#ifdef LITTLE_ENDIAN
	append_math_swap(desc, REG1, REG0, ONE, 4);
	append_store(desc, virt_to_phys(hashin+2*pstuEccPrams->uBytes), 4, LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH1);
#else
	append_store(desc, virt_to_phys(hashin+2*pstuEccPrams->uBytes), 4, LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH0);
#endif
	//do sm3
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc, virt_to_phys(hashin), 2*pstuEccPrams->uBytes+4, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_store(desc, virt_to_phys(hashout), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);
	//jump to extern descriptor
	append_jump(desc, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	append_ptr(desc, virt_to_phys(desc_ext));
	//extern descriptor
	init_job_desc(desc_ext, START_INDEX);
	for(i=1; i<ct; i++)
	{
		//calc ct
		append_math_add(desc_ext, REG0, REG0, ONE, 4);
#ifdef LITTLE_ENDIAN
		append_math_swap(desc_ext, REG1, REG0, ONE, 4);
		append_store(desc_ext, virt_to_phys(hashin+2*pstuEccPrams->uBytes), 4, LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH1);
#else
		append_store(desc_ext, virt_to_phys(hashin+2*pstuEccPrams->uBytes), 4, LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH0);
#endif
		//do sm3
		append_cmd(desc_ext, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
		append_cmd(desc_ext, 0x40000);
		append_operation(desc_ext, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
		append_fifo_load(desc_ext, virt_to_phys(hashin), 2*pstuEccPrams->uBytes+4, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
		append_store(desc_ext, virt_to_phys(hashout+i*SM3_DIGEST_LEN), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);
	}
*/
	//sync
	append_jump(desc, JUMP_CLASS_CLASS2 | JUMP_TYPE_LOCAL | JUMP_TEST_ALL | 1);
	//do t^mes
	append_fifo_load(desc, virt_to_phys(hashout), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys(hashout), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_F2M | OP_ALG_PKMODE_MOD_ADD);
	//store c2
	append_fifo_store(desc, virt_to_phys(cipher1), klen, FIFOST_CLASS_NORMAL|FIFOLD_TYPE_PK_B);

	//calculate c3
	//do sm3
	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x40000);
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	//load x2
	append_fifo_load(desc, virt_to_phys(hashin), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	//load mes
	append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	//load y2
	append_fifo_load(desc, virt_to_phys(hashin+pstuEccPrams->uBytes), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	//store c3
	append_store(desc, virt_to_phys(cipher2), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);

//	PSPRINTF("SM2Encrypt:\r\n");
//	desc_dump(desc, MAX_CAAM_DESCSIZE);
	run_descriptor_jr(desc, &jr_t);
	cipher0[0] = 0x4;
}

UINT8 SM2Decrypt(UINT8 *cipher0,
		UINT8 *cipher1,
		UINT8 *cipher2,
		UINT16 klen,
		ECC_STU_BIGINT32 *stuPrivKey,
		ECC_STU_PRAMS *pstuEccPrams,
		UINT8 *mes)
{
	UINT32 ret;
	UINT32 desc[MAX_CAAM_DESCSIZE];
	UINT32 ct = (klen+SM3_DIGEST_LEN-1)/SM3_DIGEST_LEN, i;
	//UINT32 desc_ext[MAX_CAAM_DESCSIZE];
	UINT8 hashin[HASH_IN_SIZE];
	UINT8 hashout[HASH_OUT_SIZE];

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_E_SZ);
	//calc x2,y2
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	//load p
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeP.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	//load x1
	append_fifo_load(desc, virt_to_phys(cipher0+1), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLDST_SGF | FIFOLD_IMM | FIFOLD_TYPE_PK_A0);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	//load y1
	append_fifo_load(desc, virt_to_phys(cipher0+1+pstuEccPrams->uBytes), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLDST_SGF | FIFOLD_IMM | FIFOLD_TYPE_PK_A1);
	//load a
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A3|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3 | FIFOLDST_SGF | FIFOLD_IMM);
	//load private key
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_B0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(stuPrivKey->auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	//copy private key to e_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//load b
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_B0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	//do ecc mod mult
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);
	//store x2,y2
	append_fifo_store(desc, virt_to_phys(hashin), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOST_CLASS_SWAP | FIFOST_TYPE_PKHA_B1);
	append_fifo_store(desc, virt_to_phys(hashin+pstuEccPrams->uBytes), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOST_CLASS_SWAP | FIFOST_TYPE_PKHA_B2);

	//kdf
	//calc ct
	append_seq_out_ptr(desc, virt_to_phys(hashout), ct*SM3_DIGEST_LEN, 0);
	append_cmd(desc, CMD_LOAD | LDST_CLASS_DECO | LDST_IMM | LDST_SRCDST_WORD_DECO_MATH0 | 4);
	append_cmd(desc, ct);
	append_math_sub(desc, REG1, REG1, REG1, 4);

	append_math_add(desc, REG1, REG1, ONE, 4);
#ifdef LITTLE_ENDIAN
	append_math_swap(desc, REG2, REG1, ONE, 4);
	append_store(desc, virt_to_phys(hashin+2*pstuEccPrams->uBytes), 4, LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH2);
#else
	append_store(desc, virt_to_phys(hashin+2*pstuEccPrams->uBytes), 4, LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH1);
#endif
	//do sm3
	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x40000);
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc, virt_to_phys(hashin), 2*pstuEccPrams->uBytes+4, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_seq_store(desc, SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);
	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x10000);
	append_math_sub(desc, REG3, REG0, REG1, 4);
#ifdef LITTLE_ENDIAN
	append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_INVALL | JUMP_COND_MATH_Z | 0xf1);
#else
	append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_INVALL | JUMP_COND_MATH_Z | 0xf2);
#endif

	//do t^c2
	append_fifo_load(desc, virt_to_phys(hashout), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys(hashout), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_fifo_load(desc, virt_to_phys(cipher1), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_F2M | OP_ALG_PKMODE_MOD_ADD);
	append_fifo_store(desc, virt_to_phys(mes), klen, FIFOST_CLASS_NORMAL|FIFOLD_TYPE_PK_B);

	//verify c3
	//do sm3
	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x40000);
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	//load x2
	append_fifo_load(desc, virt_to_phys(hashin), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	//load mes
	append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	//load y2
	append_fifo_load(desc, virt_to_phys(hashin+pstuEccPrams->uBytes), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	//move hash to a_ram
	append_move(desc, MOVE_WAITCOMP | MOVE_SRC_CLASS2CTX | MOVE_DEST_PK_A | SM3_DIGEST_LEN);
	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x10000);
	//load n
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue+pstuEccPrams->uBytes-SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	//load c3
	append_fifo_load(desc, virt_to_phys(cipher2), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	//do mod sub
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
	//is zero
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_INVALL | JUMP_COND_PK_0 | 0x1);

//	PSPRINTF("SM2Decrypt:\r\n");
//	desc_dump(desc, MAX_CAAM_DESCSIZE);
	ret = run_descriptor_jr(desc, &jr_t);

	if(!ret)
		return 0;//ok
	else
		return 0xff;//error
}

void EccSignature(UINT8 *mes,
		UINT16 klen,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS)
{
#ifdef PERFORMANCE_TEST
	unsigned int start, finish;
#endif
	UINT32 desc[MAX_CAAM_DESCSIZE], i;
	UINT8 one[ECC_BIGINT32_MAXLEN];
	one[0] = 1;
	for(i=1; i<ECC_BIGINT32_MAXLEN; i++)
		one[i] = 0;

	init_job_desc(desc, START_INDEX);
	append_operation(desc,	OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_E_SZ);
	append_operation(desc,	OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	//generate k
	append_cmd(desc,CMD_LOAD | LDST_CLASS_1_CCB | LDST_IMM	| LDST_SRCDST_WORD_RNGDATASZ_REG | 4);
	append_cmd(desc, SM3_DIGEST_LEN);
	append_operation(desc, OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI);
	append_seq_fifo_store(desc, SM3_DIGEST_LEN, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGFIFO);
	append_move(desc, MOVE_WAITCOMP | MOVE_SRC_OUTFIFO | MOVE_DEST_PK_A | SM3_DIGEST_LEN);
	//k mod order
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_N | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	if (pstuEccPrams->uBytes % 16)
		append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_ALL | JUMP_COND_PK_0 | 0xf5);
	else
		append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_ALL | JUMP_COND_PK_0 | 0xf7);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N11 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//do e mod n
	//load n
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_N | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	//do amodn
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	//save e mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N10 | OP_ALG_PKMODE_CPYMEM_N_SZ);

	//(x1,y1) = [k]G
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_N | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	//load p
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeP.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	//load G
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_A0 | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_A1 | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_A3 | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	//load a
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3 | FIFOLDST_SGF | FIFOLD_IMM);
	//load k to e_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N11 | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//load b
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_B0 | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes,	FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	//do ecc mod mult
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);

	//calculate r
	//load n
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_N | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	//copy x1 to a_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B1 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//do x1 mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	//load e mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//do e+x1 mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);
	//save r
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N12 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_fifo_store(desc, virt_to_phys(pSignatureR->auValue), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOLD_TYPE_PK_B | FIFOST_CLASS_SWAP);

	//calculate s
	if (pstuEccPrams->uBytes == SM3_DIGEST_LEN) {
		append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
		//load private key
		append_fifo_load(desc, virt_to_phys(pstuPrivKey->auValue), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	} else {
		append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue + pstuEccPrams->uBytes - SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
		//load private key
		append_fifo_load(desc, virt_to_phys(pstuPrivKey->auValue + pstuEccPrams->uBytes - SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	}
	//save private key
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A | OP_ALG_PKMODE_DST_REG_N13 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//load 1
	append_fifo_load(desc, virt_to_phys(one), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	//do 1+da mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);
	//copy to a_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//do (1+da)^-1 mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_INV);
	//save result
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//do r*da mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N12 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N13 | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	//do k-r*da mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N11 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
	//calc s
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	if (pstuEccPrams->uBytes == SM3_DIGEST_LEN)
		append_fifo_store(desc, virt_to_phys(pSignatureS->auValue), SM3_DIGEST_LEN, FIFOST_CLASS_NORMAL | FIFOLD_TYPE_PK_B | FIFOST_CLASS_SWAP);
	else
		append_fifo_store(desc, virt_to_phys(pSignatureS->auValue + pstuEccPrams->uBytes - SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOST_CLASS_NORMAL | FIFOLD_TYPE_PK_B | FIFOST_CLASS_SWAP);

//	PSPRINTF("SM2Signature:\r\n");
//	desc_dump(desc, MAX_CAAM_DESCSIZE);	
#ifdef PERFORMANCE_TEST
	MFSPR(start, TBL);
	for(i=0; i<100; i++)
	{
#endif
	run_descriptor_jr(desc, &jr_t);
#ifdef PERFORMANCE_TEST
	}
	MFSPR(finish, TBL);
	PSPRINTF("cost %d cycle\r\n", finish-start);
#endif

}

UINT8 EccVerification(UINT8 *mes,
		UINT16 klen,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS)
{
#ifdef PERFORMANCE_TEST
	unsigned int start, finish;
#endif
	UINT32 ret;
	UINT32 desc[MAX_CAAM_DESCSIZE], i;
	UINT8 one[ECC_BIGINT32_MAXLEN];
	one[0] = 1;
	for(i=1; i<ECC_BIGINT32_MAXLEN; i++)
		one[i] = 0;

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_E_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	//compare r and 1
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pSignatureR->auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A | OP_ALG_PKMODE_DST_REG_N10 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_fifo_load(desc, virt_to_phys(one), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N11 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_ALL | JUMP_COND_PK_BORROW | 0x1);
	//compare r and n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_INVALL | JUMP_COND_PK_BORROW | 0x2);
	//compare s and n
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pSignatureS->auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A | OP_ALG_PKMODE_DST_REG_N12 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_INVALL | JUMP_COND_PK_BORROW | 0x3);
	//compare s and 1
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N11 | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_ALL | JUMP_COND_PK_BORROW | 0x4);

	//do e mod n
	//load n
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	//do amodn
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_A_SZ);
	//save e mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N13 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//t = (r + s) mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N12 | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);

	//calculate [t]Pa
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeP.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A3|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3 | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A3 | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_B0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B0 | OP_ALG_PKMODE_DST_REG_N21 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B1 | OP_ALG_PKMODE_DST_REG_N22 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B2 | OP_ALG_PKMODE_DST_REG_N23 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	//calculate [s]G
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A3 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N21 | OP_ALG_PKMODE_DST_REG_B0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N12 | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT |pstuEccPrams->uField);

	//(x1, y1) = [s]G + [t]Pa
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A3 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N21 | OP_ALG_PKMODE_DST_REG_B0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N22 | OP_ALG_PKMODE_DST_REG_A0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N23 | OP_ALG_PKMODE_DST_REG_A1 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_ADD | pstuEccPrams->uField);

	//r' = (e + x1) mod n
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B1 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N13 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);

	//compare r' and r
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_INVALL | JUMP_COND_PK_0 | 0x5);

//	PSPRINTF("SM2Verification:\r\n");
//	desc_dump(desc, MAX_CAAM_DESCSIZE);
	
#ifdef PERFORMANCE_TEST
	MFSPR(start, TBL);
	for(i=0; i<100; i++)
	{
#endif
		ret = run_descriptor_jr(desc, &jr_t);
#ifdef PERFORMANCE_TEST
	}
	MFSPR(finish, TBL);
	PSPRINTF("cost %d cycle\r\n", finish-start);
#endif
	
	if(!ret)
		return 0;//ok
	else
		return 0xff;//error
}

void SM2SignatureWithIDA(UINT8 *mes,
		UINT16 klen,
		UINT8 *IDA,
		UINT16 entla,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS)
{
	UINT8 hashin[2];
	UINT32 desc[MAX_CAAM_DESCSIZE], i;
	//UINT32 desc_ext[MAX_CAAM_DESCSIZE];
	UINT8 one[ECC_BIGINT32_MAXLEN];
	one[0] = 1;
	for(i=1; i<ECC_BIGINT32_MAXLEN; i++)
		one[i] = 0;

	hashin[0] = ((entla*8)&0xff00)>>8;
	hashin[1] = (entla*8)&0xff;

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc,&pstuEccPrams->uBytes,4,LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
	append_load_as_imm(desc,&pstuEccPrams->uBytes,4,LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_E_SZ);
	//generate k
	append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc, SM3_DIGEST_LEN);
	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI);
	append_seq_fifo_store(desc, SM3_DIGEST_LEN, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGFIFO);
	append_move(desc, MOVE_WAITCOMP | MOVE_SRC_OUTFIFO | MOVE_DEST_PK_A | SM3_DIGEST_LEN);
	//k mod order
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	if(pstuEccPrams->uBytes%16)
		append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_ALL | JUMP_COND_PK_0 | 0xf5);
	else
		append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_ALL | JUMP_COND_PK_0 | 0xf7);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N11 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//calculate za
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc, virt_to_phys(hashin), 2, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(IDA), entla, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	//calculate e
	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x40000);
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_move(desc, MOVE_SRC_CLASS2CTX | MOVE_DEST_CLASS2INFIFO | SM3_DIGEST_LEN);
	append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);

	//do e mod n
	//move e to a_ram
	append_move(desc, MOVE_WAITCOMP | MOVE_SRC_CLASS2CTX | MOVE_DEST_PK_A | MOVE_AUX_LS | SM3_DIGEST_LEN);
	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x10000);
	//load n
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	//do amodn
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	//save e mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N10 | OP_ALG_PKMODE_CPYMEM_N_SZ);

	//jump to extern descriptor
	//append_jump(desc, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	//append_ptr(desc, virt_to_phys(desc_ext));
	//extern descriptor
	//init_job_desc(desc_ext, START_INDEX);

	//(x1,y1) = [k]G
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	//load p
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeP.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	//load G
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A3|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	//load a
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3 | FIFOLDST_SGF | FIFOLD_IMM);
	//load k to e_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N11 | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//load b
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_B0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	//do ecc mod mult
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);

	//calculate r
	//load n
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	//copy x1 to a_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B1 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//do x1 mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	//load e mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//do e+x1 mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);
	//save r
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N12 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_fifo_store(desc, virt_to_phys(pSignatureR), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL|FIFOLD_TYPE_PK_B|FIFOST_CLASS_SWAP);

	//calculate s
	if(pstuEccPrams->uBytes == SM3_DIGEST_LEN)
	{
		append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
		//load private key
		append_fifo_load(desc, virt_to_phys(pstuPrivKey), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	}
	else
	{
		append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue+pstuEccPrams->uBytes-SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
		//load private key
		append_fifo_load(desc, virt_to_phys(pstuPrivKey+pstuEccPrams->uBytes-SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	}
	//save private key
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A | OP_ALG_PKMODE_DST_REG_N13 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//load 1
	append_fifo_load(desc, virt_to_phys(one), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	//do 1+da mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);
	//copy to a_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//do (1+da)^-1 mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_INV);
	//save result
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//do r*da mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N12 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N13 | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	//do k-r*da mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N11 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
	//calc s
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	if(pstuEccPrams->uBytes == SM3_DIGEST_LEN)
		append_fifo_store(desc, virt_to_phys(pSignatureS), SM3_DIGEST_LEN, FIFOST_CLASS_NORMAL|FIFOLD_TYPE_PK_B|FIFOST_CLASS_SWAP);
	else
		append_fifo_store(desc, virt_to_phys(pSignatureS+pstuEccPrams->uBytes-SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOST_CLASS_NORMAL|FIFOLD_TYPE_PK_B|FIFOST_CLASS_SWAP);

//	PSPRINTF("SM2SignatureWithIDA:\r\n");
//	desc_dump(desc, MAX_CAAM_DESCSIZE);
	run_descriptor_jr(desc, &jr_t);
}

UINT8 SM2VerificationWithIDA(UINT8 *mes,
		UINT16 klen,
		UINT8 *IDA,
		UINT16 entla,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS)
{
	UINT32 ret;
	UINT8 hashin[2];
	UINT32 desc[MAX_CAAM_DESCSIZE], i;
	UINT32 desc_ext[MAX_CAAM_DESCSIZE];
	UINT8 one[ECC_BIGINT32_MAXLEN];
	one[0] = 1;
	for(i=1; i<ECC_BIGINT32_MAXLEN; i++)
		one[i] = 0;

	hashin[0] = ((entla*8)&0xff00)>>8;
	hashin[1] = (entla*8)&0xff;

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc,&pstuEccPrams->uBytes,4,LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_E_SZ);
	//compare r and 1
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pSignatureR), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A | OP_ALG_PKMODE_DST_REG_N10 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_fifo_load(desc, virt_to_phys(one), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N11 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_ALL | JUMP_COND_PK_BORROW | 0x1);
	//compare r and n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_INVALL | JUMP_COND_PK_BORROW | 0x2);
	//compare s and n
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pSignatureS), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A | OP_ALG_PKMODE_DST_REG_N12 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_INVALL | JUMP_COND_PK_BORROW | 0x3);
	//compare s and 1
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N11 | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_ALL | JUMP_COND_PK_BORROW | 0x4);

	//calculate za
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc, virt_to_phys(hashin), 2, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(IDA), entla, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	//calculate e
	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x40000);
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_move(desc, MOVE_SRC_CLASS2CTX | MOVE_DEST_CLASS2INFIFO | SM3_DIGEST_LEN);
	append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);

	//do e mod n
	//move e to a_ram
	append_move(desc, MOVE_WAITCOMP | MOVE_SRC_CLASS2CTX | MOVE_DEST_PK_A | MOVE_AUX_LS | SM3_DIGEST_LEN);
	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x10000);
	//load n
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	//do amodn
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	//save e mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N13 | OP_ALG_PKMODE_CPYMEM_N_SZ);

	//t = (r + s) mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N12 | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);//80

	//jump to extern descriptor
	append_jump(desc, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	append_ptr(desc, virt_to_phys(desc_ext));
	//extern descriptor
	init_job_desc(desc_ext, START_INDEX);

	//calculate [t]Pa
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc_ext, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc_ext, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc_ext, virt_to_phys(pstuEccPrams->stuPrimeP.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc_ext, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc_ext, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc_ext, virt_to_phys(pstuPubKey->stuQx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc_ext, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc_ext, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc_ext, virt_to_phys(pstuPubKey->stuQy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc_ext, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc_ext, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A3|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc_ext, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3 | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A3 | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc_ext, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc_ext, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_B0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc_ext, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B0 | OP_ALG_PKMODE_DST_REG_N21 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B1 | OP_ALG_PKMODE_DST_REG_N22 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B2 | OP_ALG_PKMODE_DST_REG_N23 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	//calculate [s]G
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc_ext, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc_ext, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc_ext, virt_to_phys(pstuEccPrams->stuGx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc_ext, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc_ext, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc_ext, virt_to_phys(pstuEccPrams->stuGy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A3 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N21 | OP_ALG_PKMODE_DST_REG_B0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N12 | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT |pstuEccPrams->uField);

	//(x1, y1) = [s]G + [t]Pa
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A3 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N21 | OP_ALG_PKMODE_DST_REG_B0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N22 | OP_ALG_PKMODE_DST_REG_A0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N23 | OP_ALG_PKMODE_DST_REG_A1 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_ADD | pstuEccPrams->uField);

	//r' = (e + x1) mod n
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc_ext, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc_ext, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc_ext, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B1 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N13 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);

	//compare r' and r
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc_ext, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
	append_jump(desc_ext, JUMP_TYPE_HALT_USER | JUMP_TEST_INVALL | JUMP_COND_PK_0 | 0x5);

//	PSPRINTF("SM2VerificationWithIDA:\r\n");
//	desc_dump(desc, MAX_CAAM_DESCSIZE);
//	desc_dump(desc_ext, MAX_CAAM_DESCSIZE);
	ret = run_descriptor_jr(desc, &jr_t);

	if(!ret)
		return 0;//ok
	else
		return 0xff;//error
}

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
                       UINT32 Mode)              // 0表示己方为A方，1表示己方为B方
{
	UINT32 ret;
	UINT8 hashin[2] = {0x2, 0x3}, self_x[ECC_BIGINT32_MAXLEN] = {0}, other_x[ECC_BIGINT32_MAXLEN] = {0};
	UINT8 point[ECC_BIGINT32_MAXLEN*2], temphash[SM3_DIGEST_LEN];
	UINT32 desc[MAX_CAAM_DESCSIZE], i;
	UINT32 desc_ext[MAX_CAAM_DESCSIZE];

	for(i=0; i<16; i++)
	{
		self_x[pstuEccPrams->uBytes-1-i] = pSelfTempPubkey->stuQx.auValue[pstuEccPrams->uBytes-1-i];
		other_x[pstuEccPrams->uBytes-1-i] = pOtherTempPubkey->stuQx.auValue[pstuEccPrams->uBytes-1-i];
	}
	self_x[pstuEccPrams->uBytes-16] |= 0x80;
	other_x[pstuEccPrams->uBytes-16] |= 0x80;

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_E_SZ);
	//calc x * r mod n
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue+pstuEccPrams->uBytes-SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys(self_x+pstuEccPrams->uBytes-SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys(pSelfTempPrikey->auValue+pstuEccPrams->uBytes-SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);

	//calc t = d + (x * r) mod n
	append_fifo_load(desc, virt_to_phys(pSelfPrikey->auValue+pstuEccPrams->uBytes-SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);

	//calc h * t mod n
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientH.auValue+pstuEccPrams->uBytes-SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N10 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	//calc [x'].R'
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeP.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pOtherTempPubkey->stuQx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pOtherTempPubkey->stuQy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A3|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3 | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A3 | OP_ALG_PKMODE_DST_REG_N11 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_B|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(other_x), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_B0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B0 | OP_ALG_PKMODE_DST_REG_N12 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);

	//calc P' + [x'].R'
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N11 | OP_ALG_PKMODE_DST_REG_A3 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N12 | OP_ALG_PKMODE_DST_REG_B0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pOtherPubkey->stuQx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pOtherPubkey->stuQy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_ADD | pstuEccPrams->uField);

	//calc V
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N11 | OP_ALG_PKMODE_DST_REG_A3 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N12 | OP_ALG_PKMODE_DST_REG_B0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B1 | OP_ALG_PKMODE_DST_REG_A0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B2 | OP_ALG_PKMODE_DST_REG_A1 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);
	append_fifo_store(desc, virt_to_phys(point), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOLD_TYPE_PK_B1 | FIFOST_CLASS_SWAP);
	append_fifo_store(desc, virt_to_phys(point+pstuEccPrams->uBytes), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOLD_TYPE_PK_B2 | FIFOST_CLASS_SWAP);

	//jump to extern descriptor
	append_jump(desc, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	append_ptr(desc, virt_to_phys(desc_ext));
	//extern descriptor
	init_job_desc(desc_ext, START_INDEX);

	//kdf
	append_math_add(desc_ext, REG0, ZERO, ONE, 4);
#ifdef LITTLE_ENDIAN
	append_math_swap(desc_ext, REG0, REG0, ONE, 4);
#endif
	append_store(desc_ext, virt_to_phys(point+2*pstuEccPrams->uBytes), 4, LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH0);
	append_operation(desc_ext, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc_ext, virt_to_phys(point), 2*pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	if(!Mode)
	{
		append_fifo_load(desc_ext, virt_to_phys(za), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
		append_fifo_load(desc_ext, virt_to_phys(zb), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	}
	else
	{
		append_fifo_load(desc_ext, virt_to_phys(zb), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
		append_fifo_load(desc_ext, virt_to_phys(za), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	}
	append_fifo_load(desc_ext, virt_to_phys(point+2*pstuEccPrams->uBytes), 4, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_store(desc_ext, virt_to_phys(pAgreedKey), AgreedKeyLen, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);

	//calc s1
	append_cmd(desc_ext, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc_ext, 0x40000);
	append_operation(desc_ext, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc_ext, virt_to_phys(point), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	if(!Mode)
	{
		append_fifo_load(desc_ext, virt_to_phys(za), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
		append_fifo_load(desc_ext, virt_to_phys(zb), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
		append_fifo_load(desc_ext, virt_to_phys(pSelfTempPubkey->stuQx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
		append_fifo_load(desc_ext, virt_to_phys(pSelfTempPubkey->stuQy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
		append_fifo_load(desc_ext, virt_to_phys(pOtherTempPubkey->stuQx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
		append_fifo_load(desc_ext, virt_to_phys(pOtherTempPubkey->stuQy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	}
	else
	{
		append_fifo_load(desc_ext, virt_to_phys(zb), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
		append_fifo_load(desc_ext, virt_to_phys(za), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
		append_fifo_load(desc_ext, virt_to_phys(pOtherTempPubkey->stuQx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
		append_fifo_load(desc_ext, virt_to_phys(pOtherTempPubkey->stuQy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
		append_fifo_load(desc_ext, virt_to_phys(pSelfTempPubkey->stuQx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
		append_fifo_load(desc_ext, virt_to_phys(pSelfTempPubkey->stuQy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	}
	append_store(desc_ext, virt_to_phys(temphash), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);
	append_cmd(desc_ext, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc_ext, 0x40000);
	append_operation(desc_ext, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc_ext, virt_to_phys(hashin), 1, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc_ext, virt_to_phys(point+pstuEccPrams->uBytes), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc_ext, virt_to_phys(temphash), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_store(desc_ext, virt_to_phys(sA), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);

	//calc s2
	append_cmd(desc_ext, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc_ext, 0x40000);
	append_operation(desc_ext, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc_ext, virt_to_phys(hashin+1), 1, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc_ext, virt_to_phys(point+pstuEccPrams->uBytes), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc_ext, virt_to_phys(temphash), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_store(desc_ext, virt_to_phys(sB), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);

	//PSPRINTF("SM2KeyExchange:\r\n");
	//desc_dump(desc, MAX_CAAM_DESCSIZE);
	//desc_dump(desc_ext, MAX_CAAM_DESCSIZE);
	ret = run_descriptor_jr(desc, &jr_t);

	if(!ret)
		return 0;//ok
	else
		return 0xff;//error
}

UINT16 EccBigFf1(ECC_STU_BIGINT32 *pstuA, UINT16 size)
{
	SINT16 i,j;
	int value;
	for(i=size-1;i>=0;i--)
	{
		value = 0x80;
		for(j=7;j>=0;j--)
		{
			if(pstuA->auValue[size-1-i]&value)
			{
				return(8*i+j);
			}
			value >>=1 ;
		}
	}
	return 0;
}

UINT8 EccBigIsBitSet(ECC_STU_BIGINT32 *pstuA, SINT16 n, UINT16 size)
{
	SINT16 i, j;

	i = size - n / 8 - 1;
	j = n % 8;

	return ((pstuA->auValue[i] >> j) & ((UINT8)1));
}

int GetRandomArray(UINT8 *pRandArray, UINT32 len)
{
	UINT32 desc[MAX_CAAM_DESCSIZE];

	init_job_desc(desc, START_INDEX);

	append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc, len);
	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI);
	append_fifo_store(desc, virt_to_phys(pRandArray), len, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE);

	return run_descriptor_jr(desc, &jr_t);
}

int PkhaModularMath(ECC_STU_BIGINT32 *pstuA,
					ECC_STU_BIGINT32 *pstuB,
					ECC_STU_BIGINT32 *pstuE,
					ECC_STU_BIGINT32 *pstuM,
					ECC_STU_BIGINT32 *pstuC,
					UINT32 mode)
{
	UINT32 desc[MAX_CAAM_DESCSIZE];

	init_job_desc(desc, START_INDEX);
	if(pstuE)
	{
		append_fifo_load(desc, virt_to_phys(pstuE->auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	}
	if(pstuM)
		append_fifo_load(desc, virt_to_phys(pstuM->auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuA)
		append_fifo_load(desc, virt_to_phys(pstuA->auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuB)
		append_fifo_load(desc, virt_to_phys(pstuB->auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | mode);
	append_fifo_store(desc, virt_to_phys(pstuC->auValue), ECC_BIGINT32_MAXLEN, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	return run_descriptor_jr(desc, &jr_t);
}

int PkhaEccMath(ECC_STU_PRAMS *pstuEccPrams,
				ECC_STU_PUBKEY *pPointA,
				ECC_STU_PUBKEY *pPointB,
				ECC_STU_BIGINT32 *pstuK,
				ECC_STU_PUBKEY *pPointC,
				UINT32 mode)
{
	UINT32 desc[MAX_CAAM_DESCSIZE];
	init_job_desc(desc, START_INDEX);

	if(pstuK)
	{
		append_fifo_load(desc, virt_to_phys(pstuK->auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeP.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	if(pPointA)
	{
		append_fifo_load(desc, virt_to_phys(pPointA->stuQx.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
		append_fifo_load(desc, virt_to_phys(pPointA->stuQy.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3 | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pPointB)
	{
		append_fifo_load(desc, virt_to_phys(pPointB->stuQx.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B1 | FIFOLDST_SGF | FIFOLD_IMM);
		append_fifo_load(desc, virt_to_phys(pPointB->stuQy.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B2 | FIFOLDST_SGF | FIFOLD_IMM);
	}
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | mode);
	append_fifo_store(desc, virt_to_phys(pPointC->stuQx.auValue), ECC_BIGINT32_MAXLEN, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B1 | FIFOST_CLASS_SWAP);
	append_fifo_store(desc, virt_to_phys(pPointC->stuQy.auValue), ECC_BIGINT32_MAXLEN, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B2 | FIFOST_CLASS_SWAP);

	return run_descriptor_jr(desc, &jr_t);
}

SINT8 EccBig32CmpBig(ECC_STU_BIGINT32 *pstuA, ECC_STU_BIGINT32 *pstuB)
{
	int i;

	for(i=0; i<32; i++)
	{
		if(pstuA->auValue[i] < pstuB->auValue[i])
			return -1;
		else if(pstuA->auValue[i] > pstuB->auValue[i])
			return 1;
	}

	return 0;
}


UINT8 EccBig32AllIs0(ECC_STU_BIGINT32 *pstuA)
{
	int i;

	for(i=0; i<32; i++)
	{
		if(pstuA->auValue[i] != 0)
			return 0;
	}

	return 1;
}

UINT8 EccPointIsOnCurve(ECC_STU_PRAMS *pstuEccPrams,
						ECC_STU_PUBKEY *pPointA)
{
	ECC_STU_BIGINT32 t[3];
	int ret = 0;

	ret = PkhaModularMath(pPointA->stuQy.auValue, pPointA->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQx.auValue, pPointA->stuQx.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQx.auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQx.auValue, pstuEccPrams->stuCoefficientA.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[1].auValue, t[2].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[1].auValue, pstuEccPrams->stuCoefficientB.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;

	return EccBig32AllIs0(&t[0]);

err:
	return 0xff;
}

UINT8 Equal_Zero(ECC_STU_BIGINT48 *pstuA)
{
	int i;

	for(i=0; i<48; i++)
	{
		if(pstuA->auValue[i] != 0)
			return 0;
	}

	return 1;
}

void Sub_Byte(ECC_STU_BIGINT48 *pstuA,SINT8 naf_value)
{
	UINT8 te_va;
	UINT8  i;

	te_va = pstuA->auValue[47];

	pstuA->auValue[47] = pstuA->auValue[47] - naf_value;

	//减掉的值为正数，可能需要借位
	if(naf_value>0)
	{

		for(i=47; i>0; i=i-1)
		{
			if(pstuA->auValue[i] >  te_va)
			{
				te_va = pstuA->auValue[i-1];
				pstuA->auValue[i-1] = pstuA->auValue[i-1] - 0x1;
			}
			else
			{
				break;
			}
		}

	}

	//减掉的值为负数，可能需要进位
	else
	{
		for(i=47; i>0; i=i-1)
		{
			if(pstuA->auValue[i] <  te_va)
			{
				te_va = pstuA->auValue[i-1];
				pstuA->auValue[i-1] = pstuA->auValue[i-1] + 0x1;
			}
			else
			{
				break;
			}
		}
	}
}

int Right2(ECC_STU_BIGINT48 *pstuA)
{
	UINT32 desc[MAX_CAAM_DESCSIZE];
	init_job_desc(desc, START_INDEX);

	append_fifo_load(desc, virt_to_phys(pstuA->auValue), 48, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_RIGHT_SHIFT_A);
	append_fifo_store(desc, virt_to_phys(pstuA->auValue), 48, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_A | FIFOST_CLASS_SWAP);

	return run_descriptor_jr(desc, &jr_t);
}

UINT16 NAF_W_Rand(ECC_STU_BIGINT48 *pstuA, SINT8 *naf_w)
{
	UINT16 i;
	UINT8 w=1;
	UINT8 rand_l;

	//随机选取l，小于10并且大于3
	GetRandomArray(&rand_l, 1);
	rand_l = (rand_l & 7) + 3;

	//rand_l = 3;

	i = 0;

	while(!Equal_Zero(pstuA))
	{
		if(i%rand_l == 0x0) //判断i 是否等于l,2l,3l,...ml,如果相等则重新选取w；
		{
			//w = (GetRandomWord()&0x3) + 1; //w 只能选择2，3，4
			do{
				GetRandomArray(&w, 1);
				w = (w & 3) + 1;

			}while(w==1); // w 等于1时，重新选择
		}

		if( (pstuA->auValue[47] & 0x1) == 0x1) //k 是基数
		{
			if(w==2) //滑动窗口为2
			{
				naf_w[i] = (pstuA->auValue[47]&0x3); //ki = k mod 2^w
				if(naf_w[i] > 2) //ki > 2^ w-1
				{
					naf_w[i] = naf_w[i] - 4;  // ki = ki - 2^w
				}
				Sub_Byte(pstuA,naf_w[i]); //k = k - ki
			}
			else if(w==3) //滑动窗口为2
			{
				naf_w[i] = (pstuA->auValue[47]&0x7); //ki = k mod 2^w
				if(naf_w[i] > 4) //ki > 2^ w-1
				{
					naf_w[i] = naf_w[i] - 8;  // ki = ki - 2^w
				}
				Sub_Byte(pstuA,naf_w[i]); //k = k - ki
			}
			else if(w==4) //滑动窗口为2
			{
				naf_w[i] = (pstuA->auValue[47]&0xf); //ki = k mod 2^w
				if(naf_w[i] > 8) //ki > 2^ w-1
				{
					naf_w[i] = naf_w[i] - 16;  // ki = ki - 2^w
				}
				Sub_Byte(pstuA,naf_w[i]); //k = k - ki
			}
			else
				asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));

		}
		else //k 是偶数
		{
			naf_w[i] = 0x0;

		}

		// k=k/2，i++；
		Right2(pstuA);
		i++;
	}

	return i;
}

int EccJacobianPointDbl(ECC_STU_PRAMS *pstuEccPrams,
						ECC_STU_JACOBIAN *pPointA,
						ECC_STU_JACOBIAN *pPointC)
{
	ECC_STU_BIGINT32 t[4];
	int ret = 0;
/*
	//t0 = ay^2
	ret = PkhaModularMath(pPointA->stuQy.auValue, pPointA->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	//t1 = ax^2
	ret = PkhaModularMath(pPointA->stuQx.auValue, pPointA->stuQx.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	//t2 = 2t1
	ret = PkhaModularMath(t[1].auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	//t1 = t1+t2
	ret = PkhaModularMath(t[1].auValue, t[2].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	//t3 = ax*t0
	ret = PkhaModularMath(pPointA->stuQx.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[3].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	//t3 = 2t3
	ret = PkhaModularMath(t[3].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[3].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	//t3 = 2t3
	ret = PkhaModularMath(t[3].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[3].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	//rx = t1^2
	ret = PkhaModularMath(t[1].auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	//t2 = 2t3
	ret = PkhaModularMath(t[3].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	//rx = rx-t2
	ret = PkhaModularMath(pPointC->stuQx.auValue, t[2].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	//rz = ay*az
	ret = PkhaModularMath(pPointA->stuQy.auValue, pPointA->stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	//rz = 2rz
	ret = PkhaModularMath(pPointC->stuQz.auValue, pPointC->stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQz.auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	//t0 = 2t0
	ret = PkhaModularMath(t[0].auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	//t4 = t0
	memcpy_c(t[4].auValue, t[0].auValue, ECC_BIGINT32_MAXLEN);
	//t0 = t0^2
	ret = PkhaModularMath(t[0].auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	//t0 = 2t0
	ret = PkhaModularMath(t[0].auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	//t3 = t3-rx
	ret = PkhaModularMath(t[3].auValue, pPointC->stuQx.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[3].auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	//ry = t1*t3
	ret = PkhaModularMath(t[1].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	//ry = ry - t0
	ret = PkhaModularMath(pPointC->stuQy.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
*/
	/* n1 = 3 * X_a^2 + a_curve * Z_a^4 */
	ret = PkhaModularMath(pPointA->stuQx.auValue, pPointA->stuQx.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQz.auValue, pPointA->stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[1].auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[1].auValue, pstuEccPrams->stuCoefficientA.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	/* Z_r = 2 * Y_a * Z_a */
	ret = PkhaModularMath(pPointA->stuQy.auValue, pPointA->stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQz.auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	/* n2 = 4 * X_a * Y_a^2 */
	ret = PkhaModularMath(pPointA->stuQy.auValue, pPointA->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[3].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQx.auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[2].auValue, t[2].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[2].auValue, t[2].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	/* X_r = n1^2 - 2 * n2 */
	ret = PkhaModularMath(t[2].auValue, t[2].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[1].auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointC->stuQx.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	/* n3 = 8 * Y_a^4 */
	ret = PkhaModularMath(t[3].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[3].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[3].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[3].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[3].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[3].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	/* Y_r = n1 * (n2 - X_r) - n3 */
	ret = PkhaModularMath(t[2].auValue, pPointC->stuQx.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;

err:
	return ret;

}

int EccJacobianPointAdd(ECC_STU_PRAMS *pstuEccPrams,
						ECC_STU_JACOBIAN *pPointA,
						ECC_STU_JACOBIAN *pPointB,
						ECC_STU_JACOBIAN *pPointC)
{
	ECC_STU_BIGINT32 t[7];
	int ret = 0, i;
/*
	ret = PkhaModularMath(pPointA->stuQz.auValue, pPointA->stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointB->stuQx.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQz.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointB->stuQy.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[1].auValue, pPointA->stuQx.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[2].auValue, pPointA->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[3].auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQz.auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[1].auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[1].auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQx.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[3].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointC->stuQx.auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointC->stuQx.auValue, t[2].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, pPointC->stuQx.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQy.auValue, t[2].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[2].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
*/
	/* n1 = X_a * Z_b^2 */
	ret = PkhaModularMath(pPointB->stuQz.auValue, pPointB->stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQx.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	/* n2 = Y_a * Z_b^3 */
	ret = PkhaModularMath(pPointB->stuQz.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQy.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	/* n3 = X_b * Z_a^2 */
	ret = PkhaModularMath(pPointA->stuQz.auValue, pPointA->stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointB->stuQx.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[3].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	/* n4 = Y_b * Z_a^3 */
	ret = PkhaModularMath(pPointA->stuQz.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointB->stuQy.auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[4].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	/* n5 = n1 - n3 */
	ret = PkhaModularMath(t[1].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[5].auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	/* n6 = n2 - n4 */
	ret = PkhaModularMath(t[2].auValue, t[4].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[6].auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	/*
	 * n5 = n6 = 0,A = B
	 * n5 = 0,n6 != 0, A+B=0
	 */
	/* 'n7' = n1 + n3 */
	ret = PkhaModularMath(t[1].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	/* 'n8' = n2 + n4 */
	ret = PkhaModularMath(t[2].auValue, t[4].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	/* Z_r = Z_a * Z_b * n5 */
	ret = PkhaModularMath(pPointA->stuQz.auValue, pPointB->stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[5].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	/* X_r = n6^2 - n5^2 * 'n7' */
	ret = PkhaModularMath(t[6].auValue, t[6].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[5].auValue, t[5].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[4].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[1].auValue, t[4].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[3].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[3].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	/* n9 = n5^2 * 'n7' - 2 * X_r */
	ret = PkhaModularMath(pPointC->stuQx.auValue, pPointC->stuQx.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[3].auValue, t[0].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	/* Y_r = (n6 * 'n9' - 'n8' * 'n5^3') / 2 */
	ret = PkhaModularMath(t[0].auValue, t[6].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[4].auValue, t[5].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[5].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[2].auValue, t[5].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
/*
	if(t[0].auValue[ECC_BIGINT32_MAXLEN-1] & (UINT8)1)
	{
		ret = PkhaModularMath(t[0].auValue, pstuEccPrams->stuPrimeP.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[0].auValue, OP_ALG_PKMODE_MOD_ADD);
		if(ret)
			goto err;
	}
	ret = PkhaModularMath(NULL, t[0].auValue, NULL, NULL, pPointC->stuQy.auValue, OP_ALG_PKMODE_RIGHT_SHIFT_B);
	if(ret)
		goto err;
*/
	ret = PkhaModularMath(NULL, NULL, NULL, pstuEccPrams->stuPrimeP.auValue, t[1].auValue, OP_ALG_PKMODE_MOD_MONT_CNST);
	if(ret)
		goto err;
	for(i=0; i<ECC_BIGINT32_MAXLEN-1; i++)
		t[2].auValue[i] = 0;
	t[2].auValue[ECC_BIGINT32_MAXLEN-1] = 2;
	ret = PkhaModularMath(t[2].auValue, NULL, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_INV);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[2].auValue, t[1].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, t[2].auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(t[0].auValue, t[2].auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
err:
	return ret;
}

int EccBlindKey(ECC_STU_PRAMS *pstuEccPrams,
				ECC_STU_BIGINT32 *pstuK,
				ECC_STU_BIGINT48 *pstuNewK)
{
	int i;
	ECC_STU_BIGINT48 stuM;
	UINT32 desc[MAX_CAAM_DESCSIZE], size=48;

	for(i=0; i<48; i++)
	{
		stuM.auValue[i] = 0xff;
	}

	init_job_desc(desc, START_INDEX);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc, 16);
	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI);
	append_seq_fifo_store(desc, 16, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGFIFO);
	append_move(desc, MOVE_WAITCOMP | MOVE_SRC_OUTFIFO | MOVE_DEST_PK_A | 16);

	append_fifo_load(desc, virt_to_phys(stuM.auValue), 48, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), 32, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_load_as_imm(desc, &size, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_A_SZ);
	append_load_as_imm(desc, &size, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_fifo_load(desc, virt_to_phys(pstuK->auValue), 32, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_load_as_imm(desc, &size, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_A_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);
	append_fifo_store(desc, virt_to_phys(pstuNewK->auValue), 48, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	return run_descriptor_jr(desc, &jr_t);
}

int EccPointDbl(ECC_STU_PRAMS *pstuEccPrams,
				ECC_STU_PUBKEY *pPointA,
				ECC_STU_PUBKEY *pPointC)
{
	int ret = 0, i;
	ECC_STU_BIGINT32 stuR2, stuOne;
	ECC_STU_JACOBIAN stuPointA, stuPointC;

	for(i=0; i<ECC_BIGINT32_MAXLEN; i++)
		stuOne.auValue[i] = 0;
	stuOne.auValue[ECC_BIGINT32_MAXLEN-1] = 1;

	//montogmery convert
	ret = PkhaModularMath(NULL, NULL, NULL, pstuEccPrams->stuPrimeP.auValue, stuR2.auValue, OP_ALG_PKMODE_MOD_MONT_CNST);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQx.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQy.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuOne.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pstuEccPrams->stuCoefficientA.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pstuEccPrams->stuCoefficientA.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	ret = EccJacobianPointDbl(pstuEccPrams, &stuPointA, &stuPointC);
	if(ret)
		goto err;

	//montogmery convert
	ret = PkhaModularMath(stuPointC.stuQx.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQy.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pstuEccPrams->stuCoefficientA.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pstuEccPrams->stuCoefficientA.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	//Jocabian convert
	ret = PkhaModularMath(stuPointC.stuQz.auValue, NULL, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQz.auValue, OP_ALG_PKMODE_MOD_INV);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, stuPointC.stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQx.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQy.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

err:
	return ret;
}

int EccPointAdd(ECC_STU_PRAMS *pstuEccPrams,
				ECC_STU_PUBKEY *pPointA,
				ECC_STU_PUBKEY *pPointB,
				ECC_STU_PUBKEY *pPointC)
{
	int ret = 0, i;
	ECC_STU_BIGINT32 stuR2, stuOne;
	ECC_STU_JACOBIAN stuPointA, stuPointB, stuPointC;

	for(i=0; i<ECC_BIGINT32_MAXLEN; i++)
		stuOne.auValue[i] = 0;
	stuOne.auValue[ECC_BIGINT32_MAXLEN-1] = 1;

	//montogmery convert
	ret = PkhaModularMath(NULL, NULL, NULL, pstuEccPrams->stuPrimeP.auValue, stuR2.auValue, OP_ALG_PKMODE_MOD_MONT_CNST);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQx.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQy.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuOne.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointB->stuQx.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointB.stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointB->stuQy.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointB.stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuOne.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointB.stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	ret = EccJacobianPointAdd(pstuEccPrams, &stuPointA, &stuPointB, &stuPointC);
	if(ret)
		goto err;

	//montogmery convert
	ret = PkhaModularMath(stuPointC.stuQx.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQy.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	//Jocabian convert
	ret = PkhaModularMath(stuPointC.stuQz.auValue, NULL, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQz.auValue, OP_ALG_PKMODE_MOD_INV);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, stuPointC.stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQx.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQy.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

err:
	return ret;
}

int EccPointMul(ECC_STU_PRAMS *pstuEccPrams,
				ECC_STU_BIGINT32 *pstuK,
				ECC_STU_PUBKEY *pPointA,
				ECC_STU_PUBKEY *pPointC)
{
	int ret = 0;
	SINT16 sNum = EccBigFf1(pstuK, ECC_BIGINT32_MAXLEN), i;
	ECC_STU_BIGINT32 stuR2, stuOne;
	ECC_STU_JACOBIAN stuPointA, stuPointC;

	for(i=0; i<ECC_BIGINT32_MAXLEN; i++)
		stuOne.auValue[i] = 0;
	stuOne.auValue[ECC_BIGINT32_MAXLEN-1] = 1;

	//montogmery convert
	ret = PkhaModularMath(NULL, NULL, NULL, pstuEccPrams->stuPrimeP.auValue, stuR2.auValue, OP_ALG_PKMODE_MOD_MONT_CNST);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQx.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pPointA->stuQy.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuOne.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pstuEccPrams->stuCoefficientA.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pstuEccPrams->stuCoefficientA.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	memcpy_c(&stuPointC, &stuPointA, 3*ECC_BIGINT32_MAXLEN);

	//calculate point mult
	for(i=sNum-1; i>=0; i--)
	{
		ret = EccJacobianPointDbl(pstuEccPrams, &stuPointC, &stuPointC);
		if(ret)
			goto err;
		if(EccBigIsBitSet(pstuK, i, ECC_BIGINT32_MAXLEN))
		{
			ret = EccJacobianPointAdd(pstuEccPrams, &stuPointA, &stuPointC, &stuPointC);
			if(ret)
				goto err;
		}
	}

	//montogmery convert
	ret = PkhaModularMath(stuPointC.stuQx.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQy.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pstuEccPrams->stuCoefficientA.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pstuEccPrams->stuCoefficientA.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	//Jocabian convert
	ret = PkhaModularMath(stuPointC.stuQz.auValue, NULL, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQz.auValue, OP_ALG_PKMODE_MOD_INV);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, stuPointC.stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQx.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQy.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

err:
	return ret;
}

int EccPointMulV2(ECC_STU_PRAMS *pstuEccPrams,
				ECC_STU_BIGINT32 *pstuK,
				ECC_STU_PUBKEY *pPointA,
				ECC_STU_PUBKEY *pPointC)
{
	int ret = 0;
	SINT16 sNum = EccBigFf1(pstuK, ECC_BIGINT32_MAXLEN), i;
	ECC_STU_BIGINT32 stuR2, stuOne;
	ECC_STU_BIGINT48 stuK;
	ECC_STU_JACOBIAN stuPointA, stuPointC;

	for(i=0; i<ECC_BIGINT32_MAXLEN; i++)
		stuOne.auValue[i] = 0;
	stuOne.auValue[ECC_BIGINT32_MAXLEN-1] = 1;

	//Jocabian convert
	ret = GetRandomArray(stuPointA.stuQz.auValue, ECC_BIGINT32_MAXLEN);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQz.auValue, stuPointA.stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQx.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQx.auValue, stuPointA.stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQx.auValue, pPointA->stuQx.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQx.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQy.auValue, pPointA->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	//montogmery convert
	ret = PkhaModularMath(NULL, NULL, NULL, pstuEccPrams->stuPrimeP.auValue, stuR2.auValue, OP_ALG_PKMODE_MOD_MONT_CNST);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQx.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQy.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQz.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pstuEccPrams->stuCoefficientA.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pstuEccPrams->stuCoefficientA.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	memcpy_c(&stuPointC, &stuPointA, 3*ECC_BIGINT32_MAXLEN);

	//blind private key
	ret = EccBlindKey(pstuEccPrams, pstuK, &stuK);
	sNum = EccBigFf1(&stuK, 48);

	//calculate point mult
	for(i=sNum-1; i>=0; i--)
	{
		ret = EccJacobianPointDbl(pstuEccPrams, &stuPointC, &stuPointC);
		if(ret)
			goto err;
		if(EccBigIsBitSet(&stuK, i, 48))
		{
			ret = EccJacobianPointAdd(pstuEccPrams, &stuPointA, &stuPointC, &stuPointC);
			if(ret)
				goto err;
		}
	}

	//montogmery convert
	ret = PkhaModularMath(stuPointC.stuQx.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQy.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pstuEccPrams->stuCoefficientA.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pstuEccPrams->stuCoefficientA.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	//Jocabian convert
	ret = PkhaModularMath(stuPointC.stuQz.auValue, NULL, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQz.auValue, OP_ALG_PKMODE_MOD_INV);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, stuPointC.stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQx.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQy.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

err:
	return ret;
}

int EccPointMulV3(ECC_STU_PRAMS *pstuEccPrams,
				ECC_STU_BIGINT32 *pstuK,
				ECC_STU_PUBKEY *pPointA,
				ECC_STU_PUBKEY *pPointC)
{
	int ret = 0;
	SINT8  naf_w[384] = {0x0};
	UINT8 hash[SM3_DIGEST_LEN];
	SINT16 sNum = EccBigFf1(pstuK, ECC_BIGINT32_MAXLEN), i;
	ECC_STU_BIGINT32 stuR2, stuOne;
	ECC_STU_BIGINT48 stuK;
	ECC_STU_JACOBIAN stuPointA, stuPointNegA, stuPointC, stuPoint3A, stuPoint5A, stuPoint7A, stuPointNeg3A, stuPointNeg5A, stuPointNeg7A;

	ret = SM3Digest(pstuEccPrams->stuPrimeP.auValue, ECC_BIGINT32_MAXLEN*6, hash);
	if(ret)
		goto err;

	ret = memcmp_c(hash, SM2ParametresHash, SM3_DIGEST_LEN);
	if(ret)
		goto err;

	if(EccPointIsOnCurve(pstuEccPrams, pPointA) != 1)
	{
		ret = 0xff;
		goto err;
	}

	for(i=0; i<ECC_BIGINT32_MAXLEN; i++)
		stuOne.auValue[i] = 0;
	stuOne.auValue[ECC_BIGINT32_MAXLEN-1] = 1;

	//Jocabian convert
	ret = GetRandomArray(stuPointA.stuQz.auValue, ECC_BIGINT32_MAXLEN);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQz.auValue, stuPointA.stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQx.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQx.auValue, stuPointA.stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQx.auValue, pPointA->stuQx.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQx.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQy.auValue, pPointA->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	//montogmery convert
	ret = PkhaModularMath(NULL, NULL, NULL, pstuEccPrams->stuPrimeP.auValue, stuR2.auValue, OP_ALG_PKMODE_MOD_MONT_CNST);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQx.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQy.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointA.stuQz.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointA.stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pstuEccPrams->stuCoefficientA.auValue, stuR2.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pstuEccPrams->stuCoefficientA.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	//blind private key
	ret = EccBlindKey(pstuEccPrams, pstuK, &stuK);
	if(ret)
		goto err;

	//calculate point mult
	//pre-caculate 3A,5A,7A,-A,-3A,-5A,-7A
	ret = EccJacobianPointDbl(pstuEccPrams, &stuPointA, &stuPointC);
	if(ret)
		goto err;
	ret = EccJacobianPointAdd(pstuEccPrams, &stuPointA, &stuPointC, &stuPoint3A);
	if(ret)
		goto err;
	ret = EccJacobianPointAdd(pstuEccPrams, &stuPoint3A, &stuPointC, &stuPoint5A);
	if(ret)
		goto err;
	ret = EccJacobianPointAdd(pstuEccPrams, &stuPoint5A, &stuPointC, &stuPoint7A);
	if(ret)
		goto err;
	memcpy_c(&stuPointNegA, &stuPointA, 3*ECC_BIGINT32_MAXLEN);
	memcpy_c(&stuPointNeg3A, &stuPoint3A, 3*ECC_BIGINT32_MAXLEN);
	memcpy_c(&stuPointNeg5A, &stuPoint5A, 3*ECC_BIGINT32_MAXLEN);
	memcpy_c(&stuPointNeg7A, &stuPoint7A, 3*ECC_BIGINT32_MAXLEN);
	ret = PkhaModularMath(pstuEccPrams->stuPrimeP.auValue, stuPointNegA.stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointNegA.stuQy.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	ret = PkhaModularMath(pstuEccPrams->stuPrimeP.auValue, stuPointNeg3A.stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointNeg3A.stuQy.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	ret = PkhaModularMath(pstuEccPrams->stuPrimeP.auValue, stuPointNeg5A.stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointNeg5A.stuQy.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	ret = PkhaModularMath(pstuEccPrams->stuPrimeP.auValue, stuPointNeg7A.stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointNeg7A.stuQy.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;

	sNum = NAF_W_Rand(&stuK,naf_w);

	if(naf_w[sNum-1] == 0x1)
	{
		memcpy_c(&stuPointC, &stuPointA, 3*ECC_BIGINT32_MAXLEN);
	}
	else if(naf_w[sNum-1] == 0x3)
	{
		memcpy_c(&stuPointC, &stuPoint3A, 3*ECC_BIGINT32_MAXLEN);
	}
	else if(naf_w[sNum-1] == 0x5)
	{
		memcpy_c(&stuPointC, &stuPoint5A, 3*ECC_BIGINT32_MAXLEN);
	}
	else if(naf_w[sNum-1] == 0x7)
	{
		memcpy_c(&stuPointC, &stuPoint7A, 3*ECC_BIGINT32_MAXLEN);
	}
	else
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));

	for(i=sNum-2;i>=0;i--)
	{
		ret = EccJacobianPointDbl(pstuEccPrams, &stuPointC, &stuPointC);
		if(ret)
			goto err;
		if(naf_w[i] == 0x1)
		{
			ret = EccJacobianPointAdd(pstuEccPrams, &stuPointA, &stuPointC, &stuPointC);
			if(ret)
				goto err;
		}
		else if(naf_w[i] == -1)
		{
			ret = EccJacobianPointAdd(pstuEccPrams, &stuPointNegA, &stuPointC, &stuPointC);
			if(ret)
				goto err;
		}
		else if(naf_w[i] == 3)
		{
			ret = EccJacobianPointAdd(pstuEccPrams, &stuPoint3A, &stuPointC, &stuPointC);
			if(ret)
				goto err;
		}
		else if(naf_w[i] == -3)
		{
			ret = EccJacobianPointAdd(pstuEccPrams, &stuPointNeg3A, &stuPointC, &stuPointC);
			if(ret)
				goto err;
		}
		else if(naf_w[i] == 5)
		{
			ret = EccJacobianPointAdd(pstuEccPrams, &stuPoint5A, &stuPointC, &stuPointC);
			if(ret)
				goto err;
		}
		else if(naf_w[i] == -5)
		{
			ret = EccJacobianPointAdd(pstuEccPrams, &stuPointNeg5A, &stuPointC, &stuPointC);
			if(ret)
				goto err;
		}
		else if(naf_w[i] == 7)
		{
			ret = EccJacobianPointAdd(pstuEccPrams, &stuPoint7A, &stuPointC, &stuPointC);
			if(ret)
				goto err;
		}
		else if(naf_w[i] == -7)
		{
			ret = EccJacobianPointAdd(pstuEccPrams, &stuPointNeg7A, &stuPointC, &stuPointC);
			if(ret)
				goto err;
		}
		else if(naf_w[i] != 0)
			asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	}

	//montogmery convert
	ret = PkhaModularMath(stuPointC.stuQx.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQx.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQy.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQy.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQz.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(pstuEccPrams->stuCoefficientA.auValue, stuOne.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pstuEccPrams->stuCoefficientA.auValue, OP_ALG_PKMODE_MOD_IN_MONTY | OP_ALG_PKMODE_MOD_OUT_MONTY | OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	//Jocabian convert
	ret = PkhaModularMath(stuPointC.stuQz.auValue, NULL, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointC.stuQz.auValue, OP_ALG_PKMODE_MOD_INV);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, stuPointC.stuQz.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQx.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQx.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQz.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuPointC.stuQy.auValue, pPointC->stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, pPointC->stuQy.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;

	if(EccPointIsOnCurve(pstuEccPrams, pPointC) != 1)
		ret = 0xff;

err:
	return ret;
}

int SM3Digest(UINT8 *pData, UINT32 len, UINT8 *pHash)
{
	UINT32 desc[MAX_CAAM_DESCSIZE];
	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc, virt_to_phys(pData), len, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_store(desc, virt_to_phys(pHash), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);
	return run_descriptor_jr(desc, &jr_t);
}

int KDFXORHash(ECC_STU_PUBKEY *pstuP, UINT16 klen, UINT8 *mes, UINT8 *c2, UINT8 *c3, UINT8 enc)
{
	UINT8 msg[ECC_BIGINT32_MAXLEN*2+4], t[SM2_MAX_MESSAGE_LEN];;
	UINT32 desc[MAX_CAAM_DESCSIZE];
	UINT32 ct[4] = {
			0x1, 0x2, 0x3, 0x4
	};
	UINT8 i, rand_l;
	int ret = 0;

	memcpy_c(msg, pstuP->stuQx.auValue, ECC_BIGINT32_MAXLEN*2);

	for(i=0; i<4; i++)
	{
		if(i == 3)
			rand_l = 0;
		else
		{
			ret = GetRandomArray(&rand_l, 1);
			if(ret)
				goto err;

			rand_l %= (4-i);
		}

		memcpy_c(msg+ECC_BIGINT32_MAXLEN*2, &ct[rand_l], 4);

		ret = SM3Digest(msg, ECC_BIGINT32_MAXLEN*2+4, t+(ct[rand_l]-1)*SM3_DIGEST_LEN);
		if(ret)
			goto err;

		if(rand_l != 3-i)
			memcpy_c(&ct[rand_l], &ct[rand_l+1], (3-i-rand_l)*4);
	}

	if(enc)
	{
		init_job_desc(desc, START_INDEX);
		append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
		append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
		append_fifo_load(desc, virt_to_phys(t), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD | OP_ALG_PKMODE_MOD_F2M);
		append_fifo_store(desc, virt_to_phys(c2), klen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
	}
	else
	{
		init_job_desc(desc, START_INDEX);
		append_fifo_load(desc, virt_to_phys(c2), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
		append_fifo_load(desc, virt_to_phys(c2), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
		append_fifo_load(desc, virt_to_phys(t), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD | OP_ALG_PKMODE_MOD_F2M);
		append_fifo_store(desc, virt_to_phys(mes), klen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
	}
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc, virt_to_phys(pstuP->stuQx.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuP->stuQy.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_store(desc, virt_to_phys(c3), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);

	ret = run_descriptor_jr(desc, &jr_t);
err:
	return ret;
}

int SM2CalculateZ(		UINT8 *IDA,
		UINT16 entla,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		UINT8 *z)
{
	UINT32 desc[MAX_CAAM_DESCSIZE];

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc, virt_to_phys(&entla), 2, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(IDA), entla/8, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGx.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGy.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQx.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQy.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_store(desc, virt_to_phys(z), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);
	return run_descriptor_jr(desc, &jr_t);
}

int SM2CalculateE(UINT8 *mes,
		UINT16 klen,
		UINT8 *IDA,
		UINT16 entla,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		UINT8 *e)
{
	UINT32 desc[MAX_CAAM_DESCSIZE];

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc, virt_to_phys(&entla), 2, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(IDA), entla/8, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGx.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGy.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQx.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQy.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);

	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x40000);
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_move(desc, MOVE_SRC_CLASS2CTX | MOVE_DEST_CLASS2INFIFO | SM3_DIGEST_LEN);
	append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_store(desc, virt_to_phys(e), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);
	return run_descriptor_jr(desc, &jr_t);
}

int SM2CalculateK(ECC_STU_PUBKEY *stuP, UINT32 *za, UINT32 *zb, UINT32 *pAgreedKey, UINT32 AgreedKeyLen)
{
	UINT32 ct = 1;
	UINT32 desc[MAX_CAAM_DESCSIZE];

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc, virt_to_phys(stuP->stuQx.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(stuP->stuQy.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(za), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(zb), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(&ct), 4, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_store(desc, virt_to_phys(pAgreedKey), AgreedKeyLen, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);
	return run_descriptor_jr(desc, &jr_t);
}

int SM2CalculateS(ECC_STU_PUBKEY *stuP, UINT32 *za, UINT32 *zb, ECC_STU_PUBKEY *pstuPubKeyA, ECC_STU_PUBKEY *pstuPubKeyB, UINT32 *sA, UINT32 *sB)
{
	UINT8 h0=0x2, h1=0x3;
	UINT8 tempHash[32];

	UINT32 desc[MAX_CAAM_DESCSIZE];

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_CLASS2_ALG|OP_ALG_ALGSEL_SM3|OP_ALG_AS_INITFINAL);
	append_fifo_load(desc, virt_to_phys(stuP->stuQx.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(za), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(zb), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuPubKeyA->stuQx.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuPubKeyA->stuQy.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuPubKeyB->stuQx.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(pstuPubKeyB->stuQy.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_store(desc, virt_to_phys(tempHash), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);

	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x40000);
	append_fifo_load(desc, virt_to_phys(&h0), 1, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(stuP->stuQy.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(tempHash), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_store(desc, virt_to_phys(sB), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);

	append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_CLRW|4);
	append_cmd(desc, 0x40000);
	append_fifo_load(desc, virt_to_phys(&h1), 1, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(stuP->stuQy.auValue), ECC_BIGINT32_MAXLEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG);
	append_fifo_load(desc, virt_to_phys(tempHash), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS2|FIFOLD_TYPE_MSG|FIFOLD_TYPE_LAST2);
	append_store(desc, virt_to_phys(sA), SM3_DIGEST_LEN, LDST_CLASS_2_CCB|LDST_SRCDST_BYTE_CONTEXT);

	return run_descriptor_jr(desc, &jr_t);
}

int SM2Init(ECC_STU_PRAMS *pstuEccPrams)
{
	pstuEccPrams->uField = 0;
	pstuEccPrams->uBytes = ECC_BIGINT32_MAXLEN;
	memcpy_c(pstuEccPrams->stuPrimeP.auValue, SM2Parametres, 224);

	return SM3Digest(pstuEccPrams->stuPrimeP.auValue, ECC_BIGINT32_MAXLEN*6, SM2ParametresHash);
}

int  EccGenerateKeyPairDpa(ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_PUBKEY *pstuPubKey)
{
	int ret = 0;
	ECC_STU_BIGINT32 stuK;

	do{
		ret = GetRandomArray(pstuPrivKey->auValue, ECC_BIGINT32_MAXLEN);
		if(ret)
			goto err;
		ret = PkhaModularMath(pstuPrivKey->auValue, NULL, NULL, pstuEccPrams->stuPrimeN.auValue, pstuPrivKey->auValue, OP_ALG_PKMODE_MOD_REDUCT);
		if(ret)
			goto err;
	}while(EccBig32AllIs0(pstuPrivKey));

#if 0
	unsigned int private_key[8] = {
			0x1649AB77, 0xA00637BD, 0x5E2EFE28, 0x3FBF3535,
			0x34AA7F7C, 0xB89463F2, 0x08DDBC29, 0x20BB0DA0
	};
	memcpy_c(pstuPrivKey->auValue, private_key, 32);
#endif

	ret = EccPointMulV3(pstuEccPrams, pstuPrivKey, (ECC_STU_PUBKEY *)(&pstuEccPrams->stuGx), pstuPubKey);

err:
	return ret;
}

int SM2EncryptDpa(UINT8 *mes,
		UINT16 klen,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_PRAMS *pstuEccPrams,
		UINT8 *cipher0,
		UINT8 *cipher1,
		UINT8 *cipher2)
{
	ECC_STU_BIGINT32 stuK;
	ECC_STU_PUBKEY stuP1, stuP2;
	int ret = 0;

	do{
		ret = GetRandomArray(stuK.auValue, ECC_BIGINT32_MAXLEN);
		if(ret)
			goto err;
		ret = PkhaModularMath(stuK.auValue, NULL, NULL, pstuEccPrams->stuPrimeN.auValue, stuK.auValue, OP_ALG_PKMODE_MOD_REDUCT);
		if(ret)
			goto err;
	}while(EccBig32AllIs0(&stuK));

#if 0
	unsigned int rng_data[8] =
	{
			0x4C62EEFD, 0x6ECFC2B9, 0x5B92FD6C, 0x3D957514,
			0x8AFA1742, 0x5546D490, 0x18E5388D, 0x49DD7B4F
	};
	memcpy_c(stuK.auValue, rng_data, 32);
#endif

	ret = EccPointMulV3(pstuEccPrams, &stuK, (ECC_STU_PUBKEY *)(&pstuEccPrams->stuGx), &stuP1);
	if(ret)
		goto err;

	ret = EccPointMulV3(pstuEccPrams, &stuK, pstuPubKey, &stuP2);
	if(ret)
		goto err;

	memcpy_c(cipher0, stuP1.stuQx.auValue, ECC_BIGINT32_MAXLEN*2);

	ret = KDFXORHash(&stuP2, klen, mes, cipher1, cipher2, 1);
	if(ret)
		goto err;

err:
	return ret;
}

int SM2DecryptDpa(UINT8 *cipher0,
		UINT8 *cipher1,
		UINT8 *cipher2,
		UINT16 klen,
		ECC_STU_BIGINT32 *stuPrivKey,
		ECC_STU_PRAMS *pstuEccPrams,
		UINT8 *mes)
{
	int ret = 0;
	ECC_STU_PUBKEY stuP1, stuP2;
	UINT8 u[SM3_DIGEST_LEN];

	memcpy_c(stuP1.stuQx.auValue, cipher0, ECC_BIGINT32_MAXLEN*2);

	ret = EccPointMulV3(pstuEccPrams, stuPrivKey, &stuP1, &stuP2);
	if(ret)
		goto err;

	ret = KDFXORHash(&stuP2, klen, mes, cipher1, u, 0);
	if(ret)
		goto err;

	ret = memcmp_c(u, cipher2, SM3_DIGEST_LEN);
err:
	return ret;
}

int EccSignatureDpa(UINT8 *mes,
		UINT16 klen,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS)
{
	ECC_STU_BIGINT32 stuK, stuR, stuT, stuK1;
	ECC_STU_PUBKEY stuPointR, stuPointS, stuPointQ, stuPointG, stuP1;
	int ret = 0;

nextA:
	do{
		ret = GetRandomArray(stuK.auValue, ECC_BIGINT32_MAXLEN);
		if(ret)
			goto err;
		ret = PkhaModularMath(stuK.auValue, NULL, NULL, pstuEccPrams->stuPrimeN.auValue, stuK.auValue, OP_ALG_PKMODE_MOD_REDUCT);
		if(ret)
			goto err;
	}while(EccBig32AllIs0(&stuK));

#if 0
	unsigned int k_standard[8] = {
			0x6CB28D99, 0x385C175C, 0x94F94E93, 0x4817663F,
			0xC176D925, 0xDD72B727, 0x260DBAAE, 0x1FB2F96F
	};
	memcpy_c(stuK.auValue, k_standard, 32);
#endif

	do{
		ret = GetRandomArray(stuR.auValue, ECC_BIGINT32_MAXLEN);
		if(ret)
			goto err;
		ret = PkhaModularMath(stuR.auValue, NULL, NULL, pstuEccPrams->stuPrimeN.auValue, stuR.auValue, OP_ALG_PKMODE_MOD_REDUCT);
		if(ret)
			goto err;
	}while(EccBig32AllIs0(&stuR));

	ret = PkhaEccMath(pstuEccPrams, (ECC_STU_PUBKEY *)(&pstuEccPrams->stuGx), NULL, &stuR, &stuPointR, OP_ALG_PKMODE_MOD_ECC_MULT | OP_ALG_PKMODE_TIME_EQ);
	if(ret)
		goto err;
	ret = PkhaEccMath(pstuEccPrams, (ECC_STU_PUBKEY *)(&pstuEccPrams->stuGx), &stuPointR, NULL, &stuPointG, OP_ALG_PKMODE_MOD_ECC_ADD);
	if(ret)
		goto err;
	ret = PkhaEccMath(pstuEccPrams, &stuPointR, NULL, &stuK, &stuPointS, OP_ALG_PKMODE_MOD_ECC_MULT | OP_ALG_PKMODE_TIME_EQ);
	if(ret)
		goto err;
	ret = PkhaModularMath(pstuEccPrams->stuPrimeP.auValue, stuPointS.stuQy.auValue, NULL, pstuEccPrams->stuPrimeP.auValue, stuPointS.stuQy.auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;
	ret = EccPointMulV3(pstuEccPrams, &stuK, &stuPointG, &stuPointQ);
	if(ret)
		goto err;
	ret = PkhaEccMath(pstuEccPrams, &stuPointQ, &stuPointS, NULL, &stuP1, OP_ALG_PKMODE_MOD_ECC_ADD);
	if(ret)
		goto err;

	ret = PkhaModularMath(mes, NULL, NULL, pstuEccPrams->stuPrimeN.auValue, mes, OP_ALG_PKMODE_MOD_REDUCT);
	if(ret)
		goto err;

	ret = PkhaModularMath(mes, stuP1.stuQx.auValue, NULL, pstuEccPrams->stuPrimeN.auValue, pSignatureR->auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	if(EccBig32AllIs0(pSignatureR))
		goto nextA;

	ret = PkhaModularMath(pSignatureR->auValue, stuK.auValue, NULL, pstuEccPrams->stuPrimeN.auValue, stuR.auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	if(EccBig32AllIs0(&stuR))
		goto nextA;

	do{
		ret = GetRandomArray(stuK1.auValue, ECC_BIGINT32_MAXLEN);
		if(ret)
			goto err;
		ret = PkhaModularMath(stuK1.auValue, NULL, NULL, pstuEccPrams->stuPrimeN.auValue, stuK1.auValue, OP_ALG_PKMODE_MOD_REDUCT);
		if(ret)
			goto err;
	}while(EccBig32AllIs0(&stuK1));

	ret = PkhaModularMath(pstuPrivKey->auValue, pstuEccPrams->stuCoefficientH.auValue, NULL, pstuEccPrams->stuPrimeN.auValue, stuT.auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuK1.auValue, stuT.auValue, NULL, pstuEccPrams->stuPrimeN.auValue, stuT.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuT.auValue, NULL, NULL, pstuEccPrams->stuPrimeN.auValue, stuT.auValue, OP_ALG_PKMODE_MOD_INV);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuK1.auValue, stuR.auValue, NULL, pstuEccPrams->stuPrimeN.auValue, stuR.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuT.auValue, stuR.auValue, NULL, pstuEccPrams->stuPrimeN.auValue, stuT.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuT.auValue, pSignatureR->auValue, NULL, pstuEccPrams->stuPrimeN.auValue, pSignatureS->auValue, OP_ALG_PKMODE_MOD_SUB_AB);
	if(ret)
		goto err;

	if(EccBig32AllIs0(pSignatureS))
		goto nextA;

err:
	return ret;
}

int EccVerificationDpa(UINT8 *mes,
		UINT16 klen,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS)
{
	int ret = 0;
	ECC_STU_BIGINT32 stuT, stuR;
	ECC_STU_PUBKEY stuP1, stuP2;

	if(EccBig32AllIs0(pSignatureS) || EccBig32AllIs0(pSignatureR))
	{
		ret = 0xff;
		goto err;
	}

	if(EccBig32CmpBig(pSignatureR, &pstuEccPrams->stuPrimeN) != -1)
	{
		ret = 0xff;
		goto err;
	}

	if(EccBig32CmpBig(pSignatureS, &pstuEccPrams->stuPrimeN) != -1)
	{
		ret = 0xff;
		goto err;
	}

	ret = PkhaModularMath(pSignatureR->auValue, pSignatureS->auValue, NULL, pstuEccPrams->stuPrimeN.auValue, stuT.auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;

	ret = EccPointMulV3(pstuEccPrams, pSignatureS, (ECC_STU_PUBKEY *)(&pstuEccPrams->stuGx), &stuP1);
	if(ret)
		goto err;
	ret = EccPointMulV3(pstuEccPrams, &stuT, pstuPubKey, &stuP2);
	if(ret)
		goto err;
	ret = PkhaEccMath(pstuEccPrams, &stuP1, &stuP2, NULL, &stuP1, OP_ALG_PKMODE_MOD_ECC_ADD);
	if(ret)
		goto err;
	ret = PkhaModularMath(mes, NULL, NULL, pstuEccPrams->stuPrimeN.auValue, mes, OP_ALG_PKMODE_MOD_REDUCT);
	if(ret)
		goto err;
	ret = PkhaModularMath(mes, stuP1.stuQx.auValue, NULL, pstuEccPrams->stuPrimeN.auValue, stuR.auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;

	ret = memcmp_c(stuR.auValue, pSignatureR->auValue, ECC_BIGINT32_MAXLEN);

err:
	return ret;
}

int SM2SignatureWithIDADpa(UINT8 *mes,
		UINT16 klen,
		UINT8 *IDA,
		UINT16 entla,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS)
{
	int ret = 0;
	UINT8 e[SM3_DIGEST_LEN];

	ret = SM2CalculateE(mes, klen, IDA, entla, pstuEccPrams, pstuPubKey, e);
	if(ret)
		goto err;

	ret = EccSignatureDpa(e, SM3_DIGEST_LEN, pstuEccPrams, pstuPrivKey, pSignatureR, pSignatureS);

err:
	return ret;
}

int SM2VerificationWithIDADpa(UINT8 *mes,
		UINT16 klen,
		UINT8 *IDA,
		UINT16 entla,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS)
{
	int ret = 0;
	UINT8 e[SM3_DIGEST_LEN];

	ret = SM2CalculateE(mes, klen, IDA, entla, pstuEccPrams, pstuPubKey, e);
	if(ret)
		goto err;

	ret = EccVerificationDpa(e, SM3_DIGEST_LEN, pstuEccPrams, pstuPubKey, pSignatureR, pSignatureS);

err:
	return ret;
}

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
                       UINT32 Mode)
{
	int ret = 0;
	ECC_STU_BIGINT32 stuT0, stuT1;
	ECC_STU_PUBKEY stuP;

	memset_c(stuT0.auValue, 0, ECC_BIGINT32_MAXLEN/2);
	memcpy_c(&stuT0.auValue[ECC_BIGINT32_MAXLEN/2], &pSelfTempPubkey->stuQx.auValue[ECC_BIGINT32_MAXLEN/2], ECC_BIGINT32_MAXLEN/2);
	stuT0.auValue[ECC_BIGINT32_MAXLEN/2] |= 0x80;

	ret = PkhaModularMath(stuT0.auValue, pSelfTempPrikey->auValue, NULL, pstuEccPrams->stuPrimeN.auValue, stuT0.auValue, OP_ALG_PKMODE_MOD_MULT);
	if(ret)
		goto err;
	ret = PkhaModularMath(stuT0.auValue, pSelfPrikey->auValue, NULL, pstuEccPrams->stuPrimeN.auValue, stuT0.auValue, OP_ALG_PKMODE_MOD_ADD);
	if(ret)
		goto err;

	memset_c(stuT1.auValue, 0, ECC_BIGINT32_MAXLEN/2);
	memcpy_c(&stuT1.auValue[ECC_BIGINT32_MAXLEN/2], &pOtherTempPubkey->stuQx.auValue[ECC_BIGINT32_MAXLEN/2], ECC_BIGINT32_MAXLEN/2);
	stuT1.auValue[ECC_BIGINT32_MAXLEN/2] |= 0x80;

	ret = EccPointMulV3(pstuEccPrams, &stuT1, pOtherTempPubkey, &stuP);
	if(ret)
		goto err;
	ret = PkhaEccMath(pstuEccPrams, &stuP, pOtherPubkey, NULL, &stuP, OP_ALG_PKMODE_MOD_ECC_ADD);
	if(ret)
		goto err;
	ret = EccPointMulV3(pstuEccPrams, &stuT0, &stuP, &stuP);
	if(ret)
		goto err;

	if(!Mode)
	{
		ret = SM2CalculateK(&stuP, za, zb, pAgreedKey, AgreedKeyLen);
		if(ret)
			goto err;

		ret = SM2CalculateS(&stuP, za, zb, pSelfTempPubkey, pOtherTempPubkey, sA, sB);
	}
	else
	{
		ret = SM2CalculateK(&stuP, zb, za, pAgreedKey, AgreedKeyLen);
		if(ret)
			goto err;
		ret = SM2CalculateS(&stuP, zb, za, pOtherTempPubkey, pSelfTempPubkey, sA, sB);
	}
	if(ret)
		goto err;

err:
	return ret;
}

//UINT8 SM2_SPI_PARA[32*5] = {0};
//extern main_io();
void  EccGenerateKeyPairEx(UINT32 *desc, ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_PUBKEY *pstuPubKey)
{
	//int ret;
	//UINT32 desc[MAX_CAAM_DESCSIZE];
	//MTMSR(0x23000);
	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_E_SZ);
	//generate private key with random data
	append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc, SM3_DIGEST_LEN);
	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
	append_seq_fifo_store(desc, SM3_DIGEST_LEN, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGFIFO);
	append_move(desc, MOVE_WAITCOMP | MOVE_SRC_OUTFIFO | MOVE_DEST_PK_A | SM3_DIGEST_LEN);
	//private key mod order
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	if(pstuEccPrams->uBytes%16)
		append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_ALL | JUMP_COND_PK_0 | 0xf5);
	else
		append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_ALL | JUMP_COND_PK_0 | 0xf7);
	append_fifo_store(desc, virt_to_phys(pstuPrivKey), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOST_CLASS_SWAP | FIFOST_TYPE_PKHA_B);
	//load p
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeP.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	//load G
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	//load a
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A3|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3 | FIFOLDST_SGF | FIFOLD_IMM);

	//copy k to e_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//load b
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_B0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	//do ecc mod mult
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);
	//store pubkey
	append_fifo_store(desc, virt_to_phys(pstuPubKey->stuQx.auValue), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOST_CLASS_SWAP | FIFOST_TYPE_PKHA_B1);
	append_fifo_store(desc, virt_to_phys(pstuPubKey->stuQy.auValue), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOST_CLASS_SWAP | FIFOST_TYPE_PKHA_B2);
	//MTMSR(0x2b000);
//	ret = run_descriptor_jr2(desc);

//	if(!ret)
//		return 0;//ok
//	else
//		return -1;//error


	//add for full speed calculate, add while(1) for cost calculate
#if 1
//	float x1=0;
	long long cpu_freq=1200000000;
	unsigned int rddata,rddata1,i,j;
	unsigned int desc1;
	unsigned int *desc1_p;
	unsigned int IRBA0 = AXIRAM_LADDR;
	unsigned int ORBA0 = AXIRAM_LADDR+0x1000;
	unsigned int IRBA1 = AXIRAM_LADDR+0x2000;
	unsigned int ORBA1 = AXIRAM_LADDR+0x3000;
	int run_cnt = CIPHER_CNT;
	int start, finish;

	//FLUSH
	W32(JR0_JRCR,0x1);
	rddata = 0;
	while((rddata&0x8)!=0x8)
	{
		rddata = R32(JR0_JRINTR);
	}
	//Reset
	W32(JR0_JRCR,0x1);

	do{
		rddata =  R32(JR0_JRINTR);
	}while((rddata&0x1)!=0);

#if SM2_SEC_1
	//FLUSH 2
	W32(JR1_JRCR,0x1);
	rddata = 0;
	while((rddata&0x8)!=0x8)
	{
		rddata = R32(JR1_JRINTR);
	}
	//Reset
	W32(JR1_JRCR,0x1);

	do{
		rddata =  R32(JR1_JRINTR);
	}while((rddata&0x1)!=0);
#endif

	desc1 = (AXIRAM_LADDR + 0x4200);
	desc1_p = (unsigned int *)desc1;

	for(j=0;j<RING_SIZE;j++)
	{
		for(i=0;i<0x180;i++)
		{
			((unsigned int *)(desc1+0x180*j))[i] = desc[i];
		}
	}

/////////////////////////////////////////////////

	W32(JR0_JRCFGRH,0x3030);

	W32(JR0_IRBARL,IRBA0);
	W32(JR0_IRBARH,AXIRAM_UPADDR);
	W32(JR0_ORBARL,ORBA0);
	W32(JR0_ORBARH,AXIRAM_UPADDR);

	W32(JR0_JRCFGRL,0x1);	//disable int sec
	W32(JR0_IRSR,RING_SIZE);
	W32(JR0_ORSR,RING_SIZE);

	for(i=0;i<RING_SIZE;i++)
	{
		W32(IRBA0+i*0x08,desc1+i*0x180);
	  	W32(IRBA0+0x4+i*0x08,AXIRAM_UPADDR); // higher address is useless for sec verification
	}


#if SM2_SEC_1
	W32(JR1_JRCFGRH,0x3030);			//SEC1_jobring

	W32(JR1_IRBARL,IRBA1);
	W32(JR1_IRBARH,AXIRAM_UPADDR);
	W32(JR1_ORBARL,ORBA1);
	W32(JR1_ORBARH,AXIRAM_UPADDR);

	W32(JR1_JRCFGRL,0x1);	//disable int sec
	W32(JR1_IRSR,RING_SIZE);
	W32(JR1_ORSR,RING_SIZE);

	for(i=0;i<RING_SIZE;i++)
	{
		W32(IRBA1+i*0x08,desc1+i*0x180);
	  	W32(IRBA1+0x4+i*0x08,AXIRAM_UPADDR); // higher address is useless for sec verification
	}
#endif

	MFSPR(start,0x10c);
	srand(start);

//while(1){
do{
	  W32(JR0_IRJAR,RING_SIZE);
#if SM2_SEC_1
	  W32(JR1_IRJAR,RING_SIZE);
#endif

	do{
		rddata =  R32(JR0_ORSFR);
	}while(rddata!=RING_SIZE);
#if SM2_SEC_1
	do{
		rddata1 =  R32(JR1_ORSFR);
	 }while(rddata1!=RING_SIZE);
#endif

	W32(JR0_ORJRR,RING_SIZE);
#if SM2_SEC_1
	  W32(JR1_ORJRR,RING_SIZE);
#endif
  }while(run_cnt--);
//}
	MFSPR(finish,0x10c);
	finish -= start;

#if SM2_SEC_1
	finish = (long long)cpu_freq*CIPHER_CNT*RING_SIZE/(16*finish)*2;
#else
	finish = (long long)cpu_freq*CIPHER_CNT*RING_SIZE/(16*finish);
#endif
	PSPRINTF("SM2_gen speed is %dtime/s\n\r", finish);
#endif
}

void EccSignatureEx(UINT32 *desc,
		UINT8 *mes,
		UINT16 klen,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_BIGINT32 *pstuPrivKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS)
{
	//UINT32 desc[MAX_CAAM_DESCSIZE], i;
	//UINT8 one[ECC_BIGINT32_MAXLEN];
	//one[0] = 1;
	//for(i=1; i<ECC_BIGINT32_MAXLEN; i++)
	//	one[i] = 0;

	init_job_desc(desc, START_INDEX);
	append_operation(desc,	OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_E_SZ);
	append_operation(desc,	OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	//generate k
	append_cmd(desc,CMD_LOAD | LDST_CLASS_1_CCB | LDST_IMM	| LDST_SRCDST_WORD_RNGDATASZ_REG | 4);
	append_cmd(desc, SM3_DIGEST_LEN);
	append_operation(desc, OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
	append_seq_fifo_store(desc, SM3_DIGEST_LEN, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGFIFO);
	append_move(desc, MOVE_WAITCOMP | MOVE_SRC_OUTFIFO | MOVE_DEST_PK_A | SM3_DIGEST_LEN);
	//k mod order
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_N | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	if (pstuEccPrams->uBytes % 16)
		append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_ALL | JUMP_COND_PK_0 | 0xf5);
	else
		append_jump(desc, JUMP_TYPE_LOCAL | JUMP_TEST_ALL | JUMP_COND_PK_0 | 0xf7);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N11 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//do e mod n
	//load n
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_N | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	//do amodn
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	//save e mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N10 | OP_ALG_PKMODE_CPYMEM_N_SZ);

	//(x1,y1) = [k]G
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_N | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	//load p
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeP.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	//load G
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_A0 | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_A1 | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_A3 | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	//load a
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3 | FIFOLDST_SGF | FIFOLD_IMM);
	//load k to e_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N11 | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	//load b
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_B0 | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes,	FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	//do ecc mod mult
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);

	//calculate r
	//load n
	if (pstuEccPrams->uBytes % 16) {
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD | LDST_CLASS_IND_CCB | LDST_IMM | LDST_SRCDST_WORD_INFO_FIFO | 4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD | NFIFOENTRY_DEST_CLASS1 | NFIFOENTRY_DTYPE_PK_N | NFIFOENTRY_PTYPE_ZEROS | (16 - pstuEccPrams->uBytes % 16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	//copy x1 to a_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B1 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//do x1 mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	//load e mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//do e+x1 mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);
	//save r
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N12 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_fifo_store(desc, virt_to_phys(pSignatureR->auValue), pstuEccPrams->uBytes, FIFOST_CLASS_NORMAL | FIFOLD_TYPE_PK_B | FIFOST_CLASS_SWAP);

	//calculate s
	if (pstuEccPrams->uBytes == SM3_DIGEST_LEN) {
		append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
		//load private key
		append_fifo_load(desc, virt_to_phys(pstuPrivKey->auValue), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	} else {
		append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue + pstuEccPrams->uBytes - SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
		//load private key
		append_fifo_load(desc, virt_to_phys(pstuPrivKey->auValue + pstuEccPrams->uBytes - SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	}
	//save private key
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A | OP_ALG_PKMODE_DST_REG_N13 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//load 1
	append_fifo_load(desc, virt_to_phys(one), SM3_DIGEST_LEN, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	//do 1+da mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);
	//copy to a_ram
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//do (1+da)^-1 mod n
	if(pstuEccPrams->uField)
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_INV);
	else
	{
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N21 | OP_ALG_PKMODE_CPYMEM_N_SZ);
		append_fifo_load(desc, virt_to_phys(one), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_N_SZ);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_BA);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_BA);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_N_SZ);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N21 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
		append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);
	}
	//save result
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//do r*da mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N12 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N13 | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	//do k-r*da mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N11 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
	//calc s
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	if (pstuEccPrams->uBytes == SM3_DIGEST_LEN)
		append_fifo_store(desc, virt_to_phys(pSignatureS->auValue), SM3_DIGEST_LEN, FIFOST_CLASS_NORMAL | FIFOLD_TYPE_PK_B | FIFOST_CLASS_SWAP);
	else
		append_fifo_store(desc, virt_to_phys(pSignatureS->auValue + pstuEccPrams->uBytes - SM3_DIGEST_LEN), SM3_DIGEST_LEN, FIFOST_CLASS_NORMAL | FIFOLD_TYPE_PK_B | FIFOST_CLASS_SWAP);

	//add for full speed calculate, add while(1) for cost calculate
#if 1
	float x1=0;
	long long timer_freq=48000000;
	unsigned int rddata,rddata1,i,j;
	unsigned int desc1;
	unsigned int *desc1_p;
	unsigned int IRBA0 = AXIRAM_LADDR;
	unsigned int ORBA0 = AXIRAM_LADDR+0x1000;
	unsigned int IRBA1 = AXIRAM_LADDR+0x2000;
	unsigned int ORBA1 = AXIRAM_LADDR+0x3000;

	int run_cnt = CIPHER_CNT;
	int start, finish;

	//FLUSH
	W32(JR0_JRCR,0x1);
	rddata = 0;
	while((rddata&0x8)!=0x8)
	{
		rddata = R32(JR0_JRINTR);
	}
	//Reset
	W32(JR0_JRCR,0x1);

	do{
		rddata =  R32(JR0_JRINTR);
	}while((rddata&0x1)!=0);

#if SM2_SEC_1
	//FLUSH 2
	W32(JR1_JRCR,0x1);
	rddata = 0;
	while((rddata&0x8)!=0x8)
	{
		rddata = R32(JR1_JRINTR);
	}
	//Reset
	W32(JR1_JRCR,0x1);

	do{
		rddata =  R32(JR1_JRINTR);
	}while((rddata&0x1)!=0);
#endif

	desc1 = (AXIRAM_LADDR + 0x4200);
	desc1_p = (unsigned int *)desc1;

	for(j=0;j<RING_SIZE;j++)
	{
		for(i=0;i<0x180;i++)
		{
			((unsigned int *)(desc1+0x180*j))[i] = desc[i];
		}
	}

/////////////////////////////////////////////////

	W32(JR0_JRCFGRH,0x3030);

	W32(JR0_IRBARL,IRBA0);
	W32(JR0_IRBARH,AXIRAM_UPADDR);
	W32(JR0_ORBARL,ORBA0);
	W32(JR0_ORBARH,AXIRAM_UPADDR);

//	W32(JR0_JRCFGRL,0x1);	//disable int sec

	W32(JR0_IRSR,RING_SIZE);
	W32(JR0_ORSR,RING_SIZE);

	for(i=0;i<RING_SIZE;i++)
	{
		W32(IRBA0+i*0x08,desc1+i*0x180);
	  	W32(IRBA0+0x4+i*0x08,AXIRAM_UPADDR); // higher address is useless for sec verification
	}

#if SM2_SEC_1
	W32(JR1_JRCFGRH,0x3030);			//SEC1_jobring

	W32(JR1_IRBARL,IRBA1);
	W32(JR1_IRBARH,AXIRAM_UPADDR);
	W32(JR1_ORBARL,ORBA1);
	W32(JR1_ORBARH,AXIRAM_UPADDR);

	W32(JR1_JRCFGRL,0x1);	//disable int sec
	W32(JR1_IRSR,RING_SIZE);
	W32(JR1_ORSR,RING_SIZE);

	for(i=0;i<RING_SIZE;i++)
	{
		W32(IRBA1+i*0x08,desc1+i*0x180);
	  	W32(IRBA1+0x4+i*0x08,AXIRAM_UPADDR); // higher address is useless for sec verification
	}
#endif

	MFSPR(start,0x10c);
	srand(start);

while(1){
  do{
	  W32(JR0_IRJAR,RING_SIZE);
#if SM2_SEC_1
	  W32(JR1_IRJAR,RING_SIZE);
#endif

	  do{
		  rddata =  R32(JR0_ORSFR);
	  }while(rddata!=RING_SIZE);
#if SM2_SEC_1
	do{
		rddata1 =  R32(JR1_ORSFR);
	 }while(rddata1!=RING_SIZE);
#endif

	  W32(JR0_ORJRR,RING_SIZE);
#if SM2_SEC_1
	  W32(JR1_ORJRR,RING_SIZE);
#endif
  }while(run_cnt--);
}
	MFSPR(finish,0x10c);
	finish -= start;
#if SM2_SEC_1
	finish = (long long)timer_freq*CIPHER_CNT*RING_SIZE/(finish)*2;
#else
	finish = (long long)timer_freq*CIPHER_CNT*RING_SIZE/(finish);
#endif
	PSPRINTF("SM2_sig speed is %dtime/s\n\r", finish);
#endif
	//PSPRINTF("SM2Signature:\r\n");
	//desc_dump(desc, MAX_CAAM_DESCSIZE);
	//run_descriptor_jr(desc, &jr_t);
}

void EccVerificationEx(UINT32 *desc,
		UINT8 *mes,
		UINT16 klen,
		ECC_STU_PRAMS *pstuEccPrams,
		ECC_STU_PUBKEY *pstuPubKey,
		ECC_STU_BIGINT32 *pSignatureR,
		ECC_STU_BIGINT32 *pSignatureS)
{
	UINT32 ret=0;
	//UINT32 desc[MAX_CAAM_DESCSIZE], i;
	//UINT8 one[ECC_BIGINT32_MAXLEN];
	//one[0] = 1;
	//for(i=1; i<ECC_BIGINT32_MAXLEN; i++)
	//	one[i] = 0;

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_E_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	//compare r and 1
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pSignatureR->auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A | OP_ALG_PKMODE_DST_REG_N10 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_fifo_load(desc, virt_to_phys(one), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N11 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_ALL | JUMP_COND_PK_BORROW | 0x1);
	//compare r and n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_INVALL | JUMP_COND_PK_BORROW | 0x2);
	//compare s and n
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pSignatureS->auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A | OP_ALG_PKMODE_DST_REG_N12 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_INVALL | JUMP_COND_PK_BORROW | 0x3);
	//compare s and 1
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N11 | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_ALL | JUMP_COND_PK_BORROW | 0x4);

	//do e mod n
	//load n
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys(mes), klen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	//do amodn
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_load_as_imm(desc, &pstuEccPrams->uBytes, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_A_SZ);
	//save e mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N13 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//t = (r + s) mod n
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N12 | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);

	//calculate [t]Pa
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeP.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuPubKey->stuQy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A3|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientA.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A3 | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A3 | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_B0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuCoefficientB.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B0 | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B0 | OP_ALG_PKMODE_DST_REG_N21 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT | pstuEccPrams->uField);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B1 | OP_ALG_PKMODE_DST_REG_N22 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B2 | OP_ALG_PKMODE_DST_REG_N23 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	//calculate [s]G
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A0|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGx.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A0 | FIFOLDST_SGF | FIFOLD_IMM);
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_A1|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuGy.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A1 | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A3 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N21 | OP_ALG_PKMODE_DST_REG_B0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N12 | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_MULT |pstuEccPrams->uField);

	//(x1, y1) = [s]G + [t]Pa
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A3 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N21 | OP_ALG_PKMODE_DST_REG_B0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N22 | OP_ALG_PKMODE_DST_REG_A0 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N23 | OP_ALG_PKMODE_DST_REG_A1 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ECC_ADD | pstuEccPrams->uField);

	//r' = (e + x1) mod n
	if(pstuEccPrams->uBytes%16)
	{
		//pad to 128b-alignment
		append_cmd(desc, CMD_LOAD|LDST_CLASS_IND_CCB|LDST_IMM|LDST_SRCDST_WORD_INFO_FIFO|4);
		append_cmd(desc, NFIFOENTRY_STYPE_PAD|NFIFOENTRY_DEST_CLASS1|NFIFOENTRY_DTYPE_PK_N|NFIFOENTRY_PTYPE_ZEROS|(16-pstuEccPrams->uBytes%16));
	}
	append_fifo_load(desc, virt_to_phys(pstuEccPrams->stuPrimeN.auValue), pstuEccPrams->uBytes, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B1 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N13 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);

	//compare r' and r
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
	append_jump(desc, JUMP_TYPE_HALT_USER | JUMP_TEST_INVALL | JUMP_COND_PK_0 | 0x5);

#if 1
#if 0
	int start, finish;
	//GPIO_M 0/2 init
//	gpio_input_test();
//	gpio_output_test();

	MFSPR(start,0x10c);
	srand(start);

	ret = run_descriptor_jr(desc, &jr_t);

//	GPIO_OUT_REG = 0x00000004;
//	while(!(GPIO_IN_REG&0x1));

	MFSPR(finish,0x10c);
	finish -= start;

	if(ret){
		PSPRINTF("SM2Verification error\r\n");
		while(1);
	}

	finish = 900000000/(16*finish);
	PSPRINTF("SM2 speed is %dtime/s\n\r", finish);
#else
//	float x1=0;
	long long timer_freq=48000000;
	unsigned int rddata,rddata1,i,j;
	unsigned int desc1;
	unsigned int *desc1_p;
	unsigned int IRBA0 = AXIRAM_LADDR;
	unsigned int ORBA0 = AXIRAM_LADDR+0x1000;
	unsigned int IRBA1 = AXIRAM_LADDR+0x2000;
	unsigned int ORBA1 = AXIRAM_LADDR+0x3000;
	int run_cnt = CIPHER_CNT;
	int start, finish;

	//FLUSH
	W32(JR0_JRCR,0x1);
	rddata = 0;
	while((rddata&0x8)!=0x8)
	{
		rddata = R32(JR0_JRINTR);
	}
	//Reset
	W32(JR0_JRCR,0x1);

	do{
		rddata =  R32(JR0_JRINTR);
	}while((rddata&0x1)!=0);

#if SM2_SEC_1
	//FLUSH 2
	W32(JR1_JRCR,0x1);
	rddata = 0;
	while((rddata&0x8)!=0x8)
	{
		rddata = R32(JR1_JRINTR);
	}
	//Reset
	W32(JR1_JRCR,0x1);

	do{
		rddata =  R32(JR1_JRINTR);
	}while((rddata&0x1)!=0);
#endif

	desc1 = (AXIRAM_LADDR + 0x4200);
	desc1_p = (unsigned int *)desc1;

	for(j=0;j<RING_SIZE;j++)
	{
		for(i=0;i<0x180;i++)
		{
			((unsigned int *)(desc1+0x180*j))[i] = desc[i];
		}
	}

/////////////////////////////////////////////////

	W32(JR0_JRCFGRH,0x3030);

	W32(JR0_IRBARL,IRBA0);
	W32(JR0_IRBARH,AXIRAM_UPADDR);
	W32(JR0_ORBARL,ORBA0);
	W32(JR0_ORBARH,AXIRAM_UPADDR);

//	W32(JR0_JRCFGRL,0x1);	//disable int sec

	W32(JR0_IRSR,RING_SIZE);
	W32(JR0_ORSR,RING_SIZE);

	for(i=0;i<RING_SIZE;i++)
	{
		W32(IRBA0+i*0x08,desc1+i*0x180);
	  	W32(IRBA0+0x4+i*0x08,AXIRAM_UPADDR); // higher address is useless for sec verification
	}

#if SM2_SEC_1
	W32(JR1_JRCFGRH,0x3030);			//SEC1_jobring

	W32(JR1_IRBARL,IRBA1);
	W32(JR1_IRBARH,AXIRAM_UPADDR);
	W32(JR1_ORBARL,ORBA1);
	W32(JR1_ORBARH,AXIRAM_UPADDR);

	W32(JR1_JRCFGRL,0x1);	//disable int sec
	W32(JR1_IRSR,RING_SIZE);
	W32(JR1_ORSR,RING_SIZE);

	for(i=0;i<RING_SIZE;i++)
	{
		W32(IRBA1+i*0x08,desc1+i*0x180);
	  	W32(IRBA1+0x4+i*0x08,AXIRAM_UPADDR); // higher address is useless for sec verification
	}
#endif

	MFSPR(start,0x10c);
	srand(start);

//while(1){
  do{
	  W32(JR0_IRJAR,RING_SIZE);
#if SM2_SEC_1
	  W32(JR1_IRJAR,RING_SIZE);
#endif

	  do{
		  rddata =  R32(JR0_ORSFR);
	  }while(rddata!=RING_SIZE);
#if SM2_SEC_1
	do{
		rddata1 =  R32(JR1_ORSFR);
	 }while(rddata1!=RING_SIZE);
#endif

	  W32(JR0_ORJRR,RING_SIZE);
#if SM2_SEC_1
	  W32(JR1_ORJRR,RING_SIZE);
#endif
  }while(run_cnt--);
//}

	MFSPR(finish,0x10c);
	finish -= start;

	/*print speed*/
#if SM2_SEC_1
	finish = (long long)timer_freq*CIPHER_CNT*RING_SIZE/(finish)*2;
#else
	finish = (long long)timer_freq*CIPHER_CNT*RING_SIZE/(finish);
#endif

	PSPRINTF("SM2_ver speed is %dtime/s\n\r", finish);

#endif
#endif
	//if(!ret)
	//	return 0;//ok
	//else
	//	return 0xff;//error
}


void lib_sm2_performance_test()
{
do{
	ECC_STU_PRAMS *para;
	para = malloc(sizeof(ECC_STU_PRAMS));
	ECC_STU_BIGINT32 *prikey[SM2_TASK_NUM] = {0};
	ECC_STU_PUBKEY *pubkey[SM2_TASK_NUM] = {0};
	//struct result *op[SM2_TASK_NUM];
	struct result op[SM2_TASK_NUM];
	unsigned int *desc[SM2_TASK_NUM] = {0};
	unsigned char *mes[SM2_TASK_NUM] = {0};
	ECC_STU_BIGINT32 *r[SM2_TASK_NUM] = {0};
	ECC_STU_BIGINT32 *s[SM2_TASK_NUM] = {0};
	unsigned int i, j, start, finish, speed;
	int ret;

	para->uField = 0;
	para->uBytes = 32;
	memcpy_c(para->stuPrimeP.auValue, SM2Parametres, 224);
//	InstallInterruptHandler0(104,jr_dequeue_irq);
	srand(start);
	MFSPR(start,0x10c);
	for(i=0; i<SM2_TASK_NUM; i++)
	{
		desc[i] = malloc(MAX_CAAM_DESCSIZE*4);
		if(!desc[i])
			COND_TRAP(31, 0, 0);
		prikey[i] = malloc(sizeof(ECC_STU_BIGINT32));
		if(!prikey[i])
			COND_TRAP(31, 0, 0);
		pubkey[i] = malloc(sizeof(ECC_STU_PUBKEY));
		if(!pubkey[i])
			COND_TRAP(31, 0, 0);
		//op[i] = malloc(sizeof(struct result));
		EccGenerateKeyPairEx(desc[i], para, prikey[i], pubkey[i]);
/*		memset_c(&op[i], 0, sizeof(struct result));
		ret = jr_enqueue_irq(desc[i], desc_done, &op[i], 0, &(jr_g[0]));
		if(ret)
			COND_TRAP(31, 0, 0);
	}

	for(i=0; i<SM2_TASK_NUM; i++)
	{
		while(op[i].done != 1);
		if(op[i].status)
			COND_TRAP(31, 0, 0);
		//free(op[i]);
//		free(pubkey[i]);
//		free(prikey[i]);
//		free(desc[i]);
*/
	}
//	MFSPR(finish,0x10c);
//	finish -= start;
//	PSPRINTF("gwnkey:%d\r\n", 900000000/16*SM2_TASK_NUM/finish);

	MFSPR(start,0x10c);
	for(i=0; i<SM2_TASK_NUM; i++)
	{
		mes[i] = malloc(32);
		if(!mes[i])
			COND_TRAP(31, 0, 0);
		for(j=0; j<32; j+=4)
			*(unsigned int *)(mes[i]+j) = rand();
		r[i] = malloc(sizeof(ECC_STU_BIGINT32));
		if(!r[i])
			COND_TRAP(31, 0, 0);
		s[i] = malloc(sizeof(ECC_STU_BIGINT32));
		if(!s[i])
			COND_TRAP(31, 0, 0);
		EccSignatureEx(desc[i], mes[i], 32, para, prikey[i], r[i], s[i]);
/*		memset_c(&op[i], 0, sizeof(struct result));
		ret = jr_enqueue_irq(desc[i], desc_done, &op[i], 0, &(jr_g[0]));
		if(ret)
			COND_TRAP(31, 0, 0);

	}

	for(i=0; i<SM2_TASK_NUM; i++)
	{
		while(op[i].done != 1);
		if(op[i].status)
			COND_TRAP(31, 0, 0);
*/	}
//	MFSPR(finish,0x10c);
//	finish -= start;
//	PSPRINTF("signature:%d\r\n", 900000000/16*SM2_TASK_NUM/finish);

	MFSPR(start,0x10c);
	for(i=0; i<SM2_TASK_NUM; i++)
	{
#if GPIO_LED
	while(1){
	gpio_m_o_h();
//	for(i=0;i<cnt_h;i++)
//		delay_ms_after_cpr(PLL_EN);
	delay(0x1000);
	EccVerificationEx(desc[i], mes[i], 32, para, pubkey[i], r[i], s[i]);

	gpio_m_o_l();
//	for(i=0;i<cnt_l;i++)
//		delay_ms_after_cpr(PLL_EN);
	delay(0x40000);
	}
#else
	EccVerificationEx(desc[i], mes[i], 32, para, pubkey[i], r[i], s[i]);
#endif
/*		memset_c(&op[i], 0, sizeof(struct result));
		ret = jr_enqueue_irq(desc[i], desc_done, &op[i], 0, &(jr_g[0]));
		if(ret)
			COND_TRAP(31, 0, 0);
	}

	for(i=0; i<SM2_TASK_NUM; i++)
	{
		while(op[i].done != 1);
		if(op[i].status)
			COND_TRAP(31, 0, 0);
*/		//free(op[i]);
		free(pubkey[i]);
		free(prikey[i]);
		free(desc[i]);
		free(mes[i]);
		free(r[i]);
		free(s[i]);
	}
	MFSPR(finish,0x10c);
	finish -= start;
//	PSPRINTF("verify:%d\r\n", 900000000/16*SM2_TASK_NUM/finish);
	free(para);
	}while(1);
	COND_TRAP(31, 0, 0);
}


void lib_sm2_test()
{
	ECC_STU_PRAMS para;
	unsigned short entla, klen, i, j;
	unsigned char mes[128];
	unsigned char ida[128];
	ECC_STU_BIGINT32 pri, pri_a, pri_b, ra, rb;
	ECC_STU_PUBKEY pub, pub_a, pub_b, pRa, pRb;
	ECC_STU_BIGINT32 r;
	ECC_STU_BIGINT32 s;
	unsigned char c1[65], c2[128], c3[32], key1[16], key2[16], sa1[32], sa2[32], sb1[32], sb2[32];

	unsigned int za[8] = {
		0xE4D1D0C3, 0xCA4C7F11, 0xBC8FF8CB, 0x3F4C02A7, 0x8F108FA0, 0x98E51A66, 0x8487240F, 0x75E20F31
	};

	unsigned int zb[8] = {
		0x6B4B6D0E, 0x276691BD, 0x4A11BF72, 0xF4FB501A, 0xE309FDAC, 0xB72FA6CC, 0x336E6656, 0x119ABD67
	};

	para.uField = 0;
	para.uBytes = 32;
	memcpy_c(para.stuPrimeP.auValue, SM2Parametres, 224);
while(1)
{
	klen = rand() % 128 + 1;
	entla = rand() % 128 + 1;

	for (i = 0; i < klen; i++) {
		mes[i] = rand();
	}
	for (j = 0; j < entla; j++) {
		ida[j] = rand();
	}

	if(EccGenerateKeyPair(&para, &pri, &pub))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));

	SM2Encrypt(mes, klen, &pub, &para, c1, c2, c3);
	if(SM2Decrypt(c1, c2, c3, klen, &pri, &para, mes))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));

	EccSignature(mes, klen, &para, &pri, &r, &s);
	if(EccVerification(mes, klen, &para, &pub, &r, &s))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));

	SM2SignatureWithIDA(mes, klen, ida, entla, &para, &pub, &pri, &r, &s);
	if(SM2VerificationWithIDA(mes, klen, ida, entla, &para, &pub, &r, &s))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));

	if(EccGenerateKeyPair(&para, &pri_a, &pub_a))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(EccGenerateKeyPair(&para, &pri_b, &pub_b))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(EccGenerateKeyPair(&para, &ra, &pRa))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(EccGenerateKeyPair(&para, &rb, &pRb))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));

	if(SM2_KeyExchange_HT(&para, &pri_a, &pRa, &ra, &pub_b, &pRb, za, zb, key1, 16, sa1, sa2, 0))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(SM2_KeyExchange_HT(&para, &pri_b, &pRb, &rb, &pub_a, &pRa, zb, za, key2, 16, sb1, sb2, 1))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(memcmp_c(sa1, sb1, 32))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(memcmp_c(sa2, sb2, 32))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
}
	asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
}

void lib_sm2_large_data_test()
{
	ECC_STU_PRAMS para;
	unsigned short entla, klen, i, j;
	unsigned char mes[512];
	unsigned char ida[512];
	ECC_STU_BIGINT32 pri, pri_a, pri_b, ra, rb;
	ECC_STU_PUBKEY pub, pub_a, pub_b, pRa, pRb;
	ECC_STU_BIGINT32 r;
	ECC_STU_BIGINT32 s;
	unsigned char c1[65], c2[128], c3[32], key1[16], key2[16], sa1[32], sa2[32], sb1[32], sb2[32];

	unsigned int za[8] = {
		0xE4D1D0C3, 0xCA4C7F11, 0xBC8FF8CB, 0x3F4C02A7, 0x8F108FA0, 0x98E51A66, 0x8487240F, 0x75E20F31
	};

	unsigned int zb[8] = {
		0x6B4B6D0E, 0x276691BD, 0x4A11BF72, 0xF4FB501A, 0xE309FDAC, 0xB72FA6CC, 0x336E6656, 0x119ABD67
	};

	unsigned int pubkey[16] = {
		0xE05C97CC, 0x0E370DA8, 0xD9D33905, 0xBD5FBA7C, 0xF74A042E, 0x53ECBA71, 0xAAB6BCC3, 0xC932D575,
		0x267BF3E0, 0x1E08FFCE, 0x51D12959, 0xD0AB1645, 0x77FD5584, 0x1C1B9293, 0x6B17883E, 0xCB5BB3AF
	};

	unsigned int prikey[8] = {
		0x5163DF45, 0x478CED20, 0xE1F72D1E, 0x39A71441, 0x2277E55A, 0x04C1DAAF, 0xD13E000A, 0xC39CF2E8
	};

	unsigned int id[40] = {
		0x88B65013, 0xEC7AFE8E, 0xF6414FF5, 0xFD2B43BC, 0x02EFA9F0, 0xEFDB7A2C, 0xB72A8E49, 0xA4198859,
		0x0F353305, 0x22572462, 0xC81ADC74, 0x58313945, 0xCDE260DA, 0xFCD61461, 0x05BF287B, 0x82C64C81,
		0xCA517AE9, 0x3DC2F87E, 0xD2E5E37C, 0xEF9637A5, 0xF314E4FA, 0x8575ACD4, 0xD1ABF7DB, 0xAD5FD25A,
		0x11EB3128, 0xCFFC9701, 0xF2FF70A1, 0x78689761, 0x58F6384D, 0x7CE14E1F, 0x00AC7B12, 0xB8AAAFC4,
		0xB6471BC9, 0xF39C0A29, 0xAF04E7A6, 0x7671AAAA, 0xAC7E9CE9, 0x8EEA2C8E, 0xC5C524F9, 0x11C969C3
	};

	unsigned int message[40] = {
		0x10F5ADB7, 0xAAE33FD3, 0xCE05797A, 0xE505E6AD, 0xAF667E5F, 0xD398B8F2, 0xB874C5D2, 0xB4A646B1,
		0xCF4CB717, 0xF866A114, 0x5AEA1A49, 0x0C58C807, 0x6EE5EF5A, 0x1A6F0644, 0x66147F18, 0xD931D693,
		0x90B0543C, 0x904D86FA, 0x5FA0AA75, 0x94119E2F, 0xAB26A4F2, 0xB0AE08BD, 0x1D741447, 0x334B7CCD,
		0x6F9DA0F7, 0x978AA6DD, 0xD59F4AE4, 0x2C89E891, 0xB8B6D468, 0x6586506F, 0xE0BEEAA4, 0xCDBCECB0,
		0xEE21B178, 0x466E8310, 0x839F4A49, 0xE9DB5254, 0x337D18B1, 0x33D8675C, 0x32890E24, 0xB61DB88A
	};

	unsigned int rs[16] = {
		0x92242A9E, 0xD623BB2F, 0x3B3178AD, 0xC912419D, 0x0B273F10, 0xD1D41AB2, 0x827B9900, 0xE73C1C32,
		0xFA3A3F33, 0xC6AD568C, 0xC9587CC7, 0x3E0CC377, 0x3D707C5F, 0xD761825E, 0x6152CE46, 0x28974763
	};

	para.uField = 0;
	para.uBytes = 32;
	memcpy_c(para.stuPrimeP.auValue, SM2Parametres2, 224);
	memcpy_c(pub.stuQx.auValue, pubkey, 64);
	memcpy_c(pri.auValue, prikey, 32);
	memcpy_c(mes, message, 160);
	memcpy_c(ida, id, 160);

	klen = 160;
	entla = 160;

	SM2SignatureWithIDA(mes, klen, ida, entla, &para, &pub, &pri, &r, &s);

	memcpy_c(r.auValue, rs, 32);
	memcpy_c(s.auValue, rs+8, 32);

	if(SM2VerificationWithIDA(mes, klen, ida, entla, &para, &pub, &r, &s))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
while(1)
{
	klen = rand() % 512 + 1;
	entla = rand() % 512 + 1;

	for (i = 0; i < klen; i++) {
		mes[i] = rand();
	}
	for (j = 0; j < entla; j++) {
		ida[j] = rand();
	}

	if(EccGenerateKeyPair(&para, &pri, &pub))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));

	SM2SignatureWithIDA(mes, klen, ida, entla, &para, &pub, &pri, &r, &s);
	if(SM2VerificationWithIDA(mes, klen, ida, entla, &para, &pub, &r, &s))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
}
	asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
}

void dpa_test1()
{
	ECC_STU_PRAMS para;
	ECC_STU_PUBKEY t[2];
	int ret;

	para.uField = 0;
	para.uBytes = 32;
	memcpy_c(para.stuPrimeP.auValue, SM2Parametres, 224);

	ret = PkhaEccMath(&para, (ECC_STU_PUBKEY *)(&para.stuGx), NULL, NULL, &t[1], OP_ALG_PKMODE_MOD_ECC_DBL);

	ret = EccPointDbl(&para, (ECC_STU_PUBKEY *)(&para.stuGx), &t[0]);

	asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
}

void dpa_test2()
{
	ECC_STU_PRAMS para;
	ECC_STU_PUBKEY stuPointP, stuPointQ, r;
	unsigned int stuPx[8] = {
			0x7DEACE5F ,0xD121BC38 ,0x5A3C6317 ,0x249F413D ,0x28C17291 ,0xA60DFD83 ,0xB835A453 ,0x92D22B0A
	};
	unsigned int stuPy[8] = {
			0x2E49D5E5 ,0x279E5FA9 ,0x1E71FD8F ,0x693A64A3 ,0xC4A94611 ,0x15A4FC9D ,0x79F34EDC ,0x8BDDEBD0
	};
	unsigned int stuQx[8] = {
			0x1657FA75 ,0xBF2ADCDC ,0x3C1F6CF0 ,0x5AB7B45E ,0x04D3ACBE ,0x8E4085CF ,0xA669CB25 ,0x64F17A9F
	};
	unsigned int stuQy[8] = {
			0x19F0115F ,0x21E16D2F ,0x5C3A485F ,0x8575A128 ,0xBBCDDF80 ,0x296A62F6 ,0xAC2EB842 ,0xDD058E50
	};
	int ret;

	para.uField = 0;
	para.uBytes = 32;
	memcpy_c(para.stuPrimeP.auValue, SM2Parametres, 224);
	memcpy_c(stuPointP.stuQx.auValue, stuPx, 32);
	memcpy_c(stuPointP.stuQy.auValue, stuPy, 32);
	memcpy_c(stuPointQ.stuQx.auValue, stuQx, 32);
	memcpy_c(stuPointQ.stuQy.auValue, stuQy, 32);

	ret = EccPointAdd(&para, &stuPointP, &stuPointQ, &r);
	asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
}

void dpa_test3()
{
	ECC_STU_PRAMS para;
	ECC_STU_BIGINT32 pri;
	ECC_STU_PUBKEY pub;
	unsigned int private_key[8] = {
			0x1649AB77, 0xA00637BD, 0x5E2EFE28, 0x3FBF3535,
			0x34AA7F7C, 0xB89463F2, 0x08DDBC29, 0x20BB0DA0
	};
	int ret;

	SM2Init(&para);
#if 1
	memcpy_c(pri.auValue, private_key, 32);
#else
	memcpy_c(pri.auValue, para.stuPrimeN, 32);
	pri.auValue[31] += 1;
#endif
	while(1)
	{
	//ret = EccPointMul(&para, &pri, (ECC_STU_PUBKEY *)(&para.stuGx), &pub);
//		ret = EccPointMulV2(&para, &pri, (ECC_STU_PUBKEY *)(&para.stuGx), &pub);
		ret = EccPointMulV3(&para, &pri, (ECC_STU_PUBKEY *)(&para.stuGx), &pub);
	}

	asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
}

void dpa_test4()
{
	ECC_STU_PRAMS para;
	ECC_STU_PUBKEY pub;
	int ret;

	para.uField = 0;
	para.uBytes = 32;
	memcpy_c(para.stuPrimeP.auValue, SM2Parametres, 224);

	ret = EccPointIsOnCurve(&para, (ECC_STU_PUBKEY *)(&para.stuGx));

	para.stuGx.auValue[0] += 1;

	ret = EccPointIsOnCurve(&para, (ECC_STU_PUBKEY *)(&para.stuGx));

	asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
}

void dpa_test5()
{
	ECC_STU_PRAMS para;
	ECC_STU_PUBKEY pub;
	ECC_STU_BIGINT32 pri;
	UINT8 c0[64], c1[19], c2[32], mes[19];
	unsigned char standard_mes[19] =
	{
		0x65, 0x6E, 0x63, 0x72, 0x79, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x73,
		0x74, 0x61, 0x6E, 0x64, 0x61, 0x72, 0x64
	};
	int ret;

	ret = SM2Init(&para);
	ret = EccGenerateKeyPairDpa(&para, &pri, &pub);
	ret = SM2EncryptDpa(standard_mes, 19, &pub, &para, c0, c1, c2);
	ret = SM2DecryptDpa(c0, c1, c2, 19, &pri, &para, mes);

	asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
}

void dpa_test6()
{
	ECC_STU_PRAMS para;
	ECC_STU_PUBKEY pub;
	ECC_STU_BIGINT32 pri, r, s;
	unsigned int e[8] = {
			0xB524F552 ,0xCD82B8B0 ,0x28476E00 ,0x5C377FB1 ,0x9A87E6FC ,0x682D48BB ,0x5D42E3D9 ,0xB9EFFE76
	};
	unsigned int private_key1[8] = {
			0x128B2FA8, 0xBD433C6C, 0x068C8D80, 0x3DFF7979,
			0x2A519A55, 0x171B1B65, 0x0C23661D, 0x15897263
	};
	unsigned int pubkey[16] = {
			0x0AE4C779 ,0x8AA0F119 ,0x471BEE11 ,0x825BE462 ,0x02BB79E2 ,0xA5844495 ,0xE97C04FF ,0x4DF2548A,
			0x7C0240F8 ,0x8F1CD4E1 ,0x6352A73C ,0x17B7F16F ,0x07353E53 ,0xA176D684 ,0xA9FE0C6B ,0xB798E857
	};
	unsigned char standard_mes1[14] =
	{
		0x6D, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x20, 0x64, 0x69, 0x67, 0x65,
		0x73, 0x74
	};
	unsigned char standard_ida[18] =
	{
		0x41, 0x4C, 0x49, 0x43, 0x45, 0x31, 0x32, 0x33, 0x40, 0x59, 0x41, 0x48,
		0x4F, 0x4F, 0x2E, 0x43, 0x4F, 0x4D
	};
	int ret;

	ret = SM2Init(&para);
	memcpy_c(pub.stuQx.auValue, pubkey, 64);
	memcpy_c(pri.auValue, private_key1, 32);
	ret = EccSignatureDpa(e, 32, &para, &pri, &r, &s);
	ret = EccVerificationDpa(e, 32, &para, &pub, &r, &s);
	ret = SM2SignatureWithIDADpa(standard_mes1, 14, standard_ida, 18*8, &para, &pub, &pri, &r, &s);
	ret = SM2VerificationWithIDADpa(standard_mes1, 14, standard_ida, 18*8, &para, &pub, &r, &s);

	asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
}

void dpa_test7()
{
	ECC_STU_PRAMS para;
	ECC_STU_BIGINT32 pri_a, pri_b, ra, rb;
	ECC_STU_PUBKEY pub_a, pub_b, pRa, pRb;
	int ret;
	unsigned int za[8] = {
		0xE4D1D0C3, 0xCA4C7F11, 0xBC8FF8CB, 0x3F4C02A7, 0x8F108FA0, 0x98E51A66, 0x8487240F, 0x75E20F31
	};
	unsigned int zb[8] = {
		0x6B4B6D0E, 0x276691BD, 0x4A11BF72, 0xF4FB501A, 0xE309FDAC, 0xB72FA6CC, 0x336E6656, 0x119ABD67
	};
	unsigned int prikey_a[8] = {
		0x6FCBA2EF ,0x9AE0AB90 ,0x2BC3BDE3 ,0xFF915D44 ,0xBA4CC78F ,0x88E2F8E7 ,0xF8996D3B ,0x8CCEEDEE
	};
	unsigned int pubkey_a[16] = {
			0x3099093B ,0xF3C137D8 ,0xFCBBCDF4 ,0xA2AE50F3 ,0xB0F216C3 ,0x122D7942 ,0x5FE03A45 ,0xDBFE1655,
			0x3DF79E8D ,0xAC1CF0EC ,0xBAA2F2B4 ,0x9D51A4B3 ,0x87F2EFAF ,0x48233908 ,0x6A27A8E0 ,0x5BAED98B
	};
	unsigned int prikey_b[8] = {
			0x5E35D7D3 ,0xF3C54DBA ,0xC72E6181 ,0x9E730B01 ,0x9A84208C ,0xA3A35E4C ,0x2E353DFC ,0xCB2A3B53
	};
	unsigned int pubkey_b[16] = {
			0x245493D4 ,0x46C38D8C ,0xC0F11837 ,0x4690E7DF ,0x633A8A4B ,0xFB3329B5 ,0xECE604B2 ,0xB4F37F43,
			0x53C0869F ,0x4B9E1777 ,0x3DE68FEC ,0x45E14904 ,0xE0DEA45B ,0xF6CECF99 ,0x18C85EA0 ,0x47C60A4C
	};
	unsigned int r_a[8] = {
			0x83A2C9C8 ,0xB96E5AF7 ,0x0BD480B4 ,0x72409A9A ,0x327257F1 ,0xEBB73F5B ,0x073354B2 ,0x48668563
	};
	unsigned int p_Ra[16] = {
			0x6CB56338 ,0x16F4DD56 ,0x0B1DEC45 ,0x8310CBCC ,0x6856C095 ,0x05324A6D ,0x23150C40 ,0x8F162BF0,
			0x0D6FCF62 ,0xF1036C0A ,0x1B6DACCF ,0x57399223 ,0xA65F7D7B ,0xF2D9637E ,0x5BBBEB85 ,0x7961BF1A
	};
	unsigned int r_b[8] = {
			0x33FE2194 ,0x0342161C ,0x55619C4A ,0x0C060293 ,0xD543C80A ,0xF19748CE ,0x176D8347 ,0x7DE71C80
	};
	unsigned int p_Rb[16] = {
			0x1799B2A2 ,0xC7782953 ,0x00D9A232 ,0x5C686129 ,0xB8F2B533 ,0x7B3DCF45 ,0x14E8BBC1 ,0x9D900EE5,
			0x54C9288C ,0x82733EFD ,0xF7808AE7 ,0xF27D0E73 ,0x2F7C73A7 ,0xD9AC98B7 ,0xD8740A91 ,0xD0DB3CF4
	};
	UINT32 key[4], sa[8], sb[8];

	ret = SM2Init(&para);
	memcpy_c(pri_a.auValue, prikey_a, 32);
	memcpy_c(pub_a.stuQx.auValue, pubkey_a, 64);
	memcpy_c(pri_b.auValue, prikey_b, 32);
	memcpy_c(pub_b.stuQx.auValue, pubkey_b, 64);

	memcpy_c(ra.auValue, r_a, 32);
	memcpy_c(pRa.stuQx.auValue, p_Ra, 64);
	memcpy_c(rb.auValue, r_b, 32);
	memcpy_c(pRb.stuQx.auValue, p_Rb, 64);

	ret = SM2KeyExchangeDpa(&para, &pri_a, &pRa, &ra, &pub_b, &pRb, za, zb, key, 16, sa, sb, 0);
	ret = SM2KeyExchangeDpa(&para, &pri_b, &pRb, &rb, &pub_a, &pRa, zb, za, key, 16, sa, sb, 1);

	asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
}

void dpa_test8()
{
	ECC_STU_PRAMS para;
	unsigned short entla, klen, entlb, i, j;
	unsigned char mes[128];
	unsigned char ida[128], idb[128];
	ECC_STU_BIGINT32 pri, pri_a, pri_b, ra, rb;
	ECC_STU_PUBKEY pub, pub_a, pub_b, pRa, pRb;
	ECC_STU_BIGINT32 r;
	ECC_STU_BIGINT32 s;
	unsigned char c1[65], c2[128], c3[32], key1[16], key2[16], sa1[32], sa2[32], sb1[32], sb2[32];

	unsigned int za[8];

	unsigned int zb[8];
	int ret, count=0;

	ret = SM2Init(&para);
	if(ret)
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
while(1)
{
	klen = rand() % 128 + 1;
	entla = rand() % 128 + 1;
	entlb = rand() % 128 + 1;

	for (i = 0; i < klen; i++) {
		mes[i] = rand();
	}
	for (j = 0; j < entla; j++) {
		ida[j] = rand();
	}
	for (j = 0; j < entlb; j++) {
		idb[j] = rand();
	}

	ret = EccGenerateKeyPairDpa(&para, &pri, &pub);
	if(ret)
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));

	ret=SM2EncryptDpa(mes, klen, &pub, &para, c1, c2, c3);
	if(ret)
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	ret=SM2DecryptDpa(c1, c2, c3, klen, &pri, &para, mes);
	if(ret)
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
/*
	if(EccSignatureDpa(mes, klen, &para, &pri, &r, &s))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(EccVerificationDpa(mes, klen, &para, &pub, &r, &s))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
*/
	ret=SM2SignatureWithIDADpa(mes, klen, ida, entla*8, &para, &pub, &pri, &r, &s);
	if(ret)
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	ret=SM2VerificationWithIDADpa(mes, klen, ida, entla*8, &para, &pub, &r, &s);
	if(ret)
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));

	if(EccGenerateKeyPairDpa(&para, &pri_a, &pub_a))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(EccGenerateKeyPairDpa(&para, &pri_b, &pub_b))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(EccGenerateKeyPairDpa(&para, &ra, &pRa))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(EccGenerateKeyPairDpa(&para, &rb, &pRb))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(SM2CalculateZ(ida, entla*8, &para, &pub_a, za))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(SM2CalculateZ(idb, entlb*8, &para, &pub_b, zb))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));

	ret=SM2KeyExchangeDpa(&para, &pri_a, &pRa, &ra, &pub_b, &pRb, za, zb, key1, 16, sa1, sa2, 0);
	if(ret)
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	ret=SM2KeyExchangeDpa(&para, &pri_b, &pRb, &rb, &pub_a, &pRa, zb, za, key2, 16, sb1, sb2, 1);
	if(ret)
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(memcmp_c(sa1, sb1, 32))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
	if(memcmp_c(sa2, sb2, 32))
		asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));

	PSPRINTF("SM2 test[%d] OK!\n\r", ++count);
}
	asm volatile("tw %0,%1,%2"::"n"(31),"r"(0),"r"(0));
}
