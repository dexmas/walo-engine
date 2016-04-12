import("state.nut");
import("menu.nut");
import("unit.nut");
import("player.nut");
import("enemy.nut");
import("chest.nut");
import("dungeon.nut");
import("game.nut");
import("hud.nut");

class AdventureGame extends CGame
{
	pMenu = null;
	pGame = null;

	pCurr = null;
	bMouseBtnPressed = false;

	pBackgroundMusic = null;

	function Configure()
	{
		//Set this values only for desktop platforms now
		//SetScreenSize(ScreenWidth, ScreenHeight);

		EnableDebug(true);
		EnableConsole(true);
		SetFPSLimit(60);
	}

	function Init()
	{
		pMenu = ::MMenuST(this);
		pGame = ::GameST(this);

		Scene.AddChild(pMenu);
		Scene.AddChild(pGame);

		local node = ::CNode3D();
		node.SetPosition(0,0,0);

		Scene.AddChild(node);

		pBackgroundMusic = ::CSoundSource();
		Scene.AddComponent(pBackgroundMusic);

		pBackgroundMusic.SetVolume(0.1);

		pMenu.Show();

		pCurr = pMenu;
	}

	function PlayBackgroundTrack(_name)
	{
		pBackgroundMusic.SetSound(ResourceCache.GetSound(_name));
		pBackgroundMusic.Play(true);
	}

	function StopBackgroundTrack()
	{
		pBackgroundMusic.Stop();
	}

	function SetState(state)
	{
		if(pCurr == state)
			return;

		if(pCurr!=0)
			pCurr.Hide();
		if(state)
			state.Show();

		pCurr = state;
	}

	function Update(dt)
	{
		if (!pCurr)
		    return;

		pCurr.Update(dt);
	}

	function KeyboardEvent(key, state)
	{
		if(!pCurr)
			return;

		pCurr.HandleKey(key, state);
	}

	function MouseEvent(x, y, state, btns)
	{
		if(!pCurr)
			return;
		
		if((btns & 1) && !bMouseBtnPressed)
		{
			pCurr.HandleTouch(x,y,0,0);
			bMouseBtnPressed = true;
		}
		else
		if((btns & 1) && bMouseBtnPressed)
			pCurr.HandleTouch(x,y,1,0);
		else
		if(!(btns & 1) && bMouseBtnPressed)
		{
			pCurr.HandleTouch(x,y,2,0);
			bMouseBtnPressed = false;
		}
	}
    
    function TouchEvent(x, y, type, i)
	{    
		if(!pCurr)
			return;

        pCurr.HandleTouch(x,y,type, i);
    }

	function GestureEvent(type, p1, p2)
	{
		if(!pCurr)
			return;

		if(type == 1)
		{
			pCurr.HandleScale(p2);
		}
	}
}

::AdventureGame();