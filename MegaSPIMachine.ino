#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
// SPI BUS Layout
// [0,14] -> 23LC1024 * 15
// 15 -> I/O0 SPI PREFIX: {000}
// 16 -> I/O1 SPI PREFIX: {001}
// [17,31] -> unused

// pro micro pin assignments
// spi enable lines, line 5 is for enable or disable
// these three lines are fed to A,B,C to on each HC138 (there are four)
constexpr auto SPIAddressLine0 = 4;
constexpr auto SPIAddressLine1 = 5;
constexpr auto SPIAddressLine2 = 6;
// these two pins control which HC138 to dispatch on
constexpr auto SPIAddressLine3 = 7;
constexpr auto SPIAddressLine4 = 8;
constexpr auto SPIAddressLine5 = 9; // set this to high to disable the bus

constexpr auto TFTCS = 10;
constexpr auto TFTReset = 18;
constexpr auto TFTDC = 19;

// the HC138 enabler codes
constexpr auto EnableLine_23LC1024_Start = 0;
constexpr auto EnableLine_23LC1024_End = 14;

constexpr auto EnableLine_IOExpander0 = 15;
constexpr auto IOExpander0_Code = 0b000;
constexpr auto EnableLine_IOExpander1 = 16;
constexpr auto IOExpander1_Code = 0b001;

// There is an implied 6 bit address on top of the 18-bit address from a 
// 23LC1024 up the 18-bits from the 23lc1024 devices

Adafruit_ILI9341 tft(TFTCS, TFTDC, TFTReset);

void setup() {
    Serial.begin(9600);
    SPI.begin();
    pinMode(SPIAddressLine0, OUTPUT);
    digitalWrite(SPIAddressLine0, HIGH);
    pinMode(SPIAddressLine1, OUTPUT);
    digitalWrite(SPIAddressLine1, HIGH);
    pinMode(SPIAddressLine2, OUTPUT);
    digitalWrite(SPIAddressLine2, HIGH);
    pinMode(SPIAddressLine3, OUTPUT);
    digitalWrite(SPIAddressLine3, HIGH);
    pinMode(SPIAddressLine4, OUTPUT);
    digitalWrite(SPIAddressLine4, HIGH);
    pinMode(SPIAddressLine5, OUTPUT);
    digitalWrite(SPIAddressLine5, HIGH);
    tft.begin();
    tft.fillScreen(ILI9341_BLACK);
}


void loop() {

}
