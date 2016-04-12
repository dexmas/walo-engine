#include "Assets/AssetsDB.hpp"
#include "Assets/Asset.hpp"

#include "Game.hpp"
#include "Render/Render.hpp"

#include "FileSystem/FileSystem.hpp"

CAssetsDB* CAssetsDB::m_Instance = 0;

CAssetsDB* CAssetsDB::Instance()
{
	if(!m_Instance)
	{
		m_Instance = new CAssetsDB();
	}

	return m_Instance;
}

CAssetsDB::CAssetsDB()
{
	
}

CAssetsDB::~CAssetsDB()
{
	for(u32 i = 0; i < EAT_COUNT; i++)
	{
		m_Assets[i].Clear();
	}
}

CImage* CAssetsDB::GetImage(const char* _path)
{
	CMap<CString, CAsset*>::TNode* n = m_Assets[EAT_IMAGE].Find(_path);
	if(n)
	{
		CAsset* res = n->Value;

		return (CImage*)res;
	}

	CImage* img = new CImage();
	CStream* stream = CFileSystem::Instance()->OpenFile(_path);

	if(stream->Open() && img->_LoadInternal(stream))
	{
		img->_SetCached(true);
		m_Assets[EAT_IMAGE].Insert(_path, img);

		delete stream;

		return img;
	}

	delete stream;
	delete img;

	return 0;
}

CTexture* CAssetsDB::GetTexture(const char* _path)
{
	CMap<CString, CAsset*>::TNode* n = m_Assets[EAT_TEXTURE].Find(_path);
	if(n)
	{
		CAsset* res = n->Value;

		return (CTexture*)res;
	}

	CTexture* tex = CGame::Instance()->GetRender()->CreateTexture();
	CStream* stream = CFileSystem::Instance()->OpenFile(_path);

	if(stream->Open() && tex->_LoadInternal(stream))
	{
		tex->_SetCached(true);
		m_Assets[EAT_TEXTURE].Insert(_path, tex);

		delete stream;

		return tex;
	}

	delete stream;
	delete tex;

	return 0;
}

CMaterial* CAssetsDB::GetMaterial(const char* _path)
{
	CMap<CString, CAsset*>::TNode* n = m_Assets[EAT_MATERIAL].Find(_path);
	if(n)
	{
		CAsset* res = n->Value;

		return (CMaterial*)res;
	}

	CMaterial* mtl = new CMaterial();
	CStream* stream = CFileSystem::Instance()->OpenFile(_path);

	if(stream->Open() && mtl->_LoadInternal(stream))
	{
		mtl->_SetCached(true);
		m_Assets[EAT_MATERIAL].Insert(_path, mtl);

		delete stream;

		return mtl;
	}

	delete stream;
	delete mtl;

	return 0;
}

CMesh* CAssetsDB::GetMesh(const char* _path)
{
	CMap<CString, CAsset*>::TNode* n = m_Assets[EAT_MESH].Find(_path);
	if(n)
	{
		CAsset* res = n->Value;

		return (CMesh*)res;
	}

	CMesh* mesh = new CMesh();
	CStream* stream = CFileSystem::Instance()->OpenFile(_path);

	if(stream->Open() && mesh->_LoadInternal(stream))
	{
		mesh->_SetCached(true);
		m_Assets[EAT_MESH].Insert(_path, mesh);

		delete stream;

		return mesh;
	}

	delete stream;
	delete mesh;

	return 0;
}

CShader* CAssetsDB::GetShader(const char* _path)
{
	CMap<CString, CAsset*>::TNode* n = m_Assets[EAT_SHADER].Find(_path);
	if(n)
	{
		CAsset* res = n->Value;

		return (CShader*)res;
	}

	EShaderType type = EST_COUNT;

	CString path(_path);
	u32 dotpos = path.FindLastChar(".", 1);
	CString ext = path.SubString(dotpos + 1, path.Size() - dotpos);
	ext.ToLower();

	if(ext == "ps")
	{
		type = EST_PIXEL;
	}
	else
	if(ext == "vs")
	{
		type = EST_VERTEX;
	}

	CShader* shdr = CGame::Instance()->GetRender()->CreateShader(type);
	CStream* stream = CFileSystem::Instance()->OpenFile(_path);

	if(stream->Open() && shdr->_LoadInternal(stream))
	{
		shdr->_SetCached(true);
		m_Assets[EAT_SHADER].Insert(_path, shdr);

		delete stream;

		return shdr;
	}

	delete stream;
	delete shdr;

	return 0;
}

CFont2D* CAssetsDB::GetFont2D(const char* _path)
{
	CMap<CString, CAsset*>::TNode* n = m_Assets[EAT_FONT2D].Find(_path);
	if(n)
	{
		CAsset* res = n->Value;

		return (CFont2D*)res;
	}

	CFont2D* fnt = new CFont2D();
	CStream* stream = CFileSystem::Instance()->OpenFile(_path);

	if(stream->Open() && fnt->_LoadInternal(stream))
	{
		fnt->_SetCached(true);
		m_Assets[EAT_FONT2D].Insert(_path, fnt);

		delete stream;

		return fnt;
	}

	delete stream;
	delete fnt;

	return 0;
}

CSoundStream* CAssetsDB::GetSound(const char* _path)
{
	CMap<CString, CAsset*>::TNode* n = m_Assets[EAT_SOUND].Find(_path);
	if(n)
	{
		CAsset* res = n->Value;

		return (CSoundStream*)res;
	}

	ESoundFormat format = ESF_COUNT;

	CString path(_path);
	u32 dotpos = path.FindLastChar(".", 1);
	CString ext = path.SubString(dotpos + 1, path.Size() - dotpos);
	ext.ToLower();

	if(ext == "wav")
	{
		format = ESF_WAV;
	}
	else
	if(ext == "ogg")
	{
		format = ESF_OGG;
	}

	CSoundStream* snd = CSoundStream::CreateStream(format);
	CStream* stream = CFileSystem::Instance()->OpenFile(_path);

	if(stream->Open() && snd->_LoadInternal(stream))
	{
		snd->_SetCached(true);
		m_Assets[EAT_SOUND].Insert(_path, snd);

		return snd;
	}

	delete stream;
	delete snd;

	return 0;
}

void CAssetsDB::_Remove(CAsset* _asset)
{
	EAssetType type = _asset->Type();

	CMap<CString, CAsset*>::CIterator it = m_Assets[type].Iterator();
	while(!it.AtEnd())
	{
		CMap<CString, CAsset*>::TNode* node = it.GetNode();
		CAsset* ast = node->Value;
		
		if(_asset == ast)
		{
			m_Assets[type].Remove(node->Key);
			delete ast;

			return;
		}
		else
			it++;
	}
}