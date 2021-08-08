#ifndef _LEDPANEL_SCENE_H_
#define _LEDPANEL_SCENE_H_

#include "ledpanel/display.h"
#include "ledpanel/time.h"

namespace ledpanel {

class Scene {
 public:
  virtual void Update(time_t time_delta_ms) = 0;
  virtual void Render(Display *display) const = 0;
};

}  // namespace ledpanel
#endif