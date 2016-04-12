#pragma once

enum EPrimitiveType
{
	EPT_TRIANGLES = 0,
	EPT_LINES
};

enum EBlendMode
{
	EBM_REPLACE = 0,
	EBM_ADD,
	EBM_MULTIPLY,
	EBM_ALPHA,
	EBM_ADDALPHA,
	EBM_PREMULALPHA,
	EBM_INVDESTALPHA,
	EBM_SUBTRACT,
	EBM_SUBTRACTALPHA,

	EBM_COUNT
};

enum ECullMode
{
	ECM_NONE = 0,
	ECM_CCW,
	ECM_CW,

	ECM_COUNT
};

enum EFillMode
{
	EFM_SOLID = 0,
	EFM_WIREFRAME,
	EFM_POINT,

	EFM_COUNT
};

enum EDepthTestMode
{
	EDTM_DISABLED = 0,
	EDTM_ALWAYS,
	EDTM_EQUAL,
	EDTM_NOTEQUAL,
	EDTM_LESS,
	EDTM_LESSEQUAL,
	EDTM_GREATER,
	EDTM_GREATEREQUAL,

	EDTM_COUNT
};

enum ETextureChanel
{
	ETC_TEXTURE0 = 0,
	ETC_TEXTURE1,
	ETC_TEXTURE2,
	ETC_TEXTURE3,

	ETC_COUNT
};

enum ETransformType
{
	ETT_VIEWPROJ = 0,
	ETT_MODEL,

	ETT_COUNT
};

enum ETextureWrap
{
	ETW_REPEAT = 0,
	ETW_MIRROR,
	ETW_CLAMP,

    ETW_COUNT
};

enum ETextureFilter
{
    ETF_NEAREST = 0,
    ETF_LINEAR,
	ETF_ANISOTROPIC,

    ETF_COUNT,
};

enum ELockState
{
	ELS_NONE = 0,
	ELS_HARDWARE,
	ELS_SHADOW,
	ELS_SCRATCH
};

enum EShaderParamType
{
	ESPT_FLOAT,
	ESPT_VECTOR2,
	ESPT_VECTOR3,
	ESPT_MATRIX4,

	ESPT_COUNT
};