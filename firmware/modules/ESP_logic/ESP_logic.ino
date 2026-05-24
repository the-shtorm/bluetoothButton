#include <GyverButton.h>
#include <BleKeyboard.h>
#include <Encoder_range.h>

#define BTN_PIN 13      // bell button on Pin 13
#define CLK_PIN 18    // encoder CLK
#define DT_PIN 19     // encoder DT
BleKeyboard bleKeyboard("SmartButton", "Manufacturer", 100); // Initialize BLE keyboard

#define EXE_INTERVAL 1000
unsigned long lastExecutedMillis = 0;


/* Object Initialization */
GButton btn(BTN_PIN);
GButton clkTrack(CLK_PIN);

Range<int> stateRange(STATE_FLASH, STATE_RIGHT, STATE_FLASH, true);
ProjectState currentState = (ProjectState)stateRange.change(0);

void setup() {
    //pinMode(13, INPUT_PULLUP);
    Serial.begin(115200);
    delay(500);
    Serial.println("Started");

    bleKeyboard.begin();
    btn.setTickMode(MANUAL);       // We will call tick() manually in loop
    btn.setClickTimeout(100);

    clkTrack.setType(HIGH_PULL); 
    clkTrack.setDirection(NORM_OPEN);

    clkTrack.setDebounce(20);

    delay(2000);
    Serial.print("Setup completed");
}


void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastExecutedMillis >= EXE_INTERVAL) {
        lastExecutedMillis = currentMillis;

        Serial.print(".");
    }

    btn.tick();
    clkTrack.tick();
    boolean click = btn.isSingle();

    if (clkTrack.isClick()) {
        int currentClkState = digitalRead(CLK_PIN);

        int step = (digitalRead(DT_PIN) != currentClkState) ? 1 : -1;
        currentState = (ProjectState)stateRange.change(step);

        Serial.printf("\nState changed to %d\n", currentState);
    }

    if (bleKeyboard.isConnected()) {
        if (click) BLE_send(currentState);
    } else {
        if (click) Serial.println("No device is connected");
    }
} 