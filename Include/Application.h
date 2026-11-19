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

	/**
	* @brief Serves as the primary entry point and central coordinator of a Forge application. 
	
	Responsible for managing the application lifecycle, initializing core engine systems, 
	coordinating runtime operations, and maintaining references to essential engine 
	components and resources.
	*/
	class FORGE_API Application
	{
	public:

		/**
		 * @brief Retrieves the active Application instance.
		 *
		 * @return Pointer to the singleton Application instance,
		 * or nullptr if no instance exists.
		 */
		static Application* Get();

		/**
		 * @brief Constructs and initializes the Forge application.
		 *
		 * Initializes application instance data, loads application strings
		 * and accelerator resources, creates the main window and input manager,
		 * initializes the graphics manager, and configures the initial camera.
		 *
		 * Initializes the application start time and last frame timestamp.
		 *
		 * @param hInstance Handle to the current application instance.
		 * @param nCMDShow Specifies how the main window should be displayed.
		 */
		Application(HINSTANCE hInstance, int nCMDShow);

		/**
		 * @brief Releases application resources and resets the singleton instance.
		 *
		 * Releases ownership of the input manager, graphics manager, and main
		 * window, then clears the singleton instance pointer.
		 */
		~Application();

		/**
		 * @brief Executes the main application loop.
		 *
		 * Processes pending Windows messages, handles accelerator keys,
		 * and executes the application's frame lifecycle while the application
		 * is running.
		 *
		 * Each frame calculates the elapsed time since the previous frame
		 * and calls BeginFrame() followed by EndFrame().
		 *
		 * The loop terminates when a WM_QUIT message is received.
		 *
		 * @return The exit code supplied by the WM_QUIT message.
		 */
		virtual int Run();

		/**
		 * @brief Begins a new rendering frame.
		 *
		 * Clears the screen to black and iterates through the application's
		 * registered meshes, submitting each mesh to the graphics manager
		 * for solid rendering without wireframe rendering.
		 *
		 * @param deltaTime Elapsed time in seconds since the previous frame.
		 */
		virtual void BeginFrame(float deltaTime);

		/**
		 * @brief Completes the current rendering frame.
		 *
		 * Presents the rendered frame to the display through the graphics manager.
		 *
		 * @param deltaTime Elapsed time in seconds since the previous frame.
		 */
		virtual void EndFrame(float deltaTime);

		/**
		 * @brief Retrieves the application's input manager.
		 *
		 * @return Pointer to the application's InputManager.
		 */
		InputManager* GetInputManager() const;

		/**
		 * @brief Retrieves the application's graphics manager.
		 *
		 * @return Pointer to the application's GraphicsManager.
		 */
		GraphicsManager* GetGraphicsManager() const;

		/**
		 * @brief Retrieves the application's active camera.
		 *
		 * @return Pointer to the application's current Camera.
		 */
		Camera* GetCamera();

		/**
		 * @brief Registers a mesh with the application.
		 *
		 * Copies the supplied mesh into the application's internal mesh
		 * collection so that it can be rendered during subsequent frames.
		 *
		 * @param mesh Pointer to the mesh to register.
		 *
		 * @return Index of the newly registered mesh in the internal
		 * collection, or -1 if the supplied pointer is nullptr.
		 */
		int AddMesh(Mesh* mesh);

		/**
		 * @brief Removes a mesh from the application's mesh collection.
		 *
		 * Searches for a mesh whose address matches the supplied pointer
		 * and erases the matching entry if found.
		 *
		 * @param mesh Pointer to the mesh instance to remove.
		 */
		void RemoveMesh(Mesh* mesh);

		/**
		 * @brief Retrieves the client area dimensions of the main window.
		 *
		 * Queries the main window's client rectangle. If the main window
		 * is unavailable, returns the default rectangle initialized to
		 * 800 by 600.
		 *
		 * @return Rectangle describing the main window's client area.
		 */
		RECT GetMainWindowViewport();

		/**
		 * @brief Retrieves the application instance handle.
		 *
		 * @return Handle to the current application instance.
		 */
		HINSTANCE GetHInstance() const;

		/**
		 * @brief Retrieves the requested main window display state.
		 *
		 * @return The display state supplied during application construction.
		 */
		int GetCMDShow() const;

		/**
		 * @brief Retrieves the application's accelerator table.
		 *s
		 * @return Handle to the loaded accelerator table.
		 */
		HACCEL GetAcceleratorTable() const;

		/**
		 * @brief Retrieves the application's title.
		 *
		 * @return Pointer to the application title string.
		 */
		LPCWSTR GetTitle() const;

		/**
		 * @brief Retrieves the application's author information.
		 *
		 * @return Pointer to the application author string.
		 */
		LPCWSTR GetAuthor() const;

		/**
		 * @brief Retrieves the registered main window class name.
		 *
		 * @return Pointer to the main window class name string.
		 */
		LPCWSTR GetMainWindowClass() const;


	protected:
		static Application* _instance;
		
		std::unique_ptr<MainWindow> _mainWindow;
		std::unique_ptr<InputManager> _inputManager;							
		std::unique_ptr<GraphicsManager> _graphicsManager;

		std::vector<Mesh> _meshes;											// collection of meshes

		Camera _currentCamera;												// Camera used for rendering

		HINSTANCE _hInstance;												// Window Instance handle    
		int _nCMDShow;														// WinMain Show Options

		WCHAR _szTitle[MAX_LOADSTRING];										// The title bar text
		WCHAR _szAuthor[MAX_LOADSTRING];									// The author of this program
		WCHAR _szWindowClass[MAX_LOADSTRING];								// The main window class name

		HACCEL _hAccelTable;												// Handle to the Accelerator Table

		std::chrono::steady_clock::time_point _applicationStartTime;		// Application start time
		std::chrono::steady_clock::time_point _lastFrameTime;				// Time since last frame
	};
}
