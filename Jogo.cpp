#include "Jogo.hpp"
#include "Jogador.hpp"
#include "Baralho.hpp"
#include "Mercado.hpp"
#include <iostream>
#include <limits>
#include <fstream>
using namespace std;
void Jogo::iniciar()
{
    cout << "Bem-vindo ao Jaipur!" << endl;
    string nome1, nome2;
    cout << "Digite o nome do Jogador 1: ";
    getline(cin, nome1);
    cout << "Digite o nome do Jogador 2: ";
    getline(cin, nome2);

    Jogador jogador1(nome1);
    Jogador jogador2(nome2);

    int selosJogador1 = 0;
    int selosJogador2 = 0;
    bool jogoFinalizado = false;

    while (!jogoFinalizado)
    {
        Baralho baralho;
        baralho.embaralhar();

        Mercado mercado;
        mercado.inicializar_fichas();
        mercado.inicializar(baralho);

        jogador1.limpar_mao_e_camelos();
        jogador2.limpar_mao_e_camelos();

        for (int i = 0; i < 5; ++i)
        {
            jogador1.pegar_carta(baralho.comprar());
            jogador2.pegar_carta(baralho.comprar());
        }

        int turno = 0;
        bool rodada_ativa = true;

        while (rodada_ativa)
        {
            Jogador &atual = (turno % 2 == 0) ? jogador1 : jogador2;

            cout << "\n-------------------------------\n";
            cout << "Turno de " << atual.nome << endl;
            mercado.mostrar();
            atual.mostrar_mao();
            atual.mostrar_camelos();

            cout << "Escolha uma acao:\n";
            cout << "1. Comprar UMA carta do mercado\n";
            cout << "2. Pegar TODOS os camelos\n";
            cout << "3. Trocar cartas com o mercado\n";
            cout << "4. Vender mercadorias\n";
            cout << "5. Encerrar partida atual\n";
            cout << "6. Salvar Jogo\n";
            cout << "7. Carregar Jogo\n";

            int opcao;
            cin >> opcao;

            if (opcao == 1)
            {
                int idx;
                cout << "Escolha a carta do mercado (0 a " << mercado.getCartas().size() - 1 << "): ";
                cin >> idx;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Entrada inválida!\n";
                    continue;
                }
                if (idx >= 0 && idx < mercado.getCartas().size())
                {
                    Carta escolhida = mercado.getCartas()[idx];
                    if (escolhida.tipo == CAMELO)
                    {
                        cout << "Use a opcao 2 para pegar camelos.\n";
                    }
                    else if (atual.mao.size() >= 7)
                    {
                        cout << "Voce ja possui 7 cartas na mao.\n";
                    }
                    else
                    {
                        atual.pegar_carta(escolhida);
                        mercado.remover_carta(idx);
                        mercado.repor(baralho);
                    }
                }
            }
            else if (opcao == 2)
            {
                if (mercado.tem_camelo())
                {
                    atual.pegar_camelos_do_mercado(mercado);
                    mercado.repor(baralho);
                }
                else
                {
                    cout << "Nao ha camelos no mercado.\n";
                }
            }
            else if (opcao == 3)
            {
                int qtd;
                cout << "Quantas cartas deseja pegar do mercado? (mínimo 2): ";
                cin >> qtd;

                if (qtd < 2)
                {
                    cout << "Deve pegar pelo menos 2 cartas.\n";
                    continue;
                }

                vector<int> indicesMercado(qtd);
                for (int i = 0; i < qtd; ++i)
                {
                    cout << "Indice da carta no mercado (" << i + 1 << "): ";
                    cin >> indicesMercado[i];
                    if (mercado.getCartas()[indicesMercado[i]].tipo == CAMELO)
                    {
                        cout << "Nao e permitido trocar por camelos.\n";
                        --i;
                    }
                }

                vector<Carta> cartasTroca;
                vector<int> indicesMao;

                for (int i = 0; i < qtd; ++i)
                {
                    cout << "Trocar por carta da mao (m) ou camelo (c)? ";
                    char tipo;
                    cin >> tipo;
                    if (tipo == 'm')
                    {
                        atual.mostrar_mao();
                        int idx;
                        cout << "Indice da carta da mao: ";
                        cin >> idx;
                        indicesMao.push_back(idx);
                    }
                    else if (tipo == 'c')
                    {
                        if (atual.camelo_vazio())
                        {
                            cout << "Voce nao tem camelos suficientes.\n";
                            --i;
                            continue;
                        }
                        cartasTroca.push_back(atual.remover_um_camelo());
                    }
                }

                // if (cartasTroca.size() == qtd)
                // {
                atual.trocar_cartas(mercado, indicesMercado, indicesMao);
                mercado.repor(baralho);
                // }
                // else
                // {
                //     cout << "A troca falhou.\n";
                // }
            }
            else if (opcao == 4)
            {
                atual.mostrar_mao();
                cout << "Digite o tipo de carta a vender (0 a 6): ";
                mostrar_tipos_de_carta();
                int tipo;
                cin >> tipo;
                cout << "Quantas cartas deseja vender?: ";
                int qtd;
                cin >> qtd;
                atual.vender_mercadorias(mercado, static_cast<TipoCarta>(tipo), qtd);
            }
            else if (opcao == 5)
            {
                rodada_ativa = false;
            }
            else if (opcao == 6)
            {
                salvar_jogo("save.txt", jogador1, jogador2, baralho, mercado, turno);
                cout << "Partida salva. Voce pode fechar o jogo com seguranca.\n";
                return;
            }
            else if (opcao == 7)
            {
                carregar_jogo("save.txt", jogador1, jogador2, baralho, mercado, turno);
                cout << "Partida carregada.\n";
                        }
            else
            {
                cout << "Opcao invalida!\n";
            }

            turno++;

            if (mercado.tres_pilhas_vazias() || baralho.vazio())
            {
                rodada_ativa = false;
            }
        }

        // CAMEL BONUS
        if (jogador1.camelo_count() > jogador2.camelo_count())
        {
            jogador1.pontos += 5;
        }
        else if (jogador2.camelo_count() > jogador1.camelo_count())
        {
            jogador2.pontos += 5;
        }

        // Mostrar resultado da rodada
        cout << "\nPontuacao da rodada:\n";
        cout << jogador1.nome << ": " << jogador1.pontos << " pontos\n";
        cout << jogador2.nome << ": " << jogador2.pontos << " pontos\n";

        if (jogador1.pontos > jogador2.pontos)
        {
            cout << jogador1.nome << " venceu a rodada!\n";
            selosJogador1++;
        }
        else if (jogador2.pontos > jogador1.pontos)
        {
            cout << jogador2.nome << " venceu a rodada!\n";
            selosJogador2++;
        }
        else
        {
            cout << "Rodada empatada!\n";
        }

        jogador1.zerar_pontos();
        jogador2.zerar_pontos();

        if (selosJogador1 == 2 || selosJogador2 == 2)
        {
            jogoFinalizado = true;
        }
    }

    cout << "\n=== FIM DE JOGO ===\n";
    if (selosJogador1 > selosJogador2)
    {
        cout << jogador1.nome << " venceu o jogo com 2 Selos de Excelencia!\n";
    }
    else
    {
        cout << jogador2.nome << " venceu o jogo com 2 Selos de Excelencia!\n";
    }
}

void Jogo::salvar_jogo(const string &nome_arquivo, const Jogador &jogador1, const Jogador &jogador2, const Baralho &baralho, const Mercado &mercado, int turno)
{
    ofstream out(nome_arquivo);
    if (!out)
    {
        cout << "Erro ao salvar o jogo." << endl;
        return;
    }

    out << jogador1.nome << '\n';
    out << jogador1.pontos << '\n';
    out << jogador1.mao.size() << '\n';
    for (const auto &carta : jogador1.mao)
        out << carta.tipo << ' ';
    out << '\n';
    out << jogador1.camelo_count() << '\n';

    out << jogador2.nome << '\n';
    out << jogador2.pontos << '\n';
    out << jogador2.mao.size() << '\n';
    for (const auto &carta : jogador2.mao)
        out << carta.tipo << ' ';
    out << '\n';
    out << jogador2.camelo_count() << '\n';

    out << baralho.tamanho() << '\n';
    for (const auto &carta : baralho.cartas_restantes())
        out << carta.tipo << ' ';
    out << '\n';

    out << mercado.getCartas().size() << '\n';
    for (const auto &carta : mercado.getCartas())
        out << carta.tipo << ' ';
    out << '\n';

    out << turno << '\n';

    out.close();
    cout << "Jogo salvo com sucesso em '" << nome_arquivo << "'!\n";
}

void Jogo::carregar_jogo(const std::string &nome_arquivo, Jogador &jogador1, Jogador &jogador2, Baralho &baralho, Mercado &mercado, int &turno)
{
    std::ifstream in(nome_arquivo);
    if (!in)
    {
        std::cout << "Erro ao carregar o jogo." << std::endl;
        return;
    }

    std::string nome;
    int pontos, tamanho_mao, qtd_camelos, tamanho_baralho, tamanho_mercado, tipo;

    // Jogador 1
    std::getline(in, jogador1.nome);
    in >> jogador1.pontos;

    in >> tamanho_mao;
    jogador1.mao.clear();
    for (int i = 0; i < tamanho_mao; ++i)
    {
        in >> tipo;
        jogador1.mao.push_back(Carta(static_cast<TipoCarta>(tipo)));
    }

    in >> qtd_camelos;
    jogador1.camelos.clear();
    for (int i = 0; i < qtd_camelos; ++i)
    {
        jogador1.camelos.push_back(Carta(CAMELO));
    }

    in.ignore(); // pular quebra de linha

    // Jogador 2
    std::getline(in, jogador2.nome);
    in >> jogador2.pontos;

    in >> tamanho_mao;
    jogador2.mao.clear();
    for (int i = 0; i < tamanho_mao; ++i)
    {
        in >> tipo;
        jogador2.mao.push_back(Carta(static_cast<TipoCarta>(tipo)));
    }

    in >> qtd_camelos;
    jogador2.camelos.clear();
    for (int i = 0; i < qtd_camelos; ++i)
    {
        jogador2.camelos.push_back(Carta(CAMELO));
    }

    // Baralho
    in >> tamanho_baralho;
    baralho.cartas.clear();
    for (int i = 0; i < tamanho_baralho; ++i)
    {
        in >> tipo;
        baralho.cartas.push_back(Carta(static_cast<TipoCarta>(tipo)));
    }

    // Mercado
    in >> tamanho_mercado;
    mercado.getCartas().clear();
    for (int i = 0; i < tamanho_mercado; ++i)
    {
        in >> tipo;
        mercado.getCartas().push_back(Carta(static_cast<TipoCarta>(tipo)));
    }

    // Turno
    in >> turno;

    in.close();
    std::cout << "Jogo carregado com sucesso de '" << nome_arquivo << "'!\n";
}
