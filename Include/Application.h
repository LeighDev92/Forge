#pragma once
#include "framework.h"
#include "resource.h"
#include "Camera.h"

namespace Forge
{
	static const int MAX_LOADSTRING = 100;

	using BindingID = uint64_t;
	struct MouseState;

	class InputManager;
	class GraphicsManager;
	class MainWindow;
	class Mesh;

	class FORGE_API Application
	{
	public:
		static Application* Get();

		Application(HINSTANCE _hInstance, int _nCMDShow);
		~Application();

		int Run();
		virtual void ProcessFrame(float deltaTime);

		InputManager* GetInputManager() const;
		GraphicsManager* GetGraphicsManager() const;
		Camera* GetCamera();
		int AddMesh(Mesh* mesh);
		void RemoveMesh(Mesh* mesh);

		RECT GetMainWindowViewport();

		HINSTANCE GetHInstance() const;
		int GetCMDShow() const;
		HACCEL GetAcceleratorTable() const;
		LPCWSTR GetTitle() const;
		LPCWSTR GetAuthor() const;
		LPCWSTR GetMainWindowClass() const;

	protected:
		static Application* instance;
		
		std::unique_ptr<MainWindow> main_window;
		std::unique_ptr<InputManager> input_manager;							
		std::unique_ptr<GraphicsManager> graphics_manager;

		std::vector<Mesh> meshes;

		Camera camera;

		HINSTANCE hInstance;									// Window Instance handle    
		int nCMDShow;											// WinMain Show Options

		WCHAR szTitle[MAX_LOADSTRING];							// The title bar text
		WCHAR szAuthor[MAX_LOADSTRING];							// The author of this program
		WCHAR szWindowClass[MAX_LOADSTRING];					// The main window class name

		HACCEL hAccelTable;										// Handle to the Accelerator Table

		std::chrono::steady_clock::time_point start_time;		// Application start time
		std::chrono::steady_clock::time_point last_frame_time;	// Time since last frame
	};
}
