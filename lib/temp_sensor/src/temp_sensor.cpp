#include "temp_sensor.h"

#include "file_system/src/file_system.h"

#define DALLAS  // or any other temperature sensor You use
// #define DHTsensor
// #define BMP180

#ifdef DALLAS

const uint8_t one_wire_bus = 4;  // ONE WIRE BUS GPIO DECLARED
OneWire one_wire(one_wire_bus);
DallasTemperature sensor(&one_wire);
#endif

#ifdef DHTsensor

#include "DHT.h"
#define DHTPIN 4  // Digital pin connected to the DHT sensor
// Uncomment whatever type you're using!
// #define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT11  // DHT 22  (AM2302), AM2321
// #define DHTTYPE DHT21   // DHT 21 (AM2301)
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
DHT dht(DHTPIN, DHTTYPE);
#endif

#ifdef BMP180
// The BMP180 barometric sensor uses I2C communication protocol.
// So, you need to use the SDA (GPIO 21) and SCL (GPIO 22) pins of the ESP32.
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;
#endif

Temperature::Temperature(const char *name, const char *id, Temp *node, PROPERTY_TYPE type, bool settable, bool retained,
                         const char *data_type)
    : Property(name, id, node, type, settable, retained, data_type) {
#ifdef DALLAS
    sensor.begin();
#endif

#ifdef DHTsensor
    dht.begin();
#endif

#ifdef BMP180
    if (!bmp.begin()) {
        Serial.println("[D] Could not find a valid BMP085/BMP180 sensor, check wiring!");
        while (true) {}
    }
#endif
}

float Temperature::GetTemperature() {
#ifdef DALLAS

    sensor.requestTemperatures();
    float temperatureC = sensor.getTempCByIndex(0);
    Serial.println("[D] Temperature: " + String(temperatureC));
    return temperatureC;
#endif

#ifdef DHTsensor

    float temperatureC = dht.readTemperature();
    if (isnan(temperatureC)) {
        Serial.println(F("[D] Failed to read from DHT sensor!"));
    }
    Serial.println("[D] Temperature: " + String(temperatureC));
    return temperatureC;
#endif

#ifdef BMP180

    float temperatureC = bmp.readTemperature();
    if (isnan(temperatureC)) {
        Serial.println(F("[D] Failed to read from BMP180 sensor!"));
    }
    Serial.print(F("[D] Temperature: "));
    Serial.print(temperatureC);
    Serial.println(F("°C "));
    return temperatureC;
#endif
}

void Temperature::HandleCurrentState() {
    if (millis() - period_timer_ > kPeriodDelay_) {
        period_timer_ = millis();
        this->SetValue(String(this->GetTemperature()));
    }
}
