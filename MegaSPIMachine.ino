// right now this machine is much simpler
// 4 * 23LC1024 SRAM SPI Memory chips
// 1 * arduino nano
// The address is 
#include <SPI.h>
#include <libbonuspin.h>

constexpr auto EnablePin0 = 2;
constexpr auto EnablePin1 = 3;
constexpr auto EnablePin2 = 4;
constexpr auto ActivatorPin = 5;

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

bonuspin::HC138<2, 3, 4> decoder;
constexpr uint8_t computeChipIndex(uint32_t value) noexcept {
    return (0b1110'0000'0000'0000'0000 & value) >> 17;
}
template<int pin>
uint8_t readRam(uint32_t address) {
    decoder.enableLine(computeChipIndex(address));
    CableSelectHolder<pin> holder;
    sendOpcode(SRAMOpcodes::READ);
    transferAddress(address);
    auto result = SPI.transfer(0x00);
    return result;
}
template<int pin>
void writeRam(uint32_t address, uint8_t value) {
    decoder.enableLine(computeChipIndex(address));
    CableSelectHolder<pin> holder;
    sendOpcode(SRAMOpcodes::WRITE);
    transferAddress(address);
    SPI.transfer(value);
}
int32_t chipIndex = -1;
constexpr auto NumberOfChips = 5;
void setup() {
    Serial.begin(115200);
    pinMode(ActivatorPin, OUTPUT);
    pinMode(ActivatorPin, HIGH);
    SPI.begin();
    // only have 5 devices connected right now so D
    for (uint32_t i = 0; i < 0x10'0000; ++i) {
        if (auto newChipIndex = computeChipIndex(i); chipIndex != newChipIndex) {
            if (newChipIndex >= NumberOfChips) {
                break;
            }
            Serial.print("Testing chip: ");
            Serial.println(newChipIndex, HEX);
            chipIndex = newChipIndex;
        }
        auto i8Val = static_cast<uint8_t>(0xFD);
        writeRam<ActivatorPin>(i, i8Val);
        if (auto result = readRam<ActivatorPin>(i); result != i8Val) {
            Serial.print("Readback mismatch, Address: 0x");
            Serial.print(i, HEX);
            Serial.print(", Expected: 0x");
            Serial.print(i8Val, HEX);
            Serial.print(", Got: 0x");
            Serial.println(result, HEX);
        } 
    }
    Serial.println("Done");
}


void loop() {

}
