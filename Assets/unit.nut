
enum TUnitState 
{
	IDDLE = 0,
	WALK = 1,
	ATTAK = 2,
	DIE = 3,
}

class Unit extends CSprite2D
{
	pWorld = null;

	X = null;
	Y = null;
	I = null;
	J = null;

	State = null;

	Dir = null;

	Frames = null;

	Path = null;

	AnimDir = null;
	Frame = null;
	AnimFPS = null;
	AnimTime = null;
	StepTime = null;

	Killed = null;
	Target = null;
	Health = null;
	Damage = null;

	pRigBody = null;
	pCollisionBox = null;

	constructor(i, j, type, world)
	{
		base.constructor();

		Frames = [];

		AnimFPS = 12;

		pWorld = world;

		SetTexture(pWorld.pGame.ResourceCache.GetTexture("data/peasant.png"));

		pRigBody = ::CRigidBody2D();
		AddComponent(pRigBody);

		SetCenter(36,46);

		Reset(i, j);

		pCollisionBox = ::CCollisionBox2D();
		AddComponent(pCollisionBox);

		pCollisionBox.SetSize(32, 32);
		pCollisionBox.SetCenter(0, -10);
	}

	function Reset(i, j)
	{
		State = TUnitState.IDDLE;

		I = i;
		J = j;

		X = i*32+16;
		Y = j*32+16;

		SetPosition(X, Y);

		pWorld.StepOnTile(this, I, J);

		StepTime = 0;
		AnimTime = 0;
		Frame = 0;

		Dir = 0;
		AnimDir = 0;

		Path = [];

		SetEnabled(true);

		Killed = false;
		Target = null;
		Health = 100;
		Damage = 10;

		SetFrame(0, 0, 72, 72);
	}

	function Stop()
	{
		Path = [];
		State = TUnitState.IDDLE;

		AnimTime = 0;
		Frame = 0;
	}

	function MoveTo(i, j)
	{
		if(Killed)
			return;
		
		Target = null;
		
		if(State == TUnitState.WALK)
		{
			local tpath = [];

			if(pWorld.pPathMap.CalcPath(Path[0].I, Path[0].J, i, j, tpath) && tpath.len())
			{
				tpath.insert(0, Path[0]);
				Path = tpath;
			}
			else
			{
				Stop();
			}
		}
		else
		{
			if(pWorld.pPathMap.CalcPath(I, J, i, j, Path) && Path.len())
			{
				StepTo(Path[0].I, Path[0].J);
			}
			else
			{
				Stop();
			}
		}
	}
	
	function StepTo(_i, _j)
	{
		State = TUnitState.WALK;
		AnimTime = 0;
		StepTime = 0;
		Frame = 0;
		FaceTo(_i, _j);
		
		pWorld.StepOutTile(this, I, J);
		pWorld.StepOnTile(this, _i, _j);
	}
	
	function FollowTarget()
	{
		local ispass = pWorld.pPathMap.GetPass(Target.I, Target.J);
		pWorld.pPathMap.SetPass(Target.I, Target.J, true);
	
		local tr = Target;
		MoveTo(Target.I, Target.J);
		Target = tr;
	
		pWorld.pPathMap.SetPass(Target.I, Target.J, ispass);
	}
	
	function AttackTarget()
	{
		Stop();
		FaceTo(Target.I, Target.J);

		State = TUnitState.ATTAK;
		Frame = 0;
		AnimTime = 0;
	}

	function Attack(unit)
	{
		if(Killed)
		{
			return;
		}
		
		Target = unit;

		local v1 = ::CVector2(I, J);
		local v2 = ::CVector2(unit.I, unit.J);
		
		if(v1.Distance(v2) < 2)
		{
			AttackTarget();
			
			::print("Attack unit.\n");
		}
		else
		{
		    FollowTarget();
			
			::print("Follow unit to attack.\n");
		}
	}
	
	function GoToInteract(_item)
	{
		if(Killed)
		{
			return;
		}
		
		Target = _item;

		local v1 = ::CVector2(I, J);
		local v2 = ::CVector2(_item.I, _item.J);

		if(v1.Distance(v2) < 2)
		{
			Interact(Target);
		}
		else
		{
		    FollowTarget();
			
			::print("Follow item to interact.\n");
		}
	}
	
	function Interact(_item)
	{		
		if(Target.getclass() == ::Chest)
		{
			::print("This is chest. Open it.\n");
			Target.Open();
		}
		else
		{
			::print("Don't know what is it. Just stop.\n");
		}
		
		Stop();
		Target = null;
	}
	
	function Hit(_unit)
	{
		if(Killed)
		{
			return;
		}
		
		Health -= _unit.Damage;
		
		if(Health <= 0)
		{
			Killed  = true;
			AnimTime = 0;
			Frame = 0;

			State = TUnitState.DIE;
		}
	}

	function FaceTo(i, j)
	{
		SetFlip(false, false);

		if(I>i)
		{
			if(J>j)
			{
				Dir = 7;
				AnimDir = 1;
				SetFlip(true, false);
			}
			else
			if(J==j)
			{
				Dir = 6;
				AnimDir = 2;
				SetFlip(true, false);
			}
			else
			{
				Dir = 5;
				AnimDir = 3;
				SetFlip(true, false);
			}
		}
		else
		if(I==i)
		{
			if(J>j)
			{
				Dir = 0;
				AnimDir = 0;
			}
			else
			if(J==j)
			{
				//No changes
			}
			else
			{
				Dir = 4;
				AnimDir = 4;
			}
		}
		else
		{
			if(J>j)
			{
				Dir = 1;
				AnimDir = 1;
			}
			else
			if(J==j)
			{
				Dir = 2;
				AnimDir = 2;
			}
			else
			{
				Dir = 3;
				AnimDir = 3;
			}
		}
	}
	
	function EndAttack()
	{
		Target.Hit(this);
		
		if(Target.Killed)
		{
			Path = [];
			State = TUnitState.IDDLE;
		}
		else
		{
			FaceTo(Target.I, Target.J);

			local v1 = ::CVector2(X, Y);
			local v2 = ::CVector2(Target.X, Target.Y);
			
			if(v1.Distance(v2) >= 35)
			{
				FollowTarget();
			}
		}
	}

	function EndStep()
	{
		local dstI = Path[0].I;
		local dstJ = Path[0].J;
		local dstX = dstI*32+16;
		local dstY = dstJ*32+16;

		X = dstX;
		Y = dstY;

		//pWorld.StepOutTile(this, I, J);

		I = dstI;
		J = dstJ;

		Path.remove(0);

		if(Target)
		{
			local v1 = ::CVector2(I, J);
			local v2 = ::CVector2(Target.I, Target.J);

			if(v1.Distance(v2) < 2)
			{
				::print("Near target.\n");
				
				if(Target.getclass() == ::Enemy)
				{
					::print("This is enemy, attack it.\n");
					AttackTarget();
				}
				else
				{
					Interact(Target);
				}
				
				return;
			}
			
			Path = [];
			if(!pWorld.pPathMap.CalcPath(I, J, Target.I, Target.J, Path))
			{
				Stop();
			}
		}
		
		if(Path.len() > 0 && pWorld.pPathMap.GetPass(Path[0].I, Path[0].J))
		{
			StepTo(Path[0].I, Path[0].J);
		}
		else
		{
			State = TUnitState.IDDLE;
			AnimTime = 0;
			Frame = 0;
		}
	}

	function Update(dt)
	{
		if(State == TUnitState.WALK)
		{
			if(StepTime > 0.7)
			{
				StepTime = 0;

				EndStep();
			}
			else
			{
				local dstI = Path[0].I;
				local dstJ = Path[0].J;
				local dstX = dstI*32+16;
				local dstY = dstJ*32+16;

				local p = ::CVector3(I*32+16, J*32+16, 0);
				local d = ::CVector3(dstX, dstY, 0);
				local c = ::CVector3(0,0,0);

				c.Interpolate(p, d, StepTime/0.7);

				X = c.X;
				Y = c.Y;
			}

			SetPosition(X, Y);
		}
		else
		if(State == TUnitState.ATTAK)
		{
			
		}

		UpdateFrame(dt);
	}

	function UpdateFrame(dt)
	{
		AnimTime += dt*AnimFPS;
		StepTime += dt;

		if(Frame == AnimTime.tointeger())
			return;
		else
			Frame = AnimTime.tointeger();
		
		local frm = [];

		if(State == TUnitState.WALK)
		{
			if(Frame > 4)
			{
				AnimTime = 0;
				Frame = 0;
			}

			frm = [AnimDir*72, Frame*72, 72, 72];
		}
		else
		if(State == TUnitState.ATTAK)
		{
			if(Frame > 4)
			{
				AnimTime = 0;
				Frame = 0;

				EndAttack();
			}

			frm = [AnimDir*72, (5+Frame)*72, 72, 72];
		}
		else
		if(State == TUnitState.DIE)
		{
			if(Frame > 3)
			{
				AnimTime = 0;
				Frame = 0;
				
				pWorld.StepOutTile(this, I, J);
				pWorld.Killed(this);
			}
			
			frm = [AnimDir*72, (10+Frame)*72, 72, 72];
		}
		else
		{
			frm = [AnimDir*72, 0*72, 72, 72];
		}

		SetFrame(frm[0], frm[1], frm[2], frm[3]);
	}
}