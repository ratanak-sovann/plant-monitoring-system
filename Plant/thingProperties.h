#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

// Network credentials
const char SSID[] = "YourNetworkSSID";                  
const char PASS[] = "YourNetworkPassword"; 

// Pin definitions for clarity and easy management
const int PUMP_PIN = LED_BUILTIN;  // Define the pin used for controlling the water pump
const int MOISTURE_SENSOR_PIN = A1;  // Analog pin used for the moisture sensor
const int DHT_SENSOR_PIN = 2;  // Digital pin used for the DHT11 sensor

// Moisture level threshold for triggering the water pump
const int MOISTURE_THRESHOLD = 30;  // Moisture percentage below which the pump will activate

void onAutoWateringChange();
void onPumpStatusChange();

// Cloud-controlled variables
int humidity;
int moisture;
int temperature;
bool auto_watering = false;
bool pump_status = false;

void initProperties() {
  ArduinoCloud.addProperty(humidity, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(moisture, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(temperature, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(auto_watering, READWRITE, ON_CHANGE, onAutoWateringChange);
  ArduinoCloud.addProperty(pump_status, READWRITE, ON_CHANGE, onPumpStatusChange);
}

// Configure WiFi connection
WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
