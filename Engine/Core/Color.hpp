#pragma once

#include "Core/Types.hpp"
#include "Core/Math.hpp"

class CColor
{
public:
	CColor():R(0.0f),G(0.0f),B(0.0f),A(1.0f){}
    
	CColor(u32 _rgba):
        R(((_rgba>>24)&0xFF)/255.0f),
        G(((_rgba>>16)&0xFF)/255.0f),
        B(((_rgba>>8)&0xFF)/255.0f),
        A(((_rgba>>0)&0xFF)/255.0f){}
    
	CColor(f32 _r, f32 _g, f32 _b, f32 _a):
		R(_r),
		G(_g),
		B(_b),
		A(_a){}

    f32 R;
    f32 G;
    f32 B;
    f32 A;
    
    operator u32() const
    {
        return (u8(A*255.0f) << 24) | (u8(B*255.0f) << 16) | (u8(G*255.0f) << 8) | u8(R*255.0f);
    }

	static const CColor RED;
	static const CColor GREEN;
	static const CColor BLUE;
	static const CColor ORANGE;
	static const CColor BLACK;
	static const CColor GRAY;
    static const CColor WHITE;
};
