/*
 * std_data.h
 *
 *  Created on: Mar 23, 2017
 *      Author: jzj
 */

#ifndef STD_DATA_H_
#define STD_DATA_H_

#pragma pack(16)
struct std_data{
	unsigned char key[96];
	unsigned char iv[32];
	unsigned char data[128];
	unsigned char stdout[144];
};
#pragma pack()

struct std_data *std_sm1;
struct std_data *std_sm6;
struct std_data *std_sm4;
struct std_data *std_ssf33;
struct std_data *std_des;
struct std_data *std_3des;
struct std_data *std_aes;

struct std_data *std_sha1;
struct std_data *std_sha256;
struct std_data *std_sha512;
struct std_data *std_sm3;

struct std_data *std_sha0;
struct std_data *std_sha224;
struct std_data *std_sha384;
struct std_data *std_md5;

struct std_data *std_crc;

#endif /* STD_DATA_H_ */
