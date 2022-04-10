#pragma once
#include "stdafx.h"

class AttributeView
{
public:
	AttributeView(const GLint id);

	void EnableVertexArray() const;
	void DisableVertexArray() const;

	void Stream(const GLenum type
		, const GLint length
		, const GLsizei stride = 0
		, const GLvoid* begin = nullptr) const;

private:
	GLint Self;
};
