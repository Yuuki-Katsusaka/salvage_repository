#include "devices.hpp"

#include <iomanip>
#include <iostream>

#include "define.hpp"

using std::cout;
using std::endl;
using std::setw;

void isrc_model::printIsrc() {
    cout << "I" << setw(3) << this->node_pos << setw(3) << this->node_neg;
    if (this->src_type == WAVE_DC)
        cout << " " << this->dc_value;
    if (this->src_type == WAVE_SINE)
        cout << " SINE(" << this->sine_offset << " " << this->sine_amp << " " << this->sine_freq << ")";
    cout << endl;
}

void vsrc_model::printVsrc() {
    cout << "V" << setw(3) << this->node_pos << setw(3) << this->node_neg;
    if (this->src_type == WAVE_DC)
        cout << " " << this->dc_value;
    if (this->src_type == WAVE_SINE)
        cout << " SINE(" << this->sine_offset << " " << this->sine_amp << " " << this->sine_freq << ")";
    cout << endl;
}

void res_model::printRes() {
    cout << "R" << setw(3) << this->node_pos << setw(3) << this->node_neg << " " << 1.f / this->conduct << endl;
}

void cap_model::printCap() {
    cout << "C" << setw(3) << this->node_pos << setw(3) << this->node_neg << " " << this->capac << endl;
}

void ind_model::printInd() {
    cout << "L" << setw(3) << this->node_pos << setw(3) << this->node_neg << " " << this->induct << endl;
}

void dio_model::printDio() {
    cout << "D" << setw(3) << this->node_pos << setw(3) << this->node_neg;

    if (this->type == MODEL_DIO) cout << " D";
    cout << "(is=" << this->is << ", rs=" << this->rs << ", n=" << this->n << ", tnon=" << this->tnon << ")" << endl;
}

void bjt_model::printBjt() {
    cout << "Q" << setw(3) << this->node_col << setw(3) << this->node_base << setw(3) << this->node_emit;

    if (this->type == MODEL_NPN) cout << " NPN";
    if (this->type == MODEL_PNP) cout << " PNP";
    cout << "(is=" << this->is << ", bf=" << this->bf << ", br=" << this->br << ", rc=" << this->rc << ", re=" << this->re << ", tnon=" << this->tnon << ")" << endl;
}

void devices::printCircuit() {
    cout << "--Target circuit netlist ----------------------------------" << endl;
    for (size_t i = 0; i < this->info.isrc_num; i++) {
        this->isrc[i].printIsrc();
    }
    for (size_t i = 0; i < this->info.vsrc_num; i++) {
        this->vsrc[i].printVsrc();
    }
    for (size_t i = 0; i < this->info.res_num; i++) {
        this->res[i].printRes();
    }
    for (size_t i = 0; i < this->info.cap_num; i++) {
        this->cap[i].printCap();
    }
    for (size_t i = 0; i < this->info.ind_num; i++) {
        this->ind[i].printInd();
    }
    for (size_t i = 0; i < this->info.dio_num; i++) {
        this->dio[i].printDio();
    }
    for (size_t i = 0; i < this->info.bjt_num; i++) {
        this->bjt[i].printBjt();
    }
    cout << "---------------------------------------------------\n"
         << endl;
}

void devices::printCktInfo() {
    cout << "-- Target circuit info ------------" << endl;
    cout << setw(15) << "equation shape"
         << " :" << this->info.shape << endl;
    cout << setw(15) << "node num"
         << " :" << this->info.node_num << endl;
    cout << "-----------------------------------\n"
         << endl;
}