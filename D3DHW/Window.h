#pragma once
#include"ChilliWin.h"
#include"D3DException.h"
#include"Keyboard.h"
#include"Mouse.h"
#include"Graphics.h"
#include <optional>
#include<memory>

//自定义的window
class Window {
public:
	class Exception : public D3DException
	{
		using D3DException::D3DException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};

private:
	//singleton manages registration/cleanup of window class
	//inner class 对外透明
	class WindowClass {
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Direct3D Engine Window";
		static WindowClass wndClass; //实例
		HINSTANCE hInst;

	};
public:
	Window(int width, int height, const char* name) ;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);	//set window title, fix FAIL
	static std::optional<int> ProcessMessages() noexcept;	//rewrite message queue
	//get Graphics through Gfx() cause need a hwnd to init Graphics
	Graphics& Gfx();
	
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	
private:
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;

public:
	Keyboard kbd;
	Mouse mouse;
};



// error exception helper macro
// get errors that returns error code
#define CHWND_EXCEPT( hr ) Window::HrException( __LINE__,__FILE__,hr ) 
// get last error which doesnot return error code
#define CHWND_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() ) 

#define CHWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )