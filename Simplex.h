#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <vector>
#include "Data.h"
	
class Simplex {
	private:
		char *instancia;
		std::vector<std::vector<double>> tableau;
		std::vector<std::vector<double>> tableauDual;
		int modo;
		int qtdVar;
		double obj_value;
		std::vector<int> VB;
		void gerarTableau(Data);
		void gerarDual(Data);
		std::vector<char> sinais;

	public:
		void printTableau();
		void printDual();
		Simplex(Data data);
		bool ehValido();
		int getColunaPivot();
		int getLinhaPivot(int);
		void solve();
		double geObjValue();
		void printValues();
		double getValue(int i);
		~Simplex();

};
#endif