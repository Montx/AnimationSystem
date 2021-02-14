#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN

#define WIN32_EXTRA_LEAN

#include "glad.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_internal.h>

#include <windows.h>
#include <windowsx.h>
#include <iostream>

#include "Application.h"
#include "SampleMeshLoading.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#if _DEBUG

#pragma comment( linker, "/subsystem:console" )

int main(int argc, const char** argv) {
	return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
}

#else

#pragma comment( linker, "/subsystem:windows" )

#endif

#pragma comment( lib, "opengl32.lib" )

/* Taken from wgl.h */

#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091

#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092

#define WGL_CONTEXT_FLAGS_ARB             0x2094

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001

#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);

typedef const char* (WINAPI* PFNWGLGETEXTENSIONSSTRINGEXTPROC) (void);

typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC) (int);

typedef int (WINAPI* PFNWGLGETSWAPINTERVALEXTPROC) (void);

/* Program Globals */

Application* gApplication = 0;
GLuint gVertexArrayObject = 0;

void ShowCustomWindow(bool show_demo_window, bool& show_another_window) {
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Custom Window");

	ImGui::Text("This is some useful text.");
	ImGui::Checkbox("Demon Window", &show_demo_window);
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGui::ColorEdit3("clear color", (float*)&clear_color);

	if (ImGui::Button("Button"))
		counter++;

	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %3.f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	if (show_another_window) {
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello from another window!");

		if (ImGui::Button("Close Me")) {
			show_another_window = false;
		}

		ImGui::End();
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {

	/* WINDOW CREATION */

	gApplication = new SampleMeshLoading();

	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = 0;
	wndclass.lpszClassName = (LPCWSTR)"Win32 Game Window";

	RegisterClassEx(&wndclass);

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect;
	SetRect(
		&windowRect,
		(screenWidth / 2) - (clientWidth / 2),
		(screenHeight / 2) - (clientHeight / 2),
		(screenWidth / 2) + (clientWidth / 2),
		(screenHeight / 2) + (clientHeight / 2));

	DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME);

	AdjustWindowRectEx(&windowRect, style, FALSE, 0);

	HWND hwnd = CreateWindowEx(
		0,
		wndclass.lpszClassName,
		(LPCWSTR)"Game Window",
		style,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		szCmdLine);

	HDC hdc = GetDC(hwnd);

	/* OPENGL CONTEXT */

	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat(hdc, &pfd);

	SetPixelFormat(hdc, pixelFormat, &pfd);

	/* Legacy Opengl Context Used to create a modern one */
	HGLRC tempRC = wglCreateContext(hdc);
	wglMakeCurrent(hdc, tempRC);

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	const int attribList[]{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, 0,
		WGL_CONTEXT_PROFILE_MASK_ARB,
		WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	/* Modern Opengl Context Creation */
	HGLRC hglrc = wglCreateContextAttribsARB(hdc, 0, attribList);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempRC);
	wglMakeCurrent(hdc, hglrc);

	if (!gladLoadGL()) {
		std::cout << "Could not initialize GLAD\n";
	}
	else {
		std::cout << "OpenGL Version " << GLVersion.major << "." << GLVersion.minor << "\n";
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	io.ConfigDockingWithShift = true;

	ImGui::StyleColorsDark();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL3_Init("#version 150");

	/* Enable VSynch Support */
	PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

	bool swapControlsSupported = strstr(_wglGetExtensionsStringEXT(), "WGL_EXT_swap_control") != 0;

	int vsynch = 0;

	if (swapControlsSupported) {
		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");

		if (wglSwapIntervalEXT(1)) {
			std::cout << "Enabled vsynch\n";
			vsynch = wglGetSwapIntervalEXT();
		}
		else {
			std::cout << "Could not enable synch\n";
		}
	}
	else {
		std::cout << "WGL_EXT_swap)control not supported\n";
	}

	/* VAO BINDING */
	glGenVertexArrays(1, &gVertexArrayObject);
	glBindVertexArray(gVertexArrayObject);

	/* SHOW WINDOW */

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	gApplication->Initialize();

	/* GAME LOOP */
	
	DWORD lastTick = GetTickCount();

	MSG msg;

	bool show_another_window = false;
	bool show_demo_window = false;

	while(true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

			if (msg.message == WM_QUIT) {
				break;
			}

			ImGui_ImplWin32_WndProcHandler(hwnd, msg.message, msg.wParam, msg.lParam);

			if (msg.message == WM_KEYDOWN) {
				if (msg.wParam == VK_F1) {
					imgui_on = !imgui_on;
				}

				if (imgui_on) {
					if (msg.wParam == VK_HOME) {
						show_demo_window = !show_demo_window;
					}
				}
			}
			
			if (!imgui_on) {
				if (msg.message == WM_MOUSEMOVE) {
					int xPos = GET_X_LPARAM(msg.lParam);
					int yPos = GET_Y_LPARAM(msg.lParam);

					if (gApplication != 0) {
						gApplication->MouseMovement(xPos, yPos);
					}
				}

				if (msg.message == WM_MOUSEWHEEL) {
					if (gApplication != 0) {
						gApplication->Zoom(GET_WHEEL_DELTA_WPARAM(msg.wParam));
					}
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (imgui_on) {
			if(show_demo_window) ImGui::ShowDemoWindow();
			gApplication->ShowDebugUi();
		}

		ImGui::Render();

		DWORD thisTick = GetTickCount();

		float dt = float(thisTick - lastTick) * 0.001f;

		lastTick = thisTick;

		if (gApplication != 0) {
			gApplication->Update(dt);
		}

		if (gApplication != 0) {
			RECT clientRect;

			GetClientRect(hwnd, &clientRect);

			clientWidth = clientRect.right - clientRect.left;
			clientHeight = clientRect.bottom - clientRect.top;

			glViewport(0, 0, clientWidth, clientHeight);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

			glPointSize(5.0f);
			glBindVertexArray(gVertexArrayObject);

			glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			float aspect = (float)clientWidth / (float)clientHeight;

			gApplication->Render(aspect);
		}

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			HGLRC hglrc = wglGetCurrentContext();
			HDC hdc = GetDC(hwnd);

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			
			wglMakeCurrent(NULL, NULL);
			wglMakeCurrent(hdc, hglrc);
		}

		if (gApplication != 0) {
			SwapBuffers(hdc);
			if (vsynch != 0) {
				glFinish();
			}
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	/* END OF GAME LOOP */

	if (gApplication != 0) {
		std::cout << "Expected application to be null on exit\n";
		delete gApplication;
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	switch (iMsg) {
		case WM_CLOSE:
			if (gApplication != 0) {
				gApplication->Shutdown();
				delete gApplication;
				gApplication = 0;
				DestroyWindow(hwnd);
			}
			else {
				std::cout << "Already shut down!\n";
			}
			break;

		case WM_DESTROY:
			if (gVertexArrayObject != 0) {
				HDC hdc = GetDC(hwnd);
				HGLRC hglrc = wglGetCurrentContext();

				glBindVertexArray(0);
				glDeleteVertexArrays(1, &gVertexArrayObject);

				gVertexArrayObject = 0;

				wglMakeCurrent(NULL, NULL);
				wglDeleteContext(hglrc);

				ReleaseDC(hwnd, hdc);

				PostQuitMessage(0);
			}
			else {
				std::cout << "Multiple destroy messages\n";
			}
			break;

			case WM_PAINT:
			case WM_ERASEBKGND:
				return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}