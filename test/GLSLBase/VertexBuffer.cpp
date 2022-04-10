#include "stdafx.h"
#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer()
	: VertexBuffer(0)
{}

VertexBuffer::VertexBuffer(const GLenum type)
	: Self(0), Type(type)
{
	glGenBuffers(1, &Self);
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::SetType(GLenum type)
{
	Type = type;
	glBindBuffer(Type, Self);
}

constexpr GLenum VertexBuffer::GetType() const
{
	return Type;
}

void VertexBuffer::Bind(const void* data, const GLsizeiptr size, const GLenum usage)
{
	glBindBuffer(Type, Self);
	glBufferData(Type, size, data, usage);
}
