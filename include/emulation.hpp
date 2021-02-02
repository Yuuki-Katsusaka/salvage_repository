#ifndef EMULATION_HPP
#define EMULATION_HPP

#include "define.hpp"
#include "devices.hpp"

// エミュレーションモード
enum emulate_mode {
    EMU_OP,   // 動作点解析
    EMU_DC,   // 直流解析
    EMU_TRAN  // 過渡解析
};

// エミュレーションに関する情報
struct emulate_info {
    emulate_mode mode;
    floating tran_step;
    floating tran_stop;

    void printInfo();
};

void startEmulation(const devices &, const emulate_info &);

#endif