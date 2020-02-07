//#include<Windows.h>
#include"App.h"
#include<sstream>
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
	case WM_CHAR:	//a message for input text
	{
		static std::string title;
		title.push_back((char)wParam);
		SetWindowText(hWnd, title.c_str());
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		std::ostringstream oss;
		oss << "(" << pt.x << "," << pt.y << ")";
		SetWindowText(hWnd, oss.str().c_str());

	}
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
	//const auto pClassName = "D3DHW";
	////register window class
	//WNDCLASSEX wc = { 0 };
	//wc.cbSize = sizeof(wc);
	//wc.style = CS_OWNDC;
	////DefWindowProc 默认的窗口处理函数，我们可以把不关心的消息都丢给它来处理
	//wc.lpfnWndProc = WndProc;
	//wc.cbClsExtra = 0;
	//wc.cbWndExtra = 0;
	//wc.hInstance = hInstance;
	//wc.hCursor = nullptr;
	//wc.hIcon = nullptr;
	//wc.hbrBackground = nullptr;
	//wc.lpszMenuName = nullptr;
	//wc.lpszClassName = pClassName;
	//RegisterClassEx(&wc);
	//
	////create window instance 
	//HWND hWnd = CreateWindowEx(
	//	0,pClassName,
	//	"Happy Hard Window",
	//	WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,
	//	200,200,640,480,
	//	nullptr,nullptr,hInstance,nullptr
	//);
	////show window
	//ShowWindow(hWnd, SW_SHOW);
	//
	////message pump
	////为窗体注册事件机制
	try {
		return App().Go();
		//Window wnd(800, 300, "A window");
		//BOOL gResult; //an int
		//MSG msg;	//注册消息
		//while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		//	// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		//	TranslateMessage(&msg);
		//	DispatchMessage(&msg);

		//	//// test wheel up and downcode
		//	//static int i = 0;
		//	//while (!wnd.mouse.IsEmpty())
		//	//{
		//	//	const auto e = wnd.mouse.Read();
		//	//	switch (e.GetType())
		//	//	{
		//	//	case Mouse::Event::Type::WheelUp:
		//	//		i++;
		//	//		{
		//	//			std::ostringstream oss;
		//	//			oss << "Up: " << i;
		//	//			wnd.SetTitle(oss.str());
		//	//		}
		//	//		break;
		//	//	case Mouse::Event::Type::WheelDown:
		//	//		i--;
		//	//		{
		//	//			std::ostringstream oss;
		//	//			oss << "Down: " << i;
		//	//			wnd.SetTitle(oss.str());
		//	//		}
		//	//		break;
		//	//	}
		//	//}

		//	//// test mouse drag code
		//	//while (!wnd.mouse.IsEmpty())
		//	//{
		//	//	const auto e = wnd.mouse.Read();
		//	//	switch (e.GetType())
		//	//	{
		//	//	case Mouse::Event::Type::Leave:
		//	//		wnd.SetTitle("Gone!");
		//	//		break;
		//	//	case Mouse::Event::Type::Move:
		//	//	{
		//	//		std::ostringstream oss;
		//	//		oss << "Mouse moved to (" << e.GetPosX() << "," << e.GetPosY() << ")";
		//	//		wnd.SetTitle(oss.str());
		//	//	}
		//	//	break;
		//	//	}
		//	//}


		//	//// do app logic (test) mouse original test
		//	//while (!wnd.mouse.IsEmpty())
		//	//{
		//	//	const auto e = wnd.mouse.Read();
		//	//	if (e.GetType() == Mouse::Event::Type::Move)
		//	//	{
		//	//		std::ostringstream oss;
		//	//		oss << "Mouse Position: (" << e.GetPosX() << "," << e.GetPosY() << ")";
		//	//		wnd.SetTitle(oss.str());
		//	//	}
		//	//}

		//	//if (wnd.kbd.KeyIsPressed(VK_SPACE)) {
		//	//	MessageBox(nullptr, "Something Happon!", "Space Key Was Pressed", MB_OK | MB_ICONEXCLAMATION);
		//	//}
		//	//if (wnd.kbd.KeyIsPressed(VK_MENU))	//alt
		//	//{
		//	//	MessageBox(nullptr, "Something Happon!", "The alt key was pressed", MB_OK | MB_ICONEXCLAMATION);
		//	//}
		//}
		//// check if GetMessage call itself borked
		//if (gResult == -1) {
		//	throw CHWND_LAST_EXCEPT();	//error
		//}
		//// wParam here is the value passed to PostQuitMessage
		//return msg.wParam;
	}
	catch (const D3DException &e) {
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...){
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;


}