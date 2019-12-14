#include "pch.h"
#include "ShaderProgram.h"
#include "Log.h"

ShaderProgram::ShaderProgram(const std::string& vShaderFile, const std::string& fShaderFile)
{
	m_vShader = new Shader(vShaderFile, VERTEX_SHADER);
	m_fShader = new Shader(fShaderFile, FRAGMENT_SHADER);
	CHECK_GL_ERROR();

	m_program = glCreateProgram();
	glAttachShader(m_program, m_vShader->Get());
	glAttachShader(m_program, m_fShader->Get());

	Link();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_program);
	delete m_vShader;
	delete m_fShader;
}

void ShaderProgram::BindAttribLoc(GLuint loc, const char* name)
{
	glBindAttribLocation(m_program, loc, name);
	CHECK_GL_ERROR();
}

void ShaderProgram::Link()
{
	//link shaders and attributes
	glLinkProgram(m_program);
	CheckForLinkErrors();
	CHECK_GL_ERROR();

	//if they were linked we no longer need them
	delete m_vShader;
	delete m_fShader;
}

void ShaderProgram::Use()
{
	
	static GLuint* pointerSP;
	if (pointerSP != &m_program) {
		pointerSP = &m_program;
		glUseProgram(*pointerSP);
	}

}

bool ShaderProgram::CheckForLinkErrors()
{
	GLint isLinked = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &isLinked);

	if (isLinked == GL_FALSE)
	{
		GLint length = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);

		char* msg = (char*)malloc(length);
		glGetProgramInfoLog(m_program, length, &length, msg);
		free(msg);

		glDeleteProgram(m_program);
		return true;
	}
	return false;
}