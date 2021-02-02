#ifndef DEVICES_HPP
#define DEVICES_HPP

#include <iostream>

#include "define.hpp"
// #include "parser.hpp"

// // 回路素子のID番号
// enum elem_id {
//     ID_ISRC,  // 電流源
//     ID_VSRC,  // 電圧源
//     ID_RES,  // 抵抗
//     ID_CAP,  // コンデンサ
//     ID_IND,  // インダクタ
//     ID_DIO,  // ダイオード
//     ID_BJT,  // バイポーラトランジスタ
//     ID_MOS,  // MOSFETトランジスタ
//     ID_MAX   // elem_idの要素数
// };

// 電源の波形タイプ
enum waveform_type {
    WAVE_DC,    // 直流
    WAVE_SINE,  // 正弦波
    WAVE_PULSE  // パルス波
};

// 素子モデルの種類
enum model_type {
    MODEL_DIO,   // ショックレーダイオード
    MODEL_NPN,   // NPN型BJT
    MODEL_PNP,   // PNP型BJT
    MODEL_NMOS,  // NチャネルMOSFET
    MODEL_PMOS   // PチャネルMOSFET
};

// 各回路素子のモデル ///////////////////////////////////////

// 電流源
struct isrc_model {
    unsigned node_pos;       // 正極のノード番号
    unsigned node_neg;       // 負極のノード番号
    waveform_type src_type;  // 波形の種類
    floating dc_value;       // (DC)電流値
    floating sine_offset;    // (SINE)DCオフセット
    floating sine_amp;       // (SINE)振幅
    floating sine_freq;      // (SINE)周波数

    void printIsrc();  // 素子モデルの出力
};

// 電圧源
struct vsrc_model {
    unsigned node_pos;       // 正極のノード番号
    unsigned node_neg;       // 負極のノード番号
    waveform_type src_type;  // 波形の種類
    floating dc_value;       // (DC)電流値
    floating sine_offset;    // (SINE)DCオフセット
    floating sine_amp;       // (SINE)振幅
    floating sine_freq;      // (SINE)周波数

    void printVsrc();  // 素子モデルの出力
};

// 抵抗
struct res_model {
    unsigned node_pos;  // 正極のノード番号
    unsigned node_neg;  // 負極のノード番号
    floating conduct;   // コンダクタンス

    void printRes();  // 素子モデルの出力
};

// コンデンサ
struct cap_model {
    unsigned node_pos;  // 正極のノード番号
    unsigned node_neg;  // 負極のノード番号
    floating capac;     // キャパシタンス

    void printCap();  // 素子モデルの出力
};

// インダクタ
struct ind_model {
    unsigned node_pos;  // 正極のノード番号
    unsigned node_neg;  // 負極のノード番号
    floating induct;    // インダクタンス

    void printInd();  // 素子モデルの出力
};

// ダイオード
struct dio_model {
    unsigned node_pos;      // 正極のノード番号
    unsigned node_neg;      // 負極のノード番号
    model_type type;        // 素子モデルの種類
    floating is   = 1e-14;  // 飽和電流
    floating rs   = 0.f;    // オーミック抵抗
    floating n    = 1.f;    // 発光係数
    floating tnon = 27.f;   // パラメータ測定温度

    void printDio();  // 素子モデルの出力
};

// バイポーラトランジスタ
struct bjt_model {
    unsigned node_col;      // コレクタ側のノード番号
    unsigned node_base;     // ベース側のノード番号
    unsigned node_emit;     // エミッタ側のノード番号
    model_type type;        // 素子モデルの種類
    floating is   = 1e-16;  // 輸送飽和電流
    floating bf   = 100.f;  // 理想的な順方向電流増幅率の最大値
    floating br   = 1.f;    // 理想的な逆方向電流増幅率の最大値
    floating rc   = 0.f;    // コレクタ抵抗
    floating re   = 0.f;    // エミッタ抵抗
    floating tnon = 27.f;   // パラメータ測定温度

    void printBjt();  // 素子モデルの出力
};
///////////////////////////////////////////////////////////

struct devices_info {
    unsigned shape;     // 回路方程式の次元
    unsigned node_num;  // 回路のノード数
    unsigned isrc_num;  // 電流源素子の使用数
    unsigned vsrc_num;  // 電圧源素子の使用数
    unsigned res_num;   // 抵抗素子の使用数
    unsigned cap_num;   // コンデンサ素子の使用数
    unsigned ind_num;   // インダクタ素子の使用数
    unsigned dio_num;   // ダイオード素子の使用数
    unsigned bjt_num;   // BJT素子の使用数
};

// ネットリストから得られた回路
struct devices {
    isrc_model isrc[MAX_ISRC];
    vsrc_model vsrc[MAX_VSRC];
    res_model res[MAX_RES];
    cap_model cap[MAX_CAP];
    ind_model ind[MAX_IND];
    dio_model dio[MAX_DIO];
    bjt_model bjt[MAX_BJT];

    devices_info info;

    void printCircuit();  // ネットリストから得られた回路の出力
    void printCktInfo();  // 回路情報の出力
};

#endif