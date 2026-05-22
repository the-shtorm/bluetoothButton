#include <GyverButton.h>
#include <BleKeyboard.h>
#include <Encoder_range.h>

#define BTN_PIN 13      // bell button on Pin 13
#define CLK_PIN = 18    // encoder CLK
#define DT_PIN = 19     // encoder DT
BleKeyboard bleKeyboard("SmartButton", "Manufacturer", 100); // Initialize BLE keyboard

#define EXE_INTERVAL 1000
unsigned long lastExecutedMillis = 0;
int lastClkState;
ProjectState currentState = (ProjectState)stateRange;


/* Object Initialization */
GButton btn(BTN_PIN);
GButton clkTrack(CLK_PIN);

Range<int> stateRange(STATE_FLASH, STATE_SANJI, STATE_FLASH, true);

void setup() {
    //pinMode(13, INPUT_PULLUP);
    Serial.begin(115200);
    delay(500);
    Serial.println("Started");

    bleKeyboard.begin();
    btn.setTickMode(MANUAL);       // We will call tick() manually in loop

    clkTrack.setType(HIGH_PULLUP); 
    clkTrack.setDirection(NORM_OPEN);

    clkTrack.setDebounce(20);

    lastClkState = digitalRead(clkPin);

    delay(2000);
    Serial.print("Setup completed");
}



void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastExecutedMillis >= EXE_INTERVAL) {
        lastExecutedMillis = currentMillis;

        Serial.print(".");
    }

    // Required: Check button state
    btn.tick();
    clkTrack.tick();
    boolean click = btn.isSingle();
    //    boolean doubleClick = btn.isDouble();
    //    boolean holded = btn.isHolded();

    if (clkTrack.isStep()) {
        int currentClkState = digitalRead(CLK_PIN);

        if (currentClkState != lastClkState && currentClkState == LOW) {
            int step = (digitalRead(dtPin) != currentClkState) ? 1 : -1;
            currentState = (ProjectState)stateRange.change(step);
        }

        Serial.print("\nState changed to " + currentState + "\n")
    }

    if (click) BLE_send(currentState);

    if (false) {
        // 2. Single Click -> Send F1
        if (click && !doubleClick) {
            Serial.println("\nSending F1...");
            bleKeyboard.write(KEY_F1);
        }

        // 2. Double Click -> Send F2
        if (doubleClick) {
        Serial.println("\nSending F2...");
        bleKeyboard.write(KEY_F2);
        }

        // 3. Long Press (Once) -> Send Shift + F10
        if (holded) {
        Serial.println("\nSending Shift+F10...");
        bleKeyboard.press(KEY_LEFT_SHIFT);
        bleKeyboard.press(KEY_F10);
        delay(100);
        bleKeyboard.releaseAll();
        }
    } 
} 