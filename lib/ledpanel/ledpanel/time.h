#ifndef _LEDPANEL_TIME_H_
#define _LEDPANEL_TIME_H_

namespace ledpanel {

using time_t = unsigned long;

class TimeProvider {
 public:
  virtual time_t GetMillis() const = 0;
};

}  // namespace ledpanel

#endif