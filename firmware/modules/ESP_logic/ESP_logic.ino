#include <GyverButton.h>
#include <BleKeyboard.h>

#define BTN_PIN 13  // Initialize button on Pin 13
BleKeyboard bleKeyboard("SmartButton", "Manufacturer", 100); // Initialize BLE keyboard

#define EXE_INTERVAL 1000
unsigned long lastExecutedMillis = 0;


/* Object Initialization */
GButton btn(BTN_PIN);


void setup() {
    //pinMode(13, INPUT_PULLUP);
    Serial.begin(115200);
    delay(500);
    Serial.println("Started");

    bleKeyboard.begin();
    btn.setTickMode(MANUAL);       // We will call tick() manually in loop

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
    boolean click = btn.isSingle();
    boolean doubleClick = btn.isDouble();
    boolean holded = btn.isHolded();



    if (bleKeyboard.isConnected()) {
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