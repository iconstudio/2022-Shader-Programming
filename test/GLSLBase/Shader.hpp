#pragma once
#include "stdafx.h"

class Shader
{
private:
	enum class BLOB_STATES { UNLOADED, LOADED };
	enum class CODE_STATES { NONE, SUCCEED, FAILED };

	GLuint Self;
	GLenum Type;
	BLOB_STATES blobStatus;
	CODE_STATES codeStatus;

	std::string Blob;
	GLuint ProgramID;

public:
	Shader();
	Shader(const GLenum type);
	Shader(const GLenum type, const Path& filename);
	~Shader();

	void SetType(const GLenum type);
	constexpr GLenum GetType() const;

	bool Load();
	bool Load(const Path& filename);
	bool Compile();

	void Attach(const GLuint program);

	constexpr bool IsUnloaded() const;
	constexpr bool IsLoaded() const;
	constexpr bool IsCompiled() const;
	constexpr bool IsSucceed() const;
	constexpr bool IsFailed() const;

	Path filepath;
};

