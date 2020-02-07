//#define _CRT_SECURE_NO_WARNINGS
//#include<Windows.h>
//#include<Wininet.h>
//#include<ShlObj.h>
//#include<iostream>
//
//int main() {
//	CoInitialize(nullptr);
//
//	IActiveDesktop* pDesktop = nullptr;
//	WCHAR wszWallpaper[MAX_PATH];
//
//	CoCreateInstance(
//		CLSID_ActiveDesktop,
//		nullptr,
//		CLSCTX_INPROC_SERVER,
//		__uuidof(IActiveDesktop),
//		reinterpret_cast<void**> (&pDesktop)
//	);
//
//	pDesktop->GetWallpaper(wszWallpaper, MAX_PATH, 0);
//	pDesktop->Release();
//	std::wcout << wszWallpaper;
//
//	IShellLink* pLink = nullptr;
//	CoCreateInstance(
//		CLSID_ShellLink,
//		nullptr,
//		CLSCTX_INPROC_SERVER,
//		__uuidof(IShellLink),
//		reinterpret_cast<void**> (&pLink)
//	);
//
//	char path[2000];
//	//convertion
//	wcstombs(path, wszWallpaper, 2000);
//
//	pLink->SetPath(path);
//
//	IPersistFile* pPersist = nullptr;
//	pLink->QueryInterface(__uuidof(IPersistFile), reinterpret_cast<void **>(&pPersist));
//
//	//persistance
//	pPersist->Save(L"C:\\wallpaper.lnk", FALSE);
//	pPersist->Release();
//	pLink->Release();
//
//	CoUninitialize();
//	std::cin.get();
//	return 0;
//
//}