// Mercado.hpp
#ifndef MERCADO_HPP
#define MERCADO_HPP

#include <vector>
#include <map>
#include "Carta.hpp"
#include "Baralho.hpp"
#include "Ficha.hpp"
using namespace std;
class Mercado
{
public:
    vector<Carta> cartas;
    map<TipoCarta, vector<Ficha>> fichas;

public:
    vector<Ficha> bonus3;
    vector<Ficha> bonus4;
    vector<Ficha> bonus5;
    void inicializar(Baralho &baralho);
    void mostrar() const;
    void repor(Baralho &baralho);
    void repor_camelo(Baralho &baralho);
    bool tem_camelo() const;
    void remover_carta(int indice);
    void trocar_cartas(const vector<int> &idx_mercado, const vector<Carta> &novas_cartas);
    bool tres_pilhas_vazias() const;
    void inicializar_fichas();

    vector<Carta> getCartas() const
    {
        return cartas;
    }

    void adicionarCarta(const Carta &carta)
    {
        cartas.push_back(carta);
    }

    bool vazio() const
    {
        return cartas.empty();
    }
};

#endif
