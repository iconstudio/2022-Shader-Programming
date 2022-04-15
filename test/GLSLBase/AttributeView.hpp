#pragma once
#include "stdafx.h"

class AttributeView
{
public:
	AttributeView(const GLint id);

	void EnableVertexArray() const;
	void DisableVertexArray() const;

	/// <summary>
	/// glVertexAttribPointer(Self, length, type, GL_FALSE, stride, begin);
	/// </summary>
	/// <param name="type"></param>
	/// <param name="length"></param>
	/// <param name="stride"></param>
	/// <param name="begin"></param>
	void Stream(const GLenum type
		, const GLint length
		, const GLsizei stride = 0
		, const GLvoid* begin = nullptr) const;

private:
	GLint Self;
};
