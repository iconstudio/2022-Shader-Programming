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

	std::cout << "���� ���̴� �ҷ����� ��: " << fn_vertex << "\n";
	if (!shVertex->Load())
	{
		std::cout << "���� ���̴� " << fn_vertex << "�� �ҷ����� �߿� ���� �߻�!\n";
		return;
	}
	std::cout << "���� ���̴� " << fn_vertex << " �ҷ����� �Ϸ�.\n";

	std::cout << "���� ���̴� ������ ��: " << fn_vertex << "\n";
	if (!shVertex->Compile())
	{
		std::cout << "���� ���̴� " << fn_vertex << "�� �������ϴ� �߿� ���� �߻�!\n";
		return;
	}
	std::cout << "���� ���̴� " << fn_vertex << "�� ������ �Ϸ�.\n";

	auto fn_frag = vs.filename();
	shFragment = std::make_shared<Shader>(GL_FRAGMENT_SHADER, fs);

	std::cout << "���� ���̴� �ҷ����� ��: " << fn_frag << "\n";
	if (!shFragment->Load())
	{
		std::cout << "���� ���̴� " << fn_frag << "�� �ҷ����� �߿� ���� �߻�!\n";
		return;
	}
	std::cout << "���� ���̴� " << fn_frag << " �ҷ����� �Ϸ�.\n";

	std::cout << "���� ���̴� ������ ��: " << fn_frag << "\n";
	if (!shFragment->Compile())
	{
		std::cout << "���� ���̴� " << fn_frag << "�� �������ϴ� �߿� ���� �߻�!\n";
		return;
	}
	std::cout << "���� ���̴� " << fn_frag << "�� ������ �Ϸ�.\n";

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

	// Attach �Ϸ�� shaderProgram�� ��ŷ��
	glLinkProgram(Self);

	// ��ũ�� �����ߴ��� Ȯ��
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

	std::cout << caption << " �� ������ �Ϸ�.\n";
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
