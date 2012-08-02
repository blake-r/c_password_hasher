#ifndef PASSHASH_SHA1_H
#define	PASSHASH_SHA1_H

#ifdef	__cplusplus
extern "C"
{
#endif

char * hmac_sha1(const char * site_tag, const char * master_key, unsigned char ** hmac_md, unsigned int * hmac_md_size);
char * base64_encode(const unsigned char * data, unsigned int size, char * result, unsigned int * result_len);

#ifdef	__cplusplus
}
#endif

#endif	/* PASSHASH_SHA1_H */

