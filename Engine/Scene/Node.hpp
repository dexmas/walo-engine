#pragma once

#include "Core/Types.hpp"
#include "Core/List.hpp"
#include "Core/Map.hpp"
#include "Core/String.hpp"
#include "Core/Matrix.hpp"
#include "Device/Event.hpp"
#include "Scene/ComponentType.hpp"

class CComponent;

class CNode
{
	friend class CGame;
	friend class CComponent;
public:
	CNode();
	virtual ~CNode();

	void AddChild(CNode* _child);
	void RemoveChild(CNode* _child);

	void AddComponent(CComponent* _comp);
	void RemoveComponent(CComponent* _component);

	void GetComponents(EComponentType _type, CList<CComponent*>& _list);
	void GetComponents(CList<CComponent*>& _list);

	CVector3 GetPosition() const {return m_Position;}
	void SetPosition(const CVector3& _position);

	CVector3 GetScale() const {return m_Scale;}
	void SetScale(const CVector3& _scale);

	CVector3 GetRotation() const {return m_Rotation;}
	void SetRotation(const CVector3& _rotation);

	bool GetEnabled() {return m_Enabled;}
	void SetEnabled(bool _enabled);

	CString GetName() const {return m_Name;}
	void SetName(const CString& _name){m_Name = _name;}

	const CMatrix4& GetTransform() { return m_Transform; }

	void HandleEvent(CEvent* _event);

	virtual void OnUpdate(f32 _dt){}
	virtual void OnEnabled(bool _enable);
	virtual void OnTransformed();
	virtual void OnEvent(CEvent* _event){}

protected:
	CString				m_Name;

	CList<CComponent*>	m_Components;
	CList<CNode*>		m_Childs;

	CNode*				m_Base;

	CVector3			m_Position;
	CVector3			m_Scale;
	CVector3			m_Rotation;

	CMatrix4			m_Transform;

	bool				m_Enabled;
	bool				m_HeirarhyEnabled;
};
