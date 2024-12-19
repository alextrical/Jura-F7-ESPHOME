#include "esphome.h"

// Optimized JuraCoffee class for ESPHome on a Wemos D1 microcontroller
class JuraCoffee : public PollingComponent, public UARTDevice {
 private:
  // Use constexpr for fixed sizes to minimize memory overhead
  static constexpr size_t NUM_SENSORS = 9;
  static constexpr size_t NUM_TEXT_SENSORS = 2;

  // Use fixed-size arrays instead of dynamic arrays
  Sensor *sensors[NUM_SENSORS];
  TextSensor *text_sensors[NUM_TEXT_SENSORS];

  // Use smaller data types where possible
  uint32_t counts[NUM_SENSORS]; // Coffee counters
  std::string tray_status, tank_status; // Machine status

  // Commands stored as regular constants
  const char *CMD_EEPROM = "RT:0000";
  const char *CMD_IC = "IC:";

 public:
  // Constructor initializes sensor and text sensor pointers
  JuraCoffee(UARTComponent *parent, 
             Sensor *sensor1, Sensor *sensor2, Sensor *sensor3, Sensor *sensor4, Sensor *sensor5, 
             Sensor *sensor6, Sensor *sensor7, Sensor *sensor8, Sensor *sensor9, 
             TextSensor *text_sensor1, TextSensor *text_sensor2)
      : UARTDevice(parent) {
    sensors[0] = sensor1;
    sensors[1] = sensor2;
    sensors[2] = sensor3;
    sensors[3] = sensor4;
    sensors[4] = sensor5;
    sensors[5] = sensor6;
    sensors[6] = sensor7;
    sensors[7] = sensor8;
    sensors[8] = sensor9;

    text_sensors[0] = text_sensor1;
    text_sensors[1] = text_sensor2;
  }

  void setup() override {
    // Set a reasonable update interval 
    this->set_update_interval(60000); // 300,000 ms = 5 minutes
  }

  void update() override {
    // Fetch and process EEPROM data
    String eeprom_data = fetchData(CMD_EEPROM);
    if (!processEEPROMData(eeprom_data)) {
      ESP_LOGE("main", "Failed to process EEPROM data.");
      return;
    }

    // Fetch and process IC data
    String ic_data = fetchData(CMD_IC);
    if (!processICData(ic_data)) {
      ESP_LOGE("main", "Failed to process IC data.");
      return;
    }

    // Publish data to sensors
    publishSensorData();
  }

 private:
  // Function to fetch data from the coffee machine
  String fetchData(const char *command) {
    String result;
    int timeout = 0;

    // Clear UART buffer
    while (available()) read();

    // Send command
    String formatted_command = String(command) + "\r\n";
    for (char c : formatted_command) {
      write(c);
      delay(8); // Allow time for UART transmission
    }

    // Read response

   // ESP_LOGD("main", "Sending command: %s", command);

    String response;
    while (!response.endsWith("\r\n")) {
      if (available()) {
        response += (char)read();
      } else {
        delay(10);
      }
      if (++timeout > 1500) {
        ESP_LOGE("main", "Timeout waiting for response to command: %s", command);
        return "";
      }
    }
    return response.substring(0, response.length() - 2); // Remove \r\n
  }

  // Parse EEPROM data and extract coffee counters
  bool processEEPROMData(const String &data) {
    if (data.isEmpty()) return false;


    //ESP_LOGD("main", "Raw EEPROM Data: %s", data.c_str());

    // Parse known fields
    counts[0] = parseHexSubstring(data, 3, 7);   // Single espresso
    counts[1] = parseHexSubstring(data, 7, 11);  // Double espresso
    counts[2] = parseHexSubstring(data, 11, 15); // Coffee
    counts[3] = parseHexSubstring(data, 15, 19); // Double coffee
    counts[4] = parseHexSubstring(data, 19, 23); // Single Ristretto
    counts[5] = parseHexSubstring(data, 27, 31); // Double Ristretto
    counts[6] = parseHexSubstring(data, 31, 35); // Brew-unit movements
    counts[7] = parseHexSubstring(data, 35, 39); // Cleanings
    counts[8] = parseHexSubstring(data, 59, 63); // Grounds from cleaning


    // Parse and log unknown fields
    uint32_t unknown_fields[7];
    unknown_fields[0] = parseHexSubstring(data, 23, 27); // Unknown field
    unknown_fields[1] = parseHexSubstring(data, 39, 43); // Unknown field
    unknown_fields[2] = parseHexSubstring(data, 43, 47); // Unknown field
    unknown_fields[3] = parseHexSubstring(data, 47, 51); // Unknown field
    unknown_fields[4] = parseHexSubstring(data, 51, 55); // Unknown field
    unknown_fields[5] = parseHexSubstring(data, 55, 59); // Unknown field
    unknown_fields[6] = parseHexSubstring(data, 63, 67); // Unknown field

    for (size_t i = 0; i < 7; ++i) {
      ESP_LOGD("main", "Unknown Field %d: %u", i + 1, unknown_fields[i]);
    }


    return true;
  }

  // Parse IC data to determine tray and tank status
  bool processICData(const String &data) {
    if (data.isEmpty()) return false;

    String hex_string = data.substring(3, 5);
    if (!isHexadecimal(hex_string)) return false;

    byte hex_to_byte = strtol(hex_string.c_str(), nullptr, 16);
    tray_status = bitRead(hex_to_byte, 4) ? "Present" : "Missing";
    tank_status = bitRead(hex_to_byte, 5) ? "Fill Tank" : "OK";

    return true;
  }

  // Publish sensor data to ESPHome
  void publishSensorData() {
    for (size_t i = 0; i < NUM_SENSORS; ++i) {
      if (sensors[i] != nullptr) {
        sensors[i]->publish_state(counts[i]);
      }
    }

    if (text_sensors[0] != nullptr) text_sensors[0]->publish_state(tray_status);
    if (text_sensors[1] != nullptr) text_sensors[1]->publish_state(tank_status);
  }

  // Parse a hex substring safely
  uint32_t parseHexSubstring(const String &data, int start, int end) {
    if (start < 0 || end > data.length() || start >= end) return 0;

    String substring = data.substring(start, end);
    if (!isHexadecimal(substring)) return 0;

    return strtol(substring.c_str(), nullptr, 16);
  }

  // Check if a string contains only hexadecimal characters
  bool isHexadecimal(const String &str) {
    for (char c : str) {
      if (!isxdigit(c)) return false;
    }
    return true;
  }

  
};
