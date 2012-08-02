#include "passhash-sha1.h"
#include <malloc.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>


static void ConvertToDigits(char * str, int seed);
static void InjectSpecialCharacter(char * str, int offset, int reserved, int seed, char c_start, int c_num);
static void RemoveSpecialCharacters(char * str, int seed);


char * hmac_sha1(const char * site_tag, const char * master_key, unsigned char ** hmac_md_out, unsigned int * hmac_md_size)
{
	static unsigned char hmac_md_local[EVP_MAX_MD_SIZE], * hmac_md_local_ptr = hmac_md_local;
	unsigned char sha1_md[SHA_DIGEST_LENGTH];
	unsigned char hmac_md[EVP_MAX_MD_SIZE];
	int size;

	SHA_CTX sha_context;
	SHA1_Init(&sha_context);
	SHA1_Update(&sha_context, &master_key, strlen(master_key));
	SHA1_Final(sha1_md, &sha_context);

	HMAC(EVP_sha1(), master_key, strlen(master_key), site_tag, strlen(site_tag), hmac_md, &size);

	if (hmac_md_out)
		*hmac_md_out = malloc(size);
	else
		hmac_md_out = &hmac_md_local_ptr;
	memcpy(*hmac_md_out, hmac_md, size);

	if (hmac_md_size) *hmac_md_size = size;

	return *hmac_md_out;
}


char * base64_encode(const unsigned char * data, unsigned int size, char * result, unsigned int * result_len)
{
	BIO *bmem, *b64;
	BUF_MEM *bptr;
	char * ret;

	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, data, size);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);

	if (result)
		ret = result;
	else
		ret = malloc(bptr->length);

	memcpy(ret, bptr->data, bptr->length - 1);
	ret[bptr->length - 1] = '\0';

	if (result_len) *result_len = bptr->length - 1;

	BIO_free_all(b64);

	return ret;
}
