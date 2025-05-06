#ifndef BARALHO_HPP
#define BARALHO_HPP

#include <vector>
#include "Carta.hpp"

class Baralho
{
public:
    std::vector<Carta> cartas;
    Baralho();
    void embaralhar();
    Carta comprar();
    bool vazio() const;
    int tamanho() const { return cartas.size(); }

private:
    void adicionar_cartas(TipoCarta tipo, int quantidade);
};

#endif