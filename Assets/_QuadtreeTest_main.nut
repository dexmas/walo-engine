
class CTestSprite extends CSprite2D
{
	X = 0.0;
	Y = 0.0;
	R = 0.0;
	
	SpeedX = 0.0;
	SpeedY = 0.0;
	
	RSpeed = 0.0;
	
	constructor()
	{
		base.constructor();
		
		X = /*0 * 120 + 60;*/::Math.Rand(0, ::Game.ScreenWidth);
		Y = /*2 * 80 + 40;*/::Math.Rand(0, ::Game.ScreenHeight);
		
		SetTexture(::Game.AssetsDB.GetTexture("test.png"));
		SetFrame(0,0,256,256);
		SetCenter(128, 128);
		SetScale(0.2, 0.2);
		SetPosition(X, Y);
		SetOrder(0.1);
		
		RSpeed = ::Math.Rand(10.0, 100.0);
		SpeedX = ::Math.Rand(-100.0, 100.0);
		SpeedY = ::Math.Rand(-100.0, 100.0);
	}
	
	function Update(dt)
	{
		X += dt * SpeedX;
		Y += dt * SpeedY;
		R += dt * RSpeed;
		
		if(X > ::Game.ScreenWidth || X < 0)
			SpeedX *= -1;
			
		if(Y > ::Game.ScreenHeight || Y < 0)
			SpeedY *= -1;
		
		SetPosition(X, Y);
		SetRotation(R);
	}
}

class CTestGame extends CGame
{
	Sprites = [];

	function Init()
	{
		Sprites.resize(64);
		for(local i = 0; i < 64; i++)
		{
			Sprites[i] = ::CTestSprite();
			AddChild(Sprites[i]);
		}
	}
}

Game <- ::CTestGame();