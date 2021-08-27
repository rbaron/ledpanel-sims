#ifndef _LEDPANEL_SIMULATIONS_FIREFLIES_H_
#define _LEDPANEL_SIMULATIONS_FIREFLIES_H_

#include <vector>

#include "ledpanel/display.h"
#include "ledpanel/random.h"
#include "ledpanel/scene.h"
#include "ledpanel/simulation.h"
#include "ledpanel/time.h"

#define LP_N_FIREFLIES 100

namespace ledpanel {
namespace simulations {

class Firefly {
 public:
  enum class State {
    IDLE,
    FLASHING_UP,
    FLASHING_DOWN,
  };

  explicit Firefly(RandomProvider *rnd);

  void Tick(time_t time_delta_ms, std::vector<Firefly> &fireflies);

  void Render(Display *display) const;

  void Interact(Firefly &other) const;

 private:
  float y_, x_;
  // Speed in pixels / millisecond.
  float speed_;
  float angle_;
  float clock_;
  State state_;

  RandomProvider *rnd_;
};

class Fireflies : public Scene {
 public:
  Fireflies(RandomProvider *rnd);
  void Update(time_t time_delta_ms) override;
  void Render(Display *display) const override;

 private:
  std::vector<Firefly> fireflies_;
  RandomProvider *rnd_;
};

}  // namespace simulations
}  // namespace ledpanel

#endif