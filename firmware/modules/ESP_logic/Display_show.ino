extern Adafruit_SSD1306 display; 
extern SystemState systemState;

void draw_screen() {
    display.clearDisplay();
    draw_bluetoooth(systemState.getBluetooth());
    draw_frame(0, 0, 102, 32, systemState.getClick());
    draw_centered_text(ToString(systemState.getState()), 0, 0, 102, 32);
    drawBatteryIcon(systemState.getBatteryLevel());


    display.display();
}

void draw_bluetoooth(boolean connected) {
    int bluXPos = 108;
    int bluYPos = 14;
    int imgSize = 18;
    if (connected) {
        display.drawBitmap(bluXPos, bluYPos, bluetooth_connected, imgSize, imgSize, SSD1306_WHITE);
    } else {
        display.drawBitmap(bluXPos, bluYPos, bluetooth_disconnected, imgSize, imgSize, SSD1306_WHITE);
    }
}

void draw_frame(int boxX, int boxY, int boxW, int boxH, boolean isClick) {
    if (!isClick) display.drawRect(boxX, boxY, boxW, boxH, SSD1306_WHITE);
}

void draw_centered_text(const char* text, int boxX, int boxY, int boxW, int boxH) {
  int16_t x1, y1;
  uint16_t w, h;

  // Set the text size before measuring
  display.setTextSize(2); // Size 2 fits beautifully inside a 32px high screen
  display.setTextColor(SSD1306_WHITE);

  // Measure the exact pixel width and height of the text string
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  // Calculate coordinates to center the text inside the bounding box area
  int xCursor = boxX + (boxW - w) / 2 - x1;
  int yCursor = boxY + (boxH - h) / 2 - y1;

  // Set cursor and print the word
  display.setCursor(xCursor, yCursor);
  display.print(text);
}

void drawBatteryIcon(int segments) {
    int xPos = 107;
    int yPos = 0;

    if (segments > 4) segments = 4;

    // Draw the outer battery rectangle frame (x, y, width, height, color)
    display.drawRect(xPos, yPos, 19, 10, WHITE);
    display.fillRect(xPos + 19, yPos + 3, 2, 4, WHITE); // Battery positive tip

    // Fill in the bars based on your segment calculations
    for (int i = 0; i < segments; i++) {
        // Draws 1 to 4 small vertical bars inside the frame
        display.fillRect(xPos + 2 + (i * 4), yPos + 2, 3, 6, WHITE);
    }
}