
class CTestCamera extends CCamera3D
{
	CamMoveSpeed = -0.005;

	CameraPos = ::CVector3(0.0, 0.0, -20.0);
	
	constructor()
	{
		base.constructor();
		
		SetPosition(CameraPos.X, CameraPos.Y, CameraPos.Z);
        SetRotation(0.0, 0.0, 0.0);
	}
	
	function InputGesture(type, p1, p2)
	{
		if(type == 1)
		{
			CameraPos += ::CVector3(0.0, 0.0, p2 * -CamMoveSpeed * 5);
			SetPosition(CameraPos.X, CameraPos.Y, CameraPos.Z);
		}
		else
		if(type == 2)
		{
			CameraPos += ::CVector3(p1 * -CamMoveSpeed/10.0, p2 * CamMoveSpeed/10.0, 0.0);
			SetPosition(CameraPos.X, CameraPos.Y, CameraPos.Z);
		}
	}
}

class CTestCube extends CModel3D
{
	CRX = 0.0;
	CRY = 0.0;
	CRZ = 0.0;
	
	X = 129;
	R = 0.0;
	
	Speed = 56.0;
	
	constructor()
	{
		base.constructor();
		
		SetPosition(5.0, 5.0, 5.0);
		SetScale(2.0, 2.0, 2.0);
		
		local mesh = ::Game.AssetsDB.GetMesh("cube.mdl");
		mesh.SetMaterial(::Game.AssetsDB.GetMaterial("test.mat"));
		SetMesh(mesh);
	}
	
	function Update(dt)
	{
		R += dt * 64.0;

		if(X + 128 > 960 || X < 128)
			Speed *= -1;

		X += dt * Speed;

        CRX += 10 * dt;
        CRY += 5  * dt;
        CRZ += 3  * dt;

        SetRotation(CRX, CRY, CRZ);
	}
}

class CTestSprite extends CSprite2D
{
	X = 129;
	R = 0.0;
	
	Speed = 56.0;
	
	constructor()
	{
		base.constructor();
		
		SetTexture(::Game.AssetsDB.GetTexture("test.png"));
		SetFrame(0,0,256,256);
		SetCenter(128, 128);
		SetPosition(250.0, 250.0);
		SetFlip(true, true);
		SetOrder(0.1);
	}
	
	function Update(dt)
	{
		R += dt * 64.0;

		if(X + 128 > 960 || X < 128)
			Speed *= -1;

		X += dt * Speed;
		
		SetPosition(X, 250.0);
		SetRotation(R);
	}
}

class CTestGame extends CGame
{
	LogoSprite = null;
	LogoText = null;
	Cube = null;
	Camera = null;

	function Init()
	{
		LogoSprite = ::CTestSprite();
		AddChild(LogoSprite);

		LogoText = ::CText2D();

        LogoText.SetFont(AssetsDB.GetFont("tfont1.fnt"));
        LogoText.SetText("Hello new WaloEngine!");
        LogoText.SetPosition(300, 200);
        LogoText.SetColor(1.0, 0, 0, 1.0);
        LogoText.SetOrder(0.3);

        AddChild(LogoText);

		Cube = ::CTestCube();
		AddChild(Cube);

		Camera = ::CTestCamera();
		AddChild(Camera);
	}
}

Game <- ::CTestGame();