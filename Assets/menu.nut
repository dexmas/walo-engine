class MMenuST extends State
{
	PlayBtn   = null;
	pFont 	  = null;
	pNameText = null;
	pBG = null;

	constructor(game)
	{
		base.constructor(game);
		
		pFont = pGame.ResourceCache.GetFont("data/shoebox.fnt");
		
		pBG = ::CSprite2D();
		pBG.SetTexture(pGame.ResourceCache.GetTexture("data/bg.png"));
		pBG.SetFrame(0,0,256,256);
		pBG.SetTiling(true, true);
		pBG.SetSize(pGame.ScreenWidth, pGame.ScreenHeight);
		
		AddChild(pBG);

		PlayBtn = ::CSprite2D();
		PlayBtn.SetTexture(pGame.ResourceCache.GetTexture("data/ui.png"));
		PlayBtn.SetFrame(0,80,100,100);
		PlayBtn.SetCenter(50,50);
		PlayBtn.SetPosition(pGame.ScreenWidth/2, pGame.ScreenHeight/2);
		PlayBtn.SetOrder(0.01);

		pNameText = ::CText2D();
		pNameText.SetFont(pFont);
		pNameText.SetText("ADVENTURE");
		pNameText.SetPosition(pGame.ScreenWidth/2 - 250, 50);
		pNameText.SetScale(2.0, 2.0);
		pNameText.SetOrder(0.01);

		AddChild(pNameText);
		AddChild(PlayBtn);
	}

	function Show()
	{
		base.Show();

		pGame.PlayBackgroundTrack("data/test.ogg");
	}

	function HandleTouch(x, y, state, i)
	{
		if(state == 0 || state == 1)
		{
			if(PlayBtn.HitTest(x,y))
			{
				PlayBtn.SetColor(1.0,0.6,0.6,1.0);
			}
			else
			{
				PlayBtn.SetColor(1.0,1.0,1.0,1.0);
			}
		}
		else
		{
			PlayBtn.SetColor(1.0,1.0,1.0,1.0);
			
			if(PlayBtn.HitTest(x,y))
			{
				pGame.SetState(pGame.pGame);
			}
		}
	}
}