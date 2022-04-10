#include "stdafx.h"
#include "Shader.hpp"
#include "Pipeline.hpp"

Shader::Shader()
	: Shader(0)
{}

Shader::Shader(const GLenum type)
	: Shader(type, "")
{}

Shader::Shader(const GLenum type, const Path& file)
	: Self(0), Type(type)
	, blobStatus(BLOB_STATES::UNLOADED), codeStatus(CODE_STATES::NONE)
	, filepath(file), Blob(), ProgramID(0)
{}

Shader::~Shader()
{
	Blob.clear();
	Blob.shrink_to_fit();

	glDeleteShader(Self);
}

void Shader::SetType(const GLenum type)
{
	Type = type;
}

constexpr GLenum Shader::GetType() const
{
	return Type;
}

bool Shader::Load()
{
	if (!IsLoaded() && filepath.empty())
	{
		return false;
	}

	return Load(filepath);
}

bool Shader::Load(const Path& filepath)
{
	std::ifstream file(filepath);

	if (file.fail())
	{
		blobStatus = BLOB_STATES::UNLOADED;

		std::cout << " ���̴� ���� " << filepath.filename() << " �ҷ����� ����!\n";

		file.close();
		return false;
	}

	std::string line;
	while (std::getline(file, line))
	{
		Blob.append(line.c_str());
		Blob.append("\n");
	}

	blobStatus = BLOB_STATES::LOADED;
	return true;
}

bool Shader::Compile()
{
	codeStatus = CODE_STATES::FAILED;

	if (IsLoaded())
	{
		Self = glCreateShader(Type);
		if (NULL == Self)
		{
			fprintf(stderr, "Error creating shader type %d\n", Type);
		}

		const GLchar* blob = Blob.c_str();
		GLint length = GLint(Blob.length());

		//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
		glShaderSource(Self, 1, &blob, &length);

		//�Ҵ�� ���̴� �ڵ带 ������
		glCompileShader(Self);

		// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
		GLint success;
		glGetShaderiv(Self, GL_COMPILE_STATUS, &success);
		if (success)
		{
			codeStatus = CODE_STATES::SUCCEED;
			return true;
		}
		else
		{
			GLchar InfoLog[1024];

			//OpenGL �� shader log �����͸� ������
			glGetShaderInfoLog(Self, 1024, NULL, InfoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", Type, InfoLog);
			printf("%s \n", blob);
		}
	}

	return false;
}

void Shader::Attach(const GLuint program)
{
	if (program != ProgramID)
	{
		if (0 != ProgramID)
		{
			glDetachShader(ProgramID, Self);
		}

		ProgramID = program;
		glAttachShader(ProgramID, Self);
	}
}

constexpr bool Shader::IsUnloaded() const
{
	return BLOB_STATES::UNLOADED == blobStatus;
}

constexpr bool Shader::IsLoaded() const
{
	return BLOB_STATES::LOADED == blobStatus;
}

constexpr bool Shader::IsCompiled() const
{
	return IsLoaded() && IsSucceed();
}

constexpr bool Shader::IsSucceed() const
{
	return CODE_STATES::SUCCEED == codeStatus;
}

constexpr bool Shader::IsFailed() const
{
	return !IsCompiled();
}
