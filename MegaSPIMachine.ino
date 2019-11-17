// right now this machine is much simpler
// 4 * 23LC1024 SRAM SPI Memory chips
// 1 * arduino nano
// The address is 
#include <SPI.h>
#include <libbonuspin.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

constexpr auto CSPin0 = 2;
constexpr auto CSPin1 = 3;
constexpr auto CSPin2 = 4;
constexpr auto CSPin3 = 5;
constexpr auto EnablePin = 6;
constexpr auto OLED_RESET = 7;

Adafruit_SSD1306 display(OLED_RESET);

enum class SRAMOpcodes : uint8_t {
    RDSR = 0x05,
    RDMR = RDSR,
    WRSR = 0x01,
    WRMR = WRSR,
    READ = 0x03,
    WRITE = 0x02,
    EDIO = 0x3B,
    EQIO = 0x38,
    RSTIO = 0xFF,
};

template<int pin>
using CableSelectHolder = bonuspin::DigitalPinHolder<pin, LOW, HIGH>;
void sendOpcode(SRAMOpcodes opcode) {
    SPI.transfer(uint8_t(opcode));
}
void transferAddress(uint32_t address) {
    SPI.transfer(uint8_t(address >> 16));
    SPI.transfer(uint8_t(address >> 8));
    SPI.transfer(uint8_t(address));
}

void setDecoderPins(byte index) {
    auto masked = index & 0xF;
    digitalWrite(CSPin0, masked & 1 ? HIGH : LOW);
    digitalWrite(CSPin1, masked & 2 ? HIGH : LOW);
    digitalWrite(CSPin2, masked & 4 ? HIGH : LOW);
    digitalWrite(CSPin3, masked & 8 ? HIGH : LOW);
}

constexpr uint8_t computeChipIndex(uint32_t value) noexcept {
    return (0b1'1110'0000'0000'0000'0000 & value) >> 17;
}
template<int pin>
uint8_t readRam(uint32_t address) {
    setDecoderPins(computeChipIndex(address));
    CableSelectHolder<pin> holder;
    sendOpcode(SRAMOpcodes::READ);
    transferAddress(address);
    auto result = SPI.transfer(0x00);
    return result;
}
template<int pin>
void writeRam(uint32_t address, uint8_t value) {
    setDecoderPins(computeChipIndex(address));
    CableSelectHolder<pin> holder;
    sendOpcode(SRAMOpcodes::WRITE);
    transferAddress(address);
    SPI.transfer(value);
}
uint32_t addr = 0;
void setup() {
    addr = 0;
    display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
    pinMode(CSPin0, OUTPUT);
    pinMode(CSPin1, OUTPUT);
    pinMode(CSPin2, OUTPUT);
    pinMode(CSPin3, OUTPUT);
    pinMode(EnablePin, OUTPUT);
    digitalWrite(CSPin0, LOW);
    digitalWrite(CSPin1, LOW);
    digitalWrite(CSPin2, LOW);
    digitalWrite(CSPin3, LOW);
    digitalWrite(EnablePin, HIGH);
    SPI.begin();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Donuts");
    display.display();
    delay(1000);

}
void loop() {
    if (addr < 0x18'0000) {
        auto i8Val = static_cast<uint8_t>(addr);
        //auto i8Val = static_cast<uint8_t>(0xFD);
        writeRam<EnablePin>(addr, i8Val);
        if (auto result = readRam<EnablePin>(addr); result != i8Val) {
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(0,0);
            display.print("cci: ");
            display.println(computeChipIndex(addr), HEX);
            display.print("addr: ");
            display.println(addr, HEX);
            display.print("expect: ");
            display.println(i8Val, HEX);
            display.print("got: ");
            display.println(result, HEX);
            display.display();
            delay(5);
        } else {
            delay(1);
        }
        ++addr;
    } else {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0,0);
        display.println("DONE!");
        display.display();
        delay(100);
    }
}
