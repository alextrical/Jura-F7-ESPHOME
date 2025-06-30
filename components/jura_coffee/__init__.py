import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, text_sensor, uart
from esphome.const import (
    CONF_ID,
    CONF_SENSORS,
    CONF_TEXT_SENSORS,
    UNIT_EMPTY,
    ICON_COUNTER,
    STATE_CLASS_TOTAL_INCREASING,
)

DEPENDENCIES = ["uart"]

# Define the namespace and class
jura_coffee_ns = cg.esphome_ns.namespace("jura_coffee")
JuraCoffee = jura_coffee_ns.class_("JuraCoffee", cg.PollingComponent, uart.UARTDevice)

# Configuration constants
CONF_UART_ID = "uart_id"

# Sensor configuration schema
SENSOR_SCHEMA = cv.Schema({
    cv.Optional("num_single_espresso"): sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_COUNTER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    cv.Optional("num_double_espresso"): sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_COUNTER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    cv.Optional("num_coffee"): sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_COUNTER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    cv.Optional("num_double_coffee"): sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_COUNTER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    cv.Optional("num_ristretto"): sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_COUNTER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    cv.Optional("num_single_capuccino"): sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_COUNTER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    cv.Optional("num_double_ristretto"): sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_COUNTER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    cv.Optional("num_brewunit_movements"): sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_COUNTER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    cv.Optional("num_clean"): sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_COUNTER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    cv.Optional("num_descaling"): sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_COUNTER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
    cv.Optional("num_coffee_Grounds"): sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_COUNTER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_TOTAL_INCREASING,
    ),
})

# Text sensor configuration schema
TEXT_SENSOR_SCHEMA = cv.Schema({
    cv.Optional("tray_status"): text_sensor.text_sensor_schema(),
    cv.Optional("tank_status"): text_sensor.text_sensor_schema(),
})

# Main component configuration schema
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(JuraCoffee),
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Optional(CONF_SENSORS): SENSOR_SCHEMA,
    cv.Optional(CONF_TEXT_SENSORS): TEXT_SENSOR_SCHEMA,
    cv.Optional("update_interval", default="60s"): cv.update_interval,
}).extend(cv.polling_component_schema("60s")).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    # Configure sensors if provided
    if CONF_SENSORS in config:
        sensors_config = config[CONF_SENSORS]
        
        if "num_single_espresso" in sensors_config:
            sens = await sensor.new_sensor(sensors_config["num_single_espresso"])
            cg.add(var.set_num_single_espresso_sensor(sens))
            
        if "num_double_espresso" in sensors_config:
            sens = await sensor.new_sensor(sensors_config["num_double_espresso"])
            cg.add(var.set_num_double_espresso_sensor(sens))
            
        if "num_coffee" in sensors_config:
            sens = await sensor.new_sensor(sensors_config["num_coffee"])
            cg.add(var.set_num_coffee_sensor(sens))
            
        if "num_double_coffee" in sensors_config:
            sens = await sensor.new_sensor(sensors_config["num_double_coffee"])
            cg.add(var.set_num_double_coffee_sensor(sens))
            
        if "num_ristretto" in sensors_config:
            sens = await sensor.new_sensor(sensors_config["num_ristretto"])
            cg.add(var.set_num_ristretto_sensor(sens))
            
        if "num_single_capuccino" in sensors_config:
            sens = await sensor.new_sensor(sensors_config["num_single_capuccino"])
            cg.add(var.set_num_single_capuccino_sensor(sens))
            
        if "num_double_ristretto" in sensors_config:
            sens = await sensor.new_sensor(sensors_config["num_double_ristretto"])
            cg.add(var.set_num_double_ristretto_sensor(sens))
            
        if "num_brewunit_movements" in sensors_config:
            sens = await sensor.new_sensor(sensors_config["num_brewunit_movements"])
            cg.add(var.set_num_brewunit_movements_sensor(sens))
            
        if "num_clean" in sensors_config:
            sens = await sensor.new_sensor(sensors_config["num_clean"])
            cg.add(var.set_num_clean_sensor(sens))
            
        if "num_descaling" in sensors_config:
            sens = await sensor.new_sensor(sensors_config["num_descaling"])
            cg.add(var.set_num_descaling_sensor(sens))
            
        if "num_coffee_Grounds" in sensors_config:
            sens = await sensor.new_sensor(sensors_config["num_coffee_Grounds"])
            cg.add(var.set_num_coffee_grounds_sensor(sens))

    # Configure text sensors if provided
    if CONF_TEXT_SENSORS in config:
        text_sensors_config = config[CONF_TEXT_SENSORS]
        
        if "tray_status" in text_sensors_config:
            sens = await text_sensor.new_text_sensor(text_sensors_config["tray_status"])
            cg.add(var.set_tray_status_sensor(sens))
            
        if "tank_status" in text_sensors_config:
            sens = await text_sensor.new_text_sensor(text_sensors_config["tank_status"])
            cg.add(var.set_tank_status_sensor(sens))