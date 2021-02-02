#include <iostream>

#include "define.hpp"
#include "devices.hpp"
#include "emulation.hpp"
#include "parser.hpp"

using std::cerr;
using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
    devices dev;
    emulate_info einf;

    if (argc != 2) {
        cerr << "Error: Arguments are missing." << endl;
        return 1;
    }

    try {
        if (!loadNetlist(argv[1], dev, einf)) {
            cout << "Error. The netlist could not be read correctly.\n";
            return 1;
        }
    } catch (const std::exception& e) {
        cerr << "Error: An unexpected error occurred when reading the netlist file." << endl;
        cerr << e.what() << endl;
    }

    dev.printCircuit();
    dev.printCktInfo();
    einf.printInfo();

    startEmulation(dev, einf);

    return 0;
}