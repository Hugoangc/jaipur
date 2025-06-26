#ifndef ESTADO_JOGO_HPP
#define ESTADO_JOGO_HPP

#include "Jogador.hpp"
#include "Baralho.hpp"
#include "Mercado.hpp"

class EstadoJogo
{
public:
  Jogador jogador1;
  Jogador jogador2;
  Baralho baralho;
  Mercado mercado;
  int turno;

  EstadoJogo(const Jogador &j1, const Jogador &j2, const Baralho &b, const Mercado &m, int t);

  bool operator==(const EstadoJogo &outro) const;

  size_t gerar_hash() const;
};

struct HashEstadoJogo
{
  size_t operator()(const EstadoJogo &estado) const
  {
    return estado.gerar_hash();
  }
};

#endif