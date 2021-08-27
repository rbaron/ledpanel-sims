#include "ledpanel/simulations/life.h"

namespace ledpanel {
namespace simulations {
namespace {

inline bool GetCell(const std::bitset<D_SIZE> &b, pos_t y, pos_t x) {
  if (x < 0 || x > LP_DISPLAY_WIDTH - 1 || y < 0 || y > LP_DISPLAY_HEIGHT - 1) {
    return false;
  }
  return b[y * LP_DISPLAY_WIDTH + x];
}

inline int AliveNeighbors(const std::bitset<D_SIZE> &b, pos_t y, pos_t x) {
  int sum = 0;
  for (pos_t dx = -1; dx <= 1; dx++) {
    for (pos_t dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0) {
        continue;
      }
      sum += GetCell(b, y + dy, x + dx);
    }
  }
  return sum;
}
}  // namespace

GameOfLife::GameOfLife(const TimeProvider *time_provider,
                       RandomProvider *random_provider)
    : first_buffer_(true) {
  for (pos_t i = 0; i < D_SIZE; i++) {
    if (random_provider->RandInt(0, 2)) {
      buff1.set(i);
    }
  }
}

void GameOfLife::Update(time_t time_delta_ms) {
  auto &src = first_buffer_ ? buff1 : buff2;
  auto &dst = first_buffer_ ? buff2 : buff1;

  for (size_t i = 0; i < D_SIZE; i++) {
    pos_t y = i / LP_DISPLAY_WIDTH;
    pos_t x = i % LP_DISPLAY_WIDTH;
    int alive_neighbors = AliveNeighbors(src, y, x);
    if (src[i]) {
      dst.set(i, alive_neighbors == 2 || alive_neighbors == 3);
    } else {
      dst.set(i, alive_neighbors == 3);
    }
  }
  first_buffer_ = !first_buffer_;
}

void GameOfLife::Render(Display *display) const {
  static const Color c(127, 40, 50);
  auto &src = first_buffer_ ? buff1 : buff2;

  for (size_t i = 0; i < D_SIZE; i++) {
    if (!src[i]) {
      continue;
    }
    pos_t y = i / LP_DISPLAY_WIDTH;
    pos_t x = i % LP_DISPLAY_WIDTH;
    display->DrawPixel(y, x, c);
  }
}

}  // namespace simulations
}  // namespace ledpanel