#include "ShaderLoader.h"
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
			std::cout << "Link failed." << std::endl;
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
			std::cout << "Shader failed to load" << std::endl;
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
			std::cout << "Shader Compile error" << std::endl;
		}

		return shaderID;
	}

	bool ShaderLoader::CompileSuccess(GLuint shader)
	{
		return false;
	}

	bool ShaderLoader::IsProgramValid(GLuint program)
	{
		return false;
	}

	bool ShaderLoader::LinkShaders(GLuint program, GLuint vertexShader, GLuint fragmentShader)
	{
		return false;
	}

	std::shared_ptr<Shader> ShaderLoader::Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		return std::shared_ptr<Shader>();
	}

}