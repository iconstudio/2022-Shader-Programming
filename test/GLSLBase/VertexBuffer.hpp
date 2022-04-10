#pragma once
#include "stdafx.h"

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const GLenum type);
	~VertexBuffer();

	void SetType(GLenum type);
	constexpr GLenum GetType() const;

	void Bind(const void* data, const GLsizeiptr size, const GLenum usage);

	GLuint Self;

private:
	GLenum Type;
};

