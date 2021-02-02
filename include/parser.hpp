#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <string>
#include <vector>

#include "define.hpp"
#include "devices.hpp"
#include "emulation.hpp"

using namespace std;

// 接頭語と乗数の辞書
namespace prefix {
const map<string, floating> dict = {
    {"t", 1e+12}, {"g", 1e+9}, {"meg", 1e+6}, {"k", 1e+3}, {"m", 1e-3}, {"mil", 1e-5}, {"u", 1e-6}, {"µ", 1e-6}, {"n", 1e-9}, {"p", 1e-12}, {"f", 1e-15}};
};

namespace top_char {
const vector<char> ignore = {'\n', '\r', '*'};  // 先頭文字無視してよい先頭文字
const vector<char> elem   = {
    'i', 'I', 'v', 'V', 'r', 'R', 'c',
    'C', 'l', 'L', 'd', 'D', 'q', 'Q'};  // 対応している回路素子の先頭文字
}  // namespace top_char

namespace dot_command {
const vector<string> ignor = {".lib", ".LIB", ".backanno", ".BACKANNO"};  // 本エミュレータでは無視するドットコマンド
const vector<string> end   = {".end", ".END"};                            // エンドコマンド
}  // namespace dot_command

namespace {
const map<string, model_type> m_type_map = {
    {"D", MODEL_DIO}, {"NPN", MODEL_NPN}, {"PNP", MODEL_PNP}};
};

struct netlist {
    vector<isrc_model> isrc;
    vector<vsrc_model> vsrc;
    vector<res_model> res;
    vector<cap_model> cap;
    vector<ind_model> ind;
    vector<dio_model> dio;
    vector<bjt_model> bjt;

    void saveDevices(devices &);
};

struct model_def {
    model_type type;
    map<string, floating> param;
};

bool loadNetlist(const string &, devices &, emulate_info &);
#endif