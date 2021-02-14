#pragma once

static int clientWidth = 1200;
static int clientHeight = 900;

static bool imgui_on = false;

class Application {
public:

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	inline Application() { }

	inline virtual ~Application() { }

	inline virtual void Initialize() { }

	inline virtual void Zoom(float zoomDelta) {}
	inline virtual void MouseMovement(float x, float y) { }

	inline virtual void Update(float inDeltaTime) { }

	inline virtual void Render(float inAspectRatio) { }

	inline virtual void ShowDebugUi() {};

	inline virtual void Shutdown() { }
};