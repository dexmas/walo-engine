class State extends CNode2D
{
	pGame = null;

	constructor(game)
	{
		base.constructor();
		pGame = game;

		SetEnabled(false);
	}

	function Update(dt)
	{
	}

	function HandleTouch(x, y, state, i)
	{
		
	}

	function HandleScale(k)
	{
		
	}

	function HandleKey(key, state)
	{
	}

	function Show()
	{
		SetEnabled(true);
	}

	function Hide()
	{
		SetEnabled(false);
	}
}