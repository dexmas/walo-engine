#include "Core/String.hpp"

CString UTF16to8(const CWString& _text)
{
    CString out;
    unsigned int codepoint = 0;
	u16* in = (u16*)_text.CStr();

    for(in;  *in != 0;  ++in)
    {
        if(*in >= 0xd800 && *in <= 0xdbff)
            codepoint = ((*in - 0xd800) << 10) + 0x10000;
        else
        {
            if(*in >= 0xdc00 && *in <= 0xdfff)
                codepoint |= *in - 0xdc00;
            else
                codepoint = *in;

            if(codepoint <= 0x7f)
                out.Append(u8(codepoint));
            else 
			if(codepoint <= 0x7ff)
            {
                out.Append(u8(0xc0 | ((codepoint >> 6) & 0x1f)));
                out.Append(u8(0x80 | (codepoint & 0x3f)));
            }
            else 
			if(codepoint <= 0xffff)
            {
                out.Append(u8(0xe0 | ((codepoint >> 12) & 0x0f)));
                out.Append(u8(0x80 | ((codepoint >> 6) & 0x3f)));
                out.Append(u8(0x80 | (codepoint & 0x3f)));
            }
            else
            {
                out.Append(u8(0xf0 | ((codepoint >> 18) & 0x07)));
                out.Append(u8(0x80 | ((codepoint >> 12) & 0x3f)));
                out.Append(u8(0x80 | ((codepoint >> 6) & 0x3f)));
                out.Append(u8(0x80 | (codepoint & 0x3f)));
            }
            codepoint = 0;
        }
    }
    return out;
}

CWString UTF8to16(const CString& _text)
{
    CWString out;
    unsigned int codepoint = 0;
	u8* in = (u8*)_text.CStr();
    int following = 0;

    for(in;  *in != 0;  ++in)
    {
        unsigned char ch = *in;
        if(ch <= 0x7f)
        {
            codepoint = ch;
            following = 0;
        }
        else 
		if(ch <= 0xbf)
        {
            if (following > 0)
            {
                codepoint = (codepoint << 6) | (ch & 0x3f);
                --following;
            }
        }
        else 
		if(ch <= 0xdf)
        {
            codepoint = ch & 0x1f;
            following = 1;
        }
        else 
		if(ch <= 0xef)
        {
            codepoint = ch & 0x0f;
            following = 2;
        }
        else
        {
            codepoint = ch & 0x07;
            following = 3;
        }
        if(following == 0)
        {
            if(codepoint > 0xffff)
            {
                out.Append(u16(0xd800 + (codepoint >> 10)));
                out.Append(u16(0xdc00 + (codepoint & 0x03ff)));
            }
            else
                out.Append(u16(codepoint));
            codepoint = 0;
        }
    }
    return out;
}