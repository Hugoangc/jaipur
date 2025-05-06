// Mercado.hpp
#ifndef MERCADO_HPP
#define MERCADO_HPP

#include <vector>
#include <map>
#include "Carta.hpp"
#include "Baralho.hpp"
#include "Ficha.hpp"

class Mercado
{
public:
    std::vector<Carta> cartas;
    std::map<TipoCarta, std::vector<Ficha>> fichas;

public:
    std::vector<Ficha> bonus3;
    std::vector<Ficha> bonus4;
    std::vector<Ficha> bonus5;
    void inicializar(Baralho &baralho);
    void mostrar();
    void repor(Baralho &baralho);
    void repor_camelo(Baralho &baralho);
    bool tem_camelo() const;
    void remover_carta(int indice);
    void trocar_cartas(const std::vector<int> &idx_mercado, const std::vector<Carta> &novas_cartas);
    bool tres_pilhas_vazias() const;
    void inicializar_fichas();
    std::vector<Carta> getCartas() const
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
