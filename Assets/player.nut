class Player extends Unit
{
	HealthPotionCount = null;
	StrangePotionCount = null;
	SpeedPotionCount = null;

	function Reset(i, j)
	{
		base.Reset(i, j);

		Health = 600;
		Damage = 35;
		
		HealthPotionCount = 0;
		StrangePotionCount = 0;
		SpeedPotionCount = 0;

		pWorld.pHUD.UpdateHUD(this);
		pWorld.pHUD.UpdateInventory(this);
	}
	
	function Hit(_unit)
	{
		base.Hit(_unit);
		
		pWorld.pHUD.UpdateHUD(this);
	}
}