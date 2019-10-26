// SPI BUS Layout
// [0,14] -> 23LC1024 * 15
// 15 -> TFT 
// 16 -> SDCARD
// 17 -> I/O0 SPI PREFIX: {000}
// 18 -> I/O1 SPI PREFIX: {001}
// [19,31] -> unused

// pro micro pin assignments
constexpr auto TFTDC = 3;
// spi enable lines, line 5 is for enable or disable
// these three lines are fed to A,B,C to on each HC138 (there are four)
constexpr auto SPIAddressLine0 = 4;
constexpr auto SPIAddressLine1 = 5;
constexpr auto SPIAddressLine2 = 6;
// these two pins control which HC138 to dispatch on
constexpr auto SPIAddressLine3 = 7;
constexpr auto SPIAddressLine4 = 8;
constexpr auto SPIAddressLine5 = 9; // set this to high to disable the bus

constexpr auto TFTReset = 10;

void setup() {
    Serial.begin(9600);
    SPI.begin();
    pinMode(SPIAddressLine0, OUTPUT);
    pinMode(SPIAddressLine1, OUTPUT);
    pinMode(SPIAddressLine2, OUTPUT);
    pinMode(SPIAddressLine3, OUTPUT);
    pinMode(SPIAddressLine4, OUTPUT);
    pinMode(SPIAddressLine5, OUTPUT);
    pinMode(TFTReset, OUTPUT);
    pinMode(TFTDC, OUTPUT); 
}


void loop() {

}
