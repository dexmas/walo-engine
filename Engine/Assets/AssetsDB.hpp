#pragma once

#include "Assets/Asset.hpp"

#include "Core/String.hpp"
#include "Core/Map.hpp"

#include "Render/Image.hpp"
#include "Render/Texture.hpp"
#include "Render/Shader.hpp"
#include "Render/Material.hpp"
#include "Render/Mesh.hpp"

#include "Scene/Scene2D/Font2D.hpp"

#include "Sound/SoundStream.hpp"

class CAssetsDB
{
	friend class CAsset;
public:
	CAssetsDB();
	~CAssetsDB();

	static CAssetsDB* Instance();

	CImage*		  GetImage(const char* _path);
	CTexture*     GetTexture(const char* _path);
	CMaterial*    GetMaterial(const char* _path);
	CMesh*		  GetMesh(const char* _path);
	CShader*      GetShader(const char* _path);
	CFont2D*      GetFont2D(const char* _path);
	CSoundStream* GetSound(const char* _path);

private:
	static CAssetsDB* m_Instance;
	void _Remove(CAsset* _asset);

	CMap<CString, CAsset*> m_Assets[EAT_COUNT];
};