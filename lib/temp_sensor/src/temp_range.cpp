#include "temp_range.h"

#include "file_system/src/file_system.h"

Range::Range(const char* name, const char* id, Temp* node, PROPERTY_TYPE type, bool settable, bool retained,
             const char* data_type, String format, String unit)
    : Property(name, id, node, type, settable, retained, data_type) {
    temp_node = node;
}

bool Range::Init(Homie* homie) {  // initializes temperature range
    bool status = true;
    LoadSettings();
    if (value_ == "") {
        if (id_ == LOWER_RANGE_ID) {
            value_ = LOWER_RANGE_DEFAULT;
        } else if (id_ == UPPER_RANGE_ID) {
            value_ = UPPER_RANGE_DEFAULT;
        }
    }
    if (!Property::Init(homie)) status = false;

    return status;
}

void Range::HandleCurrentState() {
    if (millis() - last_update_time_ > kDelayForSaveSettings_ && new_data_for_save_) {
        SaveSettings();
        new_data_for_save_ = false;
        Serial.println("Settings saved");
    }
}

void Range::HandleSettingNewValue() {
    if (temp_node != nullptr && temp_node->ValidateValue(this)) {
        new_data_for_save_ = true;
        last_update_time_ = millis();
    }
}

bool Range::LoadSettings() {
    String file_name = "/" + id_ + ".txt";
    bool status = ReadSettings(file_name.c_str(), reinterpret_cast<byte*>(&value_), sizeof(value_));
    return status;
}

bool Range::SaveSettings() {
    if (value_ == "") return false;
    String file_name = "/" + id_ + ".txt";
    bool status = WriteSettings(file_name.c_str(), reinterpret_cast<byte*>(&value_), sizeof(value_));
    return status;
}
