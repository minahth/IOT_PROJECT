/*
 * SECURITY_DRIVER.c
 *
 * Created: 02-Apr-21 2:33:35 PM
 *  Author: Mina Medhat
 */ 
#include "SECURITY_DRIVER.h"


void ENCRYPT_ALGORISM_1_V1(unsigned char *PLAIN_TEXT,unsigned char *CIPHER_TEXT,unsigned char LENGTH,unsigned char*SESSION_KEY)
{
	unsigned char i;
	for (i=0;i<LENGTH;i++)
	{
		CIPHER_TEXT[i]=PLAIN_TEXT[i]^(SESSION_KEY[i%SESSION_KEY_SIZE_V1]);
	}
}

void DECRYPT_ALGORISM_1_V1(unsigned char *PLAIN_TEXT,unsigned char *CIPHER_TEXT,unsigned char LENGTH,unsigned char*SESSION_KEY)
{
	unsigned char i;
	for (i=0;i<LENGTH;i++)
	{
		PLAIN_TEXT[i]=CIPHER_TEXT[i]^(SESSION_KEY[i%SESSION_KEY_SIZE_V1]);
	}
}

void ENCRYPT_ALGORISM_2_V1(unsigned char *PLAIN_TEXT,unsigned char *CIPHER_TEXT,unsigned char LENGTH,unsigned char*PASSWORD_KEY)
{
		unsigned char i;
		for (i=0;i<LENGTH;i++)
		{
			CIPHER_TEXT[i]=PLAIN_TEXT[i]^(PASSWORD_KEY[0]^PASSWORD_KEY[1]^PASSWORD_KEY[2]^PASSWORD_KEY[3]);/*PASSWORD SIZE 4*/
		}
}

void DECRYPT_ALGORISM_2_V1(unsigned char *PLAIN_TEXT,unsigned char *CIPHER_TEXT,unsigned char LENGTH,unsigned char*PASSWORD_KEY)
{
	unsigned char i;
	for (i=0;i<LENGTH;i++)
	{
		PLAIN_TEXT[i]=CIPHER_TEXT[i]^(PASSWORD_KEY[0]^PASSWORD_KEY[1]^PASSWORD_KEY[2]^PASSWORD_KEY[3]);/*PASSWORD SIZE 4*/
	}
}

void HASH_SESSION_KEY(unsigned char *DATA_TO_BE_HASHED,unsigned char *HASH_VALUE,unsigned char LENGTH,unsigned char*SESSION_KEY)
{
			unsigned short TEMP_HASH_VALUE;
			unsigned long HASH_ODD=1;
			unsigned long HASH_EVEN=1;
			unsigned char i;
			for (i=0;i<LENGTH;i=i+2)
			{
				HASH_ODD=(HASH_ODD*DATA_TO_BE_HASHED[i])%0xffff;
			}
			for (i=1;i<LENGTH;i=i+2)
			{
				HASH_EVEN=(HASH_EVEN*DATA_TO_BE_HASHED[i])%0xffff;
			}
			TEMP_HASH_VALUE=(HASH_ODD^HASH_EVEN)%0xffff;
			HASH_VALUE[0]=(TEMP_HASH_VALUE>>8)^SESSION_KEY[0];
			HASH_VALUE[1]=(TEMP_HASH_VALUE&0x00ff)^SESSION_KEY[1];
				
}

void HASH_PASWORD_KEY(unsigned char *DATA_TO_BE_HASHED,unsigned char *HASH_VALUE,unsigned char LENGTH,unsigned char*PASSWORD_KEY)
{
			unsigned short TEMP_HASH_VALUE;
			unsigned long HASH_ODD=1;
			unsigned long HASH_EVEN=1;
			unsigned char i;
			for (i=0;i<LENGTH;i=i+2)
			{
				HASH_ODD=(HASH_ODD*DATA_TO_BE_HASHED[i])%0xffff;
			}
			for (i=1;i<LENGTH;i=i+2)
			{
				HASH_EVEN=(HASH_EVEN*DATA_TO_BE_HASHED[i])%0xffff;
			}
			TEMP_HASH_VALUE=(HASH_ODD^HASH_EVEN)%0xffff;
			HASH_VALUE[0]=(TEMP_HASH_VALUE>>8)^PASSWORD_KEY[0]^PASSWORD_KEY[1];
			HASH_VALUE[1]=(TEMP_HASH_VALUE&0x00ff)^PASSWORD_KEY[2]^PASSWORD_KEY[3];
	
}
