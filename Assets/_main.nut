
class CTestGame extends CGame
{
	LogoSprite = null;
	LogoText = null;

	Cube = null;
	Camera = null;

	X = 129;
	R = 0.0;

	CRX = 0.0;
	CRY = 0.0;
	CRZ = 0.0;

	Speed = 56.0;

	CamMoveSpeed = -0.005;

	CameraPos = ::CVector3(0.0, 0.0, -20.0);

	function Init()
	{
		LogoSprite = ::CSprite2D();

		LogoSprite.SetTexture(ResourceCache.GetTexture("test.png"));
		LogoSprite.SetFrame(0,0,256,256);
		LogoSprite.SetCenter(128, 128);
		LogoSprite.SetPosition(250.0, 250.0);
		LogoSprite.SetFlip(true, true);
		LogoSprite.SetOrder(0.1);

		Scene.AddChild(LogoSprite);

		LogoText = ::CText2D();

        LogoText.SetFont(ResourceCache.GetFont("tfont1.fnt"));
        LogoText.SetText("Hello new WaloEngine!");
        LogoText.SetPosition(300, 200);
        LogoText.SetColor(1.0, 0, 0, 1.0);
        LogoText.SetOrder(0.3);

        Scene.AddChild(LogoText);

		Cube = ::CModel3D();
		
		Cube.SetPosition(5.0, 5.0, 5.0);
		Cube.SetScale(2.0, 2.0, 2.0);
		Cube.SetMesh(ResourceCache.GetMesh("cube.mdl"));

		Scene.AddChild(Cube);

		Camera = ::CCamera3D();
		Camera.SetPosition(CameraPos.X, CameraPos.Y, CameraPos.Z);
        Camera.SetRotation(0.0, 0.0, 0.0);

		Scene.AddChild(Camera);
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

        Cube.SetRotation(CRX, CRY, CRZ);

		LogoSprite.SetPosition(X, 250.0);
		LogoSprite.SetRotation(R);
	}

	function GestureEvent(type, p1, p2)
	{
		if(type == 1)
		{
			CameraPos += ::CVector3(0.0, 0.0, p2 * -CamMoveSpeed * 5);
			Camera.SetPosition(CameraPos.X, CameraPos.Y, CameraPos.Z);
		}
		else
		if(type == 2)
		{
			CameraPos += ::CVector3(p1 * -CamMoveSpeed/10.0, p2 * CamMoveSpeed/10.0, 0.0);
			Camera.SetPosition(CameraPos.X, CameraPos.Y, CameraPos.Z);
		}
	}
}

::CTestGame();