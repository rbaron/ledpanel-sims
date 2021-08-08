#include "ledpanel/simulations/galaxy.h"

#include <algorithm>
#include <memory>

namespace ledpanel {
namespace simulations {
namespace {

std::unique_ptr<Simulation> MakeStar(const Display *display,
                                     RandomProvider *rnd) {
  return std::make_unique<Star>(
      static_cast<pos_t>(rnd->RandInt(0, display->GetHeight())),
      static_cast<pos_t>(rnd->RandInt(0, display->GetWidth())),
      static_cast<float>((rnd->RandInt(0, 100) >= 50 ? 1 : -1) * 1 / 1e4f),
      static_cast<float>(rnd->RandInt(0, 1000)) / 1000);
}

std::unique_ptr<Simulation> MakeShootingStar(const Display *display,
                                             RandomProvider *rnd) {
  pos_t y = rnd->RandInt(0, 2) != 1 ? -1 : display->GetHeight();
  pos_t x = rnd->RandInt(0, 2) != 1 ? -1 : display->GetWidth();
  float vy = (y < 0 ? 1 : -1) * (1.0f / rnd->RandInt(10, 100));
  float vx = (x < 0 ? 1 : -1) * (1.0f / rnd->RandInt(10, 100));

  return std::make_unique<ShootingStar>(display, y, x, vy, vx);
}

std::unique_ptr<Simulation> MakeSimulation(const Display *display,
                                           RandomProvider *rnd) {
  int rand_val = rnd->RandInt(0, 100);
  if (rand_val < 10) {
    return MakeShootingStar(display, rnd);
  } else {
    return MakeStar(display, rnd);
  }
}

}  // namespace

Galaxy::Galaxy(const TimeProvider *time_provider,
               RandomProvider *random_provider, Display *display, int n_stars)
    : time_provider_(time_provider),
      random_provider_(random_provider),
      n_stars_(n_stars),
      display_(display) {
  last_update_ = time_provider_->GetMillis();
  for (int i = 0; i < n_stars; i++) {
    stars_.push_back(MakeSimulation(display, random_provider_));
  }
}

void Galaxy::Update(unsigned long time_delta_ms) {
  for (auto &star : stars_) {
    star->Tick(time_delta_ms);
  }
  stars_.erase(std::remove_if(stars_.begin(), stars_.end(),
                              [](const std::unique_ptr<Simulation> &s) {
                                return s->IsFinished();
                              }),
               stars_.end());
  // Populate new stars.
  for (int i = 0; i < n_stars_ - stars_.size(); i++) {
    stars_.push_back(MakeSimulation(display_, random_provider_));
  }
}

void Galaxy::Render(Display *display) const {
  for (const auto &star : stars_) {
    star->Render(display);
  }
}

void Star::Tick(unsigned long time_delta_ms) {
  if (brightness_ > 1.0f) {
    speed_ *= -1;
  } else if (brightness_ < 0.0f) {
    dead_ = true;
  }
  brightness_ += speed_ * time_delta_ms;
}

bool Star::IsFinished() const {
  return dead_;
};

void Star::Render(Display *display) const {
  uint8_t color = brightness_ * 255;
  Color c(color, color, color);
  display->DrawPixel(y_, x_, c);
}

void ShootingStar::Tick(time_t time_delta_ms) {
  y_ += vy_ * time_delta_ms;
  x_ += vx_ * time_delta_ms;
}

void ShootingStar::Render(Display *display) const {
  static const Color c(255, 204, 0);
  display->DrawPixel(y_, x_, c);
}

bool ShootingStar::IsFinished() const {
  return ((vx_ > 0 && x_ > display_->GetWidth()) || (vx_ < 0 && x_ < 0) ||
          (vy_ > 0 && y_ > display_->GetHeight()) || (vy_ < 0 && y_ < 0));
}

}  // namespace simulations
}  // namespace ledpanel