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

		AddChild(pMenu);
		AddChild(pGame);

		local node = ::CNode3D();
		node.SetPosition(0,0,0);

		AddChild(node);

		pBackgroundMusic = ::CSoundSource();
		AddComponent(pBackgroundMusic);

		pBackgroundMusic.SetVolume(0.1);

		pMenu.Show();

		pCurr = pMenu;
	}

	function PlayBackgroundTrack(_name)
	{
		pBackgroundMusic.SetSound(AssetsDB.GetSound(_name));
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
}

::AdventureGame();