#pragma once
#include "stdafx.h"
#include "Shader.hpp"

class Pipeline
{
public:
	Pipeline();
	Pipeline(const GLuint program);
	Pipeline(const Pipeline&) = delete;
	Pipeline(Pipeline&&) = default;

	void AssignProgram(const GLuint program);
	void LoadShaders(const Path& vs, const Path& fs);
	void AttachShader(const GLuint shader);
	void AttachShader(Shader& shader);

	void Readymade();

	void Use() const;
	void UseBuffer(VertexObjet& buffer, GLenum type);
	void UseBuffer(GLint buffer, GLenum type);

	UniformView GetUniform(const char* name) const;
	AttributeView GetAttribute(const char* name) const;

private:
	GLuint Self; // 프로그램
	std::shared_ptr<Shader> shVertex, shFragment;
};
