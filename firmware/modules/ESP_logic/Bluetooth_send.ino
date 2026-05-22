extern BleKeyboard bleKeyboard;

void BLE_send(ProjectState state) {
    if (bleKeyboard.isConnected()) {
        switch(state) {
            case STATE_FLASH: 
                Serial.println("\nSending F1...");
                bleKeyboard.write(KEY_F1);
                break;
            case STATE_KING:
                Serial.println("\nSending F2...");
                bleKeyboard.write(KEY_F2);
                break;
            case STATE_SANJI:
                Serial.println("\nSending Shift+F10...");
                bleKeyboard.press(KEY_LEFT_SHIFT);
                bleKeyboard.press(KEY_F10);
                delay(100);
                bleKeyboard.releaseAll();
                break;
        }
    }
}