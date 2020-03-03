#include "Uniform.h"

// write a float into the uniform location
void Uniform::setUniform(float f, unsigned int index)
{
  if (_mType != GL_FLOAT)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(f, index))
    glUniform1f(_mLocation + index, f);
}

// write a vec2 into the uniform location
void Uniform::setUniform(glm::vec2 vector, unsigned int index)
{
  if (_mType != GL_FLOAT_VEC2)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(vector, index))
    glUniform2f(_mLocation + index, vector.x, vector.y);
}

// write a vec3 into the uniform location
void Uniform::setUniform(glm::vec3 vector, unsigned int index)
{
  if (_mType != GL_FLOAT_VEC3)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(vector, index))
    glUniform3f(_mLocation + index, vector.x, vector.y, vector.z);
}

// write a vec3 into the uniform location
void Uniform::setUniform(glm::vec4 vector, unsigned int index)
{
  if (_mType != GL_FLOAT_VEC4)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(vector, index))
    glUniform4f(_mLocation + index, vector.x, vector.y, vector.z, vector.w);
}

// write a mat4 into the uniform location
void Uniform::setUniform(glm::mat4 mat, unsigned int index)
{
  if (_mType != GL_FLOAT_MAT4)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(mat, index))
    glUniformMatrix4fv(_mLocation + index, 1, GL_FALSE, glm::value_ptr(mat));
}

// write a mat3 into the uniform location
void Uniform::setUniform(glm::mat3 mat, unsigned int index)
{
  if (_mType != GL_FLOAT_MAT3)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(mat, index))
    glUniformMatrix3fv(_mLocation + index, 1, GL_FALSE, glm::value_ptr(mat));
}

// write a mat2 into the uniform location
void Uniform::setUniform(glm::mat2 mat, unsigned int index)
{
  if (_mType != GL_FLOAT_MAT2)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(mat, index))
    glUniformMatrix2fv(_mLocation + index, 1, GL_FALSE, glm::value_ptr(mat));
}

Uniform::Uniform(int programId, int index, GLenum type, GLenum size, const std::string& name)
: _mProgramId(programId), _mIndex(index), _mType(type), _mArraySize(size), _mName(name)
{
  _mLocation = glGetUniformLocation(programId, name.c_str());
  Log.print<Severity::debug>("Found uniform: name = ", name, ", size = ", size, ", location = ", _mLocation);

  if (_mLocation < 0)
  {
    Log.print<Severity::warning>("Uniform ", _mName, " is not valid!");
  }
}