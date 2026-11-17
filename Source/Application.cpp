#include "Application.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "MainWindow.h"
#include "Mesh.h"

namespace Forge
{
	Application* Application::instance = nullptr;

	Application* Application::Get()
	{
		return instance;
	}
	Application::Application(HINSTANCE _hInstance, int _nCMDShow)
	{
		hInstance = _hInstance;
		nCMDShow = _nCMDShow;

		instance = this;

		// Initialize global strings from string table defined in Forge.rc
		LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		LoadStringW(hInstance, IDS_AUTHOR, szAuthor, MAX_LOADSTRING);
		LoadStringW(hInstance, IDC_FORGE, szWindowClass, MAX_LOADSTRING);

		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FORGE));

		main_window = std::make_unique<MainWindow>();
		input_manager =  std::make_unique<InputManager>();

		if (main_window)
		{
			HWND hWnd = main_window->GetHWND();
			RECT rect = Application::Get()->GetMainWindowViewport();
			try {
				graphics_manager = std::make_unique<GraphicsManager>(main_window->GetHWND());
				camera.SetPosition({ 0.f,-10.f, 0.f });
				graphics_manager->SetCamera(&camera);
			}
			catch (const std::runtime_error& e) {
				OutputDebugStringA(e.what());
				exit(0);
			}
		}
		else
		{
			OutputDebugStringW(L"Application: ERROR - Unable to create Main Window\n");
			exit(-1);
		}

		start_time = last_frame_time = std::chrono::steady_clock::now();
	}

	Application::~Application()
	{
		input_manager.release();
		graphics_manager.release();
		main_window.release();
		input_manager = nullptr;
		graphics_manager = nullptr;
		main_window = nullptr;
		instance = nullptr;
	}

	int Application::Run()
	{
		MSG msg = {};

		bool running = true;
		while (running)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					running = false;
				}

				if (!TranslateAccelerator(msg.hwnd, GetAcceleratorTable(), &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			if (!running)
			{
				break;
			}

			auto time_now = std::chrono::steady_clock::now();
			auto elapsed = std::chrono::duration<float>(time_now - last_frame_time);
			float deltaTime = elapsed.count();

			if (deltaTime > 0.f)
			{
				ProcessFrame(elapsed.count());
				graphics_manager->PresentFrame();
				last_frame_time = std::chrono::steady_clock::now();
			}
		}

		return msg.wParam;
	}

	void Application::ProcessFrame(float deltaTime)
	{
		graphics_manager->ClearRenderTargetView(0.f, 0.f, 0.f);

		for (Mesh& mesh : meshes)
		{
			graphics_manager->DrawMesh(&mesh, true, false);
		}
	}

	InputManager* Application::GetInputManager() const
	{
		return input_manager.get();
	}
	
	GraphicsManager* Application::GetGraphicsManager() const
	{
		return graphics_manager.get();
	}

	Camera* Application::GetCamera()
	{
		return &camera;
	}

	int Application::AddMesh(Mesh* mesh)
	{
		if (mesh == nullptr)
		{
			return -1;
		}

		meshes.push_back(*mesh);

		return meshes.size() - 1;
	}

	void Application::RemoveMesh(Mesh* mesh)
	{
		auto it = std::find_if(meshes.begin(), meshes.end(),
			[mesh](Mesh& existingMesh)
			{
				return &existingMesh == mesh;
			});

		if (it != meshes.end())
		{
			meshes.erase(it);
		}
	}

	RECT Application::GetMainWindowViewport()
	{
		RECT rect = {800, 600};

		if (main_window)
		{
			GetClientRect(main_window->GetHWND(), &rect);
		}

		return rect;
	}
	HINSTANCE Application::GetHInstance() const
	{
		return hInstance;
	}

	int Application::GetCMDShow() const
	{
		return nCMDShow;
	}

	HACCEL Application::GetAcceleratorTable() const
	{
		return hAccelTable;
	}

	LPCWSTR Application::GetTitle() const
	{
		return szTitle;
	}

	LPCWSTR Application::GetAuthor() const
	{
		return szAuthor;
	}

	LPCWSTR Application::GetMainWindowClass() const
	{
		return szWindowClass;
	}
}