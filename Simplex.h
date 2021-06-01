#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <vector>
#include "Data.h"
	
class Simplex {
	private:
		char *instancia;
		std::vector<std::vector<double>> tableau;
		int modo;
		double obj_value;
		std::vector<int> VB;

	public:
		Simplex(Data data);
		void gerarTableau(Data data);
		void printTableau();
		bool ehValido();
		int getColunaPivot();
		int getLinhaPivot(int);
		void solve();
		double geObjValue();
		~Simplex();

};
#endif