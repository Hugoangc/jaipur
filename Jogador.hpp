#ifndef JOGADOR_HPP
#define JOGADOR_HPP

#include "Carta.hpp"
#include "Mercado.hpp"
#include <vector>
#include <string>
using namespace std;

class Jogador
{
public:
    string nome;
    int pontos;
    int fichas_bonus;
    int selos_excelencia;
    vector<Carta> mao;
    vector<Carta> camelos;

    Jogador(string nome);

    void pegar_carta(const Carta &carta);
    void pegar_camelos_do_mercado(Mercado &mercado);
    void mostrar_mao() const;
    void mostrar_camelos() const;
    bool excedeu_limite_mao() const;
    void vender_mercadorias(Mercado &mercado, TipoCarta tipo, int quantidade);
    void trocar_cartas(Mercado &mercado, const vector<int> &idx_mercado, const vector<int> &idx_mao);
    void zerar_pontos();
    Carta remover_carta_da_mao(int indice);
    Carta remover_um_camelo();
    int camelo_count() const;
    bool camelo_vazio() const;
    void limpar_mao_e_camelos();
    void realizar_troca_completa(Mercado &mercado, const vector<int> &idx_mercado, const vector<int> &idx_mao, int qtd_camelos);
};

#endif
