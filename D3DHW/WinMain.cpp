#include<Windows.h>
#include"WindowsMessageMap.h"

//自定义的窗口处理函数
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam, LPARAM lParam) {

	static WindowsMessageMap mm;
	OutputDebugString(mm(msg, lParam, wParam).c_str());


	switch (msg) {
	case WM_CLOSE:	//点击窗体关闭按钮
		PostQuitMessage(69);	//抛出给wParam
		break;
	case WM_KEYDOWN:
		if (wParam == 'F') {	//wParam对应字符ascii 
			SetWindowTextA(hWnd, "F down");
		}
		break;
	case WM_KEYUP:
		if (wParam == 'F') {	//wParam对应字符ascii 
			SetWindowTextA(hWnd, "F up");
		}
		break;
	}
	

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//windows类型的主方法
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	const auto pClassName = "D3DHW";
	//register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	//DefWindowProc 默认的窗口处理函数，我们可以把不关心的消息都丢给它来处理
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hCursor = nullptr;
	wc.hIcon = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	RegisterClassEx(&wc);
	
	//create window instance 
	HWND hWnd = CreateWindowEx(
		0,pClassName,
		"Happy Hard Window",
		WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,
		200,200,640,480,
		nullptr,nullptr,hInstance,nullptr
	);
	//show window
	ShowWindow(hWnd, SW_SHOW);
	
	//message pump
	//为窗体注册事件机制
	BOOL gResult; //an int
	MSG msg;	//注册消息
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	if (gResult == -1) {
		return -1;	//error
	}
	else {
		return msg.wParam;
	}
}