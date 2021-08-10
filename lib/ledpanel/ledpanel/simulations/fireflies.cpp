
#include "ledpanel/simulations/fireflies.h"

#include <math.h>

#include "lp_config.h"

namespace ledpanel {
namespace simulations {

Firefly::Firefly(RandomProvider *rnd) : state_(State::IDLE), rnd_(rnd) {
  y_ = rnd->RandInt(0, LP_DISPLAY_HEIGHT);
  x_ = rnd->RandInt(0, LP_DISPLAY_WIDTH);
  speed_ = 2 * 1e-3;
  angle_ = 2 * 3.14 * rnd->Rand();
  clock_ = rnd->Rand();
}

void Firefly::Tick(time_t time_delta_ms, std::vector<Firefly *> &fireflies) {
  y_ += time_delta_ms * speed_ * sin(angle_);
  x_ += time_delta_ms * speed_ * cos(angle_);

  // Wrap around
  if (y_ < 0) {
    y_ += LP_DISPLAY_HEIGHT;
  } else if (y_ >= LP_DISPLAY_WIDTH) {
    y_ -= LP_DISPLAY_HEIGHT;
  }
  if (x_ < 0) {
    x_ += LP_DISPLAY_WIDTH;
  } else if (x_ >= LP_DISPLAY_WIDTH) {
    x_ -= LP_DISPLAY_WIDTH;
  }

  // Maybe change angle.
  if (rnd_->RandInt(0, 100) < 5) {
    angle_ = 2 * 3.14 * rnd_->Rand();
  }

  // Maybe start flashing.
  if (state_ == State::IDLE) {
    if ((clock_ += 0.005) > 1.0f) {
      state_ = State::FLASHING_UP;
      clock_ = 0.0f;
      for (auto &f : fireflies) {
        this->Interact(*f);
      }
    }
  } else if (state_ == State::FLASHING_UP) {
    if ((clock_ += 0.2f) > 1.0f) {
      state_ = State::FLASHING_DOWN;
      clock_ = 1.0f;
    }
  } else if (state_ == State::FLASHING_DOWN) {
    if ((clock_ -= 0.04f) < 0.0f) {
      state_ = State::IDLE;
      clock_ = 0.0f;
    }
  }
}

void Firefly::Render(Display *display) const {
  if (state_ == State::FLASHING_UP || state_ == State::FLASHING_DOWN) {
    Color c(0, 255 * clock_, 0);
    display->DrawPixel(y_, x_, c);
  } else if (state_ == State::IDLE) {
    static const Color c(0, 0, 0);
    display->DrawPixel(y_, x_, c);
  }
}

void Firefly::Interact(Firefly &other) const {
  if (&other == this) {
    return;
  }
  float distance = fabs(x_ - other.x_) + fabs(y_ - other.y_);
  if (distance > 4) {
    return;
  }

  other.clock_ = std::min(1.0, other.clock_ + 0.1);
}

Fireflies::Fireflies(RandomProvider *rnd) : rnd_(rnd) {
  fireflies_.reserve(LP_N_FIREFLIES);
  for (int i = 0; i < LP_N_FIREFLIES; i++) {
    fireflies_.push_back(new Firefly(rnd));
  }
}

void Fireflies::Update(time_t time_delta_ms) {
  for (auto &f : fireflies_) {
    f->Tick(time_delta_ms, fireflies_);
    // for (auto &other : fireflies_) {
    //   f->Interact(*other);
    // }
  }
}

void Fireflies::Render(Display *display) const {
  for (const auto f : fireflies_) {
    f->Render(display);
  }
}

}  // namespace simulations
}  // namespace ledpanel