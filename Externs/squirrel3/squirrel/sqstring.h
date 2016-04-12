/*	see copyright notice in squirrel.h */
#ifndef _SQSTRING_H_
#define _SQSTRING_H_

inline SQHash _hashstr (const SQChar *s, size_t l)
{
		SQHash h = (SQHash)l;  /* seed */
		size_t step = (l>>5)|1;  /* if string is too long, don't hash all its chars */
		for (; l>=step; l-=step)
			h = h ^ ((h<<5)+(h>>2)+(unsigned short)*(s++));
		return h;
}

#ifdef SQUTF8 
#include "squtf8.h"
#else
// Set to empty
#define UTF8_EXIT
#define UTF8_INIT
#endif

struct SQString : public SQRefCounted
{
	SQString(){UTF8_INIT}
	~SQString(){UTF8_EXIT}
public:
	static SQString *Create(SQSharedState *ss, const SQChar *, SQInteger len = -1 );
	SQInteger Next(const SQObjectPtr &refpos, SQObjectPtr &outkey, SQObjectPtr &outval);
	void Release();
	SQSharedState *_sharedstate;
	SQString *_next; //chain for the string table
	SQInteger _len;
#ifdef SQUTF8
    SQInteger GetUtf8Length(){ if(_utf8_len<0) _utf8_len=SQGetUtf8Length(_val,_len); return _utf8_len; }
    SQInteger _utf8_len;
#else
    #error No UTF8 !!!
#endif   
	SQHash _hash;
	SQChar _val[1];
};

#endif //_SQSTRING_H_
