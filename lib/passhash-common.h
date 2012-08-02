#ifndef PASSHASH_COMMON_H
#define	PASSHASH_COMMON_H
#include "bool.h"

#ifdef	__cplusplus
extern "C"
{
#endif

char * GenerateHashWord(const char * site_tag,
						const char * master_key,
						int hash_word_size,
						bool require_digit,
						bool require_punctuation,
						bool require_mixed_case,
						bool restrict_special,
						bool restrict_to_digits);

#ifdef	__cplusplus
}
#endif

#endif	/* PASSHASH_COMMON_H */
