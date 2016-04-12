enum ESlotType
{
	EMPTY,
	HEALTH,
	STRANGE,
	SPEED
}

class HUD extends CNode2D
{
	IGMButton = null;
	HeartIcon = null;
	HealthBar = null;
	HealthBarBG = null;
	HUDBackground = null;
	HUDBackground2 = null;
	SlotBG01 = null;
	SlotBG02 = null;
	SlotBG03 = null;
	SlotIcon01 = null;
	SlotIcon02 = null;
	SlotIcon03 = null;
	SlotItemsCount01 = null;
	SlotItemsCount02 = null;
	SlotItemsCount03 = null;
	UITexture = null;
	UIFont = null;
	pWorld = null;
	pSlotTypes = null;
	pButtons = null;
	
	constructor(_world)
	{
		base.constructor();
		
		pWorld = _world;
		
		UITexture = pWorld.pGame.ResourceCache.GetTexture("data/ui.png");
		UIFont = pWorld.pGame.ResourceCache.GetFont("data/tfont1.fnt");

		pButtons = [];
	
		IGMButton = ::CSprite2D();
		IGMButton.SetTexture(UITexture);
		IGMButton.SetFrame(102,80,100,100);
		IGMButton.SetPosition(pWorld.pGame.ScreenWidth-5, 5);
		IGMButton.SetCenter(100,0);
		IGMButton.SetScale(0.9, 0.9);
		IGMButton.SetOrder(100.0 + 0.01);
		AddChild(IGMButton);

		pButtons.push(IGMButton);
		
		HeartIcon = ::CSprite2D();
		HeartIcon.SetTexture(UITexture);
		HeartIcon.SetFrame(0,0,90,80);
		HeartIcon.SetPosition(5, 10);
		HeartIcon.SetOrder(100.0 + 0.01);
		AddChild(HeartIcon);
		
		HealthBarBG = ::CSprite2D();
		HealthBarBG.SetTexture(UITexture);
		HealthBarBG.SetFrame(93,0,300-93,40);
		HealthBarBG.SetPosition(105, 27);
		HealthBarBG.SetOrder(100.0 + 0.01);
		AddChild(HealthBarBG);
		
		HealthBar = ::CSprite2D();
		HealthBar.SetColor(1.0,0.0,0.0,0.8);
		HealthBar.SetSize(200,32)
		HealthBar.SetPosition(109, 31);
		HealthBar.SetOrder(100.0 + 0.02);
		AddChild(HealthBar);
		
		HUDBackground = ::CSprite2D();
		HUDBackground.SetColor(0.2,0.2,0.2,1.0);
		HUDBackground.SetSize(pWorld.pGame.ScreenWidth, 100);
		HUDBackground.SetPosition(0, 0);
		HUDBackground.SetOrder(100.0 - 0.01);
		AddChild(HUDBackground);
		
		SlotBG01 = ::CSprite2D();
		SlotBG01.SetTexture(UITexture);
		SlotBG01.SetFrame(400,0,110,100);
		SlotBG01.SetPosition(330, 3);
		SlotBG01.SetOrder(100.0 + 0.01);
		SlotBG01.SetScale(0.88, 0.9);
		AddChild(SlotBG01);

		pButtons.push(SlotBG01);
		
		SlotBG02 = ::CSprite2D();
		SlotBG02.SetTexture(UITexture);
		SlotBG02.SetFrame(400,0,110,100);
		SlotBG02.SetPosition(445, 3);
		SlotBG02.SetOrder(100.0 + 0.01);
		SlotBG02.SetScale(0.88, 0.9);
		AddChild(SlotBG02);

		pButtons.push(SlotBG02);
		
		SlotBG03 = ::CSprite2D();
		SlotBG03.SetTexture(UITexture);
		SlotBG03.SetFrame(400,0,110,100);
		SlotBG03.SetPosition(560, 3);
		SlotBG03.SetOrder(100.0 + 0.01);
		SlotBG03.SetScale(0.88, 0.9);
		AddChild(SlotBG03);

		pButtons.push(SlotBG03);
		
		SlotIcon01 = ::CSprite2D();
		SlotIcon01.SetTexture(null);
		SlotIcon01.SetPosition(22, 6);
		SlotIcon01.SetOrder(100.0 + 0.02);
		SlotBG01.AddChild(SlotIcon01);
		
		SlotIcon02 = ::CSprite2D();
		SlotIcon02.SetTexture(null);
		SlotIcon02.SetPosition(22, 6);
		SlotIcon02.SetOrder(100.0 + 0.02);
		SlotBG02.AddChild(SlotIcon02);
		
		SlotIcon03 = ::CSprite2D();
		SlotIcon03.SetTexture(null);
		SlotIcon03.SetPosition(22, 6);
		SlotIcon03.SetOrder(100.0 + 0.02);
		SlotBG03.AddChild(SlotIcon03);

		SlotItemsCount01 = ::CText2D();
		SlotItemsCount01.SetFont(UIFont);
		SlotItemsCount01.SetColor(0.0, 0.0, 0.0, 1.0);
		SlotItemsCount01.SetPosition(75, 45);
		SlotItemsCount01.SetOrder(100.0 + 0.03);
		SlotItemsCount01.SetScale(1.7, 1.7);
		SlotBG01.AddChild(SlotItemsCount01);

		SlotItemsCount02 = ::CText2D();
		SlotItemsCount02.SetFont(UIFont);
		SlotItemsCount02.SetColor(0.0, 0.0, 0.0, 1.0);
		SlotItemsCount02.SetPosition(75, 45);
		SlotItemsCount02.SetOrder(100.0 + 0.03);
		SlotItemsCount02.SetScale(1.7, 1.7);
		SlotBG02.AddChild(SlotItemsCount02);

		SlotItemsCount03 = ::CText2D();
		SlotItemsCount03.SetFont(UIFont);
		SlotItemsCount03.SetColor(0.0, 0.0, 0.0, 1.0);
		SlotItemsCount03.SetPosition(75, 45);
		SlotItemsCount03.SetOrder(100.0 + 0.03);
		SlotItemsCount03.SetScale(1.7, 1.7);
		SlotBG03.AddChild(SlotItemsCount03);
		
		pSlotTypes = ::array(3, ESlotType.EMPTY);
	}

	function ProcessButton(_btn, _x, _y, _pressed)
	{
		if(_pressed)
		{
			if(_btn.HitTest(_x, _y))
			{
				_btn.SetColor(1.0,0.5,0.5,1.0);
				return true;
			}
			else
			{
				_btn.SetColor(1.0,1.0,1.0,1.0);
			}
		}
		else
		{
			_btn.SetColor(1.0,1.0,1.0,1.0);
			
			if(_btn.HitTest(_x, _y))
			{
				ButtonPressed(_btn);
				return true;
			}
		}

		return false;
	}

	function ButtonPressed(_btn)
	{
		if(_btn == IGMButton)
		{
			pWorld.pGame.SetState(pWorld.pGame.pMenu);
		}
		else
		if(_btn == SlotBG01)
		{
			UseSlotItem(0);
		}
		else
		if(_btn == SlotBG02)
		{
			UseSlotItem(1);
		}
		else
		if(_btn == SlotBG03)
		{
			UseSlotItem(2);
		}
	}
	
	function ProcessTouch(x, y, state)
	{
		local pressed = (state == 0 || state == 1);
		
		foreach(btn in pButtons)
		{
			if(ProcessButton(btn, x, y, pressed))
				return true;
		}

		return false;
	}
	
	function UpdateHUD(_unit)
	{
		local hp = 600.0 / _unit.Health;
		HealthBar.SetSize(200.0/hp,32);
	}
	
	function GetSlot(_type)
	{
		foreach(i,s in pSlotTypes)
		{
			if(s == _type)
				return i;
		}
		return -1;
	}
	
	function GetIcon(_id)
	{
		if(_id == 0)
			return SlotIcon01;
		else
		if(_id == 1)
			return SlotIcon02;
		else
		if(_id == 2)
			return SlotIcon03;
		else
		{
			::print("Error: Wrong id of inventory slot.\n");
			return null;
		}
	}

	function GetCounterLabel(_id)
	{
		if(_id == 0)
			return SlotItemsCount01;
		else
		if(_id == 1)
			return SlotItemsCount02;
		else
		if(_id == 2)
			return SlotItemsCount03;
		else
		{
			::print("Error: Wrong id of inventory slot.\n");
			return null;
		}
	}
	
	function GetRect(_type)
	{
		if(_type == ESlotType.HEALTH)
		{
			return [0,192,64,88];
		}
		else
		if(_type == ESlotType.STRANGE)
		{
			return [64,192,64,88];
		}
		else
		if(_type == ESlotType.SPEED)
		{
			return [128,192,64,88];
		}
		else
		{
			::print("Error: Wrong inventory item type.\n");
			return [0,0,0,0];
		}
	}

	function UseSlotItem(_id)
	{
		local _type = pSlotTypes[_id];

		if(pWorld.pUnit == null)
			return;

		if(_type == ESlotType.HEALTH)
		{
			if(pWorld.pUnit.HealthPotionCount)
			{
				pWorld.pUnit.HealthPotionCount -= 1;
				UpdateInventory(pWorld.pUnit);
			}
		}
		else
		if(_type == ESlotType.STRANGE)
		{
			if(pWorld.pUnit.StrangePotionCount)
			{
				pWorld.pUnit.StrangePotionCount -= 1;
				UpdateInventory(pWorld.pUnit);
			}
		}
		else
		if(_type == ESlotType.SPEED)
		{
			if(pWorld.pUnit.HealthPotionCount)
			{
				pWorld.pUnit.SpeedPotionCount -= 1;
				UpdateInventory(pWorld.pUnit);
			}
		}
	}
	
	function FillSlotItem(_type, _value)
	{
		local sl = GetSlot(_type);
		if(_value > 0)
		{
			if(sl == -1)
			{
				sl = GetSlot(ESlotType.EMPTY);
			}
			
			pSlotTypes[sl] = _type;
			local icon = GetIcon(sl);
			icon.SetTexture(UITexture);
			local rc = GetRect(_type);
			icon.SetFrame(rc[0], rc[1], rc[2], rc[3]);
			local cntr = GetCounterLabel(sl);
			cntr.SetText(_value.tostring());
		}
		else
		{
			if(sl != -1)
			{
				pSlotTypes[sl] = ESlotType.EMPTY;
				local icon = GetIcon(sl);
				icon.SetTexture(null);
				icon.SetFrame([0,0,0,0]);

				local cntr = GetCounterLabel(sl);
				cntr.SetText("");
			}
		}
	}
	
	function UpdateInventory(_unit)
	{	
		FillSlotItem(ESlotType.HEALTH, _unit.HealthPotionCount);
		FillSlotItem(ESlotType.STRANGE, _unit.StrangePotionCount);
		FillSlotItem(ESlotType.SPEED, _unit.SpeedPotionCount);
	}
}