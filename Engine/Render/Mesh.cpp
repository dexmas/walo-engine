#include "Render/Mesh.hpp"

#include "Game.hpp"

CMesh::CMesh():CAsset(EAT_MESH)
{
}

CMesh::~CMesh()
{
}

u32 _ReadU32(CStream* _stream)
{
	u32 ret = 0;
	_stream->Read(&ret, 4);

	return ret;
}

f32 _ReadF32(CStream* _stream)
{
	f32 ret = 0;
	_stream->Read(&ret, 4);

	return ret;
}

CString _ReadString(CStream* _stream)
{
	CString ret = "";

	while(1)
	{
		char c;
		_stream->Read(&c, 1);
		ret += c;

		if(!c)
			break;
	}

	return ret;
}

CVector3 _ReadVector3(CStream* _stream)
{
	CVector3 ret;

	_stream->Read(&ret.X, sizeof(f32));
	_stream->Read(&ret.Y, sizeof(f32));
	_stream->Read(&ret.Z, sizeof(f32));

	return ret;
}

CBBox _ReadBBox(CStream* _stream)
{
	CVector3 min = _ReadVector3(_stream);
	CVector3 max = _ReadVector3(_stream);
		
	CBBox ret(min, max);

	return ret;
}

bool CMesh::Load(CStream* _stream)
{
	CRender* rndr = CGame::Instance()->GetRender();

    u8 strid[5];
    _stream->Read(strid, 4);
	strid[4] = 0;

	if(CString((char*)strid) != "UMDL")
	{
		return false;
	}

	u32 numVertexBuffers = _ReadU32(_stream);

    for(u32 i = 0; i < numVertexBuffers; i++)
	{
		u32 vertexCount = _ReadU32(_stream);
        u32 elementMask = _ReadU32(_stream);

        _ReadU32(_stream);
        _ReadU32(_stream);
        
        CVertexBuffer* vbuf = rndr->CreateVertexBuffer();

        vbuf->SetSize(vertexCount, elementMask);
        
        u8* buffer = new u8[vbuf->GetVertexSize() * vertexCount];
        _stream->Read(buffer, vbuf->GetVertexSize() * vertexCount);
		vbuf->SetData(buffer, 0, vertexCount);

		delete buffer;

        m_VertexBuffers.Insert(i, vbuf);
	}

	 // Read index buffers
    u32 numIndexBuffers = _ReadU32(_stream);
   
    for(u32 i = 0; i < numIndexBuffers; i++)
    {
        u32 indexCount = _ReadU32(_stream);
        u32 indexSize = _ReadU32(_stream);
        
        CIndexBuffer* ibuf = rndr->CreateIndexBuffer();
        
		ibuf->SetSize(indexCount, indexSize > sizeof(u16));
        
        u8* buffer = new u8[indexCount * indexSize];
        _stream->Read(buffer, indexCount * indexSize);
        
        ibuf->SetData(buffer, 0, indexCount);
       
		delete buffer;

		m_IndexBuffers.Insert(i, ibuf);
    }

	// Read mesh buffers
    u32 numGeometries = _ReadU32(_stream);

    for(u32 i = 0; i < numGeometries; i++)
    {
        // Read bone mappings
        u32 boneMappingCount = _ReadU32(_stream);
        for (unsigned j = 0; j < boneMappingCount; ++j)
            _ReadU32(_stream);
        
        u32 numLodLevels = _ReadU32(_stream);

        for(u32 j = 0; j < numLodLevels; j++)
        {
            f32 distance = _ReadF32(_stream);
            EPrimitiveType ptype = (EPrimitiveType)_ReadU32(_stream);
            
            u32 vertexBufferRef = _ReadU32(_stream);
            u32 indexBufferRef = _ReadU32(_stream);
            u32 indexStart = _ReadU32(_stream);
            u32 indexCount = _ReadU32(_stream);
            
            if(vertexBufferRef >= m_VertexBuffers.Size())
            {
                return false;
            }

            if (indexBufferRef >= m_IndexBuffers.Size())
            {
                return false;
            }
            
            CMeshBuffer* mbuf = new CMeshBuffer();

			CVertexBuffer* vbuf = m_VertexBuffers[vertexBufferRef];
			CIndexBuffer* ibuf = m_IndexBuffers[indexBufferRef];

            mbuf->SetVertexBuffer(ptype, vbuf, 0, vbuf->GetVertexCount());
            mbuf->SetIndexBuffer(ibuf, indexStart, indexCount);
            
			m_MeshBuffers.Insert(i * numLodLevels + j, mbuf);
        }
    }

	// Read morphs
    u32 numMorphs = _ReadU32(_stream);
    for(u32 i = 0; i < numMorphs; i++)
    {
		_ReadString(_stream);
        u32 nubuffers_ = _ReadU32(_stream);
        
        for(u32 j = 0; j < nubuffers_; j++)
        {
			u32 bufferIndex = _ReadU32(_stream);
            
			u32 emask = _ReadU32(_stream); // element mask
			u32 vcount = _ReadU32(_stream); // vertex count

			// Base size: size of each vertex index
            u32 vertexSize = sizeof(u32);
            // Add size of individual elements
            if(emask & EEM_POSITION)
                vertexSize += 3*sizeof(f32);
            if(emask & EEM_NORMAL)
                vertexSize += 3*sizeof(f32);
            if(emask & EEM_TANGENT)
                vertexSize += 4*sizeof(f32);

            u8* morphData = new u8[vcount * vertexSize];
            
            _stream->Read(morphData, vcount * vertexSize);

			delete[] morphData;
		}
	}

	_ReadSkelleton(_stream);

	// Read bounding box
    m_BBox = _ReadBBox(_stream);
	
	return true;
}

void CMesh::_ReadSkelleton(CStream* _stream)
{
	u32 bones = _ReadU32(_stream);

	for(u32 i = 0; i < bones; i++)
    {
        CString name = _ReadString(_stream);
        u32 parentIndex = _ReadU32(_stream);
        CVector3 initialPosition = _ReadVector3(_stream);
        CVector3 initialRotation = _ReadVector3(_stream);
        CVector3 initialScale = _ReadVector3(_stream);
		f32 mtx[12];
        _stream->Read(mtx, 12 * sizeof(f32));
        
        // Read bone collision data
        u32 collisionMask = _ReadU32(_stream);
        if(collisionMask & 1)
            f32 radius = _ReadF32(_stream);
        if(collisionMask & 2)
            CBBox boundingBox_ = _ReadBBox(_stream);
    }
}

void CMesh::Unload()
{
}

void CMesh::SetMaterial(CMaterial* _mat)
{
	for(u32 i=0; i < m_MeshBuffers.Size(); i++)
	{
		CMeshBuffer* mbuf = m_MeshBuffers[i];

		mbuf->SetMaterial(_mat);
	}
}

void CMesh::Render()
{
	CRender* rndr = CGame::Instance()->GetRender();

	for(u32 i=0; i < m_MeshBuffers.Size(); i++)
	{
		CMeshBuffer* mbuf = m_MeshBuffers[i];
		
		mbuf->Render();
	}
}