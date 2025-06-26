#include "Arvore.hpp"
#include <iostream>
#include <limits>
#include <fstream>
#include <memory>
#include <sstream>

using namespace std;
ArvoreEstados::ArvoreEstados(const EstadoJogo &estado_inicial)
{
  raiz = new NoArvore(estado_inicial);
  no_atual = raiz;
  hashes_conhecidos.insert(raiz->estado.gerar_hash());
  cout << "Arvore de estados iniciada. Estado inicial registrado.\n";
}

ArvoreEstados::~ArvoreEstados()
{
  destruir_recursivamente(raiz);
}

void ArvoreEstados::destruir_recursivamente(NoArvore *no)
{
  if (!no)
    return;
  for (NoArvore *filho : no->filhos)
  {
    destruir_recursivamente(filho);
  }
  delete no;
}

const EstadoJogo &ArvoreEstados::get_estado_atual() const
{
  return no_atual->estado;
}

bool ArvoreEstados::adicionar_jogada(const EstadoJogo &novo_estado)
{
  size_t hash_novo = novo_estado.gerar_hash();

  if (hashes_conhecidos.count(hash_novo))
  {
    cout << "Aviso: Esta jogada resulta em um estado que já existe na arvore. Acao nao permitida.\n";
    return false;
  }

  NoArvore *novo_no = new NoArvore(novo_estado, false);
  novo_no->pai = no_atual;
  no_atual->filhos.push_back(novo_no);
  no_atual = novo_no;
  hashes_conhecidos.insert(hash_novo);
  return true;
}

bool ArvoreEstados::voltar_jogada()
{
  if (no_atual->pai != nullptr)
  {
    no_atual = no_atual->pai;
    cout << "Jogada desfeita. Retornando ao estado anterior.\n";
    return true;
  }

  cout << "Nao e possivel voltar. Você esta no estado inicial da rodada.\n";
  return false;
}

void ArvoreEstados::exportar_no_recursivamente(NoArvore *no, ofstream &arquivo) const
{
  if (!no)
    return;

  const EstadoJogo &estado = no->estado;
  const Jogador &jogador_da_vez = (estado.turno % 2 == 0) ? estado.jogador1 : estado.jogador2;

  stringstream label_stream;
  label_stream << "Turno: " << estado.turno << " (" << jogador_da_vez.nome << ")\\n"
               << "--------------------------\\n"
               << estado.jogador1.nome << " | Mão: " << estado.jogador1.mao.size()
               << " | Camelos: " << estado.jogador1.camelo_count() << " | Pts: " << estado.jogador1.pontos
               << " | Bônus: " << estado.jogador1.fichas_bonus
               << " | Selos: " << estado.jogador1.selos_excelencia << "\\n";

  label_stream << " Mão: [";
  for (size_t i = 0; i < estado.jogador1.mao.size(); ++i)
  {
    label_stream << estado.jogador1.mao[i].nome()[0];
    if (i < estado.jogador1.mao.size() - 1)
    {
      label_stream << ", ";
    }
  }
  label_stream << "]\\n";

  label_stream << estado.jogador2.nome << " | Mão: " << estado.jogador2.mao.size()
               << " | Camelos: " << estado.jogador2.camelo_count() << " | Pts: " << estado.jogador2.pontos
               << " | Bônus: " << estado.jogador2.fichas_bonus
               << " | Selos: " << estado.jogador2.selos_excelencia << "\\n";

  label_stream << " Mão: [";
  for (size_t i = 0; i < estado.jogador2.mao.size(); ++i)
  {
    label_stream << estado.jogador2.mao[i].nome()[0];
    if (i < estado.jogador2.mao.size() - 1)
    {
      label_stream << ", ";
    }
  }
  label_stream << "]\\n";

  label_stream << "--------------------------\\n"
               << "Mercado: ";

  for (const auto &carta : estado.mercado.getCartas())
  {
    label_stream << carta.nome()[0] << " ";
  }
  label_stream << "\\n"
               << "Baralho: " << estado.baralho.tamanho() << " cartas";

  if (no->eh_hipotetico)
  {
    arquivo << "  \"" << no << "\" [label=\"" << label_stream.str() << "\", style=dashed, color=gray];\n";
  }
  else
  {
    if (no == no_atual)
    {
      arquivo << "  \"" << no << "\" [label=\"" << label_stream.str() << "\", style=filled, color=lightblue];\n";
    }
    else
    {
      arquivo << "  \"" << no << "\" [label=\"" << label_stream.str() << "\"];\n";
    }
  }

  for (NoArvore *filho : no->filhos)
  {
    if (filho->eh_hipotetico)
    {
      arquivo << "  \"" << no << "\" -> \"" << filho << "\" [style=dashed, color=gray];\n";
    }
    else
    {
      arquivo << "  \"" << no << "\" -> \"" << filho << "\";\n";
    }
    exportar_no_recursivamente(filho, arquivo);
  }
}
void ArvoreEstados::exportar_para_dot(const string &nome_arquivo) const
{
  ofstream arquivo(nome_arquivo);
  if (!arquivo.is_open())
  {
    cout << "Erro ao abrir o arquivo para exportacao." << endl;
    return;
  }

  arquivo << "digraph ArvoreDeEstados {\n";
  arquivo << "  node [shape=box];\n";

  exportar_no_recursivamente(raiz, arquivo);

  arquivo << "}\n";
  arquivo.close();
  cout << "Arvore exportada para " << nome_arquivo << endl;
}

void ArvoreEstados::adicionar_filhos_hipoteticos(const vector<EstadoJogo> &estados_filhos)
{
  for (const auto &estado : estados_filhos)
  {
    size_t hash_novo = estado.gerar_hash();
    if (hashes_conhecidos.count(hash_novo) == 0)
    {
      NoArvore *novo_no = new NoArvore(estado, true);
      novo_no->pai = no_atual;
      no_atual->filhos.push_back(novo_no);
      hashes_conhecidos.insert(hash_novo);
    }
  }
  cout << estados_filhos.size() << " caminhos alternativos foram explorados e adicionados a arvore.\n";
}