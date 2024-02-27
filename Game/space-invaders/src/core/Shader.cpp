#include "Shader.h"

#include "Core.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


Shader::Shader(const std::string& filePath)
    : m_RendererID(0), m_FilePath(filePath) 
{
    ShaderProgramSource source = ParseShader(filePath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1iv(const std::string& name, const int count, const int* value)
{
    GLCall(glUniform1iv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2& value)
{
    GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
    SetUniform3f(name, value.x, value.y, value.z);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& value)
{
    GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) const
{
    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }
    
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

    if(location == -1)
    {
        std::cout << "Warning: Uniform " << name << " doesn't exist!";
    }

    m_UniformLocationCache[name] = location;

    return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream Stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string Line;
    std::stringstream Ss[2];
    ShaderType Type = ShaderType::NONE;

    while(getline(Stream, Line))
    {
        if(Line.find("#shader") != std::string::npos)
        {
            if(Line.find("vertex") != std::string::npos)
            {
                Type = ShaderType::VERTEX;
            }
            else if(Line.find("fragment") != std::string::npos)
            {
                Type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            Ss[(int)Type] << Line << "\n";
        }
    }

    return {Ss[0].str(), Ss[1].str()};
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int Vs = CompileShader(GL_VERTEX_SHADER, vertexShader); 
    unsigned int Fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); 

    GLCall(glAttachShader(program, Vs));
    GLCall(glAttachShader(program, Fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(Vs));
    GLCall(glDeleteShader(Fs));

    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* Src = source.c_str();

    GLCall(glShaderSource(id, 1, &Src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if(result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << message << "\n";

        GLCall(glDeleteShader(id));

        return 0;
    }
    
    return id;
}
