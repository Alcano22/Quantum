#include "qtmpch.h"
#include "OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>

namespace Quantum
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		QTM_ERROR("Unknown shader type '{}'", type);
		return GL_NONE;
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		m_Name = name;
		
		m_Sources[GL_VERTEX_SHADER] = vertexSrc;
		m_Sources[GL_FRAGMENT_SHADER] = fragmentSrc;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		const std::string src = ReadFile(filepath);
		PreProcess(src);

		size_t lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		const size_t lastDot = filepath.rfind('.');
		const size_t count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::PreProcess(const std::string& src)
	{
		const char* typeToken = "#type";
		const size_t typeTokenLength = strlen(typeToken);
		size_t pos = src.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			const size_t eol = src.find_first_of("\r\n", pos);
			QTM_ASSERT(eol != std::string::npos, "Syntax error")
			const size_t begin = pos + typeTokenLength + 1;
			std::string type = src.substr(begin, eol - begin);
			QTM_ASSERT(ShaderTypeFromString(type), "Invalid shader type specifier")

			const size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			pos = src.find(typeToken, nextLinePos);
			m_Sources[ShaderTypeFromString(type)] = src.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? src.size() - 1 : nextLinePos));
		}
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath) const
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (!in)
		{
			QTM_ERROR("Could not open file '{}'", filepath);
			return "";
		}

		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(result.data(), result.size());
		in.close();

		return result;
	}

	std::string GetConstants()
	{
		std::string constants;
		constants += "#define PI 3.14159265359\n";
		constants += "#define E 2.71828182846\n";
		return constants;
	} 

	void OpenGLShader::Compile()
	{
		QTM_ASSERT(m_Sources.size() <= 2, "Quantum only supports 2 shaders!")

		const GLuint program = glCreateProgram();

		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;

		const std::string constants = GetConstants();

		for (auto[type, src] : m_Sources)
		{
			std::string srcWithConstants;
			
			// Extract the #version directive if it exists
			const size_t versionPos = src.find("#version");
			if (versionPos != std::string::npos)
			{
				const size_t versionEnd = src.find_first_of("\r\n", versionPos);
				std::string versionLine = src.substr(versionPos, versionEnd - versionPos + 1);

				// Prepend the version line to the beginning, followed by the constants
				srcWithConstants = versionLine + '\n' + constants + '\n' + src.substr(versionEnd + 1);
			}
			else
			{
				// No #version found, just prepend the constants
				srcWithConstants = constants + '\n' + src;
			}
			
			const GLuint shader = glCreateShader(type);
			const GLchar* sourceCStr = srcWithConstants.c_str();
			glShaderSource(shader, 1, &sourceCStr, nullptr);
			glCompileShader(shader);

			GLint isCompiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint infoLogLen;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

				std::vector<GLchar> infoLog(infoLogLen);
				glGetShaderInfoLog(shader, infoLogLen, &infoLogLen, infoLog.data());

				glDeleteShader(shader);
				
				QTM_ERROR("ERROR::{}::COMPILATION_FAILED\n{}", type, infoLog.data());
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		m_RendererID = program;
		glLinkProgram(program);
		
		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint infoLogLen;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);

			std::vector<GLchar> infoLog(infoLogLen);
			glGetProgramInfoLog(program, infoLogLen, &infoLogLen, infoLog.data());

			glDeleteProgram(program);

			for (const GLenum shaderID : glShaderIDs)
				glDeleteShader(shaderID);

			QTM_ERROR("ERROR::PROGRAM::LINKING_FAILED\n{}", infoLog.data());
			return;
		}

		for (const GLenum shaderID : glShaderIDs)
			glDetachShader(program, shaderID);
	}

	void OpenGLShader::Bind() const
	{
		if (IsBound()) return;

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		if (!IsBound()) return;

		glUseProgram(0);
	}

	bool OpenGLShader::IsBound() const
	{
		int currentProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
		return currentProgram == m_RendererID;
	}

	int OpenGLShader::GetUniformLocation(const char* name) const
	{
		QTM_ASSERT(IsBound(), "Bind shader before setting uniforms")
		const int loc = glGetUniformLocation(m_RendererID, name);
		QTM_ASSERT(loc != -1, "Uniform '{}' not found in shader '{}'", name, m_Name)
		return loc;
	}

	void OpenGLShader::SetUniformInt(const char* name, const int value)
	{
		const int loc = GetUniformLocation(name);
		glUniform1i(loc, value);
	}

	void OpenGLShader::SetUniformInt2(const char* name, const glm::ivec2& value) 
	{
		const int loc = GetUniformLocation(name);
		glUniform2i(loc, value.x, value.y);
	}
	
	void OpenGLShader::SetUniformInt3(const char* name, const glm::ivec3& value)
	{
		const int loc = GetUniformLocation(name);
		glUniform3i(loc, value.x, value.y, value.z);
	}
	
	void OpenGLShader::SetUniformInt4(const char* name, const glm::ivec4& value)
	{
		const int loc = GetUniformLocation(name);
		glUniform4i(loc, value.x, value.y, value.z, value.w);
	}
	
	void OpenGLShader::SetUniformFloat(const char* name, const float value)
	{
		const int loc = GetUniformLocation(name);
		glUniform1f(loc, value);
	}
	
	void OpenGLShader::SetUniformFloat2(const char* name, const glm::vec2& value)
	{
		const int loc = GetUniformLocation(name);
		glUniform2f(loc, value.x, value.y);
	}
	
	void OpenGLShader::SetUniformFloat3(const char* name, const glm::vec3& value)
	{
		const int loc = GetUniformLocation(name);
		glUniform3f(loc, value.x, value.y, value.z);
	}
	
	void OpenGLShader::SetUniformFloat4(const char* name, const glm::vec4& value)
	{
		const int loc = GetUniformLocation(name);
		glUniform4f(loc, value.x, value.y, value.z, value.w);
	}
	
	void OpenGLShader::SetUniformMat2(const char* name, const glm::mat2& value)
	{
		const int loc = GetUniformLocation(name);
		glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}
	
	void OpenGLShader::SetUniformMat3(const char* name, const glm::mat3& value)
	{
		const int loc = GetUniformLocation(name);
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}
	
	void OpenGLShader::SetUniformMat4(const char* name, const glm::mat4& value)
	{
		const int loc = GetUniformLocation(name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}
}
