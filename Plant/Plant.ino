/* 
  Smart Plant Monitoring System
  This sketch integrates with the Arduino IoT Cloud to manage a smart plant monitoring system,
  using sensors and automated watering based on configurable moisture thresholds.
*/

#include <DHT11.h>
#include "thingProperties.h"

DHT11 dht11(DHT_SENSOR_PIN);

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Set the pump pin as an output
  pinMode(PUMP_PIN, OUTPUT);

  // Initialize properties defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  // Keep the cloud connection updated
  ArduinoCloud.update();

  // Read moisture level and map it to a percentage
  int moistureRaw = analogRead(MOISTURE_SENSOR_PIN);
  moisture = map(moistureRaw, 1023, 0, 100, 0);
  Serial.print("Moisture Sensor Percentage: ");
  Serial.println(moisture);

  // Read temperature and humidity, and print the results
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  if (result == 0) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C\nHumidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.println(DHT11::getErrorString(result));
  }
  
  // Control pump
  digitalWrite(PUMP_PIN, pump_status ? HIGH : LOW);

  // Automatically control the water pump based on moisture level
  autoWatering();

  // Display the pump status and auto watering status for debugging
  Serial.print("Pump status: ");
  Serial.println(pump_status ? "ON" : "OFF");
  Serial.print("Auto watering: ");
  Serial.println(auto_watering ? "ENABLED" : "DISABLED");
  Serial.println("-------------------------------------------------------");

  delay(1000);
}

/* 
  Since PumpStatus is READ_WRITE variable, onPumpStatusChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onPumpStatusChange()  {
  // Update pump status accordingly
  digitalWrite(PUMP_PIN, pump_status ? HIGH : LOW);
}

/* 
  Since AutoWatering is READ_WRITE variable, onAutoWateringChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onAutoWateringChange()  {
  // Add your code here if you to act upon AutoWatering change
}

void autoWatering() {
  if (auto_watering == true){
    if (moisture <= MOISTURE_THRESHOLD){
        pump_status = true;
    }
    else {
        pump_status = false;
    }
  }
}
