class GameST extends State
{
	Time = 0;

	pHUD = null;
	
	pTileMap = null;
	pPathMap = null;
	pFogMap = null;

	TileW = null;
	TileH = null;
	SizeI = null;
	SizeJ = null;

	pUnit = null;
	pEnemys = null;
	pChests = null;

	CameraX = null;
	CameraY = null;

	pObjectLayer = null;

	ScaleK = null;

	pDundeon = null;
	
	constructor(game)
	{
		base.constructor(game);

		pDundeon = ::CDungeon();
		
		pHUD = ::HUD(this);
		pHUD.SetPosition(0,0);
		pHUD.SetOrder(100.0);
		AddChild(pHUD);

		ScaleK = 1.0;

		TileW = 0;
		TileH = 0;
		SizeI = 0;
		SizeJ = 0;

		CameraX = 0;
		CameraY = -100;
		
		pTileMap = ::CTileMap2D();
		pTileMap.SetOrder(-10.0);

		pPathMap = ::CPathMap();
		
		AddChild(pTileMap);

		pObjectLayer = ::CNode2D();

		AddChild(pObjectLayer);

		GenerateLevel();

		Scale(1.0);
	}

	function Show()
	{
		base.Show();

		pGame.PlayBackgroundTrack("data/immortals.ogg");
	}

	function Hide()
	{
		base.Hide();
		
		pGame.StopBackgroundTrack();
	}

	function GenerateLevel()
	{
		pDundeon.Generate(128, 128, 3, 10);

		SizeI = pDundeon.dungeon.n_cols + 1;
		SizeJ = pDundeon.dungeon.n_rows + 1;

		TileW = 32;
		TileH = 32;

		pTileMap.Init(SizeI,SizeJ,TileW,TileH);
		pPathMap.Init(SizeI,SizeJ);
		
		pFogMap = ::array(SizeI*SizeJ);

		pTileMap.SetTexture(pGame.ResourceCache.GetTexture("data/summer.png"));

		local tnumi = 16;
		local tnumj = 24;

		for(local i=0;i<tnumi;i++)
		{
			for(local j=0;j<tnumj;j++)
			{
				pTileMap.AddTile(1+ j*tnumi + i, i*32, j*32, 32, 32);
			}
		}
		
		pEnemys = [];
		pChests = [];

		for(local i=0;i<SizeI;i++)
		{
			for(local j=0;j<SizeJ;j++)
			{
				local cell = pDundeon.dungeon.cell[j][i];
				
				pFogMap[j*SizeI+i] = 0.0;
				pTileMap.SetTileColor(i, j, 0xFFFFFF4F);
		  	
				if(cell == TTileType.ROOM || cell == TTileType.CORRIDOR)
				{
		  			pTileMap.SetTileID(i, j, 335);
					pPathMap.SetPass(i, j, true);
				}
				else
				if(cell == TTileType.PERIMETER)
				{
					pTileMap.SetTileID(i, j, 17);
					pPathMap.SetPass(i, j, false);
				}
				else
				if(cell == TTileType.SECRET)
				{
					pTileMap.SetTileID(i, j, 53);
					pPathMap.SetPass(i, j, false);
				}
				else
				if(cell == TTileType.ARCH ||  cell == TTileType.TRAPPED || cell == TTileType.LOCKED || cell == TTileType.UNLOCKED)
				{
					pTileMap.SetTileID(i, j, 335);
					pPathMap.SetPass(i, j, true);
				}
				else
				if(cell == TTileType.PORTC)
				{
					pTileMap.SetTileID(i, j, 53);
					pPathMap.SetPass(i, j, false);
				}
				else
				if(cell == TTileType.MONSTER)
				{
					pTileMap.SetTileID(i, j, 335);
					pPathMap.SetPass(i, j, false);
					
					local enemy = ::Enemy(i, j, 0, this);
					
					enemy.SetEnabled(false);
					
					pEnemys.push(enemy);
					pObjectLayer.AddChild(enemy);
				}
				else
				if(cell == TTileType.CHEST)
				{
					pTileMap.SetTileID(i, j, 335);
					pPathMap.SetPass(i, j, false);
					
					local chest = ::Chest(i, j, 3, 0, this);
					
					chest.SetEnabled(false);
					
					pChests.push(chest);
					pObjectLayer.AddChild(chest);
				}
				else
				{
					pTileMap.SetTileID(i, j, 146);
					pPathMap.SetPass(i, j, false);
				}
			}
		}
		
		UpdateFog(1, 1);

		pUnit = ::Player(1,1,0,this);
		pObjectLayer.AddChild(pUnit);
	}
	
	function StepOnTile(_unit, _i, _j)
	{
		//::print("Step on tile: " + _i + ", " + _j + "\n");
		
		pPathMap.SetPass(_i, _j, false);
		
		if(pFogMap[_j*SizeI+_i] > 0)
		{
			pTileMap.SetTileColor(_i, _j, 0xFF0000FF);
		}
		
		if(_unit == pUnit)
		{
			UpdateFog(_i, _j);
		}
	}
	
	function StepOutTile(_unit, _i, _j)
	{
		//::print("Step out tile: " + _i + ", " + _j + "\n");

		pPathMap.SetPass(_i, _j, true);
		
		if(pFogMap[_j*SizeI+_i] > 0)
		{
			pTileMap.SetTileColor(_i, _j, 0xFFFFFFFF);
		}
	}
	
	function UpdateFog(_i, _j)
	{
		for(local i=-7; i<=7; i++)
		{
			local newI = _i + i;
			
			if(newI < 0 || newI >= SizeI)
				continue;
				
			for(local j=-7; j<=7; j++)
			{
				local newJ = _j + j;
				
				if(newJ < 0 || newJ >= SizeJ)
					continue;

				local v1 = ::CVector2(_i, _j);
				local v2 = ::CVector2(newI, newJ);
				
				if(v1.Distance(v2) < 5)
				{
					pFogMap[newJ*SizeI+newI] = 1.0;
					
					if(pPathMap.GetPass(newI, newJ) || pTileMap.GetTileID(newI, newJ) != 335)
					{
						pTileMap.SetTileColor(newI, newJ, 0xFFFFFFFF);
					}
				}
			}
		}
	}
	
	function Killed(_unit)
	{	
		if(_unit == pUnit)
		{
			::print("Killed Player.\n");
			pUnit.Reset(1,1);
			pGame.SetState(pGame.pMenu);
		}
		else
		{
			local enm = pEnemys.find(_unit);
			
			::print("Killed Enemy. #" + enm + "\n");
			if(enm != null)
			{
				pObjectLayer.RemoveChild(_unit);
				pEnemys.remove(enm);
			}
		}
	}

	function ChestOpened(_chest)
	{
		local item = ::Math.Rand(0,100);
		if(item > 0 && item <= 33)
		{
			pUnit.HealthPotionCount++;
		}
		else
		if(item > 33 && item <= 66)
		{
			pUnit.StrangePotionCount++;
		}
		else
		{
			pUnit.SpeedPotionCount++;
		}
		
		pHUD.UpdateInventory(pUnit);
	}
	
	function UpdateCamera()
	{
		CameraX = -((pGame.ScreenWidth / 2.0) - pUnit.X * ScaleK);
		CameraY = -((pGame.ScreenHeight / 2.0) - pUnit.Y * ScaleK);

		if(CameraX < 0)
			CameraX = 0;
		if(CameraY < -100)
			CameraY = -100;
			
		if(CameraX > (SizeI * TileW * ScaleK) - pGame.ScreenWidth && CameraX > 0)
			CameraX = (SizeI * TileW * ScaleK) - pGame.ScreenWidth;
		if(CameraY > (SizeJ * TileH * ScaleK) - pGame.ScreenHeight && CameraY > 0)
			CameraY = (SizeJ * TileH * ScaleK) - pGame.ScreenHeight;

		pTileMap.SetCamera(CameraX, CameraY, 0, 0);
		pObjectLayer.SetPosition(-CameraX, -CameraY);
	}
	
	function Update(dt)
	{
		base.Update(dt);

		pUnit.Update(dt);

		foreach(e in pEnemys)
		{	
			if(!e.GetEnabled())
			{
				if(pFogMap[e.J*SizeI+e.I] > 0)
				{
					e.SetEnabled(true);
						
					::print("Activate enemy.\n");
				}
			}
			else
			{
				e.Update(dt);
			}
		}
		
		foreach(c in pChests)
		{
			if(!c.GetEnabled() && c.State != TChestState.OPENED)
			{
				if(pFogMap[c.J*SizeI+c.I] > 0)
				{
					c.SetEnabled(true);
						
					::print("Activate chest.\n");
				}
			}
			else
			{
				c.Update(dt);
			}
		}

		UpdateCamera();
	}

	function Scale(dk)
	{
		ScaleK += dk * 0.01;

		if(ScaleK < 1.0)
			ScaleK = 1.0;
		if(ScaleK > 4.0)
			ScaleK = 4.0;

		pTileMap.SetScale(ScaleK, ScaleK);
		pObjectLayer.SetScale(ScaleK, ScaleK);
	}
	
	function HandleTouch(x, y, state, i)
	{
		if(pHUD.ProcessTouch(x, y, state))
		{
			return;
		}
		
		if(state!=0 && state!=1)
		{
			for(local i=0;i<pEnemys.len();i++)
			{
				if(pEnemys[i].GetEnabled() && pEnemys[i].HitTest(x, y))
				{
					pUnit.Attack(pEnemys[i]);
					return;
				}
			}
			
			for(local i=0;i<pChests.len();i++)
			{
				if(pChests[i].GetEnabled() && pChests[i].HitTest(x, y))
				{
					pUnit.GoToInteract(pChests[i]);
					return;
				}
			}

			pUnit.MoveTo(((CameraX + x)/(32*ScaleK)).tointeger(), ((CameraY + y)/(32*ScaleK)).tointeger());
		}
	}

	function HandleScale(k)
	{
		Scale(k);
	}
}