#include "stdafx.h"

GLchar ErrorLog[1024] = { 0 };

template<typename... Ty>
void ErrorDisplay(GLint program, Ty... args)
{
	std::cout << args << " ";
	ErrorDisplay(program, args...);
}

template<>
void ErrorDisplay(GLint program)
{
	ZeroMemory(ErrorLog, sizeof(ErrorLog));
	glGetProgramInfoLog(program, sizeof(ErrorLog), NULL, ErrorLog);

	std::cout << "\n" << ErrorLog << std::endl;
}
