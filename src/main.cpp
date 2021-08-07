#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define PIN 23

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    8, 8, 4, 3, PIN,
    NEO_TILE_TOP + NEO_TILE_RIGHT + NEO_TILE_PROGRESSIVE + NEO_TILE_ZIGZAG +
        NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ZIGZAG +
        NEO_MATRIX_COLUMNS,
    NEO_GRB + NEO_KHZ800);

const uint16_t colors[] = {matrix.Color(255, 0, 0), matrix.Color(0, 255, 0),
                           matrix.Color(0, 0, 255)};

#define N_POINTS 48
std::vector<std::pair<int, int>> points;

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
  matrix.drawPixel(0, 0, colors[0]);
  matrix.drawRect(0, 0, 4, 4, colors[1]);

  randomSeed(analogRead(0));
  for (int i = 0; i < N_POINTS; i++) {
    points.emplace_back(static_cast<int>(random(matrix.width())),
                        static_cast<int>(random(matrix.height())));
  }
}

int x = matrix.width();
int pass = 0;

uint8_t col = 50;
int dir = 1;

void loop() {
  matrix.fillScreen(0);
  for (const auto &pair : points) {
    matrix.drawPixel(pair.first, pair.second, matrix.Color(col, col, col));
  }
  if (col == 255 && dir == 1) {
    dir = -1;
  } else if (col == 50 && dir == -1) {
    dir = 1;
  }
  col += dir;
  matrix.show();
  delay(10);
}