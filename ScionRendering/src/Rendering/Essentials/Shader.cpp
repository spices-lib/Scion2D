#include "Shader.h"
#include "Logger.h"

namespace SCION_RENDERING {

	GLuint Shader::GetUniformLocation(const std::string& uniformName)
	{
		if (m_UniformLocationMap.contains(uniformName))
		{
			return m_UniformLocationMap.at(uniformName);
		}

		GLuint location = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
		
		if (location == GL_INVALID_INDEX)
		{
			SCION_ERROR("Uniform: {0} not found in shader", uniformName);
			return -1;
		}

		m_UniformLocationMap[uniformName] = location;

		return location;
	}

	Shader::Shader()
		: Shader(0, "", "")
	{}

	Shader::Shader(GLuint program, const std::string vertexPath, const std::string& fragmentPath)
		: m_ShaderProgramID(program)
		, m_sVertexPath(vertexPath)
		, m_sFragmentPath(fragmentPath)
	{}

	Shader::~Shader()
	{
		if (m_ShaderProgramID > 0)
		{
			glDeleteProgram(m_ShaderProgramID);
		}
	}

	void Shader::SetUniformInt(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetUniformMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::Enable()
	{
		glUseProgram(m_ShaderProgramID);
	}

	void Shader::Disable()
	{
		glUseProgram(0);
	}

}