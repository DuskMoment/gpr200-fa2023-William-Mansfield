#pragma once
#include <sstream>
#include <fstream>

namespace wm
{
	std::string loadShaderSourceFile(const std::string& filePath);

	class Shader
	{
	public:
		Shader(const std::string& vertexShader, const std::string& fragmentShader);
		void use();
		void setInt(const std::string& name, int v) const;
		void setFloat(const std::string& name, float v) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std:: string& name, float x, float y, float z) const;
		void setVec$(const std::string& name, float x, float y, float z, float w) const;
	private:
		unsigned int m_id;// shader program handle
	};
}