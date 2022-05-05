#pragma once

#include <Arduino.h>

#include "homie.h"
#include "temp_node.h"

#define UPPER_RANGE_ID "upper-range"
#define LOWER_RANGE_ID "lower-range"

#define UPPER_RANGE_DEFAULT 30
#define LOWER_RANGE_DEFAULT 5

class Range : public Property {
 public:
    Range(const char* name, const char* id, Temp* node, PROPERTY_TYPE type, bool settable, bool retained,
          const char* data_type, String format = "", String unit = "");

    bool Init(Homie* homie);

    void HandleCurrentState();

    void HandleSettingNewValue();

 private:
    bool new_data_for_save_ = false;

    const uint16_t kDelayForSaveSettings_ = 5 * 1000;  // 5s
    uint32_t last_update_time_ = 0;
    Temp* temp_node = nullptr;
    bool LoadSettings();
    bool SaveSettings();
};
