#pragma once
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <filesystem>
#include <random>

#include "./Dependencies/glew.h"
#include "./Dependencies/wglew.h"
#include "./Dependencies/freeglut.h"

class Renderer;
class Pipeline;
class Shader;
class VertexObjet;
class VertexBuffer;
class UniformView;
class AttributeView;

using Path = std::filesystem::path;

template<typename... Ty>
void ErrorDisplay(GLint program, Ty... args);

template<>
void ErrorDisplay(GLint program);
