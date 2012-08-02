#include "passhash-common.h"
#include <stdlib.h>
#include <string.h>
#include "passhash-sha1.h"


static void ConvertToDigits(char * str, int seed);
static void InjectSpecialCharacter(char * str, int offset, int reserved, int seed, char c_start, int c_num);
static void RemoveSpecialCharacters(char * str, int seed);


char * GenerateHashWord(const char * site_tag,
						const char * master_key,
						int hash_word_size,
						bool require_digit,
						bool require_punctuation,
						bool require_mixed_case,
						bool restrict_special,
						bool restrict_to_digits)
{
	unsigned char * hmac_md;
	char * hash;
	unsigned int hmac_md_size, hash_len;

	hmac_md = hmac_sha1(site_tag, master_key, NULL, &hmac_md_size);
	hash = base64_encode(hmac_md, hmac_md_size, NULL, &hash_len);

	if (hash[hash_len - 1] == '=') --hash_len;

	int seed = 0;
	int i;
	for (i = 0; i < hash_len; ++i) seed += hash[i];

	hash[hash_word_size] = '\0';

	if (restrict_to_digits)
		ConvertToDigits(hash, seed); // "28662022101986805287847027"
	else {
		if (require_digit)
			InjectSpecialCharacter(hash, 0, 4, seed, 48, 10); // "DJRR2BXl"
		if (require_punctuation && !restrict_special)
			InjectSpecialCharacter(hash, 1, 4, seed, 33, 15); // "DJRRX&Xl"
		if (require_mixed_case) {
			InjectSpecialCharacter(hash, 2, 4, seed, 65, 26); // ---
			InjectSpecialCharacter(hash, 3, 4, seed, 97, 26); // ---
		}
		if (restrict_special)
			RemoveSpecialCharacters(hash, seed); // "28662022101986805287847027"
	}

	return hash;
}

void ConvertToDigits(char * str, int seed)
{
	char c;
	char last_digit = '\0';
	for (; *str; ++str) {
		c = *str;
		if (c < '0' || c > '9') {
			if (last_digit != '\0') {
				c = last_digit;
				last_digit = '\0';
			}
			*str = (seed + c) % 10 + '0';
		} else {
			if (last_digit == '\0') last_digit = c;
		}
	}
}


void InjectSpecialCharacter(char * str, int offset, int reserved, int seed, char c_start, int c_num)
{
	int len = strlen(str);
	int pos0 = seed % len;
	int pos = (pos0 + offset) % len;

	int i;
	int i2;
	int c;
	for (i = 0; i < len - reserved; ++i) {
		i2 = (pos0 + reserved + i) % len;
		c = str[i2];
		if (c >= c_start && c < c_start + c_num)
			return;
	}
	str[pos] = (seed + str[pos]) % c_num + c_start;
}


void RemoveSpecialCharacters(char * str, int seed)
{
	char c;
	int i;
	int offset;
	int last_aldig = -1;
	for (i = 0; *str; ++str, ++i) {
		c = *str;
		if (c < '0' || (c > '9' && c < 'A') || (c > 'Z' && c < 'a') || c > 'z') {
			if (last_aldig != -1) {
				offset = last_aldig;
				last_aldig = -1;
			} else offset = i;
			*str = (seed + offset) % 26 + 65;
		} else {
			if (last_aldig == -1) last_aldig = i;
		}
	}
}

