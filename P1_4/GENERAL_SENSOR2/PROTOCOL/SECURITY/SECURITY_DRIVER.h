/*
 * SECURITY_DRIVER.h
 *
 * Created: 02-Apr-21 2:33:29 PM
 *  Author: Mina Medhat
 */ 


#ifndef SECURITY_DRIVER_H_
#define SECURITY_DRIVER_H_
#include "SECURITY_DRIVER_DEFINE.h"
/*V1 MEANS VERSION 1*/
void ENCRYPT_ALGORISM_1_V1(unsigned char *PLAIN_TEXT,unsigned char *CIPHER_TEXT,unsigned char LENGTH,unsigned char*SESSION_KEY);/*USING SESSION KEY*/
void DECRYPT_ALGORISM_1_V1(unsigned char *PLAIN_TEXT,unsigned char *CIPHER_TEXT,unsigned char LENGTH,unsigned char*SESSION_KEY);/*USING SESSION KEY*/
void ENCRYPT_ALGORISM_2_V1(unsigned char *PLAIN_TEXT,unsigned char *CIPHER_TEXT,unsigned char LENGTH,unsigned char*PASSWORD_KEY);/*USING PASSWORD_KEY*/
void DECRYPT_ALGORISM_2_V1(unsigned char *PLAIN_TEXT,unsigned char *CIPHER_TEXT,unsigned char LENGTH,unsigned char*PASSWORD_KEY);/*USING PASSWORD_KEY*/
void HASH_SESSION_KEY(unsigned char *DATA_TO_BE_HASHED,unsigned char *HASH_VALUE,unsigned char LENGTH,unsigned char*SESSION_KEY);
void HASH_PASWORD_KEY(unsigned char *DATA_TO_BE_HASHED,unsigned char *HASH_VALUE,unsigned char LENGTH,unsigned char*PASSWORD_KEY);

#endif /* SECURITY_DRIVER_H_ */