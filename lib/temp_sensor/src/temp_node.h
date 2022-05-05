#pragma once

#include <Arduino.h>

#include "homie.h"

class Temp : public Node {
 public:
    Temp(const char* name, const char* id, Device* device);

    bool Init(Homie* homie);

    void HandleCurrentState();

    bool ValidateValue(Property* property);
    void SendNotification(String message_);

 private:
    bool notif_sended_ = false;
    float upper_range_ = 0;
    float lower_range_ = 0;

    float hysteresis_ = 0.1;

    bool new_data_for_save_ = false;

    uint32_t period_timer_ = millis();
};
