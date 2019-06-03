#include <Scene/Input/Gesturer.hpp>
#include <Scene/Input/InputSystem.hpp>

#include <Game.hpp>

CGesturer::CGesturer()
{
	Reset();
}

void CGesturer::_BeginDrag(f32 x, f32 y)
{
	m_DragStart.X = x;
	m_DragStart.Y = y;

	m_DragCache.X = x;
	m_DragCache.Y = y;

	m_bDragging = true;
}

void CGesturer::_DoDrag(f32 x, f32 y)
{
	CGestureEvent gevent(CGestureEvent::EGE_DRAG);
	gevent.Param1 = x - m_DragCache.X;
	gevent.Param2 = y - m_DragCache.Y;

	CGame::Instance()->GetSystem<CInputSystem>()->HandleInput(&gevent);

	m_DragCache.X = x;
	m_DragCache.Y = y;
}

bool CGesturer::_RecognizeDrag(f32 x, f32 y)
{
	return m_DragStart.Distance(CVector2(x,y)) > DRAG_RECOGNITION_DISTANCE;
}

void CGesturer::_BeginRotoZoom(f32 x, f32 y)
{
	m_RotStart.X = x;
	m_RotStart.Y = y;
	m_bZoomRot = true;
}

void CGesturer::_DoRotoZoom(f32 _roto, f32 _zoom)
{
	if (!m_bZoomRot)
	{
		_BeginRotoZoom(_roto, _zoom);
		return;
	}
	f32 dx = _roto - m_RotStart.X;
	f32 dy = _zoom - m_RotStart.Y;
   
	//fix bug with 0-360 step
	if(fabsf(dx) > 90.0f)
		dx=0;

	CGestureEvent gevent(CGestureEvent::EGE_ROTOZOOM);
	gevent.Param1 = dx;
	gevent.Param2 = dy;

	CGame::Instance()->GetSystem<CInputSystem>()->HandleInput(&gevent);

	m_RotStart.X = _roto;
	m_RotStart.Y = _zoom;
}

void CGesturer::_HandleTap(f32 x, f32 y)
{
	CGestureEvent gevent(CGestureEvent::EGE_TAP);
	gevent.Param1 = x;
	gevent.Param2 = y;

	CGame::Instance()->GetSystem<CInputSystem>()->HandleInput(&gevent);
}

void CGesturer::_HandleLongTap(f32 x, f32 y)
{
	CGestureEvent gevent(CGestureEvent::EGE_LONGTAP);
	gevent.Param1 = x;
	gevent.Param2 = y;

	CGame::Instance()->GetSystem<CInputSystem>()->HandleInput(&gevent);
}

bool CGesturer::HandleInput(CInputEvent* _event)
{
	bool recognizedDrag = false;

#if !defined(WALO_PLATFORM_WIN) && !defined(WALO_PLATFORM_OSX)
	if(_event->InputType == CInputEvent::EIT_TOUCH)
	{
		CTouchEvent* tevent = (CTouchEvent*)_event;
        if(tevent->TouchCount > 1)
		{
			m_bDragging = false;
			CVector2 ptFirstFinger = CVector2(tevent->Touches[0].X, tevent->Touches[0].Y);
			CVector2 ptSecondFinger = CVector2(tevent->Touches[1].X, tevent->Touches[1].Y);
			f32 zoom = ptSecondFinger.Distance(ptFirstFinger);
			f32 roto = CMath::Atan2(ptSecondFinger.Y - ptFirstFinger.Y, ptSecondFinger.X - ptFirstFinger.X);

			if(tevent->Touches[0].TouchType == CTouchEvent::ETE_BEGIN || tevent->Touches[1].TouchType == CTouchEvent::ETE_BEGIN)
			{
				_BeginRotoZoom(roto, zoom);
                return true;
			}
			else 
			if(tevent->Touches[0].TouchType == CTouchEvent::ETE_MOVE || tevent->Touches[1].TouchType == CTouchEvent::ETE_MOVE)
			{
				_DoRotoZoom(roto, zoom);
                return true;
			}
			else
			{
                if(m_bZoomRot)
                {
                    m_bZoomRot = false;
                    return false;
                }
			}
		}
		else
		{
			m_bZoomRot = false;
            
			if(tevent->Touches[0].TouchType == CTouchEvent::ETE_BEGIN)
			{
				m_DragStart.X = tevent->Touches[0].X;
				m_DragStart.Y = tevent->Touches[0].Y;
                m_bDragging = false;
                m_bPressed = true;

				m_PressTime = 0.0f;
			}
			else
            if(tevent->Touches[0].TouchType == CTouchEvent::ETE_MOVE)
			{
                if(!m_bDragging)
                {
                    recognizedDrag = _RecognizeDrag(tevent->Touches[0].X, tevent->Touches[0].Y);

                    if(recognizedDrag)
                        _BeginDrag(tevent->Touches[0].X, tevent->Touches[0].Y);
                }
				else
					_DoDrag(tevent->Touches[0].X, tevent->Touches[0].Y);
			}
			else
            {
				if(!m_bDragging)
				{
					if(m_PressTime < 1.0f)
					{
						_HandleTap(tevent->Touches[0].X, tevent->Touches[0].Y);
					}

					recognizedDrag = true;
				}

                m_bPressed = false;
                m_bDragging = false;
            }
            
            return recognizedDrag;
		}
	}
	else
#endif
	if(_event->InputType == CInputEvent::EIT_MOUSE)
	{
		CMouseEvent* mevent = (CMouseEvent*)_event;

		if((mevent->Buttons & CMouseEvent::EMBM_LEFT) && !m_bPressed)
		{
			m_DragStart.X = mevent->X;
			m_DragStart.Y = mevent->Y;

			m_bDragging = false;
			m_bPressed = true;

			m_PressTime = 0.0f;
		}
		else
        if((mevent->Buttons & CMouseEvent::EMBM_LEFT) && m_bPressed)
		{
			if(!m_bDragging)
			{
				recognizedDrag = _RecognizeDrag(mevent->X, mevent->Y);

				if(recognizedDrag)
					_BeginDrag(mevent->X, mevent->Y);
			}
			else
				_DoDrag(mevent->X, mevent->Y);
		}
		else
		{
			if((mevent->Buttons & CMouseEvent::EMBM_LEFT) == 0 && m_bPressed)
			{
				if(!m_bDragging)
				{
					_HandleTap(mevent->X, mevent->Y);
					recognizedDrag = true;
				}

				m_bPressed = false;
				m_bDragging = false;
			}
		}

		if((mevent->Buttons & CMouseEvent::EMBM_RIGHT) && !m_bZoomRot)
		{
			_BeginRotoZoom(mevent->X, mevent->Y);
            return true;
		}
		else
        if((mevent->Buttons & CMouseEvent::EMBM_RIGHT) && m_bZoomRot)
		{
			_DoRotoZoom(mevent->X, mevent->Y);
            return true;
		}
		else
        if(m_bZoomRot)
		{
			m_bZoomRot = false;
            return false;
		}
        
        return recognizedDrag;
	}
	return false;
}

void CGesturer::Reset()
{
	m_bZoomRot = false;
	m_bDragging = false;
	m_bPressed = false;
	m_PressTime = 0.0f;
}

void CGesturer::Update(f32 _dt)
{
	if(m_bPressed && !m_bDragging && !m_bZoomRot)
	{
		m_PressTime += _dt;

		if(m_PressTime > 1.0f)
		{
			_HandleLongTap(m_DragStart.X, m_DragStart.Y);

			m_bPressed = false;
			m_bDragging = false;
			m_PressTime = 0.0;
		}
	}
}
