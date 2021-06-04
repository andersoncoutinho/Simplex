#include "Simplex.h"  
#include <vector>
#include <iostream>
#include <algorithm>
	
Simplex::Simplex(Data data) {
	this->gerarTableau(data);
    this->gerarDual(data);
    this->sinais = data.getSinalRestric();
    this->modo = data.getModo();
}

void Simplex::gerarTableau(Data data) {

    double M = 1000;
    std::vector<int> indicesM;
    
    

    tableau.push_back(std::vector<double>());
    tableau[0] = data.getValoresFO();

    if(data.getModo() == MAX) {
        for(int j = 0; j < tableau[0].size(); j++) {
            tableau[0][j] = -tableau[0][j];
        }
    }
    for(int i = 0; i < data.getMatrizRestric().size(); i++) {

        tableau.push_back(std::vector<double>());
        tableau[i+1] = data.getMatrizRestric()[i];
    }

    for(int i = 0, indiceBasicas = 0; i < data.getSinalRestric().size(); i++, indiceBasicas++) {
        VB.push_back(tableau.size()+indiceBasicas+1);
        if(data.getSinalRestric(i) == '<') {
            tableau[0].push_back(0);

        } else {

            indicesM.push_back(i);
            tableau[0].push_back(M);
            if(data.getSinalRestric(i) == '>') {
                indiceBasicas++;
                tableau[0].push_back(0);
            }
        }

        for(int linha = 0; linha < data.getMatrizRestric().size(); linha++) {

            if(i == linha) {
                tableau[linha+1].push_back(1);
                if(data.getSinalRestric(i) == '>') {
                    tableau[linha+1].push_back(-1);
                }
            } else {
                tableau[linha+1].push_back(0);
                if(data.getSinalRestric(i) == '>') {
                    tableau[linha+1].push_back(0);
                }
            }
        }
    }

    tableau[0].push_back(0);
    for(int i = 0; i < data.getLimites().size(); i++) {
        tableau[i+1].push_back(data.getLimites(i));
    }

    for(int i : indicesM) {
        for(int j = 0; j < tableau[i+1].size(); j++) {
            tableau[0][j] -= (tableau[i+1][j]*M);
        }
    }

    qtdVar = data.getValoresFO().size();
}

void Simplex::gerarDual(Data data) {

    tableauDual.push_back(std::vector<double>());
    for(int i = 0; i < data.getMatrizRestric().size(); i++) {
        tableauDual[0].push_back(data.getLimites()[i]);
    }

    for(int j = 0; j < data.getMatrizRestric()[0].size(); j++) {
        tableauDual.push_back(std::vector<double>());
        for(int i = 0; i < data.getMatrizRestric().size(); i++) {
            tableauDual[j+1].push_back(data.getMatrizRestric()[i][j]);
        }
    }

    for(int i = 0; i < data.getValoresFO().size(); i++) {
        tableauDual[i+1].push_back(data.getValoresFO()[i]);
    }


}

void Simplex::solve() {

    while(!ehValido()) {    
        int colunaPivot = getColunaPivot();
        int linhaPivot = getLinhaPivot(colunaPivot);
        double elementoPivot = tableau[linhaPivot][colunaPivot];
        VB[linhaPivot-1] = colunaPivot+1;
    
        
        for(int j = 0; j < tableau[linhaPivot].size(); j++) {
            if(tableau[linhaPivot][j] != 0) {
                tableau[linhaPivot][j] /= elementoPivot;
            }
        }
        for(int i = 0; i < tableau.size(); i++) {
            double fatorMultiplicacao = (-tableau[i][colunaPivot]);
            if(i != linhaPivot) {
                for(int j = 0; j < tableau[i].size(); j++) {
                    tableau[i][j] = (tableau[i][j] + (fatorMultiplicacao * tableau[linhaPivot][j])); 

                }
            }
        }
    }

    this->obj_value = this->modo == MAX ? tableau[0][tableau[0].size()-1] : -tableau[0][tableau[0].size()-1];
    
}

bool Simplex::ehValido() {
    for(int j = 0; j < tableau[0].size()-1; j++) {
        if(tableau[0][j] < 0) {
            return false;
        }
    }
    return true;
}

int Simplex::getColunaPivot() {
    double menor = 0;
    int coluna;
    for(int j = 0; j < tableau[0].size()-1; j++) {
        if(tableau[0][j] < menor) {
            menor = tableau[0][j];
            coluna = j;
        }
    }
    return coluna;
}

int Simplex::getLinhaPivot(int j) {
    double menor = __DBL_MAX__;
    int linha;
    for(int i = 1; i < tableau.size(); i++) {
        if(tableau[i][j] > 0) {
            int razao = tableau[i][tableau[i].size()-1]/tableau[i][j];
            if(razao < menor) {
                menor = razao;
                linha = i;
            }
        }
    }
    return linha;
}

double Simplex::geObjValue() {
    return this->obj_value;
}

void Simplex::printTableau() {

    for(int i = 0; i < tableau.size(); i++) {
        if(i > 0) {
            std::cout << "x" << VB[i-1] << "   ";
        }
        for(int j = 0; j < tableau[i].size(); j++) {

            std::cout << tableau[i][j] << " ";

        }
        std::cout << std::endl;
    }

}

void Simplex::printValues() {

    std::vector<double> values(tableau[0].size());

    for(int i = 0; i < VB.size(); i++) {

        if(VB[i] <= qtdVar) {
            values.erase(values.begin()+(VB[i]-1));
            values.insert(values.begin()+(VB[i]-1), tableau[i+1][tableau[0].size()-1]);
        }
    }

    for(int i = 0; i < qtdVar; i++) {
        std::cout << "x" << i+1 << ": " << values[i] << std::endl;

    }
}

double Simplex::getValue(int i) {
    return tableau[0][i];
}

void Simplex::printDual() {

    std::cout << (modo == MAX ? "Minimizar" : "Maximizar") << std::endl
            << "w = ";
    int i, j;
    for(j = 0; j < tableauDual[0].size()-1; j++) {
        std::cout << tableauDual[0][j] << "y" << j+1 << " + "; 
    }
    std::cout << tableauDual[0][j] << "y" << j+1 << std::endl
        << "Subject to:" << std::endl;

    for(i = 1; i < tableauDual.size(); i++) {
        for(j = 0; j < tableauDual[i].size()-2; j++) {
            std::cout << tableauDual[i][j] << "y" << j+1 << " + "; 
        }
        std::cout << tableauDual[i][j] << "y" << ++j; 

        std::cout << (modo == MAX ? " \u2265 " : " \u2264 ") << tableauDual[i][j] << std::endl;
    }

    std::cout << "e" << std::endl;

    for(j = 0; j < tableauDual[0].size(); j++) {
        std::cout << "y" << j+1;
        if(sinais[j] == '=') {
            std::cout << ", livre" << std::endl;
        } else if(this->modo == MIN){
            if(sinais[j] == '<') {
                std::cout << " \u2264 0" << std::endl;
            } else {
                std::cout << " \u2265 0" << std::endl;
            }
        } else {
            if(sinais[j] == '<') {
                std::cout << " \u2265 0" << std::endl;
            } else {
                std::cout << " \u2264 0" << std::endl;
            }
        }
    }

}
	
Simplex::~Simplex() {
	
}