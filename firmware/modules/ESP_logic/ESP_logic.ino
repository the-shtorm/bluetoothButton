#include <GyverButton.h>
#include <BleKeyboard.h>
#include <Encoder_range.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Bluetooth_logo.h>
#include <System_state.h>

#define BTN_PIN 13      // bell button on Pin 13
#define CLK_PIN 18      // encoder CLK
#define DT_PIN 19       // encoder DT


// LOGGING TIMER VARIABLES
#define EXE_INTERVAL 500
unsigned long lastExecutedMillis = 0;


// DISPLAY SETTINGS
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

/* Object Initialization */
GButton btn(BTN_PIN);                                                                   // Button object
GButton clkTrack(CLK_PIN);                                                              // Encoder object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);                       // OLED display

//      Range<int> stateRange(STATE_FLASH, STATE_RIGHT, STATE_FLASH, true);     // Range object to process encoder states

SystemState systemState(STATE_FLASH, STATE_RIGHT, STATE_FLASH);        // State for current button mode 
BleKeyboard bleKeyboard("SmartButton", "Manufacturer", 100);            // Initialize BLE keyboard

void setup() {
    Serial.begin(115200);
    Serial.println("Started");

    /* --DISPLAY SETTNGS-- */
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(SSD1306_WHITE);
    display.clearDisplay();
    display.setTextSize(2);
    draw_centered_text("loading", 0, 0, 128, 32);
    display.display();

    /* --BLE AND BUTTON SETTINGS-- */
    bleKeyboard.begin();
    btn.setTickMode(MANUAL);       // We will call tick() manually in loop
    btn.setClickTimeout(100);

    clkTrack.setType(HIGH_PULL); 
    clkTrack.setDirection(NORM_OPEN);
    clkTrack.setDebounce(20);

    /* --OTHER SETUP-- */
    delay(2000);
    Serial.print("Setup completed");
}


void loop() {
    unsigned long currentMillis = millis();
    boolean click = btn.isSingle();

    if (click) systemState.setClick(click);

    if (currentMillis - lastExecutedMillis >= EXE_INTERVAL) {
        lastExecutedMillis = currentMillis;
        systemState.setClick(click);
        Serial.print(".");
    }

    btn.tick();
    clkTrack.tick();
    systemState.setBluetooth(bleKeyboard.isConnected());

    if (clkTrack.isClick()) {
        int currentClkState = digitalRead(CLK_PIN);

        int step = (digitalRead(DT_PIN) != currentClkState) ? 1 : -1;
        systemState.changeState(step);

        Serial.printf("\nState changed to %s\n", ToString(systemState.getState()));
    }

    if (bleKeyboard.isConnected()) {
        if (click) BLE_send(systemState.getState());
    } else {
        if (click) Serial.println("No device is connected");
    }

    draw_screen();
} 