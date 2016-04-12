#pragma once

#include "Core/Vector.hpp"
#include "Device/Event.hpp"

#define DRAG_RECOGNITION_DISTANCE 12

class CGesturer
{
public:
	CGesturer();

	bool HandleInput(CInputEvent* _event);
	void Reset();

private:
	void _BeginDrag(f32 x, f32 y);
	void _DoDrag(f32 x, f32 y);
	bool _RecognizeDrag(f32 x, f32 y);
	void _BeginRotoZoom(f32 x, f32 y);
	void _DoRotoZoom(f32 x, f32 y);
	void _HandleTap(f32 x, f32 y);

	bool	 m_bZoomRot;
	bool	 m_bDragging;
	bool	 m_MLBPressed;
	CVector2 m_DragStart;
	CVector2 m_DragCache;
	CVector2 m_RotStart;
};