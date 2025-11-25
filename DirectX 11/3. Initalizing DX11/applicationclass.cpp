#include "applicationclass.h"
#
#include <iostream>
#include <fstream>


ApplicationClass::ApplicationClass() { m_Direct3D = 0; }
ApplicationClass::ApplicationClass(const ApplicationClass& o) {}
ApplicationClass::~ApplicationClass() {}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
	m_Direct3D = new D3DClass;

	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"ERROR", MB_OK);
		return false;
	}

	return true;
}

void ApplicationClass::Shutdown() {
	if (m_Direct3D) {
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}


bool ApplicationClass::Frame() {
	return Render();
}


bool ApplicationClass::Render() {
	m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0);


	m_Direct3D->EndScene();
	return true;
}

void ApplicationClass::writetofile() {
	std::ofstream file("vci.txt", std::ios::app);

	char buf[128];
	int mem;

	m_Direct3D->GetVideoCardInfo(buf, mem);

	file << buf << std::endl;
	file << mem << std::endl;

	file.close();
}
