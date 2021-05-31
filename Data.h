#ifndef DATA_H
#define DATA_H

#include <vector>

#define MIN 0
#define MAX 1

using namespace std;
	
class Data  {
	private:
		int modo;
		char *instancia;
		std::vector<double> c;
		std::vector<std::vector<double>> A;
		std::vector<char> sinal;
		std::vector<double> b;
	public:
		void readData();
		void printData();
		std::vector<double> getValoresFO();
		std::vector<std::vector<double>> getMatrizRestric();
		std::vector<char> getSinalRestric();
		char getSinalRestric(int i);
		std::vector<double> getLimites();
		double getLimites(int i);
		int getModo();
		Data(int, char*);
		~Data();

};
#endif