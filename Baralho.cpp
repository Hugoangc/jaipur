#include <random>
#include "Baralho.hpp"
#include "Carta.hpp"

#include <algorithm>

#include <ctime>

Baralho::Baralho()
{
    adicionar_cartas(DIAMANTE, 6);
    adicionar_cartas(OURO, 6);
    adicionar_cartas(PRATA, 6);
    adicionar_cartas(TECIDO, 8);
    adicionar_cartas(ESPECIARIA, 8);
    adicionar_cartas(COURO, 10);
    adicionar_cartas(CAMELO, 11);
}

void Baralho::adicionar_cartas(TipoCarta tipo, int quantidade)
{
    for (int i = 0; i < quantidade; ++i)
    {
        cartas.emplace_back(tipo);
    }
}

void Baralho::embaralhar()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cartas.begin(), cartas.end(), g);
}

Carta Baralho::comprar()
{
    if (vazio())
        return Carta(VAZIO);
    Carta c = cartas.back();
    cartas.pop_back();
    return c;
}

bool Baralho::vazio() const
{
    return cartas.empty();
}
