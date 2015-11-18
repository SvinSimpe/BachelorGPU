#include "../Header/D3DApp.h"

#include <crtdbg.h>



int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{

#if defined(DEBUG) | defined(_DEBUG)

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(200); // Break at specific memory allocation point

#endif

	int retVal = 0;

	D3DApp* app = new D3DApp();

	if( FAILED( retVal = app->InitializeWindow( hInstance, nCmdShow ) ) )
		return retVal;

	if( FAILED( retVal = app->InitializeDirectX11() ) )
		return retVal;

	if( FAILED( retVal = app->InitializeSimulationComponents() ) )
		return retVal;


	retVal = app->Run();

	app->Release();

	delete app;

	return retVal;
}