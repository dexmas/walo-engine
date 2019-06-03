
class Enemy extends Unit
{
	Timer = null;

	constructor(i, j, type, world)
	{
		base.constructor(i,j,type,world);

		Timer = 0;
	}

	function Update(dt)
	{
		base.Update(dt);

		if(State == TUnitState.IDDLE)
		{
			Timer -= dt;
			
			if(Timer <= 0)
			{
				Timer = 0;
				Think();
			}
		}
	}
	
	function RandomWalk()
	{
		for(local i=0; i<256; i++)
		{
			local nextI = I + ::Math.Rand(-8, 8);
			local nextJ = J + ::Math.Rand(-8, 8);
			
			if(pWorld.pPathMap.CalcPath(I, J, nextI, nextJ, Path) && Path.len())
			{
				::print("Random walk to: " + nextI + ", " + nextJ + ".\n")
				StepTo(Path[0].I, Path[0].J);
				return;
			}
		}
	}

	function Think()
	{
		local playerX = pWorld.pUnit.X;
		local playerY = pWorld.pUnit.Y;
		
		::print("Enemy think: ");

		Stop();

		local v1 = ::CVector2(X, Y);
		local v2 = ::CVector2(playerX, playerY)
		
		if(v1.Distance(v2) < 256)
		{			
			::print("attack player.\n");
			
			Timer = 1.0;
			
			Attack(pWorld.pUnit);
		}
		else
		{
			Timer = 3.0;
			
			if(::Math.Rand(0,3) == 0)
			{
				::print("random walk.\n");
				
				RandomWalk();
			}
			else
			{
				::print("iddle.\n");
			}
		}
	}
}