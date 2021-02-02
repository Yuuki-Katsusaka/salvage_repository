
#include "parser.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "define.hpp"
#include "devices.hpp"
#include "emulation.hpp"

using namespace std;

// 大文字を小文字に変換する関数
string tolowerStr(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// 文字列の一部を置換する関数
string replaceOtherStr(string &replacedStr, string from, string to) {
    const auto pos = replacedStr.find(from);
    const int len  = from.length();

    if (pos == string::npos || from.empty()) {
        return replacedStr;
    }

    return replacedStr.replace(pos, len, to);
}

// 接頭語を考慮した値を導出する関数
floating unitVal(const string &str) {
    floating buf_val;
    string buf_str;

    istringstream ss(str);
    ss >> buf_val >> buf_str;  // 値と乗数の取得

    if (buf_str.empty()) return buf_val;

    buf_str = tolowerStr(buf_str);
    if (prefix::dict.find(buf_str) != prefix::dict.end()) {
        buf_val *= prefix::dict.at(buf_str);
    } else {
        cerr << "Error: Incorrect prefix is used." << endl;
        exit(1);
    }

    return buf_val;
}

// ノード番号を取得する関数
unsigned parseNodeNum(const string &str) {
    string buf;
    for (size_t i = 0; i < str.size(); i++) {
        if (isdigit(str[i]))
            buf += str[i];
        else
            buf.clear();
    }

    return stoi(buf);
}

// 電流源素子の構文解析
isrc_model parseIsrc(string &line) {
    isrc_model isrc;
    string buf, name, np, nn;

    line = replaceOtherStr(line, "(", " ");
    line = replaceOtherStr(line, ")", "");
    istringstream ss(line);
    ss >> name >> np >> nn;

    isrc.node_pos = parseNodeNum(np);
    isrc.node_neg = parseNodeNum(nn);

    vector<string> param;
    while (ss >> buf) param.push_back(buf);

    string type = param.at(0);
    size_t size = param.size();
    if (size == 1) {
        isrc.src_type = WAVE_DC;
        isrc.dc_value = unitVal(type);
    } else {
        if (type == "SINE") {
            isrc.src_type    = WAVE_SINE;
            isrc.sine_offset = unitVal(param.at(1));
            isrc.sine_amp    = unitVal(param.at(2));
            isrc.sine_freq   = unitVal(param.at(3));
        } else {
            cerr << "Error: Invalid waveform mode is used for the source element in the netlist." << endl;
            exit(1);
        }
    }

    return isrc;
}

// 電圧源素子の構文解析
vsrc_model parseVsrc(string &line) {
    vsrc_model vsrc;
    string buf, name, np, nn;

    line = replaceOtherStr(line, "(", " ");
    line = replaceOtherStr(line, ")", "");
    istringstream ss(line);
    ss >> name >> np >> nn;

    vsrc.node_pos = parseNodeNum(np);
    vsrc.node_neg = parseNodeNum(nn);

    vector<string> param;
    while (ss >> buf) param.push_back(buf);

    string type = param.at(0);
    size_t size = param.size();
    if (size == 1) {
        vsrc.src_type = WAVE_DC;
        vsrc.dc_value = unitVal(type);
    } else {
        if (type == "SINE") {
            vsrc.src_type    = WAVE_SINE;
            vsrc.sine_offset = unitVal(param.at(1));
            vsrc.sine_amp    = unitVal(param.at(2));
            vsrc.sine_freq   = unitVal(param.at(3));
        } else {
            cerr << "Error: Invalid waveform mode is used for the source element in the netlist." << endl;
            exit(1);
        }
    }

    return vsrc;
}

// 抵抗素子の構文解析
res_model parseRes(string &line) {
    res_model res;
    string buf, name, np, nn, val;

    istringstream ss(line);
    ss >> name >> np >> nn >> val;

    res.node_pos = parseNodeNum(np);
    res.node_neg = parseNodeNum(nn);
    res.conduct  = 1.f / unitVal(val);

    return res;
}

// コンデンサ素子の構文解析
cap_model parseCap(string &line) {
    cap_model cap;
    string buf, name, np, nn, val;

    istringstream ss(line);
    ss >> name >> np >> nn >> val;

    cap.node_pos = parseNodeNum(np);
    cap.node_neg = parseNodeNum(nn);
    cap.capac    = unitVal(val);

    return cap;
}

// インダクタ素子の構文解析
ind_model parseInd(string &line) {
    ind_model ind;
    string buf, name, np, nn, val;

    istringstream ss(line);
    ss >> name >> np >> nn >> val;

    ind.node_pos = parseNodeNum(np);
    ind.node_neg = parseNodeNum(nn);
    ind.induct   = unitVal(val);

    return ind;
}

// ダイオード素子の構文解析
dio_model parseDio(string &line, const map<string, model_def> &m_map) {
    dio_model dio;
    string buf, e_name, np, nn, m_name;

    istringstream ss(line);
    ss >> e_name >> np >> nn >> m_name;

    dio.node_pos = parseNodeNum(np);
    dio.node_neg = parseNodeNum(nn);
    dio.type     = m_map.at(m_name).type;

    for (const auto &param : m_map.at(m_name).param) {
        if (param.first == "is")
            dio.is = param.second;
        else if (param.first == "rs")
            dio.rs = param.second;
        else if (param.first == "n")
            dio.n = param.second;
        else if (param.first == "tnon")
            dio.tnon = param.second;
        else {
            cerr << "Error: Incorrect parameters are present in the diode model." << endl;
            exit(1);
        }
    }

    return dio;
}

// バイポーラトランジスタ素子の構文解析
bjt_model parseBjt(string &line, const map<string, model_def> &m_map) {
    bjt_model bjt;
    string buf, e_name, nc, nb, ne, m_name;

    istringstream ss(line);
    ss >> e_name >> nc >> nb >> ne >> m_name;

    bjt.node_col  = parseNodeNum(nc);
    bjt.node_base = parseNodeNum(nb);
    bjt.node_emit = parseNodeNum(ne);
    bjt.type      = m_map.at(m_name).type;

    for (const auto &param : m_map.at(m_name).param) {
        if (param.first == "is")
            bjt.is = param.second;
        else if (param.first == "bf")
            bjt.bf = param.second;
        else if (param.first == "br")
            bjt.br = param.second;
        else if (param.first == "rc")
            bjt.rc = param.second;
        else if (param.first == "re")
            bjt.re = param.second;
        else if (param.first == "tnon")
            bjt.tnon = param.second;
        else {
            cerr << "Error: Incorrect parameters are present in the diode model." << endl;
            exit(1);
        }
    }

    return bjt;
}

// 回路素子の構文解析
bool parseElements(string &line, netlist &nlist, const map<string, model_def> &m_map) {
    switch (line[0]) {  // 素子の種類をチェック
        case 'I':       // 電流源
            nlist.isrc.push_back(parseIsrc(line));
            break;
        case 'V':  // 電圧源
            nlist.vsrc.push_back(parseVsrc(line));
            break;
        case 'R':  // 抵抗
            nlist.res.push_back(parseRes(line));
            break;
        case 'C':  // コンデンサ
            nlist.cap.push_back(parseCap(line));
            break;
        case 'L':  // インダクタ
            nlist.ind.push_back(parseInd(line));
            break;
        case 'D':  // ダイオード
            nlist.dio.push_back(parseDio(line, m_map));
            break;
        case 'Q':
            nlist.bjt.push_back(parseBjt(line, m_map));
            break;
        default:
            cerr << "Error: Unknown circuit elements are used.";
            return false;
    }
    return true;
}

// ドットコマンドの構文解析
bool parseDotCommand(const string &line, emulate_info &einf, map<string, model_def> &m_map) {
    stringstream ss(line);
    string buf;

    ss >> buf;
    if (buf == ".OP" || buf == ".op") {  // 動作点解析
        einf.mode = EMU_OP;
    } else if (buf == ".TRAN" || buf == ".tran") {  // 過渡解析
        einf.mode = EMU_TRAN;

        string buf_step, buf_stop;
        ss >> buf_step >> buf_stop;
        einf.tran_step = unitVal(buf_step);           // サンプリング周期
        einf.tran_stop = unitVal(buf_stop);           // 終了時間
    } else if (buf == ".MODEL" || buf == ".model") {  // spiceモデルの定義
        string m_name, m_type, p_name, p_val;
        vector<string> param;  // .modelに定義されたパラメータの文字列保存用
        model_def mdef;

        // 下準備
        getline(ss, buf);
        buf = replaceOtherStr(buf, "(", " ");
        buf = replaceOtherStr(buf, ")", "");
        istringstream ss2(buf);

        ss2 >> m_name >> m_type;  // モデル名とモデルのタイプを取得
        mdef.type = m_type_map.at(m_type);

        while (ss2 >> buf) param.push_back(buf);  // モデルのパラメータを取得
        for (auto v : param) {                    // モデルのパラメータを辞書化
            v = replaceOtherStr(v, "=", " ");
            istringstream ss3(v);
            ss3 >> p_name >> p_val;
            mdef.param[tolowerStr(p_name)] = unitVal(p_val);
        }

        m_map[m_name] = mdef;
    } else {
        cerr << "Error: Dot commands that are not supported by the emulator are used." << endl;
        return false;
    }
    return true;
}

// ネットリストファイルの読み込み
// bool loadNetlist(const string &fname, netlist &nlist, circuit_info &cinf, emulate_info &einf) {
bool loadNetlist(const string &fname, devices &dev, emulate_info &einf) {
    netlist nlist;  // 一時的に回路構成を保存する変数

    // ファイルオープン
    ifstream ifs(fname, ios::in);
    if (!ifs) {
        cout << "Error. File can not open.\n";
        return false;
    }

    // ファイル前処理
    vector<string> ckt_f;   // 取得したファイルの必要な部分だけを格納する配列
    bool flag_end = false;  // .endコマンドのフラグ
    while (!ifs.eof()) {
        string buf_l, buf_d;
        getline(ifs, buf_l);  // 文字列を一行取得

        istringstream ss(buf_l);  // ドットコマンドを取得
        ss >> buf_d;

        if (find(top_char::ignore.begin(), top_char::ignore.end(), buf_l[0])  // 先頭文字が'*'，改行の場合
            != top_char::ignore.end()) {
            continue;
        } else if (find(dot_command::ignor.begin(), dot_command::ignor.end(), buf_d)  // 無視するドットコマンドの場合
                   != dot_command::ignor.end()) {
            continue;
        } else if (find(dot_command::end.begin(), dot_command::end.end(), buf_d)  // エンドコマンドの場合
                   != dot_command::end.end()) {
            flag_end = true;
            break;
        } else if (buf_l[0] == '+') {  // 先頭文字が'+'の場合
            ckt_f.back().pop_back();
            buf_l = ckt_f.back() + ' ' + buf_l.substr(1);

            ckt_f[ckt_f.size() - 1] = buf_l;
        } else {  // それ以外の解析に必要な文字列
            ckt_f.push_back(buf_l);
        }
    }

    if (!flag_end) {  // ".end"コマンドが存在しない場合，エラー
        cout << "Error: The '.end' dot command does not exist.\n";
        return false;
    }

    // ファイル解析1(ドットコマンドの解析)
    map<string, model_def> model_map;
    for (const auto &line : ckt_f) {
        if (find(top_char::elem.begin(), top_char::elem.end(), line[0])
            != top_char::elem.end()) {
            continue;
        } else if (parseDotCommand(line, einf, model_map)) {  // ENDコマンド
            continue;
        } else {  // 解析失敗
            cout << "Error: The format of the dot command is incorrect.";
            return false;
        }
    }

    // ファイル解析2(回路素子の解析)
    for (auto line : ckt_f) {
        if (line[0] == '.') {  // ドットコマンドの場合
            continue;
        } else if (find(top_char::elem.begin(), top_char::elem.end(), line[0])
                   != top_char::elem.end()) {  // 回路素子の場合
            if (!parseElements(line, nlist, model_map)) {
                cerr << "Error: The format of the circuit element is incorrect." << endl;
            }
        } else {
            cerr << "Error: The first character does not match the format." << endl;
            return false;
        }
    }

    nlist.saveDevices(dev);  // 回路構成を保存

    return true;
}

void netlist::saveDevices(devices &dev) {
    unsigned node_num = 0;

    // 各素子の使用数を取得
    if ((this->isrc.size() > MAX_ISRC) || (this->vsrc.size() > MAX_VSRC) || (this->res.size() > MAX_RES) || (this->cap.size() > MAX_CAP) || (this->ind.size() > MAX_IND) || (this->dio.size() > MAX_DIO) || (this->bjt.size() > MAX_BJT)) {
        cerr << "Error: The maximum usable number of circuit elements has been exceeded." << endl;
        exit(1);
    } else {
        dev.info.isrc_num = (unsigned)this->isrc.size();
        dev.info.vsrc_num = (unsigned)this->vsrc.size();
        dev.info.res_num  = (unsigned)this->res.size();
        dev.info.cap_num  = (unsigned)this->cap.size();
        dev.info.ind_num  = (unsigned)this->ind.size();
        dev.info.dio_num  = (unsigned)this->dio.size();
        dev.info.bjt_num  = (unsigned)this->bjt.size();
    }

    // ノード数と回路素子を取得
    for (size_t i = 0; i < this->isrc.size(); i++) {
        if (this->isrc[i].node_pos > node_num) node_num = this->isrc[i].node_pos;
        if (this->isrc[i].node_neg > node_num) node_num = this->isrc[i].node_neg;

        dev.isrc[i] = this->isrc[i];
    }
    for (size_t i = 0; i < this->vsrc.size(); i++) {
        if (this->vsrc[i].node_pos > node_num) node_num = this->vsrc[i].node_pos;
        if (this->vsrc[i].node_neg > node_num) node_num = this->vsrc[i].node_neg;

        dev.vsrc[i] = this->vsrc[i];
    }
    for (size_t i = 0; i < this->res.size(); i++) {
        if (this->res[i].node_pos > node_num) node_num = this->res[i].node_pos;
        if (this->res[i].node_neg > node_num) node_num = this->res[i].node_neg;

        dev.res[i] = this->res[i];
    }
    for (size_t i = 0; i < this->cap.size(); i++) {
        if (this->cap[i].node_pos > node_num) node_num = this->cap[i].node_pos;
        if (this->cap[i].node_neg > node_num) node_num = this->cap[i].node_neg;

        dev.cap[i] = this->cap[i];
    }
    for (size_t i = 0; i < this->ind.size(); i++) {
        if (this->ind[i].node_pos > node_num) node_num = this->ind[i].node_pos;
        if (this->ind[i].node_neg > node_num) node_num = this->ind[i].node_neg;

        dev.ind[i] = this->ind[i];
    }
    for (size_t i = 0; i < this->dio.size(); i++) {
        if (this->dio[i].node_pos > node_num) node_num = this->dio[i].node_pos;
        if (this->dio[i].node_neg > node_num) node_num = this->dio[i].node_neg;

        dev.dio[i] = this->dio[i];
    }
    for (size_t i = 0; i < this->bjt.size(); i++) {
        if (this->bjt[i].node_col > node_num) node_num = this->bjt[i].node_col;
        if (this->bjt[i].node_base > node_num) node_num = this->bjt[i].node_base;
        if (this->bjt[i].node_emit > node_num) node_num = this->bjt[i].node_emit;

        dev.bjt[i] = this->bjt[i];
    }

    dev.info.node_num = node_num;
    dev.info.shape    = node_num + (unsigned)this->vsrc.size() + (unsigned)this->ind.size();
}