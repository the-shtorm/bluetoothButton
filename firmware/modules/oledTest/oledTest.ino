#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <bluetooth_logo.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
}

int bluXPos = 108;
int bluYPos = 14;
const char* words[] = {"FLASH", "KING", "SANJI", "RIGHT"};

void loop() {
    for (int i = 0; i < 5; i++) {
        display.clearDisplay();
        drawBatteryIcon(i);
        if (i%2 == 0) display.drawBitmap(bluXPos, bluYPos, bluetooth_disconnected, 18, 18, SSD1306_WHITE);
        else display.drawBitmap(bluXPos, bluYPos, bluetooth_connected, 18, 18, SSD1306_WHITE);
        drawCenteredText(words[i%4], 0, 0, 102, 32);
        display.display();
        delay(1000);
    }
}

void drawBatteryIcon(int segments) {
    int xPos = 107;
    int yPos = 0;

    // Draw the outer battery rectangle frame (x, y, width, height, color)
    display.drawRect(xPos, yPos, 19, 10, WHITE);
    display.fillRect(xPos + 19, yPos + 3, 2, 4, WHITE); // Battery positive tip

    // Fill in the bars based on your segment calculations
    for (int i = 0; i < segments; i++) {
        // Draws 1 to 4 small vertical bars inside the frame
        display.fillRect(xPos + 2 + (i * 4), yPos + 2, 3, 6, WHITE);
    }
}

void drawCenteredText(const char* text, int boxX, int boxY, int boxW, int boxH) {
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

  // Optional: Draw the border rectangle (0 to 101 X, 0 to 31 Y)
  display.drawRect(boxX, boxY, boxW, boxH, SSD1306_WHITE);

  // Set cursor and print the word
  display.setCursor(xCursor, yCursor);
  display.print(text);
}

 