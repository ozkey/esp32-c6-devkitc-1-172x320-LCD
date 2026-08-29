#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <math.h>
#include "clock.h"

// ============================================================
// WAVESHARE ESP32-C6-LCD-1.47
// ============================================================

#define LCD_MOSI  6
#define LCD_SCLK  7
#define LCD_CS    14
#define LCD_DC    15
#define LCD_RST   21
#define LCD_BL    22

// ============================================================
// COLOURS - RGB565
// ============================================================

#define BLACK       0x0000
#define WHITE       0xFFFF
#define CREAM       0xFFDF
#define DARK_BROWN  0x49A5
#define GOLD        0xD604
#define GOLD_LIGHT  0xFEA0
#define RED         0xF800

// ============================================================
// LCD
// ============================================================

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    LCD_DC,
    LCD_CS,
    LCD_SCLK,
    LCD_MOSI,
    GFX_NOT_DEFINED
);

Arduino_GFX *display = new Arduino_ST7789(
    bus,
    LCD_RST,
    0,
    true,
    172,
    320,
    34,
    0,
    0,
    0
);

// ============================================================
// FRAMEBUFFER
// ============================================================

// 172 x 320 x 2 bytes ≈ 110 KB RAM
Arduino_Canvas *gfx = new Arduino_Canvas(
    172,
    320,
    display
);

// ============================================================
// CLOCK SETTINGS
// ============================================================

const int SCREEN_W = 172;
const int SCREEN_H = 320;

// Clock face
const int CLOCK_X = 86;
const int CLOCK_Y = 95;
const int CLOCK_R = 72;

// Pendulum
const int PENDULUM_X = 86;
const int PENDULUM_TOP = 172;
const int PENDULUM_LENGTH = 105;

// ============================================================
// DRAW CLOCK FACE
// ============================================================

void drawClockFace() {

    // Background
    gfx->fillScreen(DARK_BROWN);

    // Outer frame
    gfx->drawRect(3, 3, SCREEN_W - 6, SCREEN_H - 6, GOLD);
    gfx->drawRect(6, 6, SCREEN_W - 12, SCREEN_H - 12, GOLD);

    // Clock outer rings
    gfx->fillCircle(CLOCK_X, CLOCK_Y, CLOCK_R + 4, GOLD);
    gfx->fillCircle(CLOCK_X, CLOCK_Y, CLOCK_R, CREAM);

    // Inner ring
    gfx->drawCircle(CLOCK_X, CLOCK_Y, CLOCK_R - 3, DARK_BROWN);
    gfx->drawCircle(CLOCK_X, CLOCK_Y, CLOCK_R - 5, GOLD);

    // ========================================================
    // MINUTE MARKINGS
    // ========================================================

    for (int minute = 0; minute < 60; minute++) {

        float angle =
            (minute * 6.0 - 90.0) * DEG_TO_RAD;

        int outerX =
            CLOCK_X + cos(angle) * (CLOCK_R - 7);

        int outerY =
            CLOCK_Y + sin(angle) * (CLOCK_R - 7);

        int innerLength;

        if (minute % 5 == 0)
            innerLength = 9;
        else
            innerLength = 4;

        int innerX =
            CLOCK_X + cos(angle) * (CLOCK_R - 7 - innerLength);

        int innerY =
            CLOCK_Y + sin(angle) * (CLOCK_R - 7 - innerLength);

        gfx->drawLine(
            innerX,
            innerY,
            outerX,
            outerY,
            DARK_BROWN
        );
    }

    // ========================================================
    // NUMBERS
    // ========================================================

    gfx->setTextColor(BLACK);
    gfx->setTextSize(1);

    const char *numbers[] = {
        "12", "1", "2", "3",
        "4", "5", "6", "7",
        "8", "9", "10", "11"
    };

    for (int i = 0; i < 12; i++) {

        float angle =
            (i * 30.0 - 90.0) * DEG_TO_RAD;

        int radius = 55;

        int x =
            CLOCK_X + cos(angle) * radius;

        int y =
            CLOCK_Y + sin(angle) * radius;

        // Approximate centering
        if (i == 0 || i == 6) {
            x -= 3;
        }
        else if (i == 3 || i == 9) {
            x -= 5;
        }
        else {
            x -= 3;
        }

        y -= 3;

        gfx->setCursor(x, y);
        gfx->print(numbers[i]);
    }
}

// ============================================================
// DRAW HAND
// ============================================================

void drawHand(
    float angle,
    int length,
    int width,
    uint16_t colour
) {

    float radians =
        (angle - 90.0) * DEG_TO_RAD;

    int x =
        CLOCK_X + cos(radians) * length;

    int y =
        CLOCK_Y + sin(radians) * length;

    // Main hand
    gfx->drawLine(
        CLOCK_X,
        CLOCK_Y,
        x,
        y,
        colour
    );

    // Extra lines give thicker hands
    for (int i = 1; i < width; i++) {

        gfx->drawLine(
            CLOCK_X + i,
            CLOCK_Y,
            x + i,
            y,
            colour
        );

        gfx->drawLine(
            CLOCK_X - i,
            CLOCK_Y,
            x - i,
            y,
            colour
        );
    }
}

// ============================================================
// DRAW CLOCK HANDS
// ============================================================

void drawHands() {

    // --------------------------------------------------------
    // Get current time
    // --------------------------------------------------------

    unsigned long totalSeconds =
        millis() / 1000;

    float seconds =
        totalSeconds % 60;

    float minutes =
        (totalSeconds / 60) % 60;

    float hours =
        (totalSeconds / 3600) % 12;

    // --------------------------------------------------------
    // Calculate angles
    // --------------------------------------------------------

    float secondAngle =
        seconds * 6.0;

    float minuteAngle =
        minutes * 6.0 +
        seconds * 0.1;

    float hourAngle =
        hours * 30.0 +
        minutes * 0.5;

    // --------------------------------------------------------
    // Hour hand
    // --------------------------------------------------------

    drawHand(
        hourAngle,
        38,
        2,
        BLACK
    );

    // --------------------------------------------------------
    // Minute hand
    // --------------------------------------------------------

    drawHand(
        minuteAngle,
        52,
        1,
        BLACK
    );

    // --------------------------------------------------------
    // Second hand
    // --------------------------------------------------------

    drawHand(
        secondAngle,
        58,
        0,
        RED
    );

    // Centre
    gfx->fillCircle(
        CLOCK_X,
        CLOCK_Y,
        5,
        BLACK
    );

    gfx->fillCircle(
        CLOCK_X,
        CLOCK_Y,
        2,
        GOLD_LIGHT
    );
}

// ============================================================
// DRAW PENDULUM
// ============================================================

void drawPendulum() {

    // Time for pendulum
    float t = millis() / 1000.0;

    // Pendulum swings left/right
    float swing =
        sin(t * 2.0) * 18.0;

    // Pendulum bob position

    int bobX =
        PENDULUM_X + swing;

    int bobY =
        PENDULUM_TOP + PENDULUM_LENGTH;

    // Rod
    gfx->drawLine(
        PENDULUM_X,
        PENDULUM_TOP,
        bobX,
        bobY,
        GOLD_LIGHT
    );

    // Small rod highlight
    gfx->drawLine(
        PENDULUM_X + 1,
        PENDULUM_TOP,
        bobX + 1,
        bobY,
        GOLD
    );

    // Pendulum bob
    gfx->fillCircle(
        bobX,
        bobY,
        18,
        GOLD
    );

    gfx->fillCircle(
        bobX,
        bobY,
        14,
        GOLD_LIGHT
    );

    // Bob centre
    gfx->fillCircle(
        bobX,
        bobY,
        3,
        DARK_BROWN
    );
}

// ============================================================
// SETUP
// ============================================================

void clock_setup() {

    Serial.begin(115200);

    // Backlight
    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);

    // Start display
    display->begin();

    // Start canvas
    gfx->begin();

    // Draw first frame
    drawClockFace();
    drawHands();
    drawPendulum();

    gfx->flush();

    Serial.println("Pendulum clock started!");
}

// ============================================================
// LOOP
// ============================================================

void clock_loop() {

    // Draw entire frame into RAM
    drawClockFace();

    drawHands();

    drawPendulum();

    // Send completed frame to LCD
    gfx->flush();

    // Around 30 FPS
    delay(33);
}