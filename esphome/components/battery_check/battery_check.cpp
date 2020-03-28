#include "battery_check.h"
#include "esphome/core/log.h"
#include "driver/adc.h"
#include "driver/gpio.h"

namespace esphome {
namespace battery_check {

static const char *TAG = "battery_check";

void BatteryCheck::set_attenuation(adc_attenuation_t attenuation) { this->attenuation_ = attenuation; }

void BatteryCheck::setup() {
  ESP_LOGCONFIG(TAG, "Setting up BatteryCheck '%s'...", this->get_name().c_str());

  // Setup pin
  analogSetPinAttenuation(this->pin_, this->attenuation_);
  analogRead(this->pin_);

  // Take measurement
  float value = this->sample();
  this->publish_state(value);
}
void BatteryCheck::dump_config() {
  LOG_SENSOR("", "BatteryCheck", this);
  ESP_LOGCONFIG(TAG, "  Pin: %u", this->pin_);
  switch (this->attenuation_) {
    case ADC_0db:
      ESP_LOGCONFIG(TAG, " Attenuation: 0db (max 1.1V)");
      break;
    case ADC_2_5db:
      ESP_LOGCONFIG(TAG, " Attenuation: 2.5db (max 1.5V)");
      break;
    case ADC_6db:
      ESP_LOGCONFIG(TAG, " Attenuation: 6db (max 2.2V)");
      break;
    case ADC_11db:
      ESP_LOGCONFIG(TAG, " Attenuation: 11db (max 3.9V)");
      break;
  }
}
float BatteryCheck::get_setup_priority() const { return setup_priority::DATA; }
float BatteryCheck::sample() {
  gpio_pullup_en((gpio_num_t)this->pin_);
  gpio_pulldown_en((gpio_num_t)this->pin_);

  int value;
  adc2_get_raw((adc2_channel_t)(digitalPinToAnalogChannel(this->pin_) - 10),
               (adc_bits_width_t)3, &value);

  gpio_pullup_dis((gpio_num_t)this->pin_);
  gpio_pulldown_dis((gpio_num_t)this->pin_);

  float value_v = value / 4095.0f;
  switch (this->attenuation_) {
    case ADC_0db:
      value_v *= 1.1;
      break;
    case ADC_2_5db:
      value_v *= 1.5;
      break;
    case ADC_6db:
      value_v *= 2.2;
      break;
    case ADC_11db:
      value_v *= 3.9;
      break;
  }
  ESP_LOGD(TAG, "Battery voltage=%.6fV", value_v);
  return value_v;
}

}  // namespace battery_check
}  // namespace esphome
