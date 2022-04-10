#pragma once
#include "stdafx.h"
#include "Shader.hpp"

class Pipeline
{
private:
	GLuint Self; // ���α׷�
	std::shared_ptr<Shader> shVertex, shFragment;

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

	GLint GetUniform(const char* name) const;
	AttributeView GetAttribute(const char* name) const;
};