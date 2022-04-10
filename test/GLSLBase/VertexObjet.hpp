#pragma once
#include "stdafx.h"

class VertexObjet
{
public:
	VertexObjet();
	VertexObjet(const UINT length);

	void Assign(VertexBuffer&& single_buffer);
	void Assign(const GLenum type, const UINT count);
	void Attach(VertexBuffer* buffer, const UINT length);
	template<std::size_t _Size> void Attach(VertexBuffer buffer[_Size]);
	void Attach(std::initializer_list<VertexBuffer> buffers);

	void SetBufferType(const GLenum type);
	void SetBufferType(const UINT index, const GLenum type);
	void BindData(const void* data, const GLsizeiptr size, const GLenum usage);
	void BindData(UINT index, const void* data, const GLsizeiptr size, const GLenum usage);

	std::vector<VertexBuffer> Buffers;
};
