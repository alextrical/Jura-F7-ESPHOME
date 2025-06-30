#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace jura_coffee {

class JuraCoffee : public PollingComponent, public uart::UARTDevice {
 public:
  void setup() override;
  void update() override;
  void loop() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  void dump_config() override;

  // Sensor setters
  void set_num_single_espresso_sensor(sensor::Sensor *sensor) { num_single_espresso_sensor_ = sensor; }
  void set_num_double_espresso_sensor(sensor::Sensor *sensor) { num_double_espresso_sensor_ = sensor; }
  void set_num_coffee_sensor(sensor::Sensor *sensor) { num_coffee_sensor_ = sensor; }
  void set_num_double_coffee_sensor(sensor::Sensor *sensor) { num_double_coffee_sensor_ = sensor; }
  void set_num_ristretto_sensor(sensor::Sensor *sensor) { num_ristretto_sensor_ = sensor; }
  void set_num_single_capuccino_sensor(sensor::Sensor *sensor) { num_single_capuccino_sensor_ = sensor; }
  void set_num_double_ristretto_sensor(sensor::Sensor *sensor) { num_double_ristretto_sensor_ = sensor; }
  void set_num_brewunit_movements_sensor(sensor::Sensor *sensor) { num_brewunit_movements_sensor_ = sensor; }
  void set_num_clean_sensor(sensor::Sensor *sensor) { num_clean_sensor_ = sensor; }
  void set_num_descaling_sensor(sensor::Sensor *sensor) { num_descaling_sensor_ = sensor; }
  void set_num_coffee_grounds_sensor(sensor::Sensor *sensor) { num_coffee_grounds_sensor_ = sensor; }

  // Text sensor setters
  void set_tray_status_sensor(text_sensor::TextSensor *sensor) { tray_status_sensor_ = sensor; }
  void set_tank_status_sensor(text_sensor::TextSensor *sensor) { tank_status_sensor_ = sensor; }

 protected:
  // Helper methods
  std::string send_command(const std::string &command);
  void decode_jura_response(const std::string &response);
  uint8_t encode_byte(uint8_t data_byte);
  uint8_t decode_byte(uint8_t encoded_byte);
  void read_counters();
  void read_status();
  void clear_uart_buffer();
  long parse_hex_substring(const std::string &data, int start, int end);

  // Sensor pointers
  sensor::Sensor *num_single_espresso_sensor_{nullptr};
  sensor::Sensor *num_double_espresso_sensor_{nullptr};
  sensor::Sensor *num_coffee_sensor_{nullptr};
  sensor::Sensor *num_double_coffee_sensor_{nullptr};
  sensor::Sensor *num_ristretto_sensor_{nullptr};
  sensor::Sensor *num_single_capuccino_sensor_{nullptr};
  sensor::Sensor *num_double_ristretto_sensor_{nullptr};
  sensor::Sensor *num_brewunit_movements_sensor_{nullptr};
  sensor::Sensor *num_clean_sensor_{nullptr};
  sensor::Sensor *num_descaling_sensor_{nullptr};
  sensor::Sensor *num_coffee_grounds_sensor_{nullptr};

  // Text sensor pointers
  text_sensor::TextSensor *tray_status_sensor_{nullptr};
  text_sensor::TextSensor *tank_status_sensor_{nullptr};

  // Internal state
  bool first_update_{true};
  uint32_t last_update_{0};
  std::string last_response_;
  
  // Counter values
  long num_single_espresso_{0};
  long num_double_espresso_{0};
  long num_coffee_{0};
  long num_double_coffee_{0};
  long num_ristretto_{0};
  long num_single_capuccino_{0};
  long num_double_ristretto_{0};
  long num_brewunit_movements_{0};
  long num_clean_{0};
  long num_descaling_{0};
  long num_coffee_grounds_{0};

  // Status values
  std::string tray_status_{"Unknown"};
  std::string tank_status_{"Unknown"};
};

}  // namespace jura_coffee
}  // namespace esphome