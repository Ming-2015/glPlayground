#pragma once
class Cloneable {
protected:
  virtual void copyTo(Cloneable* cloned) const = 0;

public:
  virtual Cloneable* clone() const = 0;
};