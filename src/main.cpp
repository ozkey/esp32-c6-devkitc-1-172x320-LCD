#include <Arduino.h>
#include <Arduino_GFX_Library.h>

// ============================================================
// Waveshare ESP32-C6-LCD-1.47
// ============================================================

#define LCD_MOSI  6
#define LCD_SCLK  7
#define LCD_CS    14
#define LCD_DC    15
#define LCD_RST   21
#define LCD_BL    22

// RGB565 colours
#define BLACK   0x0000
#define WHITE   0xFFFF
#define YELLOW  0xFFE0
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0

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

Arduino_GFX *gfx = new Arduino_ST7789(
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
// ANIMATION
// ============================================================

int ballX = 86;
int ballY = 100;

int velocityX = 3;
int velocityY = 3;

const int ballRadius = 10;

// ============================================================
// SETUP
// ============================================================

void setup() {

    Serial.begin(115200);

    // Turn on LCD backlight
    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);

    // Start LCD
    gfx->begin();

    gfx->fillScreen(BLACK);

    Serial.println("Animation started!");
}

// ============================================================
// LOOP
// ============================================================

void loop() {

    // --------------------------------------------------------
    // Erase previous ball
    // --------------------------------------------------------

    gfx->fillScreen(BLACK);

    // --------------------------------------------------------
    // Draw ball
    // --------------------------------------------------------

    gfx->fillCircle(
        ballX,
        ballY,
        ballRadius,
        YELLOW
    );

    // --------------------------------------------------------
    // Update position
    // --------------------------------------------------------

    ballX += velocityX;
    ballY += velocityY;

    // --------------------------------------------------------
    // Bounce off left/right
    // --------------------------------------------------------

    if (ballX - ballRadius <= 0 ||
        ballX + ballRadius >= 172) {

        velocityX = -velocityX;
    }

    // --------------------------------------------------------
    // Bounce off top/bottom
    // --------------------------------------------------------

    if (ballY - ballRadius <= 0 ||
        ballY + ballRadius >= 320) {

        velocityY = -velocityY;
    }

    // --------------------------------------------------------
    // Small delay controls animation speed
    // --------------------------------------------------------

    delay(20);
}