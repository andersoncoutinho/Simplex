#include <iostream>
#include "Data.h"
#include "Simplex.h"



int main(int argc, char *argv[]) {

    Data data(argc, argv[1]);

    data.readData();
    
    data.printData();
    std::cout << std::endl << std::endl;

    Simplex simplex(data);
    simplex.printDual();  
    std::cout << std::endl << std::endl;
    
    //simplex.solve();
    //simplex.printTableau();
    //simplex.printValues();
    //cout << endl << "Objective Value: " << simplex.geObjValue() << endl;


    
    return 0;
}