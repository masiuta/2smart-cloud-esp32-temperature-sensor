#include "temp_node.h"

#include "temp_range.h"
#include "temp_sensor.h"

Temp::Temp(const char* name, const char* id, Device* device) : Node(name, id, device) {}

bool Temp::Init(Homie* homie) {
    bool status = true;
    if (!Node::Init(homie)) status = false;
    return status;
}

void Temp::HandleCurrentState() {
    Node::HandleCurrentState();
    float value = properties_.find(TEMP_VALUE_ID)->second->GetValue().toFloat();
    lower_range_ = properties_.find(LOWER_RANGE_ID)->second->GetValue().toFloat();
    upper_range_ = properties_.find(UPPER_RANGE_ID)->second->GetValue().toFloat();
    String message = "";

    if (value > (lower_range_ + hysteresis_) && value < (upper_range_ - hysteresis_)) {
        notif_sended_ = false;
    }
    if (value < (lower_range_ - hysteresis_)) {
        if (!notif_sended_) {
        message = "It's too cold here";
        SendNotification(message);
        notif_sended_ = true;
        }
    }
    if (value > (upper_range_ + hysteresis_)) {
        if (!notif_sended_) {
        message = "It's too hot here";
        SendNotification(message);
        notif_sended_ = true;
        }
    }
}

void Temp::SendNotification(String message_) {
        if (device_->IsSysNotifyEnabled()) {
            device_->SendNotification(message_.c_str());
            Serial.println("[D] SendNotification" + message_);
        }
}


bool Temp::ValidateValue(Property* property) {
    bool status = false;
    float validating_value = property->GetValue().toFloat();
    if (property->GetId() == UPPER_RANGE_ID) {
        float compaired_value = properties_.find(LOWER_RANGE_ID)->second->GetValue().toFloat();
        if (validating_value < compaired_value) {
            property->SetValue(String(upper_range_));
        } else {
            upper_range_ = validating_value;
            status = true;
        }
    }

    if (property->GetId() == LOWER_RANGE_ID) {
        float compaired_value = properties_.find(UPPER_RANGE_ID)->second->GetValue().toFloat();
        if (validating_value > compaired_value) {
            property->SetValue(String(lower_range_));
        } else {
            lower_range_ = validating_value;
            status = true;
        }
    }

    return status;
}
