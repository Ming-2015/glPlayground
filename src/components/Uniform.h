#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <string>
#include "../utils/Logger.h"

class Uniform
{
protected:
  int _mProgramId;
  int _mIndex;
  int _mLocation;
  GLenum _mType;
  GLsizei _mArraySize;
  // GLsizei _mElemSize; // size of each component, total size would be this * _mArraySize
  std::string _mName;

public:
  Uniform(int programId, int index, GLenum type, GLenum size, const std::string& name)
    : _mProgramId(programId), _mIndex(index), _mType(type), _mArraySize(size), _mName(name)
  {
    _mLocation = glGetUniformLocation(programId, name.c_str());
    Log.print<SeverityType::debug>("Found uniform: name = ", name, ", size = ", size, ", location = ", _mLocation);

    if (_mLocation < 0)
    {
      Log.print<SeverityType::warning>("Uniform ", _mName, " is not valid!");
    }
  }

  int getLocation() const
  {
    return _mLocation;
  }

  int getProgramId() const
  {
    return _mProgramId;
  }

  GLenum getType() const
  {
    return _mType;
  }

  GLsizei getSize() const
  {
    return _mArraySize;
  }

  const std::string& getName() const
  {
    return _mName;
  }

  void setUniform(float f)
  {
    if (_mType != GL_FLOAT)
    {
      return;
    }
  }
};