#include <Windows.h>
#include "Window.h"
#include "Renderer.h"

#include "Cube.h"
#include "Terrain.h"

#include "Shape.h"
#include "Sky.h"
#include "GeometricPrimitive.h"
#include "Sun.h"
#include "Rocket.h"
#include "Moon.h"
#include <Keyboard.h>
#include "Camera.h"
#include "Launcher.h"
#include "AntTweakBar.h"

//Create all the shaders i'm going to use and put them all in 
//differnt classes

//Initialise them all seperatly in perhaps a game class



int WINAPI wWinMain(_In_ HINSTANCE const hInstance, _In_opt_ HINSTANCE const hPrevInstance, _In_ LPWSTR const lpCmdLine, const int cmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto width = 800; auto height = 600;
	auto timeDiviser = 1000.0f;
	static ULONGLONG timeStart = 0;
	const float cameraSpeed = 0.1;
	auto renderWireframe = D3D11_FILL_SOLID;
	auto launch = false;
	std::unique_ptr<DirectX::Keyboard>const keyboard = std::make_unique<DirectX::Keyboard>();

	std::unique_ptr<Window>const window = std::make_unique<Window>(width, height);
	window->initWindow(hInstance);

	ShowWindow(window->getHandle(), cmdShow);
	std::unique_ptr<Renderer>renderer = std::make_unique<Renderer>();

	renderer->InitDevice(*window);
	renderer->CreateRenderTarget(*window);

	//const auto device = renderer->GetDevice();
	const auto deviceContext = renderer->GetDeviceContext();
	const auto device = renderer->GetDevice();
	std::unique_ptr<Camera> const camera = std::make_unique<Camera>();

	std::unique_ptr<Shape> const shape = std::make_unique<Shape>();

	std::unique_ptr<Sky> const sky = std::make_unique<Sky>();
	sky->createMeshSky(*renderer, &renderWireframe);
	sky->createShadersSky(*renderer);

	std::unique_ptr<Sun>const sun = std::make_unique<Sun>();
	sun->createMeshSun(*renderer, &renderWireframe);
	sun->createShadersSun(*renderer);

	std::unique_ptr<Moon>const moon= std::make_unique<Moon>();
	moon->createMeshMoon(*renderer, &renderWireframe);
	moon->createShadersMoon(*renderer);

	std::unique_ptr<Launcher> const launcher = std::make_unique<Launcher>();
	launcher->createMeshLauncher(*renderer, &renderWireframe);
	launcher->createShadersLauncher(*renderer);

	std::unique_ptr<Rocket>const rocket = std::make_unique<Rocket>();
	rocket->createMeshRocket(*renderer, &renderWireframe);
	rocket->createShadersRocket(*renderer);

	std::unique_ptr<Terrain>const terrain = std::make_unique<Terrain>();
	terrain->createMeshTerrain(*renderer,/* cameraX, cameraY, cameraZ,*/ &renderWireframe);
	terrain->createShadersTerrain(*renderer);

	TwInit(TW_DIRECT3D11, device);
	TwWindowSize(width, height);
	TwBar *myBar;
	myBar = TwNewBar("Missile Launcher Tweak Bar");
	TwDefine("Ant Tweak Bar");
	int barSize[2] = { 224, 320 };
	TwSetParam(myBar, nullptr, "size", TW_PARAM_INT32, 2, barSize);

	camera->setPosition(0.0f, 0.0f, -15.0f);
	bool camera3 = false;
	bool camera4 = false;
	bool camera5 = false;

	//std::unique_ptr<Cube>cube = std::make_unique<Cube>();

	//cube->createMesh(*renderer, &renderWireframe);
	//cube->createShaders(*renderer);
	



	MSG msg = { nullptr };

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}



		static float time = 0.0f;

		const auto kb = keyboard->GetState();

		//Exit Game
		if (kb.Escape)
		{
			PostQuitMessage(0);
		}

		//Increase Speed of game
		if ((kb.LeftShift && kb.T) || (kb.RightShift && kb.T))
		{
			if (timeDiviser < 500)
				timeDiviser = 500;
			else
				timeDiviser -= 500;
			keyboard->Reset();

		}
		//Decrease Speed of game
		else if (kb.T)
		{
			if (timeDiviser >= 2000)
				timeDiviser = 2000;
			else
				timeDiviser += 500;
			keyboard->Reset();
		}


		if (kb.F1)
		{
			camera->setPosition(0.0f, 0.0f, -15.0f);
			camera->setRotation(0.0f, 0.0f, 0.0f);
			camera3 = false;
			camera4 = false;
			camera5 = false;

		}
		if (kb.F2)
		{
			camera->setPosition(1.0f, -11.0f, 4.0f);
			//const XMFLOAT3 newLookAtPos(1.0f, -11.0f, 4.0);
			//camera->setLookAt(newLookAtPos);

			camera3 = false;
			camera4 = false;
			camera5 = false;
		}
		//Looking at the rocket Tracking the rocket as it flies
		if (kb.F3)
		{
			camera->setRotation(0.0f, 0.0f, 0.0f);

			camera3 = true;
			camera4 = false;
			camera5 = false;
		}
		//A close up of the rockets nose  Tracking the rocket nose as it flies
		if (kb.F4)
		{
			camera->setRotation(0.0f, 0.0f, 0.0f);

			camera3 = false;
			camera4 = true;
			camera5 = false;
		}
		//A close up of the rockets body  Tracking the rocket body as it flies
		if (kb.F5)
		{
			camera->setRotation(0.0f, 0.0f, 0.0f);

			camera3 = false;
			camera4 = false;
			camera5 = true;
		}
		//Camera Forward
		if (camera3 == false && camera4 == false && camera5 == false)
		{
			if (kb.W)
			{
				camera->AdjustPosition(camera->GetForward() * cameraSpeed);
				//camera->setLookAt(defaultlookAtPos);
			}

		//Camera Backward
		if (kb.S)
		{
			camera->AdjustPosition(camera->GetBack() * cameraSpeed);
			//camera->setLookAt(defaultlookAtPos);
		}

		//Camera Left
		if (kb.A)
		{
			camera->AdjustPosition(camera->GetLeft() * cameraSpeed);
			//camera->setLookAt(defaultlookAtPos);
		}

		//Camera Right
		if (kb.D)
		{
			camera->AdjustPosition(camera->GetRight() * cameraSpeed);
			//camera->setLookAt(defaultlookAtPos);
		}

		//Camera Up
		if (kb.PageUp)
		{
			camera->AdjustPosition(camera->GetUp() * cameraSpeed);
			//camera->setLookAt(defaultlookAtPos);

		}

		//Camera Down
		if (kb.PageDown)
		{
			camera->AdjustPosition(camera->GetDown() * cameraSpeed);
			//const XMFLOAT3 i(0.0f, 0.0f, 0.0);
			//camera->setLookAt(i);

		}
		//Pan up
		if (kb.I)
		{
			camera->AdjustRotation(-0.01f, 0.0f, 0.0);
		}

		//Pan down
		if (kb.K)
		{
			camera->AdjustRotation(0.01f, 0.0f, 0.0);
		}

		//Pan Left
		if (kb.J)
		{
			camera->AdjustRotation(0.000f, -0.01f, 0.0);
		}
		//Pan Right
		if (kb.L)
		{
			camera->AdjustRotation(0.0f, 0.01f, 0.0);
		}
	}
		//Render Mode
		if(kb.F6)
		{
			static auto i = 0;
			switch (i)
			{
				case 0:
					renderWireframe = D3D11_FILL_WIREFRAME;
					break;
				case 1:
					renderWireframe = D3D11_FILL_SOLID;
					break;
				default:
					i = 0;
					renderWireframe = D3D11_FILL_WIREFRAME;
					break;
			}
			i++;
			keyboard->Reset();
		}

		if (kb.F11)
		{
			launch = true;
		}

		//Reset Game
		if (kb.R)
		{
			timeDiviser = 1000.0f;
			timeStart = 0.0f;
			camera->setPosition(0.0f, 0.0f, -15.0f);
			camera->setRotation(0.0f, 0.0f, 0.0f);
			launch = false;
			rocket->resetRocket();
		}

		renderer->BeginFrame();
		
		camera->RenderCamera();
		const auto viewMatrix = camera->getViewMatrix();
		const auto projectionMatrix = camera->getProjectionMatrix();

		
		ULONGLONG const timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		time = (timeCur - timeStart) / timeDiviser;

		sky->createMeshSky(*renderer, &renderWireframe);
		sky->Draw(deviceContext, &viewMatrix, &projectionMatrix);

		launcher->createMeshLauncher(*renderer, &renderWireframe);
		launcher->draw(deviceContext, &viewMatrix, &projectionMatrix/*, time*/);

		rocket->createMeshRocket(*renderer, &renderWireframe);
		rocket->draw(deviceContext, &viewMatrix, &projectionMatrix, launch);
	
		sun->createMeshSun(*renderer, &renderWireframe);
		sun->draw(deviceContext, &viewMatrix, &projectionMatrix, time);
		const XMVECTOR i = sun->getLightPos();

		moon->createMeshMoon(*renderer, &renderWireframe);
		moon->draw(deviceContext, &viewMatrix, &projectionMatrix, time);
		const XMVECTOR b = moon->getLightPos();

		terrain->setMoonLightPos(&b);
		terrain->setLightPosition(&i);
		terrain->createMeshTerrain(*renderer, &renderWireframe);
		terrain->draw(deviceContext, &viewMatrix, &projectionMatrix);

		auto const pos = rocket->getWorldMatrix();
		if (camera3)
		{
			XMFLOAT4X4 iqwe;
			XMStoreFloat4x4(&iqwe, *pos);
			const float a = iqwe._42;
			camera->setPosition(0.0f, a - 4, -4.0f);
		}
		if (camera4)
		{
			XMFLOAT4X4 iqwe;
			XMStoreFloat4x4(&iqwe, *pos);
			const float a = iqwe._42;
			camera->setPosition(0.0f, a - 2, -2.0f);
		}
		if (camera5)
		{
			XMFLOAT4X4 iqwe;
			XMStoreFloat4x4(&iqwe, *pos);
			const float a = iqwe._42;
			camera->setPosition(0.0f, a - 5, -2.0f);
		}

		TwDraw();


		renderer->EndFrame();
		//cube->update();
		//renderer->Update();
	}

	return 0;
}
