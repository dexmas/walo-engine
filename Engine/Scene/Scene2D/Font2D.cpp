#include "Scene/Scene2D/Font2D.hpp"

#include "Assets/AssetsDB.hpp"

#include <pugixml.hpp>

CFont2D::CFont2D():CAsset(EAT_FONT2D)
{
	m_FontSize = 0;
}

bool CFont2D::Load(CStream* _stream)
{
	pugi::xml_document xmldoc;
	char* buffer = new char[_stream->Size() + 1];

	_stream->Read(buffer, _stream->Size());
	buffer[_stream->Size()] = 0;

	pugi::xml_parse_result res = xmldoc.load_buffer(buffer, _stream->Size());
	if(!res)
	{
		delete[] buffer;
		return false;
	}

	delete[] buffer;

	pugi::xml_node fontnode = xmldoc.child("font");
	if(!fontnode)
	{
		return false;
	}

	pugi::xml_node infonode = fontnode.child("info");

	if(infonode)
	{
		m_FontSize = infonode.attribute("size").as_int();
	}

	pugi::xml_node pagesnode = fontnode.child("pages");
	pugi::xml_node onepage = pagesnode.first_child();

	while(onepage)
	{
		u32 id = onepage.attribute("id").as_int();
		const char* fname = onepage.attribute("file").value();

		m_Texture = CAssetsDB::Instance()->GetTexture(fname);

		break;
	}

	pugi::xml_node charsnode = fontnode.child("chars");
	pugi::xml_node onechar = charsnode.first_child();

	while(onechar)
	{
		SGlyph glyph;

		u16 id = onechar.attribute("id").as_int();

		glyph.X = onechar.attribute("x").as_int();
		glyph.Y = onechar.attribute("y").as_int();
		glyph.W = onechar.attribute("width").as_int();
		glyph.H = onechar.attribute("height").as_int();
		glyph.Xo = onechar.attribute("xoffset").as_int();
		glyph.Yo = onechar.attribute("yoffset").as_int();
		glyph.Xa = onechar.attribute("xadvance").as_int();
		glyph.Chanel = onechar.attribute("chnl").as_int();

		m_GlyphMap.Insert(id, glyph);

		onechar = onechar.next_sibling();
	}

	return m_Texture && !m_GlyphMap.Empty();
}

void CFont2D::Unload()
{
	m_GlyphMap.Clear();
}

CFont2D::SGlyph* CFont2D::GetGlyph(u16 _char)
{
	CMap<u16, SGlyph>::TNode* node = m_GlyphMap.Find(_char);
	if(node)
	{
		return &(node->Value);
	}

	return 0;
}