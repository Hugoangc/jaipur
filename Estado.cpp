#include "Estado.hpp"
#include <string>
#include <functional>

EstadoJogo::EstadoJogo(const Jogador &j1, const Jogador &j2, const Baralho &b, const Mercado &m, int t)
    : jogador1(j1), jogador2(j2), baralho(b), mercado(m), turno(t) {}

bool EstadoJogo::operator==(const EstadoJogo &outro) const
{
  auto compara_jogadores = [](const Jogador &a, const Jogador &b)
  {
    return a.nome == b.nome &&
           a.pontos == b.pontos &&
           a.selos_excelencia == b.selos_excelencia &&
           a.mao == b.mao &&
           a.camelos == b.camelos;
  };

  return this->turno == outro.turno &&
         compara_jogadores(this->jogador1, outro.jogador1) &&
         compara_jogadores(this->jogador2, outro.jogador2) &&
         this->baralho.cartas == outro.baralho.cartas &&
         this->mercado.getCartas() == outro.mercado.getCartas() &&
         this->mercado.fichas == outro.mercado.fichas &&
         this->mercado.bonus3 == outro.mercado.bonus3 &&
         this->mercado.bonus4 == outro.mercado.bonus4 &&
         this->mercado.bonus5 == outro.mercado.bonus5;
}

size_t EstadoJogo::gerar_hash() const
{
  size_t hash_final = 0;
  std::hash<int> int_hasher;
  std::hash<string> str_hasher;

  auto combinar_hash = [&](size_t novo_hash)
  {
    hash_final ^= novo_hash + 0x9e3779b9 + (hash_final << 6) + (hash_final >> 2);
  };

  combinar_hash(int_hasher(turno));

  auto hash_jogador = [&](const Jogador &j)
  {
    combinar_hash(str_hasher(j.nome));
    combinar_hash(int_hasher(j.pontos));
    combinar_hash(int_hasher(j.selos_excelencia));
    for (const auto &carta : j.mao)
    {
      combinar_hash(int_hasher(carta.tipo));
    }
    for (const auto &camelo : j.camelos)
    {
      combinar_hash(int_hasher(camelo.tipo));
    }
  };

  hash_jogador(jogador1);
  hash_jogador(jogador2);

  for (const auto &carta : baralho.cartas_restantes())
  {
    combinar_hash(int_hasher(carta.tipo));
  }
  for (const auto &carta : mercado.getCartas())
  {
    combinar_hash(int_hasher(carta.tipo));
  }

  return hash_final;
}