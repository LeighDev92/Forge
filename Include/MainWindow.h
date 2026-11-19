#pragma once
#include "Window.h"

namespace Forge
{
	/**
	 * @brief The primary application window, derived from the base Window class.
	 */
	class FORGE_API MainWindow : public Window
	{
	public:
		MainWindow();
		~MainWindow();

		virtual LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	};
}
