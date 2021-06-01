#include "Simplex.h"  
#include <vector>
#include <iostream>
	
Simplex::Simplex(Data data) {
	this->gerarTableau(data);
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
	
Simplex::~Simplex() {
	
}