#pragma once

class UniformView
{
public:
	UniformView(const GLint id);

	template<typename Ty>
	void Stream(const Ty arg0) const;

	template<typename Ty>
	void Stream(const Ty arg0, const Ty arg1) const;

	template<typename Ty>
	void Stream(const Ty arg0, const Ty arg1, const Ty arg2) const;

	template<typename Ty>
	void Stream(const Ty arg0, const Ty arg1, const Ty arg2, const Ty arg3) const;

	template<typename... Ty>
	void StreamArray(const Ty... args) const;

	template<>
	void Stream<bool>(const bool arg0) const;

private:
	GLint Self;
};

template<typename Ty>
inline void UniformView::Stream(const Ty arg0) const
{
	glUniform1f(Self, arg0);
}

template<typename Ty>
inline void UniformView::Stream(const Ty arg0, const Ty arg1) const
{
	glUniform2f(Self, arg0, arg1);
}

template<typename Ty>
inline void UniformView::Stream(const Ty arg0, const Ty arg1, const Ty arg2) const
{
	glUniform3f(Self, arg0, arg1, arg2);
}

template<typename Ty>
inline void UniformView::Stream(const Ty arg0, const Ty arg1, const Ty arg2, const Ty arg3) const
{
	glUniform4f(Self, arg0, arg1, arg2, arg3);
}

template<typename ...Ty>
inline void UniformView::StreamArray(const Ty ...args) const
{

}

template<>
inline void UniformView::Stream(const bool arg0) const
{
	glUniform1i(Self, arg0);
}
