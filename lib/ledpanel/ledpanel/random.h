#ifndef _LEDPANEL_RANDOM_H_
#define _LEDPANEL_RANDOM_H_

namespace ledpanel {

class RandomProvider {
 public:
  virtual int RandInt(int min, int max) = 0;
};

}  // namespace ledpanel
#endif