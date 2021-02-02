#include "emulation.hpp"

#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;
using std::setw;

void emulate_info::printInfo() {
    cout << "-- Emulate info -------------" << endl;

    cout << setw(15) << "analysis type"
         << " :";
    if (this->mode == EMU_OP) {
        cout << ".OP" << endl;
    }
    if (this->mode == EMU_TRAN) {
        cout << ".TRAN" << endl;
        cout << setw(15) << "Tstep"
             << " :" << this->tran_step << endl;
        cout << setw(15) << "Tstop"
             << " :" << this->tran_stop << endl;
    }

    cout << "-----------------------------\n"
         << endl;
}

void startTranEmulation (const devices &dev, const emulate_info &einf){
    
}

void startEmulation(const devices &dev, const emulate_info &einf) {
    switch (einf.mode) {
        case EMU_OP:
            break;
        case EMU_TRAN:
            startTranEmulation(dev, einf);
            break;

        default:
            break;
    }
}
