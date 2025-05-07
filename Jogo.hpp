#ifndef JOGO_HPP
#define JOGO_HPP
#include <string>
#include "Jogador.hpp"
#include "Baralho.hpp"
#include "Mercado.hpp"
class Jogo
{
public:
    void iniciar();
    void salvar_jogo(const string &nome_arquivo, const Jogador &jogador1, const Jogador &jogador2, const Baralho &baralho, const Mercado &mercado, int turno);
    void carregar_jogo(const std::string &nome_arquivo, Jogador &jogador1, Jogador &jogador2, Baralho &baralho, Mercado &mercado, int &turno);
};

#endif