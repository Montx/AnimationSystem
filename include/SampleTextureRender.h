#ifndef _H_SAMPLE_
#define _H_SAMPLE_

#include "math/vec3.h"
#include "math/vec2.h"

#include "Application.h"
#include "render/Shader.h"
#include "render/Attribute.h"
#include "render/IndexBuffer.h"
#include "render/Texture.h"

#define DEG2RAD 0.0174533f

class SampleTextureRender : public Application {
protected:
	Shader* mShader;
	Attribute<vec3>* mVertexPositions;
	Attribute<vec3>* mVertexNormals;
	Attribute<vec2>* mVertexTexCoords;
	IndexBuffer* mIndexBuffer;
	Texture* mDisplayTexture;
	float mRotation;
	vec3 mTranslation;
public:
	void Initialize();
	void Update(float inDeltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
};

#endif
