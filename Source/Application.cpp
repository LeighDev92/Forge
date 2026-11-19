#include "Application.h"
#include "InputManager.h"
#include "GraphicsManager.h"
#include "MainWindow.h"
#include "Mesh.h"

namespace Forge
{
	Application* Application::_instance = nullptr;

	Application* Application::Get()
	{
		return _instance;
	}
	Application::Application(HINSTANCE hInstance, int nCMDShow)
	{
		_hInstance = hInstance;
		_nCMDShow = nCMDShow;

		_instance = this;

		// Initialize global strings from string table defined in Forge.rc
		LoadStringW(_hInstance, IDS_APP_TITLE, _szTitle, MAX_LOADSTRING);
		LoadStringW(_hInstance, IDS_AUTHOR, _szAuthor, MAX_LOADSTRING);
		LoadStringW(_hInstance, IDC_FORGE, _szWindowClass, MAX_LOADSTRING);

		_hAccelTable = LoadAccelerators(_hInstance, MAKEINTRESOURCE(IDC_FORGE));

		_mainWindow = std::make_unique<MainWindow>();
		_inputManager =  std::make_unique<InputManager>();

		if (_mainWindow)
		{
			HWND hWnd = _mainWindow->GetHWND();
			RECT rect = Application::Get()->GetMainWindowViewport();
			try {
				_graphicsManager = std::make_unique<GraphicsManager>(_mainWindow->GetHWND());
				_currentCamera.SetPosition({ 0.f,-10.f, 0.f });
				_graphicsManager->SetCamera(&_currentCamera);
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

		_applicationStartTime = _lastFrameTime = std::chrono::steady_clock::now();
	}

	Application::~Application()
	{
		_inputManager.release();
		_graphicsManager.release();
		_mainWindow.release();
		_inputManager = nullptr;
		_graphicsManager = nullptr;
		_mainWindow = nullptr;
		_instance = nullptr;
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
			auto elapsed = std::chrono::duration<float>(time_now - _lastFrameTime);
			float deltaTime = elapsed.count();

			if (deltaTime > 0.f)
			{
				BeginFrame(deltaTime);
				EndFrame(deltaTime);
				_lastFrameTime = std::chrono::steady_clock::now();
			}
		}

		return msg.wParam;
	}

	void Application::BeginFrame(float deltaTime)
	{
		_graphicsManager->ClearScreen(0.f, 0.f, 0.f);

		for (Mesh& mesh : _meshes)
		{
			_graphicsManager->DrawMesh(&mesh, true, false);
		}
	}

	void Application::EndFrame(float deltaTime)
	{
		_graphicsManager->PresentFrame();
	}

	InputManager* Application::GetInputManager() const
	{
		return _inputManager.get();
	}
	
	GraphicsManager* Application::GetGraphicsManager() const
	{
		return _graphicsManager.get();
	}

	Camera* Application::GetCamera()
	{
		return &_currentCamera;
	}

	int Application::AddMesh(Mesh* mesh)
	{
		if (mesh == nullptr)
		{
			return -1;
		}

		_meshes.push_back(*mesh);

		return _meshes.size() - 1;
	}

	void Application::RemoveMesh(Mesh* mesh)
	{
		auto it = std::find_if(_meshes.begin(), _meshes.end(),
			[mesh](Mesh& existingMesh)
			{
				return &existingMesh == mesh;
			});

		if (it != _meshes.end())
		{
			_meshes.erase(it);
		}
	}

	RECT Application::GetMainWindowViewport()
	{
		RECT rect = {800, 600};

		if (_mainWindow)
		{
			GetClientRect(_mainWindow->GetHWND(), &rect);
		}

		return rect;
	}
	HINSTANCE Application::GetHInstance() const
	{
		return _hInstance;
	}

	int Application::GetCMDShow() const
	{
		return _nCMDShow;
	}

	HACCEL Application::GetAcceleratorTable() const
	{
		return _hAccelTable;
	}

	LPCWSTR Application::GetTitle() const
	{
		return _szTitle;
	}

	LPCWSTR Application::GetAuthor() const
	{
		return _szAuthor;
	}

	LPCWSTR Application::GetMainWindowClass() const
	{
		return _szWindowClass;
	}
}