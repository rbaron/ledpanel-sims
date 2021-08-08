#ifndef _LEDPANEL_SIMULATION_H_
#define _LEDPANEL_SIMULATION_H_

#include "ledpanel/display.h"
#include "ledpanel/scene.h"
#include "ledpanel/time.h"

namespace ledpanel {

class Simulation {
 public:
  virtual void Tick(time_t time_delta_ms) = 0;
  virtual void Render(Display *display) const = 0;
  virtual bool IsFinished() const = 0;
};

}  // namespace ledpanel
#endif