#pragma once

class IndexBuffer;

enum class DrawMode {
	Points,
	LineStrip,
	LineLoop,
	Lines,
	Triangles,
	TriangleStrip,
	TriangleFan
};

void Draw(IndexBuffer& inIndexBuffer, DrawMode mode);
void Draw(unsigned int vertexCount, DrawMode mode);

void DrawInstance(IndexBuffer& inIndexBuffer, DrawMode mode, unsigned int instanceCount);
void DrawInstance(unsigned int vertexCount, DrawMode mode, unsigned int numInstances);