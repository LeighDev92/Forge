
#pragma once
#include "framework.h"

namespace Forge
{
	/**
	 * @brief Represents a base Windows application window.
	 *
	 * Provides functionality for registering a window class, creating
	 * and displaying a window, updating its state, and processing
	 * Windows messages.
	 *
	 * Designed to be extended by derived window classes.
	 */
	class FORGE_API Window
	{
	public:

		/**
		 * @brief Constructs a Window instance.
		 */
		Window();

		/**
		 * @brief Destroys the Window instance.
		 */
		~Window();

		/**
		 * @brief Registers the window class with the operating system.
		 *
		 * @return Identifier of the registered window class.
		 */
		virtual ATOM Register();

		/**
		 * @brief Creates the application window.
		 *
		 * @return TRUE if the window was successfully created;
		 * otherwise, FALSE.
		 */
		virtual BOOL Create();

		/**
		 * @brief Displays the window.
		 *
		 * @return TRUE if the operation succeeds; otherwise, FALSE.
		 */
		BOOL Show() const;

		/**
		 * @brief Updates the window.
		 *
		 * @return TRUE if the operation succeeds; otherwise, FALSE.
		 */
		BOOL Update() const;

		/**
		 * @brief Retrieves the handle associated with this window.
		 *
		 * @return Handle to the window.
		 */
		HWND GetHWND();

		/**
		 * @brief Processes a Windows message.
		 *
		 * @param hWnd Handle to the window receiving the message.
		 * @param message Windows message identifier.
		 * @param wParam Additional message-specific information.
		 * @param lParam Additional message-specific information.
		 *
		 * @return Result of processing the message.
		 */
		virtual LRESULT HandleMessage(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

	private:

		/**
		 * @brief Static window procedure responsible for dispatching
		 * Windows messages.
		 *
		 * @param hWnd Handle to the window receiving the message.
		 * @param message Windows message identifier.
		 * @param wParam Additional message-specific information.
		 * @param lParam Additional message-specific information.
		 *
		 * @return Result of message processing.
		 */
		static LRESULT CALLBACK WndProc(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

		/**
		 * @brief Thunk procedure used to associate Windows messages
		 * with the corresponding Window instance.
		 *
		 * @param hWnd Handle to the window receiving the message.
		 * @param message Windows message identifier.
		 * @param wParam Additional message-specific information.
		 * @param lParam Additional message-specific information.
		 *
		 * @return Result of message processing.
		 */
		static LRESULT CALLBACK WndProcThunk(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

	protected:

		/**
		 * @brief Handles the About dialog's Windows messages.
		 *
		 * @param hDlg Handle to the About dialog.
		 * @param message Windows message identifier.
		 * @param wParam Additional message-specific information.
		 * @param lParam Additional message-specific information.
		 *
		 * @return Result of dialog message processing.
		 */
		static INT_PTR CALLBACK About(
			HWND hDlg,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

		HWND _hWnd;
	};
}
