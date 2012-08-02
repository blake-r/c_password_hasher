#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <termios.h>
#include "passhash-common.h"


int main(int argc, char ** argv)
{
	char * site_tag = NULL;
	char * master_key = NULL;
	int hash_word_size = 8;
	int flag_query_key = false;
	int flag_digits = false;
	int flag_punctuation = false;
	int flag_mixed_case = false;
	int flag_no_special = false;
	int flag_digits_only = false;

	{
		int c;
		char *error;
		while (1) {
			int option_index = 0;
			static struct option long_options[] = {
				{ "tag"			, required_argument	, 0, 't' },
				{ "key"			, required_argument	, 0, 'k' },
				{ "length"		, required_argument	, 0, 'l' },
				{ "digits"		, no_argument		, 0, 'd' },
				{ "punctuation"	, no_argument		, 0, 'p' },
				{ "mixed-case"	, no_argument		, 0, 'm' },
				{ "no-special"	, no_argument		, 0, 1 },
				{ "digits-only"	, no_argument		, 0, 2 }
			};

			c = getopt_long(argc, argv, "t:kl:dpm", long_options, &option_index);
			if (c == -1) break;

			switch (c) {
				case 0:
					printf("option %s", long_options[option_index].name);
					if (optarg) printf(" with arg %s", optarg);
					printf("\n");
					break;
				case 't':
					if (site_tag) free(site_tag);
					site_tag = strdup(optarg);
					break;
				case 'k':
					if (optarg) {
						if (master_key) free(master_key);
						master_key = strdup(optarg);
					} else flag_query_key = true;
					break;
				case 'l':
					error = NULL;
					hash_word_size = strtol(optarg, &error, 10);
					if (error && *error) fprintf(stderr, "Error: Failed to parse length from `%s` at `%s`\n", optarg, error);
					break;
				case 'd':
					flag_digits = true;
					break;
				case 'p':
					flag_punctuation = true;
					break;
				case 'm':
					flag_mixed_case = true;
					break;
				case 1:
					flag_no_special = true;
					break;
				case 2:
					flag_digits_only = true;
					break;
			}
		}
	}

	if (!master_key && flag_query_key) {
		char buf[256];
		struct termios term, term_orig;
		tcgetattr(STDIN_FILENO, &term_orig);
		term = term_orig;
		term.c_lflag &= ~ECHO;
		term.c_lflag |= ECHONL;

		printf("Enter key: ");
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
		scanf("%255s", buf);
		tcsetattr(STDIN_FILENO, TCSANOW, &term_orig);

		master_key = strdup(buf);
	}

	if (!site_tag) puts("Error: Tag not specified.");
	if (!master_key) puts("Error: Key not specified.");
	if (hash_word_size % 2) {
		puts("Warning: Length must be multiply by two.");
		hash_word_size -= hash_word_size % 2;
	}
	if (!hash_word_size) puts("Warning: Length is zero, no output will be provided.");
	if (hash_word_size > 26) {
		puts("Warning: Length must me from 2 to 26.");
		hash_word_size %= 27;
	}
	if (flag_digits_only && flag_no_special) puts("Info: Flag digits-only make useless of no-special.");


	if (site_tag && master_key && hash_word_size > 0) {
		char *hash = GenerateHashWord(site_tag, master_key, hash_word_size, flag_digits, flag_punctuation, flag_mixed_case, flag_no_special, flag_digits_only);
		printf("Result: %s\n", hash);
		free(hash);
	}


	if (site_tag) free(site_tag);
	if (master_key) free(master_key);

	return EXIT_SUCCESS;
}
