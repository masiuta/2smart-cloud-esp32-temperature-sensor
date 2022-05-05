#include "main.h"

// instructions https://randomnerdtutorials.com/esp32-ds18b20-temperature-arduino-ide/

Cloud2Smart smartcloud;

void setup() {
    Temp *temp_node = new Temp("Temperature", "temperature", smartcloud.GetDevice());  // (name, id,device)

    Temperature *temperature = new Temperature("Value", TEMP_VALUE_ID, temp_node, SENSOR, false, true, "string");
    Range *upper_range = new Range("Upper range", UPPER_RANGE_ID, temp_node, SENSOR, true, true, "float", "-30:70");
    Range *lower_range = new Range("Lower range", LOWER_RANGE_ID, temp_node, SENSOR, true, true, "float", "-30:70");

    smartcloud.setup();
}
void loop() { smartcloud.loop(); }
