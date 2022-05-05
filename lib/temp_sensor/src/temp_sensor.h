#pragma once

#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#include "homie.h"
#include "temp_node.h"

#define TEMP_VALUE_ID "value"

class Temperature : public Property {
 public:
    Temperature(const char* name, const char* id, Temp* node, PROPERTY_TYPE type, bool settable, bool retained,
                const char* data_type);

    void HandleCurrentState();
    float GetTemperature();

 private:
    const uint16_t kPeriodDelay_ = 5 * 1000;  // update temp interval

    uint32_t period_timer_ = millis();
};
