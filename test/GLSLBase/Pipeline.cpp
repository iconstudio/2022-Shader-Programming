#include "stdafx.h"
#include "Pipeline.hpp"
#include "Shader.hpp"
#include "VertexObjet.hpp"
#include "UniformView.hpp"
#include "AttributeView.hpp"

Pipeline::Pipeline() : Pipeline(0) {}

Pipeline::Pipeline(const GLuint program)
	: Self(program)
	, shVertex(nullptr), shFragment(nullptr)
{}

void Pipeline::AssignProgram(const GLuint program)
{
	Self = program;
}

void Pipeline::LoadShaders(const Path& vs, const Path& fs)
{
	auto fn_vertex = vs.filename();
	shVertex = std::make_shared<Shader>(GL_VERTEX_SHADER, vs);

	std::cout << "정점 쉐이더 불러오는 중: " << fn_vertex << "\n";
	if (!shVertex->Load())
	{
		std::cout << "정점 쉐이더 " << fn_vertex << "를 불러오는 중에 오류 발생!\n";
		return;
	}
	std::cout << "정점 쉐이더 " << fn_vertex << " 불러오기 완료.\n";

	std::cout << "정점 쉐이더 컴파일 중: " << fn_vertex << "\n";
	if (!shVertex->Compile())
	{
		std::cout << "정점 쉐이더 " << fn_vertex << "를 컴파일하는 중에 오류 발생!\n";
		return;
	}
	std::cout << "정점 쉐이더 " << fn_vertex << "의 컴파일 완료.\n";

	auto fn_frag = vs.filename();
	shFragment = std::make_shared<Shader>(GL_FRAGMENT_SHADER, fs);

	std::cout << "조각 쉐이더 불러오는 중: " << fn_frag << "\n";
	if (!shFragment->Load())
	{
		std::cout << "조각 쉐이더 " << fn_frag << "를 불러오는 중에 오류 발생!\n";
		return;
	}
	std::cout << "조각 쉐이더 " << fn_frag << " 불러오기 완료.\n";

	std::cout << "조각 쉐이더 컴파일 중: " << fn_frag << "\n";
	if (!shFragment->Compile())
	{
		std::cout << "조각 쉐이더 " << fn_frag << "를 컴파일하는 중에 오류 발생!\n";
		return;
	}
	std::cout << "조각 쉐이더 " << fn_frag << "의 컴파일 완료.\n";

	AttachShader(*shVertex);
	AttachShader(*shFragment);
}

void Pipeline::AttachShader(const GLuint shader)
{
	glAttachShader(Self, shader);
}

void Pipeline::AttachShader(Shader& shader)
{
	shader.Attach(Self);
}

void Pipeline::Readymade()
{
	GLint Success = 0;

	// Attach 완료된 shaderProgram을 링킹함
	glLinkProgram(Self);

	// 링크가 성공했는지 확인
	glGetProgramiv(Self, GL_LINK_STATUS, &Success);

	const auto& vpath = shVertex->filepath;
	const auto& fpath = shFragment->filepath;

	if (!Success)
	{
		//ErrorDisplay(Self, " Error linking shader program: ", vpath.filename(), fpath.filename());
		return;
	}

	glValidateProgram(Self);
	glGetProgramiv(Self, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		//ErrorDisplay(Self, " Error validating shader program: ", vpath.filename(), fpath.filename());
		return;
	}

	glUseProgram(Self);

	auto caption = vpath.filename().string();
	auto it = caption.end() - 3;
	caption.erase(it, caption.end());

	std::cout << caption << " → 컴파일 완료.\n";
}

void Pipeline::Use() const
{
	glUseProgram(Self);
}

void Pipeline::UseBuffer(VertexObjet& buffer, GLenum type)
{
	buffer.SetBufferType(type);
}

void Pipeline::UseBuffer(GLint buffer, GLenum type)
{
	glBindBuffer(type, buffer);
}

UniformView Pipeline::GetUniform(const char* name) const
{
	auto id = glGetUniformLocation(Self, name);
	return (UniformView(id));
}

AttributeView Pipeline::GetAttribute(const char* name) const
{
	auto id = glGetAttribLocation(Self, name);
	return (AttributeView(id));
}
