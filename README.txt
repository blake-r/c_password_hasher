c_password_hasher
=================

C Password Hasher (http://wijjo.com/passhash/)

how to build
============

cmake .
make

how to install
==============

sudo make install

how to use
==========

passhash [-tkldpm] --tag <tag> [--key <key>] [--length <length>] [--punctuation] [--mixed-case] [--no-special] [--digits-only]

Where:
	-t (--tag)			tag word;
	--key				key word;
	-k					key word will be prompted;
	-l (--length)		length of result word (from 2 to 26, must be multuply by 2);
	-d (--digits)		result must contain at least one digit;
	-p (--punctuation)	result must contain at least one punctuation character;
	-m (--mixed-case)	result must contain letters in both upper and lower case;
	--no-special		result must not contain any special characters (but only alpha and digits);
	--digits-only		result must contain only digits;

how to uninstall
================

sudo rm /usr/local/bin/passhash

