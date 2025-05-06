// Mercado.cpp
#include "Mercado.hpp"
#include <iostream>
#include <algorithm>
#include <random>
void Mercado::inicializar(Baralho &baralho)
{
    for (int i = 0; i < 5; ++i)
    {
        cartas.push_back(baralho.comprar());
    }
}

void Mercado::mostrar()
{
    std::cout << "Cartas do Mercado: \n";
    for (int i = 0; i < cartas.size(); ++i)
    {
        std::cout << i << ": " << cartas[i].toString() << std::endl;
    }
}

void Mercado::repor(Baralho &baralho)
{
    if (cartas.size() < 5)
    {
        while (cartas.size() < 5 && !baralho.vazio())
        {
            cartas.push_back(baralho.comprar());
        }
    }
}

void Mercado::repor_camelo(Baralho &baralho)
{
    while (cartas.size() < 5 && !baralho.vazio())
    {
        Carta c = baralho.comprar();
        if (c.tipo == CAMELO)
        {
            cartas.push_back(c);
        }
    }
}

bool Mercado::tem_camelo() const
{
    for (const Carta &c : cartas)
    {
        if (c.tipo == CAMELO)
        {
            return true;
        }
    }
    return false;
}

void Mercado::remover_carta(int indice)
{
    if (indice >= 0 && indice < cartas.size())
    {
        cartas.erase(cartas.begin() + indice);
    }
}

void Mercado::trocar_cartas(const std::vector<int> &idx_mercado, const std::vector<Carta> &novas_cartas)
{
    for (size_t i = 0; i < idx_mercado.size(); ++i)
    {
        cartas[idx_mercado[i]] = novas_cartas[i];
    }
}

bool Mercado::tres_pilhas_vazias() const
{
    int vazias = 0;
    for (const auto &ficha : fichas)
    {
        if (ficha.second.empty())
        {
            vazias++;
        }
    }
    return vazias >= 3;
}

void Mercado::inicializar_fichas()
{
    // Diamante: valores 7~5
    fichas[DIAMANTE] = {Ficha(7), Ficha(7), Ficha(5), Ficha(5), Ficha(5)};
    // Ouro: valores 6~5
    fichas[OURO] = {Ficha(6), Ficha(6), Ficha(5), Ficha(5), Ficha(5)};
    // Prata: valores 5~5
    fichas[PRATA] = {Ficha(5), Ficha(5), Ficha(5), Ficha(5), Ficha(5)};
    // Tecido: valores 5~1
    fichas[TECIDO] = {Ficha(5), Ficha(3), Ficha(3), Ficha(2), Ficha(1), Ficha(1), Ficha(1)};
    // Especiarias: valores 5~1
    fichas[ESPECIARIA] = {Ficha(5), Ficha(3), Ficha(3), Ficha(2), Ficha(1), Ficha(1), Ficha(1)};
    // Couro: valores 4~1
    fichas[COURO] = {Ficha(4), Ficha(3), Ficha(2), Ficha(1), Ficha(1), Ficha(1), Ficha(1), Ficha(1), Ficha(1)}; // 9 fichas

    // Bônus de 3 cartas: 3 fichas entre 1~3
    bonus3 = {Ficha(1), Ficha(2), Ficha(3)};
    // Bônus de 4 cartas: 3 fichas entre 4~6
    bonus4 = {Ficha(4), Ficha(5), Ficha(6)};
    // Bônus de 5 cartas: 3 fichas entre 8~10
    bonus5 = {Ficha(8), Ficha(9), Ficha(10)};

    // Embaralhar as fichas bônus
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(bonus3.begin(), bonus3.end(), g);
    std::shuffle(bonus4.begin(), bonus4.end(), g);
    std::shuffle(bonus5.begin(), bonus5.end(), g);
}