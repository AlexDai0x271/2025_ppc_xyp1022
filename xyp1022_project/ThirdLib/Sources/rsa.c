#include "rsa.h"
#include "desc.h"
#include "desc_constr.h"
#include "e500mc_asm.h"

//data from wychen
unsigned char key1_r2[512] = {
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x55,0x52,0x3c,0x7f,0xc4,0x52,0x3f,0x90,
		0xef,0xa0,0x0d,0xf3,0x77,0x4a,0x25,0x9f,0x2e,0x62,0xb4,0xc5,0xd9,0x9c,0xb5,0xad,
		0xb3,0x00,0xa0,0x28,0x5e,0x53,0x01,0x93,0x0e,0x0c,0x70,0xfb,0x68,0x76,0x93,0x9c,
		0xe6,0x16,0xce,0x62,0x4a,0x11,0xe0,0x08,0x6d,0x34,0x1e,0xbc,0xac,0xa0,0xa1,0xf5,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x55,0x52,0x3c,0x7f,0xc4,0x52,0x3f,0x90,
		0xef,0xa0,0x0d,0xf3,0x77,0x4a,0x25,0x9f,0x2e,0x62,0xb4,0xc5,0xd9,0x9c,0xb5,0xad,
		0xb3,0x00,0xa0,0x28,0x5e,0x53,0x01,0x93,0x0e,0x0c,0x70,0xfb,0x68,0x76,0x93,0x9c,
		0xe6,0x16,0xce,0x62,0x4a,0x11,0xe0,0x08,0x6d,0x34,0x1e,0xbc,0xac,0xa0,0xa1,0xf5,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x55,0x52,0x3c,0x7f,0xc4,0x52,0x3f,0x90,
		0xef,0xa0,0x0d,0xf3,0x77,0x4a,0x25,0x9f,0x2e,0x62,0xb4,0xc5,0xd9,0x9c,0xb5,0xad,
		0xb3,0x00,0xa0,0x28,0x5e,0x53,0x01,0x93,0x0e,0x0c,0x70,0xfb,0x68,0x76,0x93,0x9c,
		0xe6,0x16,0xce,0x62,0x4a,0x11,0xe0,0x08,0x6d,0x34,0x1e,0xbc,0xac,0xa0,0xa1,0xf5,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x55,0x52,0x3c,0x7f,0xc4,0x52,0x3f,0x90,
		0xef,0xa0,0x0d,0xf3,0x77,0x4a,0x25,0x9f,0x2e,0x62,0xb4,0xc5,0xd9,0x9c,0xb5,0xad,
		0xb3,0x00,0xa0,0x28,0x5e,0x53,0x01,0x93,0x0e,0x0c,0x70,0xfb,0x68,0x76,0x93,0x9c,
		0xe6,0x16,0xce,0x62,0x4a,0x11,0xe0,0x08,0x6d,0x34,0x1e,0xbc,0xac,0xa0,0xa1,0xf5,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x55,0x52,0x3c,0x7f,0xc4,0x52,0x3f,0x90,
		0xef,0xa0,0x0d,0xf3,0x77,0x4a,0x25,0x9f,0x2e,0x62,0xb4,0xc5,0xd9,0x9c,0xb5,0xad,
		0xb3,0x00,0xa0,0x28,0x5e,0x53,0x01,0x93,0x0e,0x0c,0x70,0xfb,0x68,0x76,0x93,0x9c,
		0xe6,0x16,0xce,0x62,0x4a,0x11,0xe0,0x08,0x6d,0x34,0x1e,0xbc,0xac,0xa0,0xa1,0xf5,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x55,0x52,0x3c,0x7f,0xc4,0x52,0x3f,0x90,
		0xef,0xa0,0x0d,0xf3,0x77,0x4a,0x25,0x9f,0x2e,0x62,0xb4,0xc5,0xd9,0x9c,0xb5,0xad,
		0xb3,0x00,0xa0,0x28,0x5e,0x53,0x01,0x93,0x0e,0x0c,0x70,0xfb,0x68,0x76,0x93,0x9c,
		0xe6,0x16,0xce,0x62,0x4a,0x11,0xe0,0x08,0x6d,0x34,0x1e,0xbc,0xac,0xa0,0xa1,0xf5,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x55,0x52,0x3c,0x7f,0xc4,0x52,0x3f,0x90,
		0xef,0xa0,0x0d,0xf3,0x77,0x4a,0x25,0x9f,0x2e,0x62,0xb4,0xc5,0xd9,0x9c,0xb5,0xad,
		0xb3,0x00,0xa0,0x28,0x5e,0x53,0x01,0x93,0x0e,0x0c,0x70,0xfb,0x68,0x76,0x93,0x9c,
		0xe6,0x16,0xce,0x62,0x4a,0x11,0xe0,0x08,0x6d,0x34,0x1e,0xbc,0xac,0xa0,0xa1,0xf5,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x55,0x52,0x3c,0x7f,0xc4,0x52,0x3f,0x90,
		0xef,0xa0,0x0d,0xf3,0x77,0x4a,0x25,0x9f,0x2e,0x62,0xb4,0xc5,0xd9,0x9c,0xb5,0xad,
		0xb3,0x00,0xa0,0x28,0x5e,0x53,0x01,0x93,0x0e,0x0c,0x70,0xfb,0x68,0x76,0x93,0x9c,
		0xe6,0x16,0xce,0x62,0x4a,0x11,0xe0,0x08,0x6d,0x34,0x1e,0xbc,0xac,0xa0,0xa1,0xf5
};
unsigned char Rsa_SelfCheck_E[] = "\x01\x00\x01";

unsigned char Rsa_SelfCheck_M[256] = {
		0xE4,0xE5,0xFA,0xAE,0xC4,0x4E,0xC9,0x63,0xD9,0x01,0x13,0x0B,0x70,0xF0,0x0C,0xB9,
		0x3D,0xAE,0x77,0x01,0xD8,0x31,0x57,0xEA,0xF8,0x19,0x89,0x54,0xB4,0x79,0x34,0x50,
		0xEB,0x80,0xFB,0xEF,0xED,0xA6,0x16,0xBE,0xE2,0x52,0x3A,0x4E,0xF8,0x90,0x78,0xDF,
		0x73,0x10,0x8A,0xC7,0x0A,0x69,0xD5,0xE6,0x80,0x38,0x44,0xCB,0x4D,0xB2,0xF2,0xE2,
		0x43,0xBC,0x58,0x2A,0xCC,0x55,0x49,0x65,0x43,0xA0,0x89,0x9A,0xEB,0x41,0xF0,0x7D,
		0xC1,0xE4,0xCB,0x4D,0xD2,0x5D,0xB1,0xC2,0xA5,0x42,0x64,0x8F,0x92,0x32,0xAE,0x8B,
		0x1A,0x02,0xB6,0x1D,0xA9,0x65,0xC5,0x23,0xF4,0x3C,0x98,0xF1,0xF7,0xBF,0x94,0x37,
		0xD7,0x74,0x4C,0x1F,0xFB,0x45,0x58,0x13,0x6F,0xA6,0xD0,0xF0,0xF4,0xB1,0xBD,0x8B,
		0x9E,0x39,0x35,0x5C,0x55,0x03,0x09,0xB3,0x3D,0x90,0xD1,0x12,0xCB,0x70,0x97,0x29,
		0x3D,0x1F,0x8F,0x61,0x56,0xDA,0x12,0xCC,0xEB,0x13,0x83,0xC8,0xC2,0xBC,0x70,0xDD,
		0xAB,0x33,0x20,0x25,0xC7,0xBA,0x86,0x84,0x04,0x91,0x2D,0x0E,0x11,0x48,0x44,0x1A,
		0x93,0x21,0xB2,0x41,0x4A,0x5C,0x06,0x56,0x0B,0x37,0x57,0x92,0xE9,0xED,0x42,0x9B,
		0xA1,0x83,0xA8,0x88,0x94,0x64,0x29,0xA5,0x9A,0x77,0x38,0xD0,0x1F,0xB2,0x21,0x79,
		0x47,0xD0,0x2E,0xF3,0xA1,0x54,0x5E,0x97,0xA1,0x2C,0x27,0x6B,0x69,0x01,0x0E,0x4C,
		0xC9,0x03,0xE2,0x40,0x39,0x28,0x27,0x8E,0x36,0xD6,0xF9,0x96,0x92,0x65,0xD5,0x21,
		0x5B,0x43,0x1C,0xE9,0x68,0x58,0x54,0xE4,0xC1,0xFC,0x61,0x92,0xEF,0x84,0xAB,0xBD
};

unsigned char Rsa_SelfCheck_D[256] = {
		0x79,0x12,0xb0,0x73,0x4a,0xc0,0xc5,0xc3,0x7b,0xa0,0x16,0xdb,0xc1,0x64,0x27,0xc6,
		0xbf,0xba,0x8d,0x96,0x3b,0x2d,0x76,0x6c,0x81,0xc8,0xb8,0x1c,0x46,0xcd,0xa9,0x7f,
		0x77,0xe2,0x24,0xe0,0xb6,0xda,0x58,0x43,0xa5,0x60,0x8b,0x19,0x04,0x5f,0x1e,0xa0,
		0x4a,0xb3,0xa0,0x44,0x41,0xdc,0xd3,0x58,0x75,0xcf,0xcd,0x8b,0x9f,0xd0,0x9c,0xaa,
		0x53,0xdb,0xc6,0xe2,0xf4,0x6b,0xd4,0x43,0xac,0xee,0xb8,0x7a,0x6a,0x1f,0xd9,0xbe,
		0x1b,0x16,0x12,0xf7,0x59,0xe3,0x57,0x48,0x36,0xb3,0x94,0x8f,0x60,0x6c,0x50,0x68,
		0xa1,0x5c,0x77,0xc9,0xde,0xb5,0x46,0xfe,0x8a,0x8c,0x96,0xf5,0xbd,0xee,0xe6,0xce,
		0x56,0xb7,0x5c,0x20,0xba,0x7e,0xe6,0x64,0x79,0x75,0x4a,0x25,0x79,0xb8,0x9e,0x21,
		0xc0,0x6f,0x4e,0x3b,0xa1,0xa6,0x15,0xc0,0x3d,0x85,0x50,0x0b,0xfb,0x60,0x84,0x5e,
		0x88,0x42,0x23,0x21,0x08,0x77,0x5a,0x69,0xcf,0x31,0x9d,0xb6,0x29,0x2b,0xf2,0xd4,
		0xee,0xe5,0x49,0x68,0xb1,0x95,0xa5,0x71,0x4e,0xdd,0xc8,0x3d,0x4e,0xa7,0x01,0x3b,
		0x4b,0x9c,0x95,0x1e,0xa6,0x24,0x6b,0x5b,0xe4,0x1d,0xf4,0x95,0x01,0xc4,0xfc,0x75,
		0xac,0xcd,0x37,0x53,0x49,0xbf,0x8d,0x08,0x5a,0xf5,0x00,0x2d,0xd4,0x80,0x29,0xed,
		0x97,0xcf,0xb5,0x8c,0x16,0x94,0x36,0x3d,0x1b,0xa7,0xcb,0x41,0x53,0xf0,0x02,0x07,
		0x75,0xbf,0x5f,0xe4,0xe1,0xbe,0xdd,0x6d,0xe1,0x63,0x6c,0x8e,0x0d,0x14,0x44,0x8e,
		0x04,0x55,0xe0,0x48,0x13,0x64,0x39,0x3d,0xf6,0xac,0xcb,0xaf,0x7b,0x2b,0x25,0xc1
};

unsigned char Rsa_SelfCheck_P[128] = {
		0xF7,0x07,0x8E,0x22,0x60,0x1D,0x69,0xBA,0x31,0x1C,0x49,0x23,0x2E,0x72,0x3D,0xA0,
		0xC3,0x14,0x3B,0xC2,0xCA,0x87,0x82,0xB5,0xB7,0x0D,0x7E,0x5F,0xD4,0x53,0x23,0xFC,
		0xBB,0xA6,0x74,0xA4,0x91,0xD3,0x60,0xA4,0x32,0x69,0x70,0x70,0xB8,0x64,0x21,0xED,
		0x8F,0x6B,0x3B,0x2C,0x54,0x18,0x58,0xB2,0x79,0x49,0xEE,0x11,0x3B,0xF9,0x7A,0x74,
		0x58,0xF2,0x26,0x3E,0xFB,0xF2,0xC2,0x84,0xFE,0x28,0xE1,0xE2,0xD3,0xC6,0x59,0xE6,
		0x1F,0xE8,0x19,0x5E,0x89,0xF2,0x47,0xC6,0x60,0x65,0xA0,0xEA,0x8F,0x50,0x24,0x07,
		0x4D,0xA0,0xBC,0xC1,0x5D,0x29,0xF2,0xA1,0x9A,0x1E,0x1E,0x0B,0xF0,0x55,0x96,0xEF,
		0x19,0x54,0x57,0x92,0x63,0x16,0x5D,0x87,0xEF,0x7A,0xFE,0x78,0xE4,0x0F,0x3D,0xD1
};

unsigned char Rsa_SelfCheck_Q[128] = {
		0xED,0x35,0xDF,0x5C,0x2A,0x28,0xD3,0x3E,0x8A,0x0A,0x38,0x5C,0x5A,0x93,0xAB,0x17,
		0xA4,0xFF,0x67,0x89,0x0B,0xCC,0x8C,0x76,0xA2,0x7C,0x19,0xA1,0xAF,0x87,0x0A,0x20,
		0x74,0xB4,0xCE,0x14,0xD0,0x62,0xF8,0xB7,0x2D,0xDC,0x72,0x3B,0x88,0xBA,0x8A,0xCD,
		0xCB,0x39,0xDC,0x4C,0x6F,0x99,0x53,0x51,0x86,0xE1,0x6F,0x99,0x74,0xDD,0xD3,0x1B,
		0x9D,0x00,0x66,0x95,0x2E,0x1B,0xA4,0x8E,0x67,0x2B,0x24,0x4B,0xFD,0x30,0x9C,0x9A,
		0x7D,0xB7,0xBB,0x75,0x2C,0x2D,0x51,0x46,0x42,0xE3,0x68,0x9F,0x71,0xE2,0xAC,0xE0,
		0x6F,0x61,0x6B,0xC8,0xA7,0x12,0xFC,0x9B,0xBD,0xA0,0x95,0x67,0x28,0x8A,0x99,0x8F,
		0xA2,0x49,0x02,0xB9,0x62,0x24,0xC4,0xCF,0xA3,0x14,0xB6,0xF0,0xF6,0x97,0x6E,0x2D
};

unsigned char Rsa_SelfCheck_QINV[128] = {
		0x46,0xE3,0x69,0xF0,0xE1,0x37,0x6E,0x9E,0xFA,0x76,0x30,0x7E,0xB8,0x62,0x74,0x77,
		0xB3,0x66,0x29,0xC3,0x61,0xDD,0xAA,0x79,0xC2,0xAC,0xDD,0x07,0x47,0x7D,0x11,0x17,
		0x2A,0x26,0xB2,0xEA,0x26,0xD6,0x0F,0x79,0x14,0x1A,0x81,0x9F,0x8C,0x56,0xCF,0x52,
		0xFC,0xC9,0xEE,0xC4,0x1E,0x3E,0x4D,0x34,0xB0,0x07,0x98,0x55,0xD2,0x84,0xA4,0xBB,
		0x5F,0xE3,0x1C,0x50,0x60,0xE9,0xD0,0xB0,0x9F,0x11,0x81,0x98,0xA3,0xBE,0x50,0xDB,
		0x18,0xA9,0x34,0xD9,0x70,0xA8,0x60,0xDD,0x9E,0xD4,0x0A,0x55,0xEE,0x1C,0x97,0x10,
		0xF0,0x74,0x23,0x1C,0x2D,0xAD,0x5C,0x9A,0x0C,0x69,0x29,0xD0,0x88,0x89,0xAA,0x58,
		0xB0,0xC0,0xD9,0x83,0x86,0x24,0x5A,0xDA,0x82,0x07,0x74,0x3D,0xC9,0x94,0x2C,0x3A
};

unsigned char Rsa_SelfCheck_DP[128] = {
		0x5F,0x29,0xF2,0xF0,0x7D,0x34,0x77,0x89,0x79,0xE7,0xDA,0xA9,0x09,0xF8,0x8A,0x93,
		0x46,0xF3,0xA1,0x7E,0x0E,0x05,0x9C,0x2E,0x22,0xC1,0xB9,0x00,0x73,0xCC,0x3F,0x9D,
		0xDA,0x30,0x6A,0xD9,0x07,0x30,0x76,0x17,0x6B,0x30,0x41,0x57,0xBB,0x41,0x1F,0xEC,
		0xE3,0x86,0x3C,0xBA,0x7B,0xFF,0x11,0x14,0x58,0x33,0x49,0xDA,0x2B,0xCF,0x95,0x04,
		0x03,0x4B,0xED,0xBD,0xEF,0xD3,0x7A,0x6A,0x3F,0x7B,0x42,0x61,0xDE,0x91,0x98,0x0A,
		0x56,0x1A,0x54,0xC3,0x9D,0xC2,0xCF,0x31,0xB5,0xEB,0x10,0xCD,0xAB,0xBE,0x04,0xD3,
		0x27,0x6D,0x97,0xA9,0xAA,0x19,0x41,0xD6,0x4D,0x87,0xF2,0xAD,0x48,0xBD,0xC0,0x32,
		0x86,0x00,0x7E,0xE7,0xEE,0xF2,0x2B,0xBF,0xF5,0x5A,0xDD,0x73,0x76,0xF0,0x04,0x31
};

unsigned char Rsa_SelfCheck_DQ[128] = {
		0x6F,0xE5,0x80,0xD6,0xD6,0x20,0x69,0xFD,0xCF,0x5A,0x20,0xDA,0x17,0xF4,0x1E,0xBA,
		0x49,0xA0,0x1D,0x73,0xFD,0x77,0xE1,0x2E,0xB9,0x59,0x37,0x32,0x19,0x3D,0xE7,0x5E,
		0x06,0x0F,0xA0,0x43,0x1B,0x46,0x04,0xB3,0x4C,0x53,0xF8,0x22,0xF3,0x4D,0xA3,0xDA,
		0x8E,0x2A,0x5E,0x2C,0x44,0x5C,0x9C,0x7C,0x11,0xD7,0x58,0x30,0x98,0x7B,0x67,0x71,
		0x58,0x2F,0x6F,0x34,0xA2,0x0A,0x72,0x26,0xA6,0xAE,0x28,0x32,0x1C,0x5E,0x89,0x11,
		0x2B,0x8A,0xB7,0x47,0xFF,0xAD,0xE4,0xEF,0xF2,0xEF,0x99,0x53,0xD5,0x82,0x2B,0x30,
		0xB7,0xF9,0xC9,0xC1,0x5E,0x66,0xCB,0x78,0xA0,0x0C,0x17,0x4E,0x11,0x46,0xC8,0x47,
		0x84,0x33,0x52,0xB0,0xA6,0x86,0x57,0xBC,0xE6,0xDE,0x07,0xAA,0x45,0x78,0x8B,0x85
};

unsigned char Rsa_SelfCheck_Data[256] = {
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00
};

unsigned char Rsa_SelfCheck_Cipher[256] = {
		0x11,0x66,0xB1,0x44,0x44,0xF4,0x7B,0x4F,0x4A,0x28,0xB2,0x0B,0x0E,0x69,0xE4,0xB6,
		0x38,0xDF,0xA2,0x74,0xAC,0x66,0xC9,0x85,0x49,0x49,0x09,0x9C,0xFD,0xF8,0xFF,0x79,
		0x71,0x8E,0x62,0xC3,0x8F,0xF3,0xB1,0x50,0x66,0x17,0x95,0x84,0x08,0x96,0x97,0x5B,
		0x1E,0x46,0x85,0xB1,0x21,0x52,0xAA,0xA8,0xDA,0x96,0xCE,0x4A,0x0E,0x01,0xBC,0xCB,
		0x21,0xD5,0xBF,0xB3,0xDD,0xFA,0xD4,0x71,0xBC,0x29,0x3B,0x7F,0xC3,0x90,0x2A,0x53,
		0xC9,0x66,0xD7,0x71,0xA5,0xC4,0xCE,0x41,0xD4,0xF0,0x16,0x86,0x01,0xAB,0xC6,0x72,
		0xC0,0x82,0xF6,0xE0,0xD4,0xD2,0xDD,0x72,0xFD,0xCE,0x94,0xB7,0x8D,0x9E,0xCB,0x1A,
		0x97,0x78,0x06,0xBE,0xCE,0xE5,0x4C,0xEB,0x42,0xA9,0x02,0xAA,0x83,0xA0,0x2D,0xB6,
		0x7E,0x3D,0xBD,0xB0,0x38,0x6D,0x93,0x50,0x16,0xBF,0x49,0x93,0xA6,0xD3,0x3B,0x18,
		0xDF,0x2B,0x6C,0x48,0xE8,0xA7,0x18,0xF4,0x37,0xF1,0x43,0xB7,0x37,0xB0,0x87,0x9E,
		0x3B,0xE1,0x29,0x00,0x3C,0xD1,0x43,0xB3,0x9C,0xE9,0x47,0xE9,0xF2,0x18,0xF5,0xB3,
		0xC9,0x47,0x8C,0xC1,0x8F,0x12,0x86,0x8A,0x1D,0xAC,0xEB,0x7E,0xFF,0x6E,0x3D,0x62,
		0x44,0xC9,0x58,0x84,0x4B,0xEA,0x30,0x1B,0x4B,0xFE,0x4E,0x1A,0xD4,0xF7,0x62,0x94,
		0xB9,0xC9,0x45,0xB4,0x33,0xB5,0xDA,0x4B,0x41,0x53,0x67,0x25,0xBC,0x48,0xF3,0xB7,
		0x62,0x6B,0xF2,0x06,0x38,0x88,0x21,0x13,0x34,0x1D,0x74,0x0A,0x28,0x12,0x1A,0xED,
		0x07,0xE4,0x46,0x37,0x93,0x13,0x07,0xD4,0xC1,0xC7,0x76,0x99,0xC5,0x5C,0xE8,0x4C
};

int RAND_pseudo_bytes(unsigned char *buf, int num)
{
	int i;

	for(i=0; i<num; i++)
	{
		buf[i] = rand() & 0xff;
	}

	return 0;
}

int memcmp_back(unsigned char *src,unsigned char *dst,int size)
{
	int i;
	for(i=size-1;i<0;i--)
	{
		if(*src > *dst)
			return 1;
		else if(*src < *dst)
			return -1;
		else
		{
			src--;
			dst--;
		}
	}
	return 0;
}

void RsaPubKey(unsigned char *to, unsigned int *tlen, unsigned char *from, unsigned int flen, struct rsa_param *rsa)
{
	BIGNUM *f, *r;
	unsigned int num = rsa->n->dmax*4, i, j;

	r = BN_new();
	r->d = (BN_ULONG *)OPENSSL_malloc(num);
	r->dmax = (num+3)/4;

	f = BN_new();
	f->d = (BN_ULONG *)OPENSSL_malloc(num);
	f->dmax = (num+3)/4;

	for(i=0; i<flen; i++)
	{
		((unsigned char *)f->d)[num-i-1] = from[flen-i-1];
	}

	f->top = (num+3)/4;
	bn_correct_top(f);

//	pkha_fp_mod_exp((unsigned char *)f->d, flen, (unsigned char *)rsa->e->d, rsa->e->top*4, (unsigned char *)rsa->n->d, rsa->n->top*4, (unsigned char *)r->d);

	unsigned int *desc,*desc_raw;
	desc_raw = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DATA_ALIGN);
	desc = (unsigned int *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);

	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->n->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)f->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
//	append_key(desc, E_dma, E_size, CLASS_1 | KEY_DEST_PKHA_E);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->e->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);
	append_fifo_store(desc, virt_to_phys((unsigned char *)r->d), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	run_descriptor_jr(desc, &jr_t);

	*tlen = j = num;

	for(i=0; i<num; i++)
	{
		if(((unsigned char *)r->d)[i] != 0)
		{
			*tlen = num-i;
			break;
		}
	}

	for(i = 0; i < *tlen; i++)
		to[i] = ((unsigned char *)r->d)[i+num-*tlen];

	free(desc_raw);
	BN_free(f);
	BN_free(r);
}

void RsaPubKey_LIT(unsigned char *to, unsigned int *tlen, unsigned char *from, unsigned int flen, struct rsa_param *rsa)
{
	BIGNUM *f, *r;
	unsigned int num = rsa->n->dmax*4, i, j;

	r = BN_new();
	r->d = (BN_ULONG *)OPENSSL_malloc(num);
	r->dmax = (num+3)/4;

	f = BN_new();
	f->d = (BN_ULONG *)OPENSSL_malloc(num);
	f->dmax = (num+3)/4;

	for(i=0; i<flen; i++)
	{
		//((unsigned char *)f->d)[num-i-1] = from[flen-i-1];
		((unsigned char *)f->d)[i] = from[i];
	}

	f->top = (num+3)/4;
	bn_correct_top(f);

//	pkha_fp_mod_exp((unsigned char *)f->d, flen, (unsigned char *)rsa->e->d, rsa->e->top*4, (unsigned char *)rsa->n->d, rsa->n->top*4, (unsigned char *)r->d);

	unsigned int *desc,*desc_raw;
	desc_raw = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DATA_ALIGN);
	desc = (unsigned int *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);

	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->n->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys((unsigned char *)f->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
//	append_key(desc, E_dma, E_size, CLASS_1 | KEY_DEST_PKHA_E);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->e->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);
	append_fifo_store(desc, virt_to_phys((unsigned char *)r->d), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);

	*tlen = j = num;

	for(i=0; i<num; i++)
	{
		if(((unsigned char *)r->d)[j-1-i] != 0)
		{
			*tlen = num-i;
			break;
		}
	}

	for(i = 0; i < num; i++)
		to[i] = ((unsigned char *)r->d)[i];

	free(desc_raw);
	BN_free(f);
	BN_free(r);
}

/*
1. the original function is
void rsa_private_decrypt(struct rsa_param *rsa, unsigned int flen, unsigned char *from, unsigned char *to)
2. whether need to delete
    for (j = 0; j < (num - i); j++)
        to[j] = 0;
*/
void RsaPrivKey(unsigned char *to, unsigned int *tlen, unsigned char *from, unsigned int flen, struct rsa_param *rsa)
{
	BIGNUM *f, *r;
	unsigned int num = rsa->n->dmax*4, i, j;

	r = BN_new();
	r->d = (BN_ULONG *)OPENSSL_malloc(num);
	r->dmax = (num+3)/4;

	f = BN_new();
	f->d = (BN_ULONG *)OPENSSL_malloc(num);
	f->dmax = (num+3)/4;

	for(i=0; i<flen; i++)
	{
		((unsigned char *)f->d)[num-i-1] = from[flen-i-1];
	}

	f->top = (num+3)/4;
	bn_correct_top(f);

//	pkha_fp_mod_exp((unsigned char *)f->d, flen, (unsigned char *)rsa->e->d, rsa->e->top*4, (unsigned char *)rsa->n->d, rsa->n->top*4, (unsigned char *)r->d);

	unsigned int *desc,*desc_raw;
	desc_raw = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DATA_ALIGN);
	desc = (unsigned int *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);

	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->n->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)f->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
//	append_key(desc, E_dma, E_size, CLASS_1 | KEY_DEST_PKHA_E);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->d->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);
	append_fifo_store(desc, virt_to_phys((unsigned char *)r->d), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	run_descriptor_jr(desc, &jr_t);

	*tlen = j = num;

	for(i=0; i<num; i++)
	{
		if(((unsigned char *)r->d)[i] != 0)
		{
			*tlen = num-i;
			break;
		}
	}

	for(i = 0; i < *tlen; i++)
		to[i] = ((unsigned char *)r->d)[i+num-*tlen];

	free(desc_raw);
	BN_free(f);
	BN_free(r);
}

void RsaPrivKey_LIT(unsigned char *to, unsigned int *tlen, unsigned char *from, unsigned int flen, struct rsa_param *rsa)
{
	BIGNUM *f, *r;
	unsigned int num = rsa->n->dmax*4, i, j;

	r = BN_new();
	r->d = (BN_ULONG *)OPENSSL_malloc(num);
	r->dmax = (num+3)/4;

	f = BN_new();
	f->d = (BN_ULONG *)OPENSSL_malloc(num);
	f->dmax = (num+3)/4;

	for(i=0; i<flen; i++)
	{
//		((unsigned char *)f->d)[num-i-1] = from[flen-i-1];
		((unsigned char *)f->d)[i] = from[i];
	}

	f->top = (num+3)/4;
	bn_correct_top(f);

//	pkha_fp_mod_exp((unsigned char *)f->d, flen, (unsigned char *)rsa->e->d, rsa->e->top*4, (unsigned char *)rsa->n->d, rsa->n->top*4, (unsigned char *)r->d);

	unsigned int *desc,*desc_raw;
	desc_raw = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DATA_ALIGN);
	desc = (unsigned int *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);

	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->n->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys((unsigned char *)f->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
//	append_key(desc, E_dma, E_size, CLASS_1 | KEY_DEST_PKHA_E);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->d->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);
	append_fifo_store(desc, virt_to_phys((unsigned char *)r->d), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);

	*tlen = j = num;
	for(i=0; i<num; i++)
	{
		if(((unsigned char *)r->d)[j-1-i] != 0)
		{
			*tlen = num-i;
			break;
		}
	}

	for(i = 0; i < num; i++)
		to[i] = ((unsigned char *)r->d)[i];

	free(desc_raw);
	BN_free(f);
	BN_free(r);
}

void RsaPrivKeyCRT(unsigned char *to, unsigned int *tlen, unsigned char *from, unsigned int flen, struct rsa_param *rsa)
{
	BIGNUM *f, *r, *m1, *m2, *h;
	unsigned int num = rsa->n->dmax*4, num_half = rsa->n->dmax*2, i, j, *jump_cmd_now;

	r = BN_new();
	r->d = (BN_ULONG *)OPENSSL_malloc(num);
	r->dmax = (num+3)/4;

	f = BN_new();
	f->d = (BN_ULONG *)OPENSSL_malloc(num);
	f->dmax = (num+3)/4;

	m1 = BN_new();
	m1->d = (BN_ULONG *)OPENSSL_malloc(num);
	m1->dmax = (num+3)/4;

	m2 = BN_new();
	m2->d = (BN_ULONG *)OPENSSL_malloc(num);
	m2->dmax = (num+3)/4;

	h = BN_new();
	h->d = (BN_ULONG *)OPENSSL_malloc(num);
	h->dmax = (num+3)/4;

	for(i=0; i<flen; i++)
	{
		((unsigned char *)f->d)[num-i-1] = from[flen-i-1];
	}

	f->top = (num+3)/4;
	bn_correct_top(f);

	unsigned int *desc,*desc_raw;
	desc_raw = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DATA_ALIGN);
	desc = (unsigned int *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

//	memcpy_c(r2, key1_r2, blen);
	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);

	//m1 = c exp dP mod P
	//first step: c exp dP mod N ->  B
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->n->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)f->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->dmp1->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N10 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

//	append_fifo_store(desc, virt_to_phys(m1), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
	//second step: B mod P -> m1
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
//	append_fifo_store(desc, virt_to_phys(m1), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
	append_fifo_store(desc, virt_to_phys((unsigned char *)m1->d), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	//m2 = c exp dQ mod Q
	//first step: c exp dQ mod N -> B
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->n->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)f->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->dmq1->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	//second step: B mod Q -> m2
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->q->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_fifo_store(desc, virt_to_phys((unsigned char *)m2->d), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	//h = qInv*(m1-m2) mod P
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)m1->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
	append_fifo_store(desc, virt_to_phys((unsigned char *)h->d), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);	//h' = m1-m2

	jump_cmd_now = append_jump(desc,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc, jump_cmd_now);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc, &num_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_N_SZ);
	append_load_as_imm(desc, &num_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_A_SZ);
	append_load_as_imm(desc, &num_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d+num_half), num_half, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->iqmp->d+num_half), num_half, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)h->d+num_half), num_half, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
//	append_fifo_store(desc, virt_to_phys(h), blen_tmp, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N30 | OP_ALG_PKMODE_CPYMEM_N_SZ);

	//m = m2 + h*q
	jump_cmd_now = append_jump(desc,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc, jump_cmd_now);

	//we know the h must be smaller than R2, so choose R2 to be the N
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->n->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->q->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
//	append_load_as_imm(desc, &blen, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
//	append_fifo_load(desc, virt_to_phys(h), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N30 | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_load_as_imm(desc, &num, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
//	append_fifo_load(desc, virt_to_phys(h), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
//	append_fifo_store(desc, virt_to_phys(tmp), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);	//h*q

	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->n->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)m2->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);
	append_fifo_store(desc, virt_to_phys((unsigned char *)r->d), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	run_descriptor_jr(desc, &jr_t);

	*tlen = j = num;

	for(i=0; i<num; i++)
	{
		if(((unsigned char *)r->d)[i] != 0)
		{
			*tlen = num-i;
			break;
		}
	}

	for(i = 0; i < *tlen; i++)
		to[i] = ((unsigned char *)r->d)[i+num-*tlen];

	free(desc_raw);
	BN_free(f);
	BN_free(r);
	BN_free(m1);
	BN_free(m2);
	BN_free(h);
}

void RsaPrivKeyCRT_LIT(unsigned char *to, unsigned int *tlen, unsigned char *from, unsigned int flen, struct rsa_param *rsa)
{
	BIGNUM *f, *r, *m1, *m2, *h;
	unsigned int num = rsa->n->dmax*4, num_half = rsa->n->dmax*2, i, j, *jump_cmd_now;

	r = BN_new();
	r->d = (BN_ULONG *)OPENSSL_malloc(num);
	r->dmax = (num+3)/4;

	f = BN_new();
	f->d = (BN_ULONG *)OPENSSL_malloc(num);
	f->dmax = (num+3)/4;

	m1 = BN_new();
	m1->d = (BN_ULONG *)OPENSSL_malloc(num);
	m1->dmax = (num+3)/4;

	m2 = BN_new();
	m2->d = (BN_ULONG *)OPENSSL_malloc(num);
	m2->dmax = (num+3)/4;

	h = BN_new();
	h->d = (BN_ULONG *)OPENSSL_malloc(num);
	h->dmax = (num+3)/4;

	for(i=0; i<flen; i++)
	{
//		((unsigned char *)f->d)[num-i-1] = from[flen-i-1];
		((unsigned char *)f->d)[i] = from[i];
	}

	f->top = (num+3)/4;
	bn_correct_top(f);

	unsigned int *desc,*desc_raw;
	desc_raw = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DATA_ALIGN);
	desc = (unsigned int *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

//	memcpy_c(r2, key1_r2, blen);
	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);

	//m1 = c exp dP mod P
	//first step: c exp dP mod N ->  B
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->n->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys((unsigned char *)f->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->dmp1->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N10 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

//	append_fifo_store(desc, virt_to_phys(m1), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
	//second step: B mod P -> m1
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N10 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
//	append_fifo_store(desc, virt_to_phys(m1), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
	append_fifo_store(desc, virt_to_phys((unsigned char *)m1->d), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	//m2 = c exp dQ mod Q
	//first step: c exp dQ mod N -> B
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->n->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys((unsigned char *)f->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->dmq1->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_E | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_EXPO);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_SRC_SZ);

	//second step: B mod Q -> m2
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->q->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A | OP_ALG_PKMODE_CPYMEM_SRC_SZ);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_fifo_store(desc, virt_to_phys((unsigned char *)m2->d), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	//h = qInv*(m1-m2) mod P
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys((unsigned char *)m1->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
	append_fifo_store(desc, virt_to_phys((unsigned char *)h->d), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);	//h' = m1-m2

	jump_cmd_now = append_jump(desc,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc, jump_cmd_now);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc, &num_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_N_SZ);
	append_load_as_imm(desc, &num_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_A_SZ);
	append_load_as_imm(desc, &num_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d), num_half, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->iqmp->d), num_half, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_fifo_load(desc, virt_to_phys((unsigned char *)h->d), num_half, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
//	append_fifo_store(desc, virt_to_phys(h), blen_tmp, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N30 | OP_ALG_PKMODE_CPYMEM_N_SZ);

	//m = m2 + h*q
	jump_cmd_now = append_jump(desc,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc, jump_cmd_now);

	//we know the h must be smaller than R2, so choose R2 to be the N
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->n->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->q->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
//	append_load_as_imm(desc, &blen, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
//	append_fifo_load(desc, virt_to_phys(h), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N30 | OP_ALG_PKMODE_DST_REG_B | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_load_as_imm(desc, &num, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
//	append_fifo_load(desc, virt_to_phys(h), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
//	append_fifo_store(desc, virt_to_phys(tmp), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);	//h*q

	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->n->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys((unsigned char *)m2->d), num, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_ADD);
	append_fifo_store(desc, virt_to_phys((unsigned char *)r->d), num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	run_descriptor_jr(desc, &jr_t);

	*tlen = j = num;
	for(i=0; i<num; i++)
	{
		if(((unsigned char *)r->d)[j-1-i] != 0)
		{
			*tlen = num-i;
			break;
		}
	}

	for(i = 0; i < num; i++)
		to[i] = ((unsigned char *)r->d)[i];

	free(desc_raw);
	BN_free(f);
	BN_free(r);
	BN_free(m1);
	BN_free(m2);
	BN_free(h);
}

/*
 * RsaGenerateKeyPair_move_len is for move_len test,
 * here can't succeed when 4096 rsa, for move_len's maximal size is 256byte
 */
void RsaGenerateKeyPair_move_len(struct rsa_param *rsa, unsigned int bits)
{
	BIGNUM *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL, *r5 = NULL ,*r6 = NULL, *r7 = NULL, *r8 = NULL, *r9 = NULL, *tmp = NULL;
	unsigned int blen = (bits+7)/8, blen_half = (bits+7)/16, i, *jump_cmd_pri_p, *jump_cmd_pri_q, *jump_cmd_gcd, *jump_cmd_cmp, *jump_cmd_now;
	unsigned int e_num, wlen = (blen+3)/4;

	unsigned int *rng_seed = (unsigned int *)OPENSSL_malloc(32*2);
	for(i=0; i<8; i++)
		rng_seed[i] = rand();

	r0 = BN_new();
	r0->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r0->dmax = wlen;
	r1 = BN_new();
	r1->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r1->dmax = wlen;
	r2 = BN_new();
	r2->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r2->dmax = wlen;
	r3 = BN_new();
	r3->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r3->dmax = wlen;
	r4 = BN_new();
	r4->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r4->dmax = wlen;
	r5 = BN_new();
	r5->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r5->dmax = wlen;
	r6 = BN_new();
	r6->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r6->dmax = wlen;
	r7 = BN_new();
	r7->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r7->dmax = wlen;
	r8 = BN_new();
	r8->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r8->dmax = wlen;
	r9 = BN_new();
	r9->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r9->dmax = wlen;
	tmp = BN_new();
	tmp->d = (BN_ULONG *)OPENSSL_malloc(blen);
	tmp->dmax = wlen;

	/* set r0, for prime test */
	r0->dmax = wlen;
	r0->top = blen/4;
	r0->flags = BN_FLG_MALLOCED;
	memset(r0->d, 0, wlen);

	/* set r1, the value is 5, for prime test */
	r1->dmax = wlen;
	r1->top = 1;
	r1->flags = BN_FLG_MALLOCED;
	memset(r1->d, 0, wlen);
	((unsigned char *)r1->d)[blen/2-1] = 0x05;

	memcpy_c((unsigned char *)r2->d, key1_r2, blen);
	r2->top = r2->dmax = wlen;

    /* set tmp, the high bit is 1,other is 0, for P Q bit test */
	tmp->dmax = wlen;
	tmp->top = wlen;
	tmp->flags = BN_FLG_MALLOCED;
	memset(tmp->d, 0, wlen);
	((unsigned char *)tmp->d)[0] = 0xC0;

	/* set r5, the value is 2. the bottom of rand_p and rand_q is 1(odd) */
	r5->dmax = wlen;
	r5->top = 1;
	r5->flags = BN_FLG_MALLOCED;
	memset(r5->d, 0, wlen);
	((unsigned char *)r5->d)[blen/2-1] = 0x02;

	/* set r6, the value is 1, for gcd (q-1) and (p-1) */
	r6->dmax = wlen;
	r6->top = 1;
	r6->flags = BN_FLG_MALLOCED;
	memset(r6->d, 0, wlen);
	((unsigned char *)r6->d)[blen/2-1] = 0x01;

#if RSA_RAND_E
//	BN_rand(rsa->e, e_bits_random, 0, 1);
	if(e_bits_random % 8 == 0)
		e_num = e_bits_random/8;
	else
		e_num = e_bits_random/8 + 1;
#endif


#ifndef	RSA_RAND_P_Q
    memcpy_c((unsigned char *)rsa->p->d+128, Rsa_SelfCheck_P, 128);
    memcpy_c((unsigned char *)rsa->q->d+128, Rsa_SelfCheck_Q, 128);
#endif

#ifndef RSA_RAND_E
    for(i=0; i<3; i++)
    {
    	((unsigned char *)rsa->e->d)[blen-1-i] = Rsa_SelfCheck_E[i];
    }
//    memcpy_c((unsigned char *)rsa->e->d, Rsa_SelfCheck_E, 3);
#endif

    /* begin of desc */
    unsigned int *desc,*desc_raw;
	desc_raw = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DESC_ALIGN);
	desc = (unsigned int *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
    /* desc_expand */
    unsigned int *desc_exp,*desc_raw_exp;
	desc_raw_exp = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DESC_ALIGN);
	desc_exp = (unsigned int *)(((unsigned long)desc_raw_exp+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	/* desc_expand_ex */
    unsigned int *desc_exp_ex,*desc_raw_exp_ex;
	desc_raw_exp_ex = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DESC_ALIGN);
	desc_exp_ex = (unsigned int *)(((unsigned long)desc_raw_exp_ex+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	/* desc_crt */
    unsigned int *desc_crt,*desc_raw_crt;
    desc_raw_crt = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DESC_ALIGN);
	desc_crt = (unsigned int *)(((unsigned long)desc_raw_crt+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	/* init desc */
	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load(desc, virt_to_phys(rng_seed), 32, LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);

//r0 from RNG
	append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc, blen/4);	//here the size of RNG should same with the size of data in the following fifo_store
	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
	append_fifo_store(desc, virt_to_phys((unsigned char *)r0->d + blen/4), blen/4, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE | FIFOST_CLASS_SWAP);

	jump_cmd_pri_p = desc_end(desc);
#ifdef RSA_RAND_P_Q
	append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc, blen/2);	//here the size of RNG should same with the size of data in the following fifo_store
	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);

//different here with RsaGenerateKeyPair, RNG data to RNGFIFO, and then to P by move_len
	append_seq_fifo_store(desc, blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGFIFO);
	append_load_as_imm(desc, &blen_half, 4, LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH0);
	append_move_len(desc, MOVE_WAITCOMP | MOVE_SRC_OUTFIFO | MOVE_DEST_PK_A | 0);			//move_len for the maximal size is 256bytes, so can't ust at 4096 rsa
	append_fifo_store(desc, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_A | FIFOST_CLASS_SWAP);
//	append_fifo_store(desc, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE | FIFOST_CLASS_SWAP);

	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)r5->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
	set_jump_before(jump_cmd_pri_p, jump_cmd_now);

	append_fifo_load(desc, virt_to_phys((unsigned char *)r2->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
    append_fifo_load(desc, virt_to_phys((unsigned char *)tmp->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	jump_cmd_now = append_jump(desc, JUMP_CLASS_NONE | JUMP_COND_PK_BORROW | JUMP_TEST_ALL);
	set_jump_before(jump_cmd_pri_p, jump_cmd_now);
#endif
    /* primality test of p, rsa->p is N, r0 is A, r1 is B */
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)r0->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)r1->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_PRIMALITY);
//	append_fifo_store(desc, virt_to_phys((unsigned char *)r4->d), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
	jump_cmd_now = append_jump(desc, JUMP_CLASS_NONE | JUMP_COND_PK_PRIME | JUMP_TEST_INVALL);
 	set_jump_before(jump_cmd_pri_p, jump_cmd_now);

//	run_descriptor_jr(desc, &jr_t);
    /* calculate p-1: OP_ALG_PKMODE_MOD_SUB_AB: r2 is N,  rsa->p is A, r6 is B, no E, r7 is OUT */
   	append_fifo_load(desc, virt_to_phys((unsigned char *)r2->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
   	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
   	append_fifo_load(desc, virt_to_phys((unsigned char *)r6->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
   	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
   	append_fifo_store(desc, virt_to_phys((unsigned char *)r7->d+blen/2), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N30 | OP_ALG_PKMODE_CPYMEM_N_SZ);

   	/* when e is 11, and P Q is random, GCD test if 0, regen P */
#ifndef RSA_RAND_E
	append_fifo_load(desc, virt_to_phys((unsigned char *)r7->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->e->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
	set_jump_before(jump_cmd_pri_p, jump_cmd_now);
#endif

	/* jump to desc_exp_ex */
	append_jump(desc, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	append_ptr(desc, virt_to_phys (desc_exp_ex));
	init_job_desc(desc_exp_ex, START_INDEX);

	jump_cmd_pri_q = desc_end(desc_exp_ex);
#ifdef RSA_RAND_P_Q
	append_cmd(desc_exp_ex, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc_exp_ex, blen/2);	//here the size of RNG should same with the size of data in the following fifo_store
	append_operation(desc_exp_ex,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);

	append_seq_fifo_store(desc_exp_ex, blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGFIFO);
	append_load_as_imm(desc_exp_ex, &blen_half, 4, LDST_CLASS_DECO | LDST_SRCDST_WORD_DECO_MATH0);
	append_move_len(desc_exp_ex, MOVE_WAITCOMP | MOVE_SRC_OUTFIFO | MOVE_DEST_PK_A |  0);
	append_fifo_store(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_A | FIFOST_CLASS_SWAP);
//	append_fifo_store(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE | FIFOST_CLASS_SWAP);

	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r5->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
	set_jump_before(jump_cmd_pri_q, jump_cmd_now);

	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r2->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
    append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)tmp->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	jump_cmd_now = append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_COND_PK_BORROW | JUMP_TEST_ALL);
	set_jump_before(jump_cmd_pri_q, jump_cmd_now);
#endif

    /* primality test of q, rsa->q is N, r1 is A, r3 is B */
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
    append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r0->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
    append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r1->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
   	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_PRIMALITY);
   	jump_cmd_now = append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_COND_PK_PRIME | JUMP_TEST_INVALL);
   	set_jump_before(jump_cmd_pri_q, jump_cmd_now);

    /* calculate q-1: OP_ALG_PKMODE_MOD_SUB_AB: r2 is N,  rsa->q is A, r6 is B, no E, r1 is OUT */
   	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r2->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
   	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
   	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r6->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
   	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
   	append_fifo_store(desc_exp_ex, virt_to_phys((unsigned char *)r8->d+blen/2), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
   	//store q-1 in N20
   	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_N_SZ);

#ifndef RSA_RAND_E
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r8->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->e->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
	set_jump_before(jump_cmd_pri_q, jump_cmd_now);
#endif
//	run_descriptor_jr(desc, &jr_t);
	/* when P Q random, one desc bigger than 128, JUMP to another desc */
	/* JUMP to desc2 */
	append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	append_ptr(desc_exp_ex, virt_to_phys (desc_exp));
	init_job_desc(desc_exp, START_INDEX);
//	append_load(desc_exp, virt_to_phys(rng_seed), 32, LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);
    jump_cmd_gcd = desc_end(desc_exp);
#if RSA_RAND_E
	append_cmd(desc_exp, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc_exp, e_num);	//here the size of RNG should same with the size of data in the following fifo_store
	append_operation(desc_exp,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
 	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->e->d+blen-e_num), e_num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE | FIFOST_CLASS_SWAP);
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);

	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r7->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->e->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
//	set_jump_before(jump_cmd_pri_p, jump_cmd_now);
	set_jump_before(jump_cmd_gcd, jump_cmd_now);
	/* GCD : q-1 and e */
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r8->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->e->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
//	set_jump_before(jump_cmd_pri_q, jump_cmd_now);
	set_jump_before(jump_cmd_gcd, jump_cmd_now);
#endif

#ifndef MODE_CRT
	/* make sure P > Q,if P < Q, exchange */
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r2->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
    append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	jump_cmd_now = append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_COND_PK_BORROW | JUMP_TEST_INVALL | 0x07);
//	set_jump_before(jump_cmd_cmp, jump_cmd_now);
   	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
   	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_A | FIFOST_CLASS_SWAP);
//	append_move(desc_exp,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);
//   run_descriptor_jr(desc, &jr_t);
#endif
   	/* calculate n, OP_ALG_PKMODE_MOD_MULT: r2 is N,  rsa->p is A, rsa->q is B, no E, rsa->n is OUT*/
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r2->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->n->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	/* calculate d, OP_ALG_PKMODE_MOD_MULT: r2 is N,  r0 is A, r1 is B, no E, r3 is OUT */
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r2->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r7->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r8->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)r9->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
	/* add to serial fifo_store and fifo_load r9*/
//	append_move(desc_exp,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);

	/* OP_ALG_PKMODE_MOD_INV:  r3 is N,  rsa->e is A, no B, no E, rsa->d is OUT */
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r9->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->e->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_INV);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->d->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

#ifdef MODE_CRT
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r2->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	jump_cmd_now = append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_COND_PK_BORROW | JUMP_TEST_INVALL | 0x0B);

	//if P<Q: P->A->B1->Q;	Q->B->A1->P
   	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
   	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_A | FIFOST_CLASS_SWAP);

   	//if P<Q: r7(P-1)->N30->B3->N20, so the real smaller one of PQ is in N20
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_B2 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N30 | OP_ALG_PKMODE_DST_REG_B3 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B2 | OP_ALG_PKMODE_DST_REG_N30 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B3 | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//P-1 in A3, Q-1 in A2, and now P>Q
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);

	//N30: P-1; N20: Q-1
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc_exp, &blen, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N30 | OP_ALG_PKMODE_DST_REG_A3 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A3 | OP_ALG_PKMODE_DST_REG_N | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->d->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->dmp1->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A2 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A2 | OP_ALG_PKMODE_DST_REG_N | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->d->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->dmq1->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	/* jump to desc_exp_ex */
	append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	append_ptr(desc_exp, virt_to_phys (desc_crt));
	init_job_desc(desc_crt, START_INDEX);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc_crt, &blen_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_N_SZ);
	append_load_as_imm(desc_crt, &blen_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_A_SZ);
	append_load_as_imm(desc_crt, &blen_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
	append_fifo_load(desc_crt, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_SGF | FIFOLD_IMM);
	append_fifo_load(desc_crt, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_SGF | FIFOLD_IMM);
	append_operation(desc_crt, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_INV);
	append_fifo_store(desc_crt, virt_to_phys((unsigned char *)rsa->iqmp->d+blen/2), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
#endif
	run_descriptor_jr(desc, &jr_t);

	free(desc_raw);
	free(desc_raw_exp);
	free(desc_raw_exp_ex);
	free(desc_raw_crt);

	rsa->e->top = wlen;
	bn_correct_top(rsa->e);

	rsa->n->top = wlen;
	bn_correct_top(rsa->n);

	rsa->d->top = wlen;
	bn_correct_top(rsa->d);

#ifdef MODE_CRT
	rsa->dmp1->top = wlen;
	bn_correct_top(rsa->dmp1);

	rsa->dmq1->top = wlen;
	bn_correct_top(rsa->dmq1);

	rsa->iqmp->top = wlen;
	bn_correct_top(rsa->iqmp);
#endif

	BN_free(r0);
	BN_free(r1);
	BN_free(r2);
	BN_free(r3);
	BN_free(r4);
	BN_free(r5);
	BN_free(r6);
	BN_free(r7);
	BN_free(r8);
	BN_free(r9);
	BN_free(tmp);
	free(rng_seed);
}

void RsaGenerateKeyPair(struct rsa_param *rsa, unsigned int bits)
{
	BIGNUM *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL, *r5 = NULL ,*r6 = NULL, *r7 = NULL, *r8 = NULL, *r9 = NULL, *tmp = NULL;
	unsigned int blen = (bits+7)/8, blen_half = (bits+7)/16, i, *jump_cmd_pri_p, *jump_cmd_pri_q, *jump_cmd_gcd, *jump_cmd_cmp, *jump_cmd_now;
	unsigned int e_num, wlen = (blen+3)/4;

	unsigned int *rng_seed = (unsigned int *)OPENSSL_malloc(64);
	for(i=0; i<8; i++)
		rng_seed[i] = rand();

	r0 = BN_new();
	r0->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r0->dmax = wlen;
	r1 = BN_new();
	r1->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r1->dmax = wlen;
	r2 = BN_new();
	r2->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r2->dmax = wlen;
	r3 = BN_new();
	r3->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r3->dmax = wlen;
	r4 = BN_new();
	r4->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r4->dmax = wlen;
	r5 = BN_new();
	r5->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r5->dmax = wlen;
	r6 = BN_new();
	r6->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r6->dmax = wlen;
	r7 = BN_new();
	r7->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r7->dmax = wlen;
	r8 = BN_new();
	r8->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r8->dmax = wlen;
	r9 = BN_new();
	r9->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r9->dmax = wlen;
	tmp = BN_new();
	tmp->d = (BN_ULONG *)OPENSSL_malloc(blen);
	tmp->dmax = wlen;

	/* set r0, for prime test */
	r0->dmax = wlen;
	r0->top = blen/4;
	r0->flags = BN_FLG_MALLOCED;
	memset(r0->d, 0, wlen);

	/* set r1, the value is 5, for prime test */
	r1->dmax = wlen;
	r1->top = 1;
	r1->flags = BN_FLG_MALLOCED;
	memset(r1->d, 0, wlen);
	((unsigned char *)r1->d)[blen/2-1] = 0x05;

	memcpy_c((unsigned char *)r2->d, key1_r2, blen);
	r2->top = r2->dmax = wlen;

    /* set tmp, the high bit is 1,other is 0, for P Q bit test */
	tmp->dmax = wlen;
	tmp->top = wlen;
	tmp->flags = BN_FLG_MALLOCED;
	memset(tmp->d, 0, wlen);
	((unsigned char *)tmp->d)[0] = 0xC0;

	/* set r5, the value is 2. the bottom of rand_p and rand_q is 1(odd) */
	r5->dmax = wlen;
	r5->top = 1;
	r5->flags = BN_FLG_MALLOCED;
	memset(r5->d, 0, wlen);
	((unsigned char *)r5->d)[blen/2-1] = 0x02;

	/* set r6, the value is 1, for gcd (q-1) and (p-1) */
	r6->dmax = wlen;
	r6->top = 1;
	r6->flags = BN_FLG_MALLOCED;
	memset(r6->d, 0, wlen);
	((unsigned char *)r6->d)[blen/2-1] = 0x01;

#ifdef RSA_RAND_E
//	BN_rand(rsa->e, e_bits_random, 0, 1);
	if(e_bits_random % 8 == 0)
		e_num = e_bits_random/8;
	else
		e_num = e_bits_random/8 + 1;
#endif


#ifndef	RSA_RAND_P_Q
    memcpy_c((unsigned char *)rsa->p->d+128, Rsa_SelfCheck_P, 128);
    memcpy_c((unsigned char *)rsa->q->d+128, Rsa_SelfCheck_Q, 128);
#endif

#ifndef RSA_RAND_E
    for(i=0; i<3; i++)
    {
    	((unsigned char *)rsa->e->d)[blen-1-i] = Rsa_SelfCheck_E[i];
    }
//    memcpy_c((unsigned char *)rsa->e->d, Rsa_SelfCheck_E, 3);
#endif

    /* begin of desc */
    unsigned int *desc,*desc_raw;
	desc_raw = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DESC_ALIGN);
	desc = (unsigned int *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
    /* desc_expand */
    unsigned int *desc_exp,*desc_raw_exp;
	desc_raw_exp = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DESC_ALIGN);
	desc_exp = (unsigned int *)(((unsigned long)desc_raw_exp+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	/* desc_expand_ex */
    unsigned int *desc_exp_ex,*desc_raw_exp_ex;
	desc_raw_exp_ex = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DESC_ALIGN);
	desc_exp_ex = (unsigned int *)(((unsigned long)desc_raw_exp_ex+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	/* desc_crt */
    unsigned int *desc_crt,*desc_raw_crt;
    desc_raw_crt = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DESC_ALIGN);
	desc_crt = (unsigned int *)(((unsigned long)desc_raw_crt+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	/* init desc */
	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load(desc, virt_to_phys(rng_seed), 32, LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);

//r0 from RNG
	append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc, blen/4);	//here the size of RNG should same with the size of data in the following fifo_store
//	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
	append_fifo_store(desc, virt_to_phys((unsigned char *)r0->d + blen/4), blen/4, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE | FIFOST_CLASS_SWAP);
	jump_cmd_now = append_jump(desc,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc, jump_cmd_now);

	jump_cmd_pri_p = desc_end(desc);
#ifdef RSA_RAND_P_Q
	append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc, blen/2);	//here the size of RNG should same with the size of data in the following fifo_store
//	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
	append_fifo_store(desc, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE | FIFOST_CLASS_SWAP);
	jump_cmd_now = append_jump(desc,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc, jump_cmd_now);

	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)r5->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
	set_jump_before(jump_cmd_pri_p, jump_cmd_now);

	append_fifo_load(desc, virt_to_phys((unsigned char *)r2->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
    append_fifo_load(desc, virt_to_phys((unsigned char *)tmp->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	jump_cmd_now = append_jump(desc, JUMP_CLASS_NONE | JUMP_COND_PK_BORROW | JUMP_TEST_ALL);
	set_jump_before(jump_cmd_pri_p, jump_cmd_now);
#endif
    /* primality test of p, rsa->p is N, r0 is A, r1 is B */
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)r0->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)r1->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_PRIMALITY);
//	append_fifo_store(desc, virt_to_phys((unsigned char *)r4->d), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
	jump_cmd_now = append_jump(desc, JUMP_CLASS_NONE | JUMP_COND_PK_PRIME | JUMP_TEST_INVALL);
 	set_jump_before(jump_cmd_pri_p, jump_cmd_now);

//	run_descriptor_jr(desc, &jr_t);
    /* calculate p-1: OP_ALG_PKMODE_MOD_SUB_AB: r2 is N,  rsa->p is A, r6 is B, no E, r7 is OUT */
   	append_fifo_load(desc, virt_to_phys((unsigned char *)r2->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
   	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
   	append_fifo_load(desc, virt_to_phys((unsigned char *)r6->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_VLF | FIFOLD_IMM);
   	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
   	append_fifo_store(desc, virt_to_phys((unsigned char *)r7->d+blen/2), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N30 | OP_ALG_PKMODE_CPYMEM_N_SZ);		//store P-1 at N30

   	/* when e is 11, and P Q is random, GCD test if 0, regen P */
#ifndef RSA_RAND_E
	append_fifo_load(desc, virt_to_phys((unsigned char *)r7->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->e->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
	set_jump_before(jump_cmd_pri_p, jump_cmd_now);
#endif

	/* jump to desc_exp_ex */
	append_jump(desc, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	append_ptr(desc, virt_to_phys (desc_exp_ex));
	init_job_desc(desc_exp_ex, START_INDEX);

	jump_cmd_pri_q = desc_end(desc_exp_ex);
#ifdef RSA_RAND_P_Q
	append_cmd(desc_exp_ex, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc_exp_ex, blen/2);	//here the size of RNG should same with the size of data in the following fifo_store
//	append_operation(desc_exp_ex,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
	append_operation(desc_exp_ex,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
	append_fifo_store(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE | FIFOST_CLASS_SWAP);
	jump_cmd_now = append_jump(desc_exp_ex,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp_ex, jump_cmd_now);

	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r5->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
	set_jump_before(jump_cmd_pri_q, jump_cmd_now);

	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r2->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
    append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)tmp->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	jump_cmd_now = append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_COND_PK_BORROW | JUMP_TEST_ALL);
	set_jump_before(jump_cmd_pri_q, jump_cmd_now);
#endif

    /* primality test of q, rsa->q is N, r1 is A, r3 is B */
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
    append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r0->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
    append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r1->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_VLF | FIFOLD_IMM);
   	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_PRIMALITY);
   	jump_cmd_now = append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_COND_PK_PRIME | JUMP_TEST_INVALL);
   	set_jump_before(jump_cmd_pri_q, jump_cmd_now);
	
    /* calculate q-1: OP_ALG_PKMODE_MOD_SUB_AB: r2 is N,  rsa->q is A, r6 is B, no E, r1 is OUT */
   	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r2->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
   	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
   	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r6->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_VLF | FIFOLD_IMM);
   	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
   	append_fifo_store(desc_exp_ex, virt_to_phys((unsigned char *)r8->d+blen/2), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
   	//store q-1 in N20
   	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_N_SZ);
//	run_descriptor_jr(desc, &jr_t);
#ifndef RSA_RAND_E
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r8->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->e->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
	set_jump_before(jump_cmd_pri_q, jump_cmd_now);
#endif
//	run_descriptor_jr(desc, &jr_t);
	/* when P Q random, one desc bigger than 128, JUMP to another desc */
	/* JUMP to desc2 */
	append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	append_ptr(desc_exp_ex, virt_to_phys (desc_exp));
	init_job_desc(desc_exp, START_INDEX);
//	append_load(desc_exp, virt_to_phys(rng_seed), 32, LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);
    jump_cmd_gcd = desc_end(desc_exp);
#ifdef RSA_RAND_E
	append_cmd(desc_exp, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc_exp, e_num);	//here the size of RNG should same with the size of data in the following fifo_store
//	append_operation(desc_exp,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
	append_operation(desc_exp,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
 	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->e->d+blen-e_num), e_num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE | FIFOST_CLASS_SWAP);
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);

	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r7->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->e->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
//	set_jump_before(jump_cmd_pri_p, jump_cmd_now);
	set_jump_before(jump_cmd_gcd, jump_cmd_now);
	/* GCD : q-1 and e */
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r8->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->e->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
//	set_jump_before(jump_cmd_pri_q, jump_cmd_now);
	set_jump_before(jump_cmd_gcd, jump_cmd_now);
#endif

#ifndef MODE_CRT
	/* make sure P > Q,if P < Q, exchange */
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r2->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
    append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	jump_cmd_now = append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_COND_PK_BORROW | JUMP_TEST_INVALL | 0x07);
//	set_jump_before(jump_cmd_cmp, jump_cmd_now);
   	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
   	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_A | FIFOST_CLASS_SWAP);
//	append_move(desc_exp,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);
//   run_descriptor_jr(desc, &jr_t);
#endif
   	/* calculate n, OP_ALG_PKMODE_MOD_MULT: r2 is N,  rsa->p is A, rsa->q is B, no E, rsa->n is OUT*/
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r2->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->n->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
	
//	run_descriptor_jr(desc, &jr_t);
	/* calculate d, OP_ALG_PKMODE_MOD_MULT: r2 is N,  r0 is A, r1 is B, no E, r3 is OUT */
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r2->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r7->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r8->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)r9->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	/* add to serial fifo_store and fifo_load r9*/
//	append_move(desc_exp,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);

	/* OP_ALG_PKMODE_MOD_INV:  r3 is N,  rsa->e is A, no B, no E, rsa->d is OUT */
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r9->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->e->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_INV);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->d->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
	
#ifdef MODE_CRT
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r2->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	jump_cmd_now = append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_COND_PK_BORROW | JUMP_TEST_INVALL | 0x0B);

	//if P<Q: P->A->B1->Q;	Q->B->A1->P
   	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
   	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_A | FIFOST_CLASS_SWAP);

   	//if P<Q: r7(P-1)->N30->B3->N20, so the real smaller one of PQ is in N20
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_B2 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N30 | OP_ALG_PKMODE_DST_REG_B3 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B2 | OP_ALG_PKMODE_DST_REG_N30 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B3 | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//P-1 in A3, Q-1 in A2, and now P>Q
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);

	//N30: P-1; N20: Q-1
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc_exp, &blen, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N30 | OP_ALG_PKMODE_DST_REG_A3 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A3 | OP_ALG_PKMODE_DST_REG_N | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->d->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->dmp1->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A2 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A2 | OP_ALG_PKMODE_DST_REG_N | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->d->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->dmq1->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);

	/* jump to desc_exp_ex */
	append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	append_ptr(desc_exp, virt_to_phys (desc_crt));
	init_job_desc(desc_crt, START_INDEX);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc_crt, &blen_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_N_SZ);
	append_load_as_imm(desc_crt, &blen_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_A_SZ);
	append_load_as_imm(desc_crt, &blen_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
	append_fifo_load(desc_crt, virt_to_phys((unsigned char *)rsa->p->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N | FIFOLDST_VLF | FIFOLD_IMM);
	append_fifo_load(desc_crt, virt_to_phys((unsigned char *)rsa->q->d+blen/2), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A | FIFOLDST_VLF | FIFOLD_IMM);
	append_operation(desc_crt, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_INV);
	append_fifo_store(desc_crt, virt_to_phys((unsigned char *)rsa->iqmp->d+blen/2), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B | FIFOST_CLASS_SWAP);
#endif
	run_descriptor_jr(desc, &jr_t);

	free(desc_raw);
	free(desc_raw_exp);
	free(desc_raw_exp_ex);
	free(desc_raw_crt);

	rsa->e->top = wlen;
	bn_correct_top(rsa->e);

	rsa->n->top = wlen;
	bn_correct_top(rsa->n);

	rsa->d->top = wlen;
	bn_correct_top(rsa->d);

#ifdef MODE_CRT
	rsa->dmp1->top = wlen;
	bn_correct_top(rsa->dmp1);

	rsa->dmq1->top = wlen;
	bn_correct_top(rsa->dmq1);

	rsa->iqmp->top = wlen;
	bn_correct_top(rsa->iqmp);
#endif

	BN_free(r0);
	BN_free(r1);
	BN_free(r2);
	BN_free(r3);
	BN_free(r4);
	BN_free(r5);
	BN_free(r6);
	BN_free(r7);
	BN_free(r8);
	BN_free(r9);
	BN_free(tmp);
	free(rng_seed);
}

void RsaGenerateKeyPair_LIT(struct rsa_param *rsa, unsigned int bits)
{
	BIGNUM *r0 = NULL, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL, *r5 = NULL ,*r6 = NULL, *r7 = NULL, *r8 = NULL, *r9 = NULL, *tmp = NULL;
	unsigned int blen = (bits+7)/8, blen_half = (bits+7)/16, i, *jump_cmd_pri_p, *jump_cmd_pri_q, *jump_cmd_gcd, *jump_cmd_cmp, *jump_cmd_now;
	unsigned int e_num, wlen = (blen+3)/4;

	unsigned int *rng_seed = (unsigned int *)OPENSSL_malloc(32*2);
	for(i=0; i<8; i++)
		rng_seed[i] = rand();

	r0 = BN_new();
	r0->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r0->dmax = wlen;
	r1 = BN_new();
	r1->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r1->dmax = wlen;
	r2 = BN_new();
	r2->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r2->dmax = wlen;
	r3 = BN_new();
	r3->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r3->dmax = wlen;
	r4 = BN_new();
	r4->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r4->dmax = wlen;
	r5 = BN_new();
	r5->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r5->dmax = wlen;
	r6 = BN_new();
	r6->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r6->dmax = wlen;
	r7 = BN_new();
	r7->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r7->dmax = wlen;
	r8 = BN_new();
	r8->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r8->dmax = wlen;
	r9 = BN_new();
	r9->d = (BN_ULONG *)OPENSSL_malloc(blen);
	r9->dmax = wlen;
	tmp = BN_new();
	tmp->d = (BN_ULONG *)OPENSSL_malloc(blen);
	tmp->dmax = wlen;

	/* set r0, for prime test */
	r0->dmax = wlen;
	r0->top = blen/4;
	r0->flags = BN_FLG_MALLOCED;
	memset(r0->d, 0, wlen);

	/* set r1, the value is 5, for prime test */
	r1->dmax = wlen;
	r1->top = 1;
	r1->flags = BN_FLG_MALLOCED;
	memset(r1->d, 0, wlen);
	((unsigned char *)r1->d)[0] = 0x05;

//	memcpy_c((unsigned char *)r2->d, key1_r2, blen);
	for(i=0; i<blen; i++)
	{
		((unsigned char *)r2->d)[i] = key1_r2[blen-i-1];
	}
	r2->top = r2->dmax = wlen;

    /* set tmp, the high bit is 1,other is 0, for P Q bit test */
	tmp->dmax = wlen;
	tmp->top = wlen;
	tmp->flags = BN_FLG_MALLOCED;
	memset(tmp->d, 0, wlen);
	((unsigned char *)tmp->d)[blen/2-1] = 0xC0;

	/* set r5, the value is 2. the bottom of rand_p and rand_q is 1(odd) */
	r5->dmax = wlen;
	r5->top = 1;
	r5->flags = BN_FLG_MALLOCED;
	memset(r5->d, 0, wlen);
	((unsigned char *)r5->d)[0] = 0x02;

	/* set r6, the value is 1, for gcd (q-1) and (p-1) */
	r6->dmax = wlen;
	r6->top = 1;
	r6->flags = BN_FLG_MALLOCED;
	memset(r6->d, 0, wlen);
	((unsigned char *)r6->d)[0] = 0x01;

#if RSA_RAND_E
//	BN_rand(rsa->e, e_bits_random, 0, 1);
	if(e_bits_random % 8 == 0)
		e_num = e_bits_random/8;
	else
		e_num = e_bits_random/8 + 1;
#endif


#ifndef	RSA_RAND_P_Q
//    memcpy_c((unsigned char *)rsa->p->d+128, Rsa_SelfCheck_P, 128);
//    memcpy_c((unsigned char *)rsa->q->d+128, Rsa_SelfCheck_Q, 128);
	for(i=0; i<128; i++)
	{
		((unsigned char *)rsa->p->d)[i] = Rsa_SelfCheck_P[127-i];
		((unsigned char *)rsa->q->d)[i] = Rsa_SelfCheck_Q[127-i];
	}
#endif

#ifndef RSA_RAND_E
    for(i=0; i<3; i++)
    {
    	((unsigned char *)rsa->e->d)[i] = Rsa_SelfCheck_E[i];
    }
//    memcpy_c((unsigned char *)rsa->e->d, Rsa_SelfCheck_E, 3);
#endif

    /* begin of desc */
    unsigned int *desc,*desc_raw;
	desc_raw = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DESC_ALIGN);
	desc = (unsigned int *)(((unsigned long)desc_raw+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
    /* desc_expand */
    unsigned int *desc_exp,*desc_raw_exp;
	desc_raw_exp = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DESC_ALIGN);
	desc_exp = (unsigned int *)(((unsigned long)desc_raw_exp+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	/* desc_expand_ex */
    unsigned int *desc_exp_ex,*desc_raw_exp_ex;
	desc_raw_exp_ex = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DESC_ALIGN);
	desc_exp_ex = (unsigned int *)(((unsigned long)desc_raw_exp_ex+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));
	/* desc_crt */
    unsigned int *desc_crt,*desc_raw_crt;
    desc_raw_crt = (unsigned int *)OPENSSL_malloc(MAX_CAAM_DESCSIZE*4+DESC_ALIGN);
	desc_crt = (unsigned int *)(((unsigned long)desc_raw_crt+(DESC_ALIGN-1))&(~(DESC_ALIGN-1)));

	/* init desc */
	init_job_desc(desc, START_INDEX);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_A_RAM | OP_ALG_PKMODE_B_RAM | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_E_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load(desc, virt_to_phys(rng_seed), 32, LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);

//r0 from RNG
	append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc, blen/4);	//here the size of RNG should same with the size of data in the following fifo_store
//	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
	append_fifo_store(desc, virt_to_phys((unsigned char *)r0->d), blen/4, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE);
	jump_cmd_now = append_jump(desc,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc, jump_cmd_now);

	jump_cmd_pri_p = desc_end(desc);
#ifdef RSA_RAND_P_Q
	append_cmd(desc, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc, blen/2);	//here the size of RNG should same with the size of data in the following fifo_store
//	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
	append_operation(desc,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
	append_fifo_store(desc, virt_to_phys((unsigned char *)rsa->p->d), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE);
	jump_cmd_now = append_jump(desc,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc, jump_cmd_now);

	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys((unsigned char *)r5->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
	set_jump_before(jump_cmd_pri_p, jump_cmd_now);

	append_fifo_load(desc, virt_to_phys((unsigned char *)r2->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
    append_fifo_load(desc, virt_to_phys((unsigned char *)tmp->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	jump_cmd_now = append_jump(desc, JUMP_CLASS_NONE | JUMP_COND_PK_BORROW | JUMP_TEST_ALL);
	set_jump_before(jump_cmd_pri_p, jump_cmd_now);
#endif
    /* primality test of p, rsa->p is N, r0 is A, r1 is B */
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys((unsigned char *)r0->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_fifo_load(desc, virt_to_phys((unsigned char *)r1->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_PRIMALITY);
//	append_fifo_store(desc, virt_to_phys((unsigned char *)r4->d), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
	jump_cmd_now = append_jump(desc, JUMP_CLASS_NONE | JUMP_COND_PK_PRIME | JUMP_TEST_INVALL);
 	set_jump_before(jump_cmd_pri_p, jump_cmd_now);

//	run_descriptor_jr(desc, &jr_t);
    /* calculate p-1: OP_ALG_PKMODE_MOD_SUB_AB: r2 is N,  rsa->p is A, r6 is B, no E, r7 is OUT */
   	append_fifo_load(desc, virt_to_phys((unsigned char *)r2->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
   	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->p->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
   	append_fifo_load(desc, virt_to_phys((unsigned char *)r6->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
   	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
   	append_fifo_store(desc, virt_to_phys((unsigned char *)r7->d), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N30 | OP_ALG_PKMODE_CPYMEM_N_SZ);		//store P-1 at N30

   	/* when e is 11, and P Q is random, GCD test if 0, regen P */
#ifndef RSA_RAND_E
	append_fifo_load(desc, virt_to_phys((unsigned char *)r7->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc, virt_to_phys((unsigned char *)rsa->e->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
	set_jump_before(jump_cmd_pri_p, jump_cmd_now);
#endif

	/* jump to desc_exp_ex */
	append_jump(desc, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	append_ptr(desc, virt_to_phys (desc_exp_ex));
	init_job_desc(desc_exp_ex, START_INDEX);

	jump_cmd_pri_q = desc_end(desc_exp_ex);
#ifdef RSA_RAND_P_Q
	append_cmd(desc_exp_ex, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc_exp_ex, blen/2);	//here the size of RNG should same with the size of data in the following fifo_store
//	append_operation(desc_exp_ex,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
	append_operation(desc_exp_ex,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
	append_fifo_store(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE);
	jump_cmd_now = append_jump(desc_exp_ex,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp_ex, jump_cmd_now);

	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r5->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
	set_jump_before(jump_cmd_pri_q, jump_cmd_now);

	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r2->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
    append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)tmp->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	jump_cmd_now = append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_COND_PK_BORROW | JUMP_TEST_ALL);
	set_jump_before(jump_cmd_pri_q, jump_cmd_now);
#endif

    /* primality test of q, rsa->q is N, r1 is A, r3 is B */
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
    append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r0->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
    append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r1->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
   	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_PRIMALITY);
   	jump_cmd_now = append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_COND_PK_PRIME | JUMP_TEST_INVALL);
   	set_jump_before(jump_cmd_pri_q, jump_cmd_now);

    /* calculate q-1: OP_ALG_PKMODE_MOD_SUB_AB: r2 is N,  rsa->q is A, r6 is B, no E, r1 is OUT */
   	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r2->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
   	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->q->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
   	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r6->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
   	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB);
   	append_fifo_store(desc_exp_ex, virt_to_phys((unsigned char *)r8->d), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
   	//store q-1 in N20
   	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_N_SZ);

#ifndef RSA_RAND_E
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)r8->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc_exp_ex, virt_to_phys((unsigned char *)rsa->e->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_operation(desc_exp_ex, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
	set_jump_before(jump_cmd_pri_q, jump_cmd_now);
#endif
//	run_descriptor_jr(desc, &jr_t);
	/* when P Q random, one desc bigger than 128, JUMP to another desc */
	/* JUMP to desc2 */
	append_jump(desc_exp_ex, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	append_ptr(desc_exp_ex, virt_to_phys (desc_exp));
	init_job_desc(desc_exp, START_INDEX);
//	append_load(desc_exp, virt_to_phys(rng_seed), 32, LDST_CLASS_1_CCB | LDST_SRCDST_BYTE_CONTEXT);
    jump_cmd_gcd = desc_end(desc_exp);
#if RSA_RAND_E
	append_cmd(desc_exp, CMD_LOAD|LDST_CLASS_1_CCB|LDST_IMM|LDST_SRCDST_WORD_RNGDATASZ_REG|4);
	append_cmd(desc_exp, e_num);	//here the size of RNG should same with the size of data in the following fifo_store
//	append_operation(desc_exp,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG | OP_ALG_RNG4_AI | OP_ALG_RNG4_NZB);
	append_operation(desc_exp,OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG);
 	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->e->d), e_num, FIFOST_CLASS_NORMAL | FIFOST_TYPE_RNGSTORE);
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);

	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r7->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->e->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
//	set_jump_before(jump_cmd_pri_p, jump_cmd_now);
	set_jump_before(jump_cmd_gcd, jump_cmd_now);
	/* GCD : q-1 and e */
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r8->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->e->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_GCD);
	jump_cmd_now = append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_COND_PK_GCD_1 | JUMP_TEST_INVALL);
//	set_jump_before(jump_cmd_pri_q, jump_cmd_now);
	set_jump_before(jump_cmd_gcd, jump_cmd_now);
#endif

#ifndef MODE_CRT
	/* make sure P > Q,if P < Q, exchange */
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r2->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
    append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	jump_cmd_now = append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_COND_PK_BORROW | JUMP_TEST_INVALL | 0x07);
//	set_jump_before(jump_cmd_cmp, jump_cmd_now);
   	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
   	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_A);
//	append_move(desc_exp,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);
//   run_descriptor_jr(desc, &jr_t);
#endif
   	/* calculate n, OP_ALG_PKMODE_MOD_MULT: r2 is N,  rsa->p is A, rsa->q is B, no E, rsa->n is OUT*/
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r2->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->n->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
//	run_descriptor_jr(desc, &jr_t);

	/* calculate d, OP_ALG_PKMODE_MOD_MULT: r2 is N,  r0 is A, r1 is B, no E, r3 is OUT */
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r2->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r7->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r8->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_MULT);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)r9->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
	/* add to serial fifo_store and fifo_load r9*/
//	append_move(desc_exp,MOVE_WAITCOMP|MOVE_SRC_MATH1|MOVE_DEST_MATH2|(8<<MOVE_LEN_SHIFT));
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);

	/* OP_ALG_PKMODE_MOD_INV:  r3 is N,  rsa->e is A, no B, no E, rsa->d is OUT */
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r9->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->e->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_INV);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->d->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

#ifdef MODE_CRT
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)r2->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_B);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_SUB_AB | OP_ALG_PKMODE_MOD_F2M);
	jump_cmd_now = append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_COND_PK_BORROW | JUMP_TEST_INVALL | 0x0B);

	//if P<Q: P->A->B1->Q;	Q->B->A1->P
   	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->p->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
   	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->q->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_A);

   	//if P<Q: r7(P-1)->N30->B3->N20, so the real smaller one of PQ is in N20
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_B2 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N30 | OP_ALG_PKMODE_DST_REG_B3 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B2 | OP_ALG_PKMODE_DST_REG_N30 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_B3 | OP_ALG_PKMODE_DST_REG_N20 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	//P-1 in A3, Q-1 in A2, and now P>Q
	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);

	//N30: P-1; N20: Q-1
	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc_exp, &blen, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N30 | OP_ALG_PKMODE_DST_REG_A3 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A3 | OP_ALG_PKMODE_DST_REG_N | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->d->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->dmp1->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	jump_cmd_now = append_jump(desc_exp,JUMP_CLASS_CLASS1|JUMP_TYPE_LOCAL|JUMP_TEST_ALL);
	set_jump_tgt_here(desc_exp, jump_cmd_now);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_N20 | OP_ALG_PKMODE_DST_REG_A2 | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_SRC_REG_A2 | OP_ALG_PKMODE_DST_REG_N | OP_ALG_PKMODE_CPYMEM_N_SZ);
	append_fifo_load(desc_exp, virt_to_phys((unsigned char *)rsa->d->d), blen, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_operation(desc_exp, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_REDUCT);
	append_fifo_store(desc_exp, virt_to_phys((unsigned char *)rsa->dmq1->d), blen, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);

	/* jump to desc_exp_ex */
	append_jump(desc_exp, JUMP_CLASS_NONE | JUMP_TYPE_NONLOCAL | JUMP_TEST_ALL | JUMP_DESC_EXP_128_DESC | 0);
	append_ptr(desc_exp, virt_to_phys (desc_crt));
	init_job_desc(desc_crt, START_INDEX);

	append_operation(desc, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_N_RAM | OP_ALG_PKMODE_CLEARMEM);
	append_load_as_imm(desc_crt, &blen_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_N_SZ);
	append_load_as_imm(desc_crt, &blen_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_A_SZ);
	append_load_as_imm(desc_crt, &blen_half, 4, LDST_CLASS_1_CCB | LDST_SRCDST_WORD_PKHA_B_SZ);
	append_fifo_load(desc_crt, virt_to_phys((unsigned char *)rsa->p->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_N);
	append_fifo_load(desc_crt, virt_to_phys((unsigned char *)rsa->q->d), blen/2, FIFOLD_CLASS_CLASS1 | FIFOLD_TYPE_PK_A);
	append_operation(desc_crt, OP_TYPE_PK | OP_ALG_PK | OP_ALG_PKMODE_MOD_INV);
	append_fifo_store(desc_crt, virt_to_phys((unsigned char *)rsa->iqmp->d), blen/2, FIFOST_CLASS_NORMAL | FIFOST_TYPE_PKHA_B);
#endif
	run_descriptor_jr(desc, &jr_t);

	free(desc_raw);
	free(desc_raw_exp);
	free(desc_raw_exp_ex);
	free(desc_raw_crt);

	rsa->e->top = wlen;
	bn_correct_top(rsa->e);

	rsa->n->top = wlen;
	bn_correct_top(rsa->n);

	rsa->d->top = wlen;
	bn_correct_top(rsa->d);

#ifdef MODE_CRT
	rsa->dmp1->top = wlen;
	bn_correct_top(rsa->dmp1);

	rsa->dmq1->top = wlen;
	bn_correct_top(rsa->dmq1);

	rsa->iqmp->top = wlen;
	bn_correct_top(rsa->iqmp);
#endif

	BN_free(r0);
	BN_free(r1);
	BN_free(r2);
	BN_free(r3);
	BN_free(r4);
	BN_free(r5);
	BN_free(r6);
	BN_free(r7);
	BN_free(r8);
	BN_free(r9);
	BN_free(tmp);
	free(rng_seed);
}

void rsa_3030_E_BIG()
{
	uint32_t i, j, count = 1, tmp, ret = -1, len1, len2;
	unsigned char buf1[input_bits_random/8], buf2[input_bits_random/8];
	unsigned int blen = (input_bits_random+7)/8;
	unsigned int ptext_input = 0, wlen = (blen+3)/4;

do{
	struct rsa_param *rsa = (struct rsa_param *)OPENSSL_malloc((sizeof(BN_ULONG)*8));		//ptr 64bbit

	/* We need the RSA components non-NULL */
	if (!rsa->n && ((rsa->n = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);
	if (!rsa->d && ((rsa->d = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);
	if (!rsa->e && ((rsa->e = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);
	if (!rsa->p && ((rsa->p = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);
	if (!rsa->q && ((rsa->q = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);

	rsa->p->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->p->dmax = wlen;

	rsa->q->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->q->dmax = wlen;

	rsa->e->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->e->dmax = wlen;

	rsa->n->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->n->dmax = wlen;

	rsa->d->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->d->dmax = wlen;

	/* expand the dmax of p q e n d, same the word length */
	if (bn_wexpand(rsa->p, wlen) == NULL)
		COND_TRAP(31, 0, 0);
	if (bn_wexpand(rsa->q, wlen) == NULL)
		COND_TRAP(31, 0, 0);
	if (bn_wexpand(rsa->e, wlen) == NULL)
		COND_TRAP(31, 0, 0);
	if (bn_wexpand(rsa->n, wlen) == NULL)
		COND_TRAP(31, 0, 0);
	if (bn_wexpand(rsa->d, wlen) == NULL)
		COND_TRAP(31, 0, 0);

#ifdef MODE_CRT
	if (!rsa->dmp1 && ((rsa->dmp1 = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);
	if (!rsa->dmq1 && ((rsa->dmq1 = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);
	if (!rsa->iqmp && ((rsa->iqmp = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);

	rsa->dmp1->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->dmp1->dmax = wlen;

	rsa->dmq1->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->dmq1->dmax = wlen;

	rsa->iqmp->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->iqmp->dmax = wlen;

	if (bn_wexpand(rsa->dmp1, wlen) == NULL)
		COND_TRAP(31, 0, 0);

	if (bn_wexpand(rsa->dmq1, wlen) == NULL)
		COND_TRAP(31, 0, 0);

	if (bn_wexpand(rsa->iqmp, wlen) == NULL)
		COND_TRAP(31, 0, 0);
#endif

	ptext_input = rand()%(blen-2) + 1;
//	memset(buf1, 0, blen);
//	memset(buf2, 0, blen);

#ifdef RSA_RAND_P_Q
	RsaGenerateKeyPair(rsa, input_bits_random);
#else
	RsaGenerateKeyPair(rsa, 2048);

	if(memcmp((unsigned char *)rsa->n->d, Rsa_SelfCheck_M, 256))
		COND_TRAP(31, 0, 0);
	if(memcmp((unsigned char *)rsa->d->d, Rsa_SelfCheck_D, 256))
		COND_TRAP(31, 0, 0);
#endif

#ifdef RSA_RAND_P_Q
	/* if P Q in random mode, random the original text */
	unsigned char ptext[ptext_input];
		do{
			RAND_pseudo_bytes(ptext, ptext_input);
		}while(ptext[0] == 0);
#endif

#ifdef RSA_RAND_P_Q
	RsaPubKey(buf1, &len1, ptext, ptext_input, rsa);

#ifdef MODE_CRT
	RsaPrivKeyCRT(buf2, &len2, buf1, len1, rsa);
#else
	RsaPrivKey(buf2, &len2, buf1, len1, rsa);
#endif

	for(i=0; i<len2; i++)
	{
		if(ptext[i] != buf2[i])
			COND_TRAP(31, 0, 0);
	}
#else
unsigned char tmp_check[256];

	memcpy(tmp_check, Rsa_SelfCheck_Data, 256);

	RsaPubKey(buf1, &len1, tmp_check, 2048/8, rsa);

//memcmp_back for the least byte of BIG byte stream at the last
	if(memcmp_back(buf1, Rsa_SelfCheck_Cipher, 256))
		COND_TRAP(31, 0, 0);

#ifdef MODE_CRT
	RsaPrivKeyCRT(buf2, &len2, buf1, 2048/8, rsa);
#else
	RsaPrivKey(buf2, &len2, buf1, 2048/8, rsa);
#endif

	if(memcmp_back(buf2, Rsa_SelfCheck_Data, 256))
		COND_TRAP(31, 0, 0);
#endif

#ifdef 	RSA_RAND_P_Q
#ifdef MODE_CRT
	RsaPrivKeyCRT(buf1, &len1, ptext, ptext_input, rsa);
#else
	RsaPrivKey(buf1, &len1, ptext, ptext_input, rsa);
#endif
	RsaPubKey(buf2, &len2, buf1, len1, rsa);

	if(memcmp_back(buf2, ptext, len2))
		COND_TRAP(31, 0, 0);
#endif

	BN_free(rsa->n);
	BN_free(rsa->d);
	BN_free(rsa->e);
	BN_free(rsa->p);
	BN_free(rsa->q);

#ifdef MODE_CRT
	BN_free(rsa->dmp1);
	BN_free(rsa->dmq1);
	BN_free(rsa->iqmp);
#endif

	free(rsa);
	PSPRINTF("count is: %d", count++);
	}while(1);
}

void rsa_3030_E_LIT()
{
	uint32_t i, j, count = 1, tmp, ret = -1, len1, len2;
	unsigned char buf1[input_bits_random/8], buf2[input_bits_random/8];
	unsigned int blen = (input_bits_random+7)/8;
	unsigned int ptext_input = 0, wlen = (blen+3)/4;

do{
	struct rsa_param *rsa = (struct rsa_param *)OPENSSL_malloc((sizeof(BN_ULONG)*8));	//for while use

	/* We need the RSA components non-NULL */
	if (!rsa->n && ((rsa->n = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);
	if (!rsa->d && ((rsa->d = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);
	if (!rsa->e && ((rsa->e = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);
	if (!rsa->p && ((rsa->p = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);
	if (!rsa->q && ((rsa->q = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);

	rsa->p->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->p->dmax = wlen;

	rsa->q->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->q->dmax = wlen;

	rsa->e->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->e->dmax = wlen;

	rsa->n->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->n->dmax = wlen;

	rsa->d->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->d->dmax = wlen;

	/* expand the dmax of p q e n d, same the word length */
	if (bn_wexpand(rsa->p, wlen) == NULL)
		COND_TRAP(31, 0, 0);
	if (bn_wexpand(rsa->q, wlen) == NULL)
		COND_TRAP(31, 0, 0);
	if (bn_wexpand(rsa->e, wlen) == NULL)
		COND_TRAP(31, 0, 0);
	if (bn_wexpand(rsa->n, wlen) == NULL)
		COND_TRAP(31, 0, 0);
	if (bn_wexpand(rsa->d, wlen) == NULL)
		COND_TRAP(31, 0, 0);

#ifdef MODE_CRT
	if (!rsa->dmp1 && ((rsa->dmp1 = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);
	if (!rsa->dmq1 && ((rsa->dmq1 = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);
	if (!rsa->iqmp && ((rsa->iqmp = BN_new()) == NULL))
		COND_TRAP(31, 0, 0);

	rsa->dmp1->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->dmp1->dmax = wlen;

	rsa->dmq1->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->dmq1->dmax = wlen;

	rsa->iqmp->d = (BN_ULONG *)OPENSSL_malloc(blen);
	rsa->iqmp->dmax = wlen;

	if (bn_wexpand(rsa->dmp1, wlen) == NULL)
		COND_TRAP(31, 0, 0);

	if (bn_wexpand(rsa->dmq1, wlen) == NULL)
		COND_TRAP(31, 0, 0);

	if (bn_wexpand(rsa->iqmp, wlen) == NULL)
		COND_TRAP(31, 0, 0);
#endif

	ptext_input = rand()%(blen-2) + 1;
	memset(buf1, 0, blen);
	memset(buf2, 0, blen);

#ifdef RSA_RAND_P_Q
	RsaGenerateKeyPair_LIT(rsa, input_bits_random);
#else
	RsaGenerateKeyPair_LIT(rsa, 2048);

	if(memcmp((unsigned char *)rsa->n->d, Rsa_SelfCheck_M, 256))
		COND_TRAP(31, 0, 0);
	if(memcmp((unsigned char *)rsa->d->d, Rsa_SelfCheck_D, 256))
		COND_TRAP(31, 0, 0);
#endif

#ifdef RSA_RAND_P_Q
	/* if P Q in random mode, random the original text */
	unsigned char ptext[ptext_input];
		do{
			RAND_pseudo_bytes(ptext, ptext_input);
		}while(ptext[0] == 0);
#endif

#ifdef RSA_RAND_P_Q
	RsaPubKey_LIT(buf1, &len1, ptext, ptext_input, rsa);

#ifdef MODE_CRT
	RsaPrivKeyCRT_LIT(buf2, &len2, buf1, len1, rsa);
#else
	RsaPrivKey_LIT(buf2, &len2, buf1, len1, rsa);
#endif

	for(i=0; i<len2; i++)
	{
		if(ptext[i] != buf2[i])
			COND_TRAP(31, 0, 0);
	}
#else
unsigned char tmp_check[256];

	memcpy(tmp_check, Rsa_SelfCheck_Data, 256);

	RsaPubKey_LIT(buf1, &len1, tmp_check, 2048/8, rsa);

//memcmp_back for the least byte of BIG byte stream at the last
	if(memcmp_back(buf1, Rsa_SelfCheck_Cipher, 256))
		COND_TRAP(31, 0, 0);

#ifdef MODE_CRT
	RsaPrivKeyCRT_LIT(buf2, &len2, buf1, 2048/8, rsa);
#else
	RsaPrivKey_LIT(buf2, &len2, buf1, 2048/8, rsa);
#endif

	if(memcmp_back(buf2, Rsa_SelfCheck_Data, 256))
		COND_TRAP(31, 0, 0);
#endif

#ifdef RSA_RAND_P_Q
#ifdef MODE_CRT
	RsaPrivKeyCRT_LIT(buf1, &len1, ptext, ptext_input, rsa);
#else
	RsaPrivKey_LIT(buf1, &len1, ptext, ptext_input, rsa);
#endif
	RsaPubKey_LIT(buf2, &len2, buf1, len1, rsa);

	if(memcmp_back(buf2, ptext, len2))
		COND_TRAP(31, 0, 0);
#endif

	BN_free(rsa->n);
	BN_free(rsa->d);
	BN_free(rsa->e);
	BN_free(rsa->p);
	BN_free(rsa->q);

#ifdef MODE_CRT
	BN_free(rsa->dmp1);
	BN_free(rsa->dmq1);
	BN_free(rsa->iqmp);
#endif

	free(rsa);
	PSPRINTF("count is: %d", count++);
	}while(1);
}


