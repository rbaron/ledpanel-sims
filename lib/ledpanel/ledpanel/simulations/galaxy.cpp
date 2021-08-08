#include "ledpanel/simulations/galaxy.h"

namespace ledpanel {
namespace simulations {

Galaxy::Galaxy(const TimeProvider *time_provider,
               RandomProvider *random_provider, Display *display, int n_stars)
    : time_provider_(time_provider),
      random_provider_(random_provider),
      n_stars_(n_stars) {
  last_update_ = time_provider_->GetMillis();
  for (int i = 0; i < n_stars; i++) {
    stars_.emplace_back(
        static_cast<pos_t>(random_provider_->RandInt(0, display->GetHeight())),
        static_cast<pos_t>(random_provider_->RandInt(0, display->GetWidth())),
        static_cast<float>((random_provider_->RandInt(0, 100) >= 50 ? 1 : -1) *
                           1 / 1e5f),
        static_cast<float>(random_provider_->RandInt(0, 1000)) / 1000

    );
  }
}

void Galaxy::Update(unsigned long time_delta_ms) {
  for (auto &star : stars_) {
    star.Tick(time_delta_ms);
  }
}

void Galaxy::Render(Display *display) const {
  for (auto &star : stars_) {
    star.Render(display);
  }
}

void Star::Tick(unsigned long time_delta_ms) {
  if (brightness_ > 1.0f || brightness_ < 0.0f) {
    speed_ *= -1;
  }
  brightness_ += speed_ * time_delta_ms;
}

void Star::Render(Display *display) const {
  uint8_t color = brightness_ * 255;
  Color c(color, color, color);
  display->DrawPixel(y_, x_, c);
}

}  // namespace simulations
}  // namespace ledpanel