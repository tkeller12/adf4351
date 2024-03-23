#ifndef _ADF4351_h
#define _ADF4351_h

#include <Arduino.h>
#include <SPI.h>

union REGISTER0 {
    struct {
        uint8_t CTRL : 3;       // 00000000 00000000 00000000 00000???
        uint16_t FRAC : 12;     // 00000000 00000000 0??????? ?????000
        uint16_t INT : 16;      // 0??????? ???????? ?0000000 00000000
        uint8_t RES0 : 1;       // ?0000000 00000000 00000000 00000000
    } bits;
    struct{
        uint8_t BYTE0;
        uint8_t BYTE1;
        uint8_t BYTE2;
        uint8_t BYTE3;
    } bytes;
    uint32_t word;
};

union REGISTER1 {
    struct {
        uint8_t CTRL : 3;        // 00000000 00000000 00000000 00000???
        uint16_t MOD : 12;       // 00000000 00000000 0??????? ?????000
        uint16_t PHASE : 12;     // 00000??? ???????? ?0000000 00000000
        uint8_t PRESCALER :1;    // 0000?000 00000000 00000000 00000000
        uint8_t PHASE_ADJUST:1;  // 000?0000 00000000 00000000 00000000
        uint8_t RES0 : 3;        // ???00000 00000000 00000000 00000000
    } bits;
    struct{
        uint8_t BYTE0;
        uint8_t BYTE1;
        uint8_t BYTE2;
        uint8_t BYTE3;
    } bytes;
    uint32_t word;
};


union REGISTER2 {
    struct {
        uint8_t CTRL : 3;              // 00000000 00000000 00000000 00000???
        uint8_t COUNTER_RESET:1;       // 00000000 00000000 00000000 0000?000
        uint8_t CP_THREE_STATE:1;      // 00000000 00000000 00000000 000?0000
        uint8_t POWER_DOWN:1;          // 00000000 00000000 00000000 00?00000
        uint8_t PD_POLARITY:1;         // 00000000 00000000 00000000 0?000000
        uint8_t LDP:1;                 // 00000000 00000000 00000000 ?0000000
        uint8_t LDF:1;                 // 00000000 00000000 0000000? 00000000
        uint8_t CHARGE_PUMP_CURRENT:4; // 00000000 00000000 000????0 00000000
        uint8_t DOUBLE_BUFFER:1;       // 00000000 00000000 00?00000 00000000
        uint16_t R_COUNTER:10;         // 00000000 ???????? ??000000 00000000
        uint8_t RDIV2:1;               // 0000000? 00000000 00000000 00000000
        uint8_t REFERENCE_DOUBLER:1;   // 000000?0 00000000 00000000 00000000
        uint8_t MUXOUT:3;              // 000???00 00000000 00000000 00000000
        uint8_t LOW_NOISE_SPUR_MODE:2; // 0??00000 00000000 00000000 00000000
        uint8_t RES0:1;                // ?0000000 00000000 00000000 00000000
    } bits;
    struct{
        uint8_t BYTE0;
        uint8_t BYTE1;
        uint8_t BYTE2;
        uint8_t BYTE3;
    } bytes;
    uint32_t word;
};

union REGISTER3 {
    struct{
        uint8_t CTRL:3;                  // 00000000 00000000 00000000 00000???
        uint16_t CLOCK_DIVIDER:12;       // 00000000 00000000 0??????? ?????000
        uint8_t CLK_DIV_MODE:2;          // 00000000 0000000? ?0000000 00000000
        uint8_t RES0:1;                  // 00000000 000000?0 00000000 00000000
        uint8_t CSR:1;                   // 00000000 00000?00 00000000 00000000
        uint8_t RES1:2;                  // 00000000 000??000 00000000 00000000
        uint8_t CHARGE_CANCEL:1;         // 00000000 00?00000 00000000 00000000
        uint8_t ABP:1;                   // 00000000 0?000000 00000000 00000000
        uint8_t BAND_SELECT_CLOCK_MODE:1;// 00000000 ?0000000 00000000 00000000
        uint8_t RES2;                    // ???????? 00000000 00000000 00000000
    } bits;
    struct{
        uint8_t BYTE0;
        uint8_t BYTE1;
        uint8_t BYTE2;
        uint8_t BYTE3;
    } bytes;
    uint32_t word;
};

union REGISTER4 {
    struct{
        uint8_t CTRL:3;                       // 00000000 00000000 00000000 00000???
        uint8_t OUTPUT_POWER:2;               // 00000000 00000000 00000000 000??000
        uint8_t RF_OUTPUT_ENABLE:1;           // 00000000 00000000 00000000 00?00000
        uint8_t AUX_OUTPUT_POWER:2;           // 00000000 00000000 00000000 ??000000
        uint8_t AUX_OUTPUT_ENABLE:1;          // 00000000 00000000 0000000? 00000000
        uint8_t AUX_OUTPUT_SELECT:1;          // 00000000 00000000 000000?0 00000000
        uint8_t MTLD:1;                       // 00000000 00000000 00000?00 00000000
        uint8_t VCO_POWER_DOWN:1;             // 00000000 00000000 0000?000 00000000
        uint8_t BAND_SELECT_CLOCK_DIVIDER:8;  // 00000000 0000???? ????0000 00000000
        uint8_t RF_DIVIDER_SELECT:3;          // 00000000 0???0000 00000000 00000000
        uint8_t FEEDBACK_SELECT:1;            // 00000000 ?0000000 00000000 00000000
        uint8_t RES0:8;                       // ???????? 00000000 00000000 00000000
    } bits;
    struct{
        uint8_t BYTE0;
        uint8_t BYTE1;
        uint8_t BYTE2;
        uint8_t BYTE3;
    } bytes;
    uint32_t word;
};

union REGISTER5 {
    struct {
        uint8_t CTRL:3;                    // 00000000 00000000 00000000 00000???
        uint16_t RES0:16;                  // 00000000 00000??? ???????? ?????000
        uint8_t RES1:2;                    // 00000000 000??000 00000000 00000000
        uint8_t RES2:1;                    // 00000000 00?00000 00000000 00000000
        uint8_t LD_PIN_MODE:2;             // 00000000 ??000000 00000000 00000000
        uint8_t RES3;                      // ???????? 00000000 00000000 00000000
    } bits;
    struct{
        uint8_t BYTE0;
        uint8_t BYTE1;
        uint8_t BYTE2;
        uint8_t BYTE3;
    } bytes;
    uint32_t word;
};

class ADF4351 {
    public:
        void begin();
        void writeRegister(uint32_t word);
        void initRegisters();
        void writeAllRegisters();
        void setFrequency(uint32_t freq);
        void setPower(uint8_t power, bool channel = 0);
        void rfEnable(bool rf_enable, bool channel = 0);
        ADF4351(uint8_t LE); // Constructor

        REGISTER0 REG0;
        REGISTER1 REG1;
        REGISTER2 REG2;
        REGISTER3 REG3;
        REGISTER4 REG4;
        REGISTER5 REG5;

        uint8_t CS;
        const uint32_t SCLK       = 2000000;///< SPI SCLK frequency
        const uint32_t F_REF = 25000; // Reference frequency in kHz
};


#endif