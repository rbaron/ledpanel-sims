#ifndef _LEDPANEL_SIMULATIONS_GALAXY_H_
#define _LEDPANEL_SIMULATIONS_GALAXY_H_

#include <memory>
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
      : y_(y), x_(x), speed_(speed), brightness_(brightness), dead_(false) {}

  void Tick(time_t time_delta_ms) override;
  void Render(Display *display) const override;
  bool IsFinished() const override;

 private:
  pos_t y_, x_;
  float speed_;
  float brightness_;
  bool dead_;
};

class ShootingStar : public Simulation {
 public:
  ShootingStar(const Display *display, float y, float x, float vy, float vx)
      : display_(display), y_(y), x_(x), vy_(vy), vx_(vx) {}

  void Tick(time_t time_delta_ms) override;
  void Render(Display *display) const override;
  bool IsFinished() const override;

 private:
  const Display *display_;
  float y_, x_, vy_, vx_;
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
  const Display *display_;
  std::vector<std::unique_ptr<Simulation>> stars_;
};

}  // namespace simulations
}  // namespace ledpanel
#endif