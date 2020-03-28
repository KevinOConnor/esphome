import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor
from esphome.core import coroutine_with_priority
from esphome.const import CONF_ATTENUATION, CONF_ID, CONF_PIN, ICON_FLASH, UNIT_VOLT


ATTENUATION_MODES = {
    '0db': cg.global_ns.ADC_0db,
    '2.5db': cg.global_ns.ADC_2_5db,
    '6db': cg.global_ns.ADC_6db,
    '11db': cg.global_ns.ADC_11db,
}


def validate_adc_pin(value):
    return pins.analog_pin(value, only_ADC1=False)


battery_check_ns = cg.esphome_ns.namespace('battery_check')
BatteryCheck = battery_check_ns.class_('BatteryCheck', sensor.Sensor,
                                       cg.Component)

CONFIG_SCHEMA = sensor.sensor_schema(UNIT_VOLT, ICON_FLASH, 2).extend({
    cv.GenerateID(): cv.declare_id(BatteryCheck),
    cv.Required(CONF_PIN): validate_adc_pin,
    cv.SplitDefault(CONF_ATTENUATION, esp32='6db'):
        cv.enum(ATTENUATION_MODES, lower=True),
})

@coroutine_with_priority(80.0)
def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)

    cg.add(var.set_pin(config[CONF_PIN]))

    if CONF_ATTENUATION in config:
        cg.add(var.set_attenuation(config[CONF_ATTENUATION]))
