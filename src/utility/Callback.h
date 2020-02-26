/*
 * Callback.h
 *
 *  Created on: Feb 26, 2020
 *      Author: ycai
 */

#ifndef SRC_UTILITY_CALLBACK_H_
#define SRC_UTILITY_CALLBACK_H_

template <typename Ret, typename ...Params>
class Callback {
public:
  virtual Ret invoke(Params...) = 0;
  virtual ~Callback() = 0;
};

#endif /* SRC_UTILITY_CALLBACK_H_ */
