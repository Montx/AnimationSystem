#pragma once

enum class EInput {
	None,
	F1
};

class Application {
public:

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	inline Application() { }

	inline virtual ~Application() { }

	inline virtual void Initialize() { }

	inline virtual void HandleInput(EInput input) { }

	inline virtual void Update(float inDeltaTime) { }

	inline virtual void Render(float inAspectRatio) { }

	inline virtual void Shutdown() { }
};