#include "WaloGame.hpp"

int main(int _argc, void** _argv)
{
	CGame* pGame = new CWaloGame();

	if(!pGame->Init())
    {
        return -1;
    }

	while(!pGame->GetDevice()->GetClosed())
	{
		pGame->Run();
	}

	pGame->Free();

	delete pGame;
    
	return 0;
}