#pragma once

#include "Game.hpp"
#include "Scene/Scene3D/Camera3D.hpp"
#include "Scene/Scene3D/StaticModel.hpp"
#include "Scene/Scene2D/Sprite2D.hpp"
#include "Scene/Scene2D/Progress2D.hpp"
#include "Scene/Input/InputListener.hpp"
#include "Scene/Update/UpdateComponent.hpp"

class CTestSprite: public CNode
{
public:
	CTestSprite()
	{
		CSprite2D* sprite = new CSprite2D();
		sprite->SetTexture(CAssetsDB::Instance()->GetTexture("test.png"));
		sprite->SetFrame(CRect(0, 0, 256, 256));
		sprite->SetSize(CVector2(64, 64));

		AddComponent(sprite);
		AddComponent(new CUpdateComponent());

		SetPosition(CVector3(CMath::Rand(0, 960), CMath::Rand(0, 640), 0.0));
		m_Speed = CVector3(CMath::Rand(-256, 256), CMath::Rand(-256, 256), 0.0f);
		m_RSpeed = CMath::Rand(-90, 90);
	}

	void OnUpdate(f32 _dt)
	{
		CVector3 pos = GetPosition() + m_Speed * _dt;
		CVector3 rot = GetRotation() + CVector3(0.0f, 0.0f, m_RSpeed) * _dt;

		if(pos.X < 0 || pos.X > 960)
			m_Speed.X *= -1;

		if(pos.Y < 0 || pos.Y > 640)
			m_Speed.Y *= -1;

		SetPosition((const CVector3&)pos);
		SetRotation((const CVector3&)rot);
	}

private:
	CVector3 m_Speed;
	f32      m_RSpeed;
};

class CTestProgress: public CNode
{
public:
	CTestProgress()
	{
		m_Prog= new CProgress2D();
		m_Prog->SetSize(CVector2(256, 256));
		m_Prog->SetCenter(CVector2(64, 64));
		m_Prog->SetType(CProgress2D::EPT_RADIAL);
		m_Prog->SetColor(CColor::BLUE);
		m_Prog->SetTexture(CAssetsDB::Instance()->GetTexture("circle.png"));
		m_Prog->SetFrame(CRect(0,0,256,256));

		AddComponent(m_Prog);
		AddComponent(new CUpdateComponent());

		SetPosition(CVector3(960.0f/2.0f, 640.0f/2.0f, 0.0f));

		m_Value = 0.0f;
		m_Speed = 0.1f;

		m_Prog->SetValue(m_Value);
	}

	void OnUpdate(f32 _dt)
	{
		m_Value += (m_Speed * _dt);
		
		if(m_Value > 1.0f)
		{
			m_Value = 1.0f;
			m_Speed *= -1;
		}

		if(m_Value < 0.0f)
		{
			m_Value = 0.0f;
			m_Speed *= -1;
		}

		m_Prog->SetValue(m_Value);
	}

	f32 m_Value;
	f32 m_Speed;

	CProgress2D* m_Prog;
};

class CWaloGame: public CGame
{
public:
	bool OnInit()
	{
		m_Level = new CNode();
		m_Level->SetName("Level");
		AddChild(m_Level);

		CMesh* mesh = CAssetsDB::Instance()->GetMesh("tile.mdl");
		mesh->SetMaterial(CAssetsDB::Instance()->GetMaterial("tile.mat"));

		CStaticModel* smodel = new CStaticModel();
		smodel->SetMesh(mesh);

		m_Level->AddComponent(smodel);

		m_Camera = new CNode();
		m_Camera->SetName("Camera");
		AddChild(m_Camera);

		CCamera3D* cam3d = new CCamera3D();
		m_Camera->AddComponent(cam3d);

		m_Camera->SetPosition(CVector3(0.0f, -2.0f, -10.0f));
		m_Camera->SetRotation(CVector3(0.0f, 0.0f, 0.0f));

		AddComponent(new CInputListener());
		AddComponent(new CUpdateComponent());

		CTestProgress* prog = new CTestProgress();
		AddChild(prog);

		/*for(u32 i=0;i<256;i++)
		{
			CTestSprite* sprite = new CTestSprite();
			AddChild(sprite);
		}*/

		return true;
	}

	void OnEvent(CEvent* _event)
	{
		if(_event->Type == EET_INPUT)
		{
			CInputEvent* ievent = (CInputEvent*)_event;

			if(ievent->InputType == CInputEvent::EIT_GESTURE)
			{
				CGestureEvent* gevent = (CGestureEvent*)ievent;

				if(gevent->Gesture == CGestureEvent::EGE_DRAG)
				{
					CVector3 pos = m_Camera->GetPosition();
					pos += CVector3(gevent->Param1, -(gevent->Param2), 0.0f);

					m_Camera->SetPosition(pos);
				}
			}
		}
	}

	void OnUpdate(f32 _dt)
	{
		CVector3 rot = m_Level->GetRotation();
		rot += CVector3(0.0f, _dt * 10.0f, 0.0f);

		m_Level->SetRotation(rot);
	}

	void OnFree()
	{
	}

	void OnSuspend()
	{
	}

	void OnResume()
	{
	}

private:
	CNode* m_Camera;
	CNode* m_Level;

	CList<CNode*> m_Sprites;
};
