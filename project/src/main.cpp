//External includes
#ifdef ENABLE_VLD
#include "vld.h"
#endif
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "Timer.h"
#include "Renderer.h"
#include "Scene.h"

using namespace dae;

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	const uint32_t width = 640;
	const uint32_t height = 480;

	SDL_Window* pWindow = SDL_CreateWindow(
		"RayTracer - Alessandro Manzini",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	const auto pTimer = new Timer();
	const auto pRenderer = new Renderer(pWindow);

	const auto pScene = new Scene_W4_ReferenceScene();
	pScene->Initialize();

	//Start loop
	pTimer->Start();

	// Start Benchmark
	// pTimer->StartBenchmark();

	float printTimer = 0.f;
	bool isLooping = true;
	bool takeScreenshot = false;
	while (isLooping)
	{
		//--------- Get input events ---------
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				if ( e.key.keysym.scancode == SDL_SCANCODE_X )
				{
					takeScreenshot = true;
				}
				else if ( e.key.keysym.scancode == SDL_SCANCODE_F2 )
				{
					pRenderer->ToggleShadows( );
				}
				else if ( e.key.keysym.scancode == SDL_SCANCODE_F3 )
				{
					pRenderer->ToggleLightingMode( );
				}
				break;
			}
		}

		//--------- Update ---------
		pScene->Update(pTimer);

		//--------- Render ---------
		pRenderer->Render(pScene);

		//--------- Timer ---------
		pTimer->Update();
		printTimer += pTimer->GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "dFPS: " << pTimer->GetdFPS() << std::endl;
		}

		//Save screenshot after full render
		if (takeScreenshot)
		{
			if (!pRenderer->SaveBufferToImage())
				std::cout << "Screenshot saved!" << std::endl;
			else
				std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
			takeScreenshot = false;
		}
	}
	pTimer->Stop();

	// Todo (1) - Test Vector3
	float dotResult{};
	dotResult = Vector3::Dot( Vector3::UnitX, Vector3::UnitX ); // 1
	dotResult = Vector3::Dot( Vector3::UnitX, -Vector3::UnitX ); // -1
	dotResult = Vector3::Dot( Vector3::UnitX, Vector3::UnitY ); // 0

	Vector3 crossResult{};
	crossResult = Vector3::Cross( Vector3::UnitZ, Vector3::UnitX ); // (0, 1, 0)
	crossResult = Vector3::Cross( Vector3::UnitX, Vector3::UnitZ ); // (0, -1, 0)

	//Shutdown "framework"
	delete pScene;
	delete pRenderer;
	delete pTimer;

	ShutDown(pWindow);
	return 0;
}