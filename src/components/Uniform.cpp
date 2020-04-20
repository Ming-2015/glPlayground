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
    glProgramUniform1f(_mProgramId, _mLocation + index, f);
}

void Uniform::setUniform(int i, unsigned int index)
{
  if (
    _mType != GL_INT
    && _mType != GL_SAMPLER_2D
  )
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(i, index))
    glProgramUniform1i(_mProgramId, _mLocation + index, i);
}

// write a vec2 into the uniform location
void Uniform::setUniform(const glm::vec2& vector, unsigned int index)
{
  if (_mType != GL_FLOAT_VEC2)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(vector, index))
    glProgramUniform2f(_mProgramId, _mLocation + index, vector.x, vector.y);
}

// write a vec3 into the uniform location
void Uniform::setUniform(const glm::vec3& vector, unsigned int index)
{
  if (_mType != GL_FLOAT_VEC3)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(vector, index))
    glProgramUniform3f(_mProgramId, _mLocation + index, vector.x, vector.y, vector.z);
}

// write a vec3 into the uniform location
void Uniform::setUniform(const glm::vec4& vector, unsigned int index)
{
  if (_mType != GL_FLOAT_VEC4)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(vector, index))
    glProgramUniform4f(_mProgramId, _mLocation + index, vector.x, vector.y, vector.z, vector.w);
}

// write a mat4 into the uniform location
void Uniform::setUniform(const glm::mat4& mat, unsigned int index)
{
  if (_mType != GL_FLOAT_MAT4)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(mat, index))
    glProgramUniformMatrix4fv(_mProgramId, _mLocation + index, 1, GL_FALSE, glm::value_ptr(mat));
}

void Uniform::setUniform(const std::vector<glm::mat4>& mats, unsigned int index)
{
  if (_mType != GL_FLOAT_MAT4)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  bool dataChanged = false;
  for (int i = 0; i < mats.size(); i++)
  {
    if (_registerData(mats[i], index + i))
    {
      dataChanged = true;
    }
  }

  if (dataChanged)
    glProgramUniformMatrix4fv(_mProgramId, _mLocation + index, mats.size(), GL_FALSE, glm::value_ptr(mats.at(0)));
}

// write a mat3 into the uniform location
void Uniform::setUniform(const glm::mat3& mat, unsigned int index)
{
  if (_mType != GL_FLOAT_MAT3)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(mat, index))
    glProgramUniformMatrix3fv(_mProgramId, _mLocation + index, 1, GL_FALSE, glm::value_ptr(mat));
}

// write a mat2 into the uniform location
void Uniform::setUniform(const glm::mat2& mat, unsigned int index)
{
  if (_mType != GL_FLOAT_MAT2)
  {
    Log.print<Severity::warning>("Trying to write Uniform \"", _mName, "\" with an invalid type!");
    return;
  }

  if (_registerData(mat, index))
    glProgramUniformMatrix2fv(_mProgramId, _mLocation + index, 1, GL_FALSE, glm::value_ptr(mat));
}

Uniform::Uniform(int programId, int index, GLenum type, GLenum size, const std::string& name)
: _mProgramId(programId), _mIndex(index), _mType(type), _mArraySize(size), _mName(name)
{
  _mLocation = glGetUniformLocation(programId, name.c_str());

  if (_mLocation < 0)
  {
    Log.print<Severity::warning>("Uniform ", _mName, " is not valid!");
  }
}

Uniform::~Uniform()
{
  Log.print<Severity::info>("Removing uniform ", _mName, ": ", _dataToHex());
}