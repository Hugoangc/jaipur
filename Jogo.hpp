#ifndef JOGO_HPP
#define JOGO_HPP
#include <string>
#include "Jogador.hpp"
#include "Baralho.hpp"
#include "Mercado.hpp"
#include "Estado.hpp"
using namespace std;

vector<EstadoJogo> gerar_jogadas_possiveis(const EstadoJogo &estado_atual);
void gerar_combinacoes_indices(int start, int k, vector<int> &combo_atual, vector<vector<int>> &todas_as_combos, int n);
int minimax(const EstadoJogo &estado_node, int profundidade, bool eh_jogador_max);
int minimax_alfabeta(const EstadoJogo &estado_node, int profundidade, int alpha, int beta, bool eh_jogador_max);
EstadoJogo encontrar_melhor_jogada_minmax(const EstadoJogo &estado_atual, int profundidade);
EstadoJogo encontrar_melhor_jogada_podaAB(const EstadoJogo &estado_atual, int profundidade);
class Jogo
{
public:
    void iniciar();
    void salvar_jogo(const string &nome_arquivo, const Jogador &jogador1, const Jogador &jogador2, const Baralho &baralho, const Mercado &mercado, int turno);
    void carregar_jogo(const string &nome_arquivo, Jogador &jogador1, Jogador &jogador2, Baralho &baralho, Mercado &mercado, int &turno);
};

#endif