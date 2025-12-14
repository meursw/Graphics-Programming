#include "applicationclass.h"

ApplicationClass::ApplicationClass() {}
ApplicationClass::ApplicationClass(const ApplicationClass& o) {}
ApplicationClass::~ApplicationClass() {}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {

	return true;
}

void ApplicationClass::Shutdown()
{

	return;
}


bool ApplicationClass::Frame() {

	return true;
}


bool ApplicationClass::Render() {

	return true;
}
