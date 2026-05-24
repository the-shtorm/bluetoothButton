#include <GyverButton.h>
#include <BleKeyboard.h>
#include <Encoder_range.h>
#include <SevSeg.h>

#define BTN_PIN 13      // bell button on Pin 13
#define CLK_PIN 18      // encoder CLK
#define DT_PIN 19       // encoder DT


// LOGGING TIMER VARIABLES
#define EXE_INTERVAL 1000
unsigned long lastExecutedMillis = 0;


// DISPLAY SETTINGS
byte numDigits = 4;
bool resistorsOnSegments = true;
byte hardwareConfig = COMMON_CATHODE;
bool updateWithDelays = false; 
bool leadingZeros = false; 
bool disableDecPoint = false;

/* Object Initialization */
GButton btn(BTN_PIN);               // Button object
GButton clkTrack(CLK_PIN);          // Encoder object
SevSeg sevSeg;                      // Segment display

Range<int> stateRange(STATE_FLASH, STATE_RIGHT, STATE_FLASH, true);     // Range object to process encoder states
ProjectState currentState = (ProjectState)stateRange.change(0);         // State for current button mode
BleKeyboard bleKeyboard("SmartButton", "Manufacturer", 100);            // Initialize BLE keyboard

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("Started");

    // Define which ESP32 pins are connected to Digit pins 12, 9, 8, 6
    byte digitPins[] = {14, 27, 26, 25};

    // Segment pins: A (11), B (7), C (4), D (2), E (1), F (10), G (5), DP (3) on the 3641AS
    byte segmentPins[] = {4, 12, 15, 23, 22, 21, 32, 33};

    bleKeyboard.begin();
    btn.setTickMode(MANUAL);       // We will call tick() manually in loop
    btn.setClickTimeout(100);

    clkTrack.setType(HIGH_PULL); 
    clkTrack.setDirection(NORM_OPEN);

    clkTrack.setDebounce(20);

    sevSeg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
               updateWithDelays, leadingZeros, disableDecPoint);
    sevSeg.setBrightness(90);

    Show_state(currentState);

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
        Show_state(currentState);

        Serial.printf("\nState changed to %d\n", currentState);
    }

    if (bleKeyboard.isConnected()) {
        if (click) BLE_send(currentState);
    } else {
        if (click) Serial.println("No device is connected");
    }

    sevSeg.refreshDisplay();
} 