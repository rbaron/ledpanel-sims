#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <math.h>

#include "ledpanel/display.h"
#include "ledpanel/scene.h"
#include "ledpanel/simulation.h"
#include "ledpanel/simulations/fireflies.h"
#include "ledpanel/simulations/galaxy.h"
#include "ledpanel/simulations/life.h"
#include "ledpanel/time.h"

#define PIN 23

using pos_t = ledpanel::pos_t;
using Color = ledpanel::Color;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    8, 8, 4, 4, PIN,
    NEO_TILE_TOP + NEO_TILE_RIGHT + NEO_TILE_PROGRESSIVE + NEO_TILE_ZIGZAG +
        NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ZIGZAG +
        NEO_MATRIX_COLUMNS,
    NEO_GRB + NEO_KHZ800);

const uint16_t colors[] = {matrix.Color(255, 0, 0), matrix.Color(0, 255, 0),
                           matrix.Color(0, 0, 255)};

class LedPanelDisplay : public ledpanel::Display {
 public:
  void DrawPixel(pos_t y, pos_t x, const Color &c) override {
    matrix.drawPixel(x, y, matrix.Color(c.r, c.g, c.b));
    // Serial.printf("Drew pixel %d %d %d\n", y, x, c.r);
  };

  pos_t GetHeight() const override {
    return matrix.height();
  }

  pos_t GetWidth() const override {
    return matrix.width();
  }
};

LedPanelDisplay led_panel_display;

class ArduinoTimeProvider : public ledpanel::TimeProvider {
 public:
  ledpanel::time_t GetMillis() const override {
    return millis();
  }
};

class ArduinoRandomProvider : public ledpanel::RandomProvider {
 public:
  int RandInt(int min, int max) override {
    return min + random(max - min);
  }
  float Rand() override {
    return static_cast<float>(random(1 << 15)) / (1 << 15);
  }
};

ArduinoTimeProvider time_provider;
ArduinoRandomProvider random_provider;

#define N_STARS 32

ledpanel::Scene *scene = NULL;

void setup() {
  Serial.begin(9600);

  scene = new ledpanel::simulations::Galaxy(&time_provider, &random_provider,
                                            &led_panel_display, N_STARS);
  // scene =
  //     new ledpanel::simulations::GameOfLife(&time_provider,
  //     &random_provider);
  // scene = new ledpanel::simulations::Fireflies(&random_provider);

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
  matrix.drawPixel(0, 0, colors[0]);
  matrix.drawRect(0, 0, 4, 4, colors[1]);
}

unsigned long last_millis = millis();

void loop() {
  unsigned long current_millis = millis();

  matrix.fillScreen(0);

  scene->Update(current_millis - last_millis);
  scene->Render(&led_panel_display);

  last_millis = current_millis;

  matrix.show();
  // delay(100);
}