
#include "adf4351.h"
#include "Arduino.h"

ADF4351::ADF4351(uint8_t LE) {
    CS = LE;
}

void ADF4351::begin() {
    pinMode(CS, OUTPUT);
    digitalWrite(CS, HIGH);
    initRegisters();
    SPI.begin();
    SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE0));
}

void ADF4351::initRegisters() {
    // REGISTER 0
    REG0.bits.CTRL = 0; // Control bits
    REG0.bits.INT = 83;  // 16-bits
    REG0.bits.FRAC = 0; // 12-bits

    // REGISTER 1
    REG1.bits.CTRL = 1; // Control bits
    REG1.bits.MOD = 2500; // MOD determines frequency resolution, RESOLUTION = F_PFD / MOD
    REG1.bits.PHASE = 1; // 1 is recommended
    REG1.bits.PRESCALER = 1; // 0 is 4/5, 1 is 8/9
    REG1.bits.PHASE_ADJUST = 0; // 0 is phase adjust off

    // REGISTER 2
    REG2.bits.CTRL = 2; // Control bits
    REG2.bits.COUNTER_RESET = 0; // 0 for normal operation
    REG2.bits.CP_THREE_STATE = 0; // 0 for normal operation
    REG2.bits.POWER_DOWN = 0; // 0 for normal operation
    REG2.bits.PD_POLARITY = 1; // 1 for passive loop or non-inverting loop filter
    REG2.bits.LDP = 0; // 0 is 10 ns window
    REG2.bits.LDF = 0; // 0 for FRAC-N, 1 for INT-N
    REG2.bits.CHARGE_PUMP_CURRENT = 0; // 0 is slowest
    REG2.bits.DOUBLE_BUFFER = 0; //
    REG2.bits.R_COUNTER = 1; // Divider for reference
    REG2.bits.RDIV2 = 0; // Reference divide by 2
    REG2.bits.REFERENCE_DOUBLER = 0; // Reference doubler
    REG2.bits.MUXOUT = 6; // 6 is digital lock detect
    REG2.bits.LOW_NOISE_SPUR_MODE = 0; // 0 is low noise mode

    // REGISTER 3
    REG3.bits.CTRL = 3; // Control bits
    REG3.bits.CLOCK_DIVIDER = 0; // Clock divider value for resync
    REG3.bits.CLK_DIV_MODE = 0; // 0 disables clock divider
    REG3.bits.CSR = 0; // 0 disables slip cycle reduction
    REG3.bits.CHARGE_CANCEL = 0; // 0 disables charge cancellation, should be 0 in N-frac mode 
    REG3.bits.ABP = 0; // Anti-Backlash Pulse, 0 for FRAC-N, 1 for INT-N
    REG3.bits.BAND_SELECT_CLOCK_MODE = 1; // recommended for high PDF

    // REGISTER 4
    REG4.bits.CTRL = 4; // Control bits
    REG4.bits.OUTPUT_POWER = 3; // 0 is -4 dBm, 3 is 5 dBm
    REG4.bits.RF_OUTPUT_ENABLE = 1; // 1 is enabled
    REG4.bits.AUX_OUTPUT_POWER = 0; // 0 is -4 dBm
    REG4.bits.AUX_OUTPUT_ENABLE = 0; // 0 is disabled
    REG4.bits.AUX_OUTPUT_SELECT = 0; // 0 is divided output
    REG4.bits.MTLD = 0; // 1 is MUTE until lock enabled
    REG4.bits.VCO_POWER_DOWN = 0; // 0 is VCO powered up
    REG4.bits.BAND_SELECT_CLOCK_DIVIDER = 25; // 
    REG4.bits.RF_DIVIDER_SELECT = 4; // 4 is divide by 16
    REG4.bits.FEEDBACK_SELECT = 1; // 1 is fundamental, 0 is divided

    // REGISTER 5
    REG5.bits.CTRL = 5; // Control bits
    REG5.bits.LD_PIN_MODE = 1; // 0 is Lock detect low, 1 for digital lock detect
}

void ADF4351::writeRegister(uint32_t word) {
    digitalWrite(CS, LOW);
    SPI.transfer16((word >> 16) & 0xffff); // Write Two MSB
    SPI.transfer16(word & 0xffff); // Write Two LSB
    digitalWrite(CS, HIGH);
}

void ADF4351::writeAllRegisters() {
    writeRegister(REG5.word);
    writeRegister(REG4.word);
    writeRegister(REG3.word);
    writeRegister(REG2.word);
    writeRegister(REG1.word);
    writeRegister(REG0.word);
}

void ADF4351::setFrequency(uint32_t freq) {
    uint8_t divisor = 1;  // divisor in multiples of 2 minum 1
    uint8_t divisor_count = 0;  // divisor in multiples of 2 minum 1
    uint32_t temp_freq = 0; // Used for divisor calculation
    uint32_t VCO_CUTOFF = 2200000; // VCO frequency kHz
    uint32_t VCO_FREQ;
    uint32_t FRAC;
    uint32_t INT;

    temp_freq = VCO_CUTOFF / freq;
    while (temp_freq > 0) {
        temp_freq /= 2;
        divisor_count += 1;
        divisor *= 2;
    }

    VCO_FREQ = freq * divisor;

    INT = VCO_FREQ / F_REF;
    FRAC = (VCO_FREQ - (INT * F_REF)) * REG1.bits.MOD / F_REF ;

    REG4.bits.RF_DIVIDER_SELECT = divisor_count;
    REG0.bits.INT = INT;
    REG0.bits.FRAC = FRAC;

    // writeAllRegisters();
    // We Should only need to write REG4 and REG0
    writeRegister(REG4.word);
    writeRegister(REG0.word);
}

void ADF4351::setPower(uint8_t power, bool channel = 0) {
    // 0 -> -4dBm
    // 1 -> -1dBm
    // 2 -> +2dBm
    // 3 -> +5dBm

    if (power > 3) // Check if value is out of range
        {
        return; // Just return if value out of range
        }

    if (channel == 0) {
        REG4.bits.OUTPUT_POWER = power; // 0 is -4 dBm, 3 is 5 dBm
    }
    else {
        REG4.bits.AUX_OUTPUT_POWER = power; // 0 is -4 dBm, 3 is 5 dBm
    }
    
    writeRegister(REG4.word); // Should only need to write REG4

}

void ADF4351::rfEnable(bool rf_enable, bool channel = 0) {
    // channel = 0 -> Output A
    if (channel == 0) {
        REG4.bits.RF_OUTPUT_ENABLE = rf_enable;
        }
    else {
        // channel = 1 -> Output B
        REG4.bits.AUX_OUTPUT_ENABLE = rf_enable;
        }
    writeRegister(REG4.word);
}