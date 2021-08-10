#ifndef _LEDPANEL_RANDOM_H_
#define _LEDPANEL_RANDOM_H_

namespace ledpanel {

class RandomProvider {
 public:
  virtual int RandInt(int min, int max) = 0;
  // Returns a floating point random between 0 and 1.
  virtual float Rand() = 0;
};

}  // namespace ledpanel
#endif