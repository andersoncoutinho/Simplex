#include <iostream>
#include "Data.h"
#include "Simplex.h"



int main(int argc, char *argv[]) {

    Data data(argc, argv[1]);

    data.readData();
    
    data.printData();

    Simplex simplex(data);
    simplex.printTableau();
    simplex.solve();
    simplex.printTableau();
    cout << endl << "Objective Value: " << simplex.geObjValue() << endl;


    
    return 0;
}