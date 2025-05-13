#ifndef BARALHO_HPP
#define BARALHO_HPP

#include <vector>
#include "Carta.hpp"
using namespace std;

class Baralho
{
public:
    vector<Carta> cartas;
    Baralho();
    void embaralhar();
    Carta comprar();
    bool vazio() const;
    int tamanho() const { return cartas.size(); }
    const vector<Carta> &cartas_restantes() const { return cartas; }

private:
    void adicionar_cartas(TipoCarta tipo, int quantidade);
};

#endif