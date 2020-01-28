//#include<Windows.h>
#include"Window.h"
#include<sstream>
#include"WindowsMessageMap.h"

//�Զ���Ĵ��ڴ�������
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam, LPARAM lParam) {

	static WindowsMessageMap mm;
	OutputDebugString(mm(msg, lParam, wParam).c_str());


	switch (msg) {
	case WM_CLOSE:	//�������رհ�ť
		PostQuitMessage(69);	//�׳���wParam
		break;
	case WM_KEYDOWN:
		if (wParam == 'F') {	//wParam��Ӧ�ַ�ascii 
			SetWindowTextA(hWnd, "F down");
		}
		break;
	case WM_KEYUP:	
		if (wParam == 'F') {	//wParam��Ӧ�ַ�ascii 
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

//windows���͵�������
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
	////DefWindowProc Ĭ�ϵĴ��ڴ������������ǿ��԰Ѳ����ĵ���Ϣ��������������
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
	////Ϊ����ע���¼�����
	Window wnd(800, 300, "A window");
	BOOL gResult; //an int
	MSG msg;	//ע����Ϣ
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	if (gResult == -1) {
		return -1;	//error
	}
	return msg.wParam;
	

}