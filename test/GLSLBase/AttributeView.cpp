#include "stdafx.h"
#include "AttributeView.hpp"

AttributeView::AttributeView(const GLint id)
	: Self(id)
{}

void AttributeView::EnableVertexArray() const
{
	glEnableVertexAttribArray(Self);
}

void AttributeView::DisableVertexArray() const
{
	glDisableVertexAttribArray(Self);
}

void AttributeView::Stream(const GLenum type, const GLint length, const GLsizei stride, const GLvoid* begin) const
{
	glVertexAttribPointer(Self, length, type, GL_FALSE, stride, begin);
}
