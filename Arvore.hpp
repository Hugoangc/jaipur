#ifndef ARVORE_ESTADOS_HPP
#define ARVORE_ESTADOS_HPP

#include "Estado.hpp"
#include <vector>
#include <unordered_set>

// Estrutura do Nó da Árvore
struct NoArvore
{
  EstadoJogo estado;
  NoArvore *pai;
  std::vector<NoArvore *> filhos;
  bool eh_hipotetico;

  NoArvore(const EstadoJogo &e, bool hipotetico = false)
      : estado(e), pai(nullptr), eh_hipotetico(hipotetico) {}
};

class ArvoreEstados
{
private:
  NoArvore *raiz;
  NoArvore *no_atual;
  std::unordered_set<size_t> hashes_conhecidos;

public:
  ArvoreEstados(const EstadoJogo &estado_inicial);
  ~ArvoreEstados(); 
  void adicionar_filhos_hipoteticos(const vector<EstadoJogo> &estados_filhos);


  bool adicionar_jogada(const EstadoJogo &novo_estado);
  bool voltar_jogada();
  const EstadoJogo &get_estado_atual() const;
  void exportar_para_dot(const string &nome_arquivo) const;

private:
  void destruir_recursivamente(NoArvore *no);
  void exportar_no_recursivamente(NoArvore *no, ofstream &arquivo) const;
};

#endif