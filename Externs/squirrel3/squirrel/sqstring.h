/*  see copyright notice in squirrel.h */
#ifndef _SQSTRING_H_
#define _SQSTRING_H_

inline SQHash _hashstr_lua5(const SQChar *s, size_t l)
{
        SQHash h = (SQHash)l;  /* seed */
        size_t step = (l>>5)+1;  /* if string is too long, don't hash all its chars */
        for (; l>=step; l-=step)
            h ^= ((h<<5)+(h>>2)+(SQChar)(s[l-1]));
        return h;
}
//djb2
inline SQHash _hashstr_djb2(const SQChar *s, size_t l)
{
  SQHash hash = SQHash(5381+l);
  size_t step = (l>>5)+1;  /* if string is too long, don't hash all its chars */

  for (; l>=step; l-=step)
    hash = hash * 33 + s[l-1];

  return hash;
}

//fnv1
inline SQHash _hashstr_fnv1a(const SQChar *s, size_t l)
{
  SQHash result = 2166136261U;
  size_t step = (l>>5)+1;  /* if string is too long, don't hash all its chars */

  for (; l>=step; l-=step)
    result = (result ^ s[l-1])* 16777619;
  return result;
}
//__forceinline SQHash _hashstr (const SQChar *s, size_t l){return _hashstr_lua5(s, l);}//worst
//__forceinline SQHash _hashstr (const SQChar *s, size_t l){return _hashstr_djb2(s, l);}//good
__forceinline SQHash _hashstr (const SQChar *s, size_t l){return _hashstr_fnv1a(s, l);}

struct SQString : public SQRefCounted
{
    SQString(){}
    ~SQString(){}
public:
    static SQString *Create(SQSharedState *ss, const SQChar *, SQInteger len = -1 );
    SQInteger Next(const SQObjectPtr &refpos, SQObjectPtr &outkey, SQObjectPtr &outval);
    void Release();
    SQSharedState *_sharedstate;
    SQString *_next; //chain for the string table
    SQInteger _len;
    SQHash _hash;
    SQChar _val[1];
};



#endif //_SQSTRING_H_
