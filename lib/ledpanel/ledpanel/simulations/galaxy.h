#ifndef _LEDPANEL_SIMULATIONS_GALAXY_H_
#define _LEDPANEL_SIMULATIONS_GALAXY_H_

#include <vector>

#include "ledpanel/display.h"
#include "ledpanel/random.h"
#include "ledpanel/scene.h"
#include "ledpanel/simulation.h"
#include "ledpanel/time.h"

namespace ledpanel {
namespace simulations {

enum class StarType {
  CYCLIC,
  EXPLODING,
  SHOOTING,
};

class Star : public Simulation {
 public:
  Star(pos_t y, pos_t x, float speed, float brightness)
      : y_(y), x_(x), speed_(speed), brightness_(brightness) {}

  void Tick(time_t time_delta_ms) override;
  void Render(Display *display) const override;
  bool IsFinished() const override {
    return false;
  };

 private:
  pos_t y_, x_;
  float speed_;
  float brightness_;
};

class Galaxy : public Scene {
 public:
  Galaxy(const TimeProvider *time_provider, RandomProvider *random_provider,
         Display *display, int n_stars);

  void Update(time_t time_delta_ms) override;
  void Render(Display *display) const override;

 private:
  time_t last_update_;
  const TimeProvider *time_provider_;
  RandomProvider *random_provider_;
  int n_stars_;
  std::vector<Star> stars_;
};

}  // namespace simulations
}  // namespace ledpanel
#endif