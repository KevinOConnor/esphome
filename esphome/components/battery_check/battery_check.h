#pragma once

#include "esphome/core/component.h"
#include "esphome/core/esphal.h"
#include "esphome/core/defines.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace battery_check {

class BatteryCheck : public sensor::Sensor, public Component {
 public:
  /// Set the attenuation for this pin.
  void set_attenuation(adc_attenuation_t attenuation);
  /// Setup ADC
  void setup() override;
  void dump_config() override;
  /// `HARDWARE_LATE` setup priority.
  float get_setup_priority() const override;
  void set_pin(uint8_t pin) { this->pin_ = pin; }
  float sample();

 protected:
  uint8_t pin_;
  adc_attenuation_t attenuation_{ADC_0db};
};

}  // namespace battery_check
}  // namespace esphome
