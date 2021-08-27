#ifndef _LEDPANEL_SIMULATIONS_LIFE_H_
#define _LEDPANEL_SIMULATIONS_LIFE_H_

#include <bitset>
#include <memory>

#include "ledpanel/display.h"
#include "ledpanel/random.h"
#include "ledpanel/scene.h"
#include "ledpanel/simulation.h"
#include "ledpanel/time.h"
#include "lp_config.h"

namespace ledpanel {
namespace simulations {

#define D_SIZE (LP_DISPLAY_HEIGHT * LP_DISPLAY_WIDTH)

class GameOfLife : public Scene {
 public:
  GameOfLife(const TimeProvider *time_provider,
             RandomProvider *random_provider);

  void Update(time_t time_delta_ms) override;
  void Render(Display *display) const override;

 private:
  std::bitset<D_SIZE> buff1, buff2;
  bool first_buffer_;
};

}  // namespace simulations
}  // namespace ledpanel
#endif