#ifndef DEF_HPP
#define DEF_HPP

#include <cstdint>

#ifdef DOUBLE
using floating = double;
#else
using floating = float;
#endif

// 数学定数
namespace math_const {
const floating PI = 3.141592653589793;  // 円周率
}  // namespace math_const

// 回路に関する定数
enum circuit_const {
    MAX_N    = 100,  // 回路方程式の次元
    MAX_NODE = 100   // 回路の最大節点数
};

// 回路素子に関する定数
enum elem_const {
    MAX_ISRC = 10,  // 電流源の最大数
    MAX_VSRC = 10,  // 電圧源の最大数
    MAX_RES  = 10,  // 抵抗の最大数
    MAX_CAP  = 10,  // コンデンサの最大数
    MAX_IND  = 10,  // インダクタの最大数
    MAX_DIO  = 10,  // ダイオードの最大数
    MAX_BJT  = 10,  // バイポーラトランジスタの最大数
    MAX_MOS  = 10   // MOSFETトランジスタの最大数
};

#endif