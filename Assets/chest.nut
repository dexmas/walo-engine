enum TChestState 
{
	CLOSED = 0,
	OPENED = 1,
}

class Chest extends CSprite2D
{
	I = null;
	J = null;
	pWorld = null;
	State = null;
	Frame = null;
	AnimFPS = null;
	AnimTime = null;
	Dir = null;
	Type = null;
	bEnabled = null;

	pCollisionBox = null;
	
	constructor(_i, _j, _type, _dir, _world)
	{
		base.constructor();
		
		pWorld = _world;
		
		I = _i;
		J = _j;
		
		State = TChestState.CLOSED;

		SetTexture(pWorld.pGame.ResourceCache.GetTexture("data/chest.png"));
		
		SetCenter(16,36);
		SetPosition(I*32+16, J*32+16);
		
		Frame = 0;
		AnimFPS = 12;
		AnimTime = 0;
		Dir = _dir;
		Type = _type;
		bEnabled = true;
		
		if(Dir > 3)
		{
			Dir = 3;
		}
		
		if(Dir == 3)
		{
			Dir = 1;
			SetFlip(true, false);
		}
		
		if(Type > 3)
		{
			Type = 3;
		}

		SetFrame((Dir*4+Type)*32, 0, 32, 48);

		pCollisionBox = ::CCollisionBox2D();
		AddComponent(pCollisionBox);

		pCollisionBox.SetSize(32, 32);
		pCollisionBox.SetCenter(0, -3);
	}
	
	function Open()
	{
		State = TChestState.OPENED;
	}
	
	function UpdateFrame(dt)
	{	
		local frm = [];

		if(State == TChestState.OPENED)
		{
			AnimTime += dt*AnimFPS;

			if(Frame == AnimTime.tointeger())
				return;
			else
				Frame = AnimTime.tointeger();
		
			if(Frame>3)
			{
				Frame = 3;
				
				pWorld.ChestOpened(this);

				RemoveComponent(pCollisionBox);

				bEnabled = false;
			}
			frm = [(Dir*4+Type)*32, Frame*48, 32, 48];
		}
		else
		{
			frm = [(Dir*4+Type)*32, 0, 32, 48];
		}
		
		SetFrame(frm[0], frm[1], frm[2], frm[3]);
	}
	
	function Update(_dt)
	{
		if(bEnabled)
		{
			UpdateFrame(_dt);
		}
	}
}