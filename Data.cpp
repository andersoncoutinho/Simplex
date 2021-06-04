#include "Data.h"  
#include <iostream>
#include <fstream>

Data::Data(int qtdParam, char *instancia) {
    
    if(qtdParam < 2) {
        std::cout << "Faltando parametros." << std::endl;
        exit(1);
    } else if (qtdParam > 2) {
        std::cout << "Excesso de parametros." << std::endl;
        exit(1);
    }

    this->instancia = instancia;
}

void Data::readData() {

    std::fstream arquivo;
    std::string valor, linha;

    arquivo.open(instancia);
    if(!arquivo.is_open()) {
        std::cout << "Erro ao tentar abrir instância" << std::endl;
        exit(1);
    }

    std::getline(arquivo, linha);
    this->modo = linha == "MAX" ? MAX : MIN;

    std::getline(arquivo, linha);
    for(int i = 0; i < linha.size(); i++) {
        if(linha[i] == ',' || linha[i] == '\n') {
            valor.push_back('\0');
            c.push_back(stod(valor));
            valor.clear();
        } else {
            valor.push_back(linha[i]);
        }
    }

    std::getline(arquivo, linha);
    std::getline(arquivo, linha);
    std::getline(arquivo, linha);

    int i = 0;
    while(true) {       
        std::getline(arquivo, linha);
        
        if(isalpha(linha[0])) {
            break;
        }
        
        A.push_back(std::vector<double>());
        for(int j = 0; j < linha.size(); j++) {
            if(linha[j] == ',' || linha[j] == '\n') {
                valor.push_back('\0');
                A[i].push_back(stod(valor));
                valor.clear();
            } else {
                valor.push_back(linha[j]);
            }
        }
        i++;
    }

    std::getline(arquivo, linha);
    std::getline(arquivo, linha);

    std::getline(arquivo, linha);
    for(int i = 0; i < linha.size(); i++) {
        if(linha[i] == ',') {
            sinal.push_back(linha[i-1]);

        }
    }

    std::getline(arquivo, linha);
    std::getline(arquivo, linha);
    std::getline(arquivo, linha);

    std::getline(arquivo, linha);
    for(int i = 0; i < linha.size(); i++) {
        if(linha[i] == ',') {
            valor.push_back('\0');
            b.push_back(stod(valor));
            valor.clear();
        } else {
            valor.push_back(linha[i]);
        }
    }
}

void Data::printData() {

    std::cout << ((this->modo == MAX) ? "Maximizar\n" : "Minimizar\n")
    << "z = ";
    int i;
    for(i = 0; i < c.size()-1; i++) {
        if(c[i] != 1) {
            std::cout << c[i];
        }
        std::cout << "x" << i+1 << " + ";
    }
    std::cout << c[i] << "x" << i+1 << std::endl;
    
    bool temAnterior = false;
    std::cout << "Subject to:" << std::endl;
    int j;
    for(i = 0; i < A.size(); i++) {
        for(j = 0; j < A[i].size() - 1; j++) {
            if(A[i][j] != 0) {
                if(temAnterior) {
                    std::cout << " + ";
                }
                if(A[i][j] != 1) {
                    std::cout << A[i][j];
                }
                std::cout << "x" << j+1;
                temAnterior = true;
            } else {
                temAnterior = false;
            }
        }
        if(A[i][j] != 0) {
            if(temAnterior) {
                std::cout << " + ";
            }
            if(A[i][j] != 1) {
                std::cout << A[i][j];
            }
            std::cout << "x" << j+1;
            temAnterior = false;
        }

        std::string than;
        if(sinal[i] == '<') {
            than = "\u2264";
        } else if(sinal[i] == '>') {
            than = "\u2265";
        } else {
            than = "=";
        }

        std::cout << " " << than << " " << b[i] << std::endl;
    }

    for(int j = 1; j <= getValoresFO().size(); j++) {
        std::cout << "x" << j << " \u2265 0" << std::endl;
    }


}

std::vector<double> Data::getValoresFO() {
    return this->c;
}

std::vector<std::vector<double>> Data::getMatrizRestric() {
    return this->A;
}

std::vector<char> Data::getSinalRestric() {
    return this->sinal;
}

char Data::getSinalRestric(int i) {
    return this->sinal[i];
}

std::vector<double> Data::getLimites() {
    return this->b;
}

int Data::getModo() {
    return this->modo;
}

double Data::getLimites(int i) {
    return b[i];
}

Data::~Data() {

}