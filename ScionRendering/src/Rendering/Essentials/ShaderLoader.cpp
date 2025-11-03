#include "ShaderLoader.h"
#include <Logger.h>
#include <fstream>
#include <string>

namespace SCION_RENDERING {

	GLuint ShaderLoader::CreateProgram(const std::string& vertexShader, const std::string& fragmentShader)
	{
		const GLuint program = glCreateProgram();

		const GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexShader);
		const GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		if (vertex == 0 || fragment == 0)
			return 0;

		if (!LinkShaders(program, vertex, fragment))
		{
			SCION_ERROR("Link failed.");
			return 0;
		}

		return program;
	}

	GLuint ShaderLoader::CompileShader(GLuint ShaderType, const std::string& filePath)
	{
		const GLuint shaderID = glCreateShader(ShaderType);

		std::ifstream ifs(filePath);

		if (ifs.fail())
		{
			SCION_ERROR("Shader failed to load");
			return 0;
		}

		std::string contents("");
		std::string line;

		while (std::getline(ifs, line))
		{
			contents += line + "\n";
		}

		ifs.close();

		const char* contentsPtr = contents.c_str();
		glShaderSource(shaderID, 1, &contentsPtr, nullptr);

		glCompileShader(shaderID);

		if (!CompileSuccess(shaderID))
		{
			SCION_ERROR("Shader Compile error");
		}

		return shaderID;
	}

	bool ShaderLoader::CompileSuccess(GLuint shader)
	{
		GLint status;

		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

		if (status != GL_TRUE)
		{
			GLint maxLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::string errorLog(maxLength, ' ');

			glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog.data());

			SCION_ERROR("Shader Compilation failed: {0}", errorLog);

			glDeleteShader(shader);
			return false;
		}

		return true;
	}

	bool ShaderLoader::IsProgramValid(GLuint program)
	{
		GLint status;

		glGetProgramiv(program, GL_LINK_STATUS, &status);

		if (status != GL_TRUE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::string errorLog(maxLength, ' ');

			glGetProgramInfoLog(program, maxLength, &maxLength, errorLog.data());

			SCION_ERROR("Shader's failed to link: {0}", errorLog);

			return false;
		}

		return true;
	}

	bool ShaderLoader::LinkShaders(GLuint program, GLuint vertexShader, GLuint fragmentShader)
	{
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		if (!IsProgramValid(program))
		{
			glDeleteProgram(program);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return false;
		}

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return true;
	}

	std::shared_ptr<Shader> ShaderLoader::Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		GLint program = CreateProgram(vertexShaderPath, fragmentShaderPath);

		if (program)
		{
			return std::make_shared<Shader>(program, vertexShaderPath, fragmentShaderPath);
		}

		return nullptr;
	}

}