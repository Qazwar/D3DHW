#include<Windows.h>

//�Զ���Ĵ��ڴ�����
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE:	//�������رհ�ť
		PostQuitMessage(69);	//�׳���wParam
		break;
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
	const auto pClassName = "D3DHW";
	//register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	//DefWindowProc Ĭ�ϵĴ��ڴ����������ǿ��԰Ѳ����ĵ���Ϣ��������������
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
	//Ϊ����ע���¼�����
	BOOL gResult; //an int
	MSG msg;	//ע����Ϣ
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