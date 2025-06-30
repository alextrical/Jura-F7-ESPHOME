#include "jura_coffee.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include <cstring>

namespace esphome {
namespace jura_coffee {

static const char *const TAG = "jura_coffee";

void JuraCoffee::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Jura Coffee...");
  
  // Clear UART buffer on startup
  this->clear_uart_buffer();
  
  // Initial status check
  this->read_status();
}

void JuraCoffee::update() {
  ESP_LOGD(TAG, "Updating Jura Coffee sensors...");
  
  // Read counters from the coffee machine
  this->read_counters();
  
  // Read status information
  this->read_status();
  
  // Update all sensors with current values
  if (this->num_single_espresso_sensor_ != nullptr) {
    this->num_single_espresso_sensor_->publish_state(this->num_single_espresso_);
  }
  if (this->num_double_espresso_sensor_ != nullptr) {
    this->num_double_espresso_sensor_->publish_state(this->num_double_espresso_);
  }
  if (this->num_coffee_sensor_ != nullptr) {
    this->num_coffee_sensor_->publish_state(this->num_coffee_);
  }
  if (this->num_double_coffee_sensor_ != nullptr) {
    this->num_double_coffee_sensor_->publish_state(this->num_double_coffee_);
  }
  if (this->num_ristretto_sensor_ != nullptr) {
    this->num_ristretto_sensor_->publish_state(this->num_ristretto_);
  }
  if (this->num_single_capuccino_sensor_ != nullptr) {
    this->num_single_capuccino_sensor_->publish_state(this->num_single_capuccino_);
  }
  if (this->num_double_ristretto_sensor_ != nullptr) {
    this->num_double_ristretto_sensor_->publish_state(this->num_double_ristretto_);
  }
  if (this->num_brewunit_movements_sensor_ != nullptr) {
    this->num_brewunit_movements_sensor_->publish_state(this->num_brewunit_movements_);
  }
  if (this->num_clean_sensor_ != nullptr) {
    this->num_clean_sensor_->publish_state(this->num_clean_);
  }
  if (this->num_descaling_sensor_ != nullptr) {
    this->num_descaling_sensor_->publish_state(this->num_descaling_);
  }
  if (this->num_coffee_grounds_sensor_ != nullptr) {
    this->num_coffee_grounds_sensor_->publish_state(this->num_coffee_grounds_);
  }
  
  // Update text sensors
  if (this->tray_status_sensor_ != nullptr) {
    this->tray_status_sensor_->publish_state(this->tray_status_);
  }
  if (this->tank_status_sensor_ != nullptr) {
    this->tank_status_sensor_->publish_state(this->tank_status_);
  }
}

void JuraCoffee::loop() {
  // Handle any incoming UART data
  while (this->available()) {
    uint8_t byte;
    this->read_byte(&byte);
    // Process incoming data if needed
  }
}

void JuraCoffee::dump_config() {
  ESP_LOGCONFIG(TAG, "Jura Coffee:");
  if (this->num_single_espresso_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Single Espresso", this->num_single_espresso_sensor_);
  }
  if (this->num_double_espresso_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Double Espresso", this->num_double_espresso_sensor_);
  }
  if (this->num_coffee_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Coffee", this->num_coffee_sensor_);
  }
  if (this->num_double_coffee_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Double Coffee", this->num_double_coffee_sensor_);
  }
  if (this->num_ristretto_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Ristretto", this->num_ristretto_sensor_);
  }
  if (this->num_single_capuccino_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Single Cappuccino", this->num_single_capuccino_sensor_);
  }
  if (this->num_double_ristretto_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Double Ristretto", this->num_double_ristretto_sensor_);
  }
  if (this->num_brewunit_movements_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Brew Unit Movements", this->num_brewunit_movements_sensor_);
  }
  if (this->num_clean_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Cleanings", this->num_clean_sensor_);
  }
  if (this->num_descaling_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Descalings", this->num_descaling_sensor_);
  }
  if (this->num_coffee_grounds_sensor_ != nullptr) {
    LOG_SENSOR("  ", "Coffee Grounds", this->num_coffee_grounds_sensor_);
  }
  
  if (this->tray_status_sensor_ != nullptr) {
    LOG_TEXT_SENSOR("  ", "Tray Status", this->tray_status_sensor_);
  }
  if (this->tank_status_sensor_ != nullptr) {
    LOG_TEXT_SENSOR("  ", "Tank Status", this->tank_status_sensor_);
  }
}

std::string JuraCoffee::send_command(const std::string &command) {
  ESP_LOGD(TAG, "Sending command: %s", command.c_str());
  
  // Clear any existing data in UART buffer
  this->clear_uart_buffer();
  
  // Encode and send the command
  std::string full_command = command + "\r\n";
  for (char c : full_command) {
    // Jura uses a special encoding where each bit is encoded as two bits
    // This is a simplified version - you may need to adjust based on your specific model
    for (int bit = 0; bit < 8; bit += 2) {
      uint8_t encoded_byte = 0xFF;
      
      // Extract 2 bits and encode them
      uint8_t two_bits = (c >> bit) & 0x03;
      if (two_bits & 0x01) encoded_byte &= ~(1 << 2);  // Bit 0 -> Bit 2
      if (two_bits & 0x02) encoded_byte &= ~(1 << 5);  // Bit 1 -> Bit 5
      
      this->write_byte(encoded_byte);
      delay(8);  // 8ms delay between bytes as per Jura protocol
    }
  }
  
  // Wait for response
  delay(100);
  
  // Read response
  std::string response = "";
  uint32_t start_time = millis();
  
  while (millis() - start_time < 1000) {  // 1 second timeout
    if (this->available()) {
      uint8_t byte;
      this->read_byte(&byte);
      
      // Decode the byte (reverse of encoding process)
      uint8_t decoded_byte = 0;
      if (!(byte & (1 << 2))) decoded_byte |= 0x01;
      if (!(byte & (1 << 5))) decoded_byte |= 0x02;
      
      response += static_cast<char>(decoded_byte);
      
      // Check for end of response
      if (response.length() >= 2 && response.substr(response.length() - 2) == "\r\n") {
        break;
      }
    }
    delay(10);
  }
  
  ESP_LOGD(TAG, "Received response: %s", response.c_str());
  return response;
}

void JuraCoffee::read_counters() {
  ESP_LOGD(TAG, "Reading counters from Jura...");
  
  // Read the RT (Read Total) command to get all counters
  std::string response = this->send_command("RT:0000");
  
  if (response.length() > 10) {
    // Parse the hex response - format varies by model but typically:
    // RT:00800001448B00000046001400062CB4005600208C7E00020000001A0000007D
    
    // Skip the "RT:" prefix
    std::string data = response.substr(3);
    
    if (data.length() >= 64) {  // Ensure we have enough data
      // Parse counters based on positions (this may need adjustment for your model)
      this->num_single_espresso_ = this->parse_hex_substring(data, 0, 4);     // Position 0-3
      this->num_double_espresso_ = this->parse_hex_substring(data, 4, 8);     // Position 4-7
      this->num_coffee_ = this->parse_hex_substring(data, 8, 12);             // Position 8-11
      this->num_double_coffee_ = this->parse_hex_substring(data, 12, 16);     // Position 12-15
      this->num_ristretto_ = this->parse_hex_substring(data, 16, 20);         // Position 16-19
      this->num_single_capuccino_ = this->parse_hex_substring(data, 20, 24);  // Position 20-23
      this->num_double_ristretto_ = this->parse_hex_substring(data, 24, 28);  // Position 24-27
      this->num_brewunit_movements_ = this->parse_hex_substring(data, 28, 32);// Position 28-31
      this->num_clean_ = this->parse_hex_substring(data, 32, 36);             // Position 32-35
      this->num_descaling_ = this->parse_hex_substring(data, 36, 40);         // Position 36-39
      this->num_coffee_grounds_ = this->parse_hex_substring(data, 40, 44);    // Position 40-43
      
      ESP_LOGD(TAG, "Counter values updated");
    } else {
      ESP_LOGW(TAG, "Response too short to parse counters");
    }
  } else {
    ESP_LOGW(TAG, "No valid response received for counter query");
  }
}

void JuraCoffee::read_status() {
  ESP_LOGD(TAG, "Reading status from Jura...");
  
  // Read status information - commands may vary by model
  std::string tray_response = this->send_command("ST:0001");  // Status query 1
  std::string tank_response = this->send_command("ST:0002");  // Status query 2
  
  // Parse tray status
  if (tray_response.length() > 5) {
    std::string tray_data = tray_response.substr(5); // Skip "ST:0001"
    if (tray_data.find("00") != std::string::npos) {
      this->tray_status_ = "OK";
    } else if (tray_data.find("01") != std::string::npos) {
      this->tray_status_ = "Missing";
    } else {
      this->tray_status_ = "Unknown";
    }
  }
  
  // Parse tank status
  if (tank_response.length() > 5) {
    std::string tank_data = tank_response.substr(5); // Skip "ST:0002"
    if (tank_data.find("00") != std::string::npos) {
      this->tank_status_ = "OK";
    } else if (tank_data.find("01") != std::string::npos) {
      this->tank_status_ = "Empty";
    } else {
      this->tank_status_ = "Unknown";
    }
  }
}

void JuraCoffee::clear_uart_buffer() {
  while (this->available()) {
    uint8_t byte;
    this->read_byte(&byte);
  }
}

long JuraCoffee::parse_hex_substring(const std::string &data, int start, int end) {
  if (start >= data.length() || end > data.length() || start >= end) {
    return 0;
  }
  
  std::string hex_str = data.substr(start, end - start);
  
  // // Convert hex string to long
  // try {
    return std::strtol(hex_str.c_str(), nullptr, 16);
  // } catch (...) {
  //   ESP_LOGW(TAG, "Failed to parse hex substring: %s", hex_str.c_str());
  //   return 0;
  // }
}

}  // namespace jura_coffee
}  // namespace esphome