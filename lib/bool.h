#ifndef PASSHASH_LIB_BOOL_H
#define	PASSHASH_LIB_BOOL_H


#ifndef bool
	typedef unsigned char bool;
	enum
	{
		false = 0u,
		true = 1u
	};
	#define bool bool
#endif//bool


#endif	/* PASSHASH_LIB_BOOL_H */
