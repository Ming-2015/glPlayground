#include <glm/glm.hpp>
#include <string>
#include <sstream>

class glmPrint {
public:

  static std::string printMat4(const glm::mat4& mat) 
  {
    std::stringstream str;
    for (int i = 0; i < 4; i++)
    {
      str << "(";
      for (int j = 0; j < 3; j++)
      {
        str << mat[i][j];
        str << ", ";
      }
      str << mat[i][3];
      str << ")\n";
    }
    std::string copy = str.str();
    return copy;
  }

  static std::string printVec(const glm::vec3& vec)
  {
    std::stringstream str;
    str << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    std::string copy = str.str();
    return copy;
  }

  static std::string printVec(const glm::vec4& vec)
  {
    std::stringstream str;
    str << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    std::string copy = str.str();
    return copy;
  }

  static std::string printVec(const glm::uvec4& vec)
  {
    std::stringstream str;
    str << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    std::string copy = str.str();
    return copy;
  }
};