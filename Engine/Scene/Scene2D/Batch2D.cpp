#include <Scene/Scene2D/Batch2D.hpp>
#include <Scene/Scene2D/Object2D.hpp>

#include <Core/Log.hpp>

bool CBatch2D::Compare(CObject2D* _drw)
{
	if(m_Material != _drw->GetMaterial())
		return false;
	if(m_Texture != _drw->GetTexture())
		return false;
	if(m_Clip != _drw->m_Clipped)
		return false;

	if(m_Clip && !(m_ClipRect == _drw->m_ClipRect))
	{
		return false;
	}

	return true;
}

void CBatch2D::Add(CObject2D* _drw)
{
	m_List.PushBack(_drw);

	if(_drw->m_CurrentBatch)
	{
		_drw->m_CurrentBatch->Remove(_drw);
	}

	_drw->m_CurrentBatch = this;

	//LOG("Object added to batch. Current batch size: %d.\n", m_List.Size());

	m_BatchDirty = true;
}

void CBatch2D::Remove(CObject2D* _drw)
{
	m_List.Erase(_drw);

	_drw->m_CurrentBatch = 0;

	//LOG("Object removed from batch. Current batch size: %d.\n", m_List.Size());

	m_BatchDirty = true;
}

void CBatch2D::Update(CObject2D* _drw)
{
	if(m_BatchDirty)
		return;

	CMap<CObject2D*, u32>::TNode* node = m_VertexMap.Find(_drw);
		
	if(node)
	{
		CPODArray<SVertex2D>& vertexes = _drw->GetVertexes();
		u32 offset = node->Value;

		m_VertexBuffer->SetData(&vertexes[0], offset, vertexes.Size());
	}
}

void CBatch2D::Render()
{
	u32 vcount = 0;
	u32 offset = 0;

	if(m_BatchDirty)
	{
		m_VertexMap.Clear();

		CList<CObject2D*>::CIterator it = m_List.Begin();

		while(it != m_List.End())
		{
			vcount += (*it)->GetVertexes().Size();

			++it;
		}

		m_IndexCount = (vcount >> 2) * 6;

		m_VertexBuffer->SetSize(vcount, cVertex2DFormat);
		m_IndexBuffer->SetSize(m_IndexCount, false);

		for(u32 i = 0; i < m_IndexCount / 6; i++)
		{
			u16 indexes[6];

			indexes[0] = i * 4 + 0;
			indexes[1] = i * 4 + 1;
			indexes[2] = i * 4 + 2;
			indexes[3] = i * 4 + 2;
			indexes[4] = i * 4 + 3;
			indexes[5] = i * 4 + 0;

			m_IndexBuffer->SetData(indexes, i * 6, 6);
		}

		it = m_List.Begin();

		while(it != m_List.End())
		{
			CPODArray<SVertex2D>& vertexes = (*it)->GetVertexes();
			u32 size = vertexes.Size();

			m_VertexBuffer->SetData(&vertexes[0], offset, size);
			m_VertexMap.Insert((*it), offset);

			offset += size;

			++it;
		}

		m_BatchDirty = false;
	}

	m_Material->SetTexture(ETC_TEXTURE0, m_Texture);
	m_Render->SetMaterial(m_Material);
	m_Render->SetVertexBuffer(m_VertexBuffer);
	m_Render->SetIndexBuffer(m_IndexBuffer);
	m_Render->SetScissor(m_Clip, m_ClipRect);

	m_Render->RenderIndexed(EPT_TRIANGLES, 0, m_IndexCount);
}

#ifdef _DEBUG
void CBatch2D::DebugRender()
{
	CList<CObject2D*>::CIterator it = m_List.Begin();

	while(it != m_List.End())
	{
		(*it)->DebugRender();

		++it;
	}
}
#endif