/*
 * MemberFunctionCallback.h
 *
 *  Created on: Feb 26, 2020
 *      Author: ycai
 */

#ifndef SRC_UTILITY_MEMBERFUNCTIONCALLBACK_H_
#define SRC_UTILITY_MEMBERFUNCTIONCALLBACK_H_

#include "Callback.h"

template <typename T, typename Method, typename Ret, typename ...Params>
class MemberFunctionCallback : Callback<Ret, Params>{
private:
  void *_object;
  Method _method;

public:
  MemberFunctionCallback(void *object, Method method)
    : _object(object), _method(method) {}

  virtual Ret invoke(Params... params)
  {
    T* obj = static_cast<T *>(_object);
    return (obj->*_method)(params);
  }
  virtual ~MemberFunctionCallback();
};

#endif /* SRC_UTILITY_MEMBERFUNCTIONCALLBACK_H_ */
