/*	see copyright notice in squirrel.h */
#ifndef _SQUTF8_H_
#define _SQUTF8_H_

/////////////////////////////////////////////////////////////////////
// File included from sqstring.h for optional internal UTF8 support 

#ifdef SQUTF8 

// Help functions to support Utf8 strings from SQString
void SQClearUtf8Iter(SQString* s);
SQInteger SQGetUtf8Length(const SQChar *ps, SQInteger byte_len); 

#define UTF8_EXIT SQClearUtf8Iter(this);
#define UTF8_INIT _utf8_len=-1;

struct SQString;

#define SQ_N_UTF8_ITER  2   // Two iterators is enough for many purposes
struct SQUtf8Iter {
    SQUtf8Iter(const char* ps=NULL, int len=0) { Init(ps,len); }
    
    const char* Goto(int pos);
    int ToIndex(const char *ps);
    //int Length( );
    int ByteLength( ){ return _end-_ps; }
    int GetUniChar(int pos);
    int GetUniChar();
    const char* Get(){ return _cps; }
    bool AtEnd(){ return _cps==_end; }
    
    const char*_ps;  // Base of string
    const char*_cps; // Current position 
    int _pos;        // UTF8 character position
    const char*_end; // End of string
    
    void Init(const char* ps, int len);
protected:    
    int _len;        // UTF8 character length
    bool StepFwd( );
    bool StepBack( );
};

SQInteger SQGetUtf8Length(const SQChar *ps, SQInteger byte_len); 
SQUtf8Iter& SQGetUtf8Iter(SQString* s);
SQUtf8Iter& SQGetUtf8Iter(const SQChar* s, SQInteger len=-1);
void SQClearUtf8Iter(SQString *s);

#endif // SQUTF8 

#endif // _SQUTF8_H_
