#include "stdafx.h"
#include "VertexObjet.hpp"
#include "VertexBuffer.hpp"

VertexObjet::VertexObjet()
	: VertexObjet(1)
{}

VertexObjet::VertexObjet(const UINT length)
	: Buffers(length)
{}

void VertexObjet::Assign(VertexBuffer&& single_buffer)
{
	Buffers.push_back(std::move(single_buffer));
}

void VertexObjet::Assign(const GLenum type, const UINT count)
{
	if (0 < count)
	{
		for (UINT i = 0; i < count; ++i)
		{
			Buffers.push_back(std::move(VertexBuffer(type)));
		}
	}
}

void VertexObjet::Attach(VertexBuffer* buffer, const UINT length)
{
	if (0 < length)
	{
		Buffers.reserve(Buffers.capacity() + length);
		
		std::for_each_n(buffer, length, [&](VertexBuffer& vbo) {
			Buffers.push_back(std::move(vbo));
		});
	}
}

template<std::size_t _Size>
inline void VertexObjet::Attach(VertexBuffer buffer[_Size])
{
	Attach(buffer, _Size);
}

void VertexObjet::Attach(std::initializer_list<VertexBuffer> buffers)
{
	auto sz = buffers.size();
	if (0 < sz)
	{
		Buffers.reserve(Buffers.capacity() + sz);

		std::for_each(buffers.begin(), buffers.end(), [&](const VertexBuffer& vbo) {
			Buffers.push_back(vbo);
		});
	}
}

void VertexObjet::SetBufferType(const GLenum type)
{
	std::for_each(Buffers.begin(), Buffers.end(), [&](VertexBuffer& vbo) {
		vbo.SetType(type);
	});
}

void VertexObjet::SetBufferType(const UINT index, const GLenum type)
{
	auto& vbo = Buffers.at(index);
	vbo.SetType(type);
}

void VertexObjet::BindData(const void* data, const GLsizeiptr size, const GLenum usage)
{
	std::for_each(Buffers.begin(), Buffers.end(), [&](VertexBuffer& vbo) {
		vbo.Bind(data, size, usage);
	});
}

void VertexObjet::BindData(const UINT index, const void* data, const GLsizeiptr size, const GLenum usage)
{
	auto& vbo = Buffers.at(index);
	vbo.Bind(data, size, usage);
}
