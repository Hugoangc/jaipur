#include "Jogo.hpp"
#include "Jogador.hpp"
#include "Baralho.hpp"
#include "Mercado.hpp"
#include "Arvore.hpp"
#include <iostream>
#include <limits>
#include <fstream>
#include <memory>

using namespace std;

vector<EstadoJogo> gerar_jogadas_possiveis(const EstadoJogo &estado_atual)
{
    vector<EstadoJogo> jogadas_possiveis;
    int turno_atual = estado_atual.turno;
    const Jogador &jogador_da_vez = (turno_atual % 2 == 0) ? estado_atual.jogador1 : estado_atual.jogador2;

    // 1. Gerar jogadas de "Comprar UMA carta"
    for (int i = 0; i < estado_atual.mercado.getCartas().size(); ++i)
    {
        const Carta &carta_mercado = estado_atual.mercado.getCartas()[i];
        if (carta_mercado.tipo != CAMELO && jogador_da_vez.mao.size() < 7)
        {
            EstadoJogo novo_estado = estado_atual;
            Jogador &jogador_a_modificar = (turno_atual % 2 == 0) ? novo_estado.jogador1 : novo_estado.jogador2;

            jogador_a_modificar.pegar_carta(carta_mercado);
            novo_estado.mercado.remover_carta(i);
            novo_estado.mercado.repor(novo_estado.baralho);
            novo_estado.turno++;
            jogadas_possiveis.push_back(novo_estado);
        }
    }

    // 2. Gerar jogada de "Pegar TODOS os camelos"
    if (estado_atual.mercado.tem_camelo())
    {
        EstadoJogo novo_estado = estado_atual;
        Jogador &jogador_a_modificar = (turno_atual % 2 == 0) ? novo_estado.jogador1 : novo_estado.jogador2;

        jogador_a_modificar.pegar_camelos_do_mercado(novo_estado.mercado);
        novo_estado.mercado.repor(novo_estado.baralho);
        novo_estado.turno++;
        jogadas_possiveis.push_back(novo_estado);
    }

    // 3. Gerar jogadas de "Vender mercadorias"
    map<TipoCarta, int> contagem_mao;
    for (const auto &carta : jogador_da_vez.mao)
    {
        contagem_mao[carta.tipo]++;
    }

    for (auto const &[tipo, qtd_total] : contagem_mao)
    {
        int min_venda = (tipo == DIAMANTE || tipo == OURO || tipo == PRATA) ? 2 : 1;
        for (int qtd_vender = min_venda; qtd_vender <= qtd_total; ++qtd_vender)
        {
            EstadoJogo novo_estado = estado_atual;
            Jogador &jogador_a_modificar = (turno_atual % 2 == 0) ? novo_estado.jogador1 : novo_estado.jogador2;

            jogador_a_modificar.vender_mercadorias(novo_estado.mercado, tipo, qtd_vender);
            novo_estado.turno++;
            jogadas_possiveis.push_back(novo_estado);
        }
    }

    return jogadas_possiveis;
}

void Jogo::iniciar()
{
    cout << "Bem-vindo ao Jaipur!" << endl;
    string nome1, nome2;
    cout << "Digite o nome do Jogador 1: ";
    getline(cin, nome1);
    cout << "Digite o nome do Jogador 2: ";
    getline(cin, nome2);

    Jogador base_j1(nome1);
    Jogador base_j2(nome2);

    bool jogoFinalizado = false;

    while (!jogoFinalizado)
    {
        Baralho baralho_inicial;
        baralho_inicial.embaralhar();

        Mercado mercado_inicial;
        mercado_inicial.inicializar_fichas();
        mercado_inicial.inicializar(baralho_inicial);

        base_j1.limpar_mao_e_camelos();
        base_j2.limpar_mao_e_camelos();
        base_j1.zerar_pontos();
        base_j2.zerar_pontos();

        for (int i = 0; i < 5; ++i)
        {
            base_j1.pegar_carta(baralho_inicial.comprar());
            base_j2.pegar_carta(baralho_inicial.comprar());
        }

        EstadoJogo estado_inicial(base_j1, base_j2, baralho_inicial, mercado_inicial, 0);
        unique_ptr<ArvoreEstados> arvore = make_unique<ArvoreEstados>(estado_inicial);

        bool rodada_ativa = true;

        while (rodada_ativa)
        {
            const EstadoJogo &estado_atual = arvore->get_estado_atual();

            Jogador j1 = estado_atual.jogador1;
            Jogador j2 = estado_atual.jogador2;
            Mercado m = estado_atual.mercado;
            int turno_atual = estado_atual.turno;

            Jogador &jogador_da_vez = (turno_atual % 2 == 0) ? j1 : j2;

            cout << "\n-------------------------------\n";
            cout << "Turno de " << jogador_da_vez.nome << " (Selos: " << jogador_da_vez.selos_excelencia << ")" << endl;
            m.mostrar();
            jogador_da_vez.mostrar_mao();
            jogador_da_vez.mostrar_camelos();

            cout << "Escolha uma acao:\n";
            cout << "1. Comprar UMA carta do mercado\n";
            cout << "2. Pegar TODOS os camelos\n";
            cout << "3. Trocar cartas com o mercado\n";
            cout << "4. Vender mercadorias\n";
            cout << "5. Encerrar partida atual (termina a rodada)\n";
            // cout << "6. Salvar Jogo\n"; // Salvar/Carregar desativado na versão com árvore
            // cout << "7. Carregar Jogo\n";
            cout << "8. Voltar jogada\n";
            cout << "9. Explorar caminhos alternativos\n";

            int opcao;
            cin >> opcao;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida! Por favor, digite um numero.\n";
                continue;
            }

            if (opcao == 8)
            {
                arvore->voltar_jogada();
                continue;
            }

            if (opcao == 5)
            {
                rodada_ativa = false;
                continue;
            }

            EstadoJogo proximo_estado = estado_atual;
            bool jogada_realizada = false;

            // Obtém referências aos componentes DENTRO do 'proximo_estado' para modificá-los
            Jogador &jogador_a_modificar = (proximo_estado.turno % 2 == 0) ? proximo_estado.jogador1 : proximo_estado.jogador2;
            Baralho &baralho_a_modificar = proximo_estado.baralho;
            Mercado &mercado_a_modificar = proximo_estado.mercado;

            if (opcao == 1)
            {
                int idx;
                cout << "Escolha a carta do mercado (0 a " << mercado_a_modificar.getCartas().size() - 1 << "): ";
                cin >> idx;
                if (!cin.fail() && idx >= 0 && idx < mercado_a_modificar.getCartas().size())
                {
                    Carta escolhida = mercado_a_modificar.getCartas()[idx];
                    if (escolhida.tipo == CAMELO)
                    {
                        cout << "Use a opcao 2 para pegar camelos.\n";
                    }
                    else if (jogador_a_modificar.mao.size() >= 7)
                    {
                        cout << "Voce ja possui 7 cartas na mao.\n";
                    }
                    else
                    {
                        jogador_a_modificar.pegar_carta(escolhida);
                        mercado_a_modificar.remover_carta(idx);
                        mercado_a_modificar.repor(baralho_a_modificar);
                        jogada_realizada = true;
                    }
                }
                else
                {
                    cout << "Entrada ou indice invalido!\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            else if (opcao == 2)
            {
                if (mercado_a_modificar.tem_camelo())
                {
                    jogador_a_modificar.pegar_camelos_do_mercado(mercado_a_modificar);
                    mercado_a_modificar.repor(baralho_a_modificar);
                    jogada_realizada = true;
                }
                else
                {
                    cout << "Nao ha camelos no mercado.\n";
                }
            }

            else if (opcao == 3)
            {

                int qtd;
                cout << "Quantas cartas deseja trocar? (minimo 2): ";
                cin >> qtd;

                if (cin.fail() || qtd < 2)
                {
                    cout << "Quantidade inválida. A troca deve ser de pelo menos 2 cartas.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Valida se a troca excede o limite da mão
                if ((jogador_da_vez.mao.size() - qtd + qtd) > 7)
                {
                    cout << "Essa troca excederia o limite de 7 cartas na mao.\n";
                    continue;
                }

                vector<int> indicesMercado;
                cout << "Digite os " << qtd << " indices das cartas do mercado para PEGAR (ex: 1 3 4): ";
                for (int i = 0; i < qtd; ++i)
                {
                    int idx;
                    cin >> idx;
                    if (cin.fail() || idx < 0 || idx >= m.getCartas().size() || m.getCartas()[idx].tipo == CAMELO)
                    {
                        cout << "Indice '" << idx << "' invalido ou é um camelo. Tente novamente.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        indicesMercado.clear();
                        i = -1; // Reseta o loop
                        continue;
                    }
                    indicesMercado.push_back(idx);
                }

                // --- Coleta de cartas do jogador para a troca ---
                vector<int> indicesMao;
                int camelosParaTrocar = 0;
                cout << "Agora, escolha as " << qtd << " cartas para DAR em troca.\n";
                for (int i = 0; i < qtd; ++i)
                {
                    cout << "Troca " << i + 1 << "/" << qtd << ": carta da mao (m) ou camelo (c)? ";
                    char tipo;
                    cin >> tipo;

                    if (tipo == 'm')
                    {
                        jogador_da_vez.mostrar_mao();
                        cout << "Digite o indice da carta da mao: ";
                        int idx_mao;
                        cin >> idx_mao;
                        if (cin.fail() || idx_mao < 0 || idx_mao >= jogador_da_vez.mao.size())
                        {
                            cout << "Indice da mao inválido. Tente novamente.\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            i--;
                            continue;
                        }
                        indicesMao.push_back(idx_mao);
                    }
                    else if (tipo == 'c')
                    {
                        if (camelosParaTrocar >= jogador_da_vez.camelo_count())
                        {
                            cout << "Você não tem camelos suficientes. Tente novamente.\n";
                            i--;
                            continue;
                        }
                        camelosParaTrocar++;
                    }
                    else
                    {
                        cout << "Opção inválida. Tente 'm' ou 'c'.\n";
                        i--;
                        continue;
                    }
                }

                // o número de cartas a dar deve ser igual ao número de cartas a pegar
                if (indicesMao.size() + camelosParaTrocar != qtd)
                {
                    cout << "Erro: o número de cartas oferecidas não corresponde ao número de cartas a pegar. A troca foi cancelada.\n";
                    continue;
                }

                jogador_a_modificar.realizar_troca_completa(mercado_a_modificar, indicesMercado, indicesMao, camelosParaTrocar);
                jogada_realizada = true;
            }
            else if (opcao == 4) // Vender
            {
                jogador_a_modificar.mostrar_mao();
                cout << "Digite o tipo de carta a vender (0 a 6): ";
                mostrar_tipos_de_carta();
                int tipo_int;
                cin >> tipo_int;
                cout << "Quantas cartas deseja vender?: ";
                int qtd;
                cin >> qtd;

                if (!cin.fail() && tipo_int >= 0 && tipo_int <= 5 && qtd > 0)
                {
                    // Guardar pontos antes de vender para verificar se a venda foi bem-sucedida
                    int pontos_antes = jogador_a_modificar.pontos;
                    jogador_a_modificar.vender_mercadorias(mercado_a_modificar, static_cast<TipoCarta>(tipo_int), qtd);
                    if (jogador_a_modificar.pontos > pontos_antes)
                    {
                        jogada_realizada = true;
                    }
                }
                else
                {
                    cout << "Entrada inválida!\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            else if (opcao == 9)
            {
                cout << "Gerando jogadas alternativas a partir do estado atual... Isso pode levar um momento.\n";
                vector<EstadoJogo> alternativas = gerar_jogadas_possiveis(estado_atual);
                arvore->adicionar_filhos_hipoteticos(alternativas);
                continue;
            }
            else
            {
                cout << "Opcao invalida!\n";
            }

            if (jogada_realizada)
            {
                proximo_estado.turno++;
                if (!arvore->adicionar_jogada(proximo_estado))
                {
                    cout << "Esta jogada levou a um estado já conhecido. Tente outra ação.\n";
                }
            }

            const EstadoJogo &estado_para_verificacao = arvore->get_estado_atual();
            if (estado_para_verificacao.mercado.tres_pilhas_vazias() || estado_para_verificacao.baralho.vazio())
            {
                rodada_ativa = false;
            }
        }

        cout << "\n=== FIM DA RODADA ===\n";
        const EstadoJogo &estado_final = arvore->get_estado_atual();
        arvore->exportar_para_dot("arvore_rodada.dot");
        Jogador j1_final = estado_final.jogador1;
        Jogador j2_final = estado_final.jogador2;

        if (j1_final.camelo_count() > j2_final.camelo_count())
        {
            cout << j1_final.nome << " ganha o bonus de 5 pontos por ter mais camelos!\n";
            j1_final.pontos += 5;
        }
        else if (j2_final.camelo_count() > j1_final.camelo_count())
        {
            cout << j2_final.nome << " ganha o bonus de 5 pontos por ter mais camelos!\n";
            j2_final.pontos += 5;
        }

        cout << "\nPontuacao da rodada:\n";
        cout << j1_final.nome << ": " << j1_final.pontos << " pontos\n";
        cout << j2_final.nome << ": " << j2_final.pontos << " pontos\n";

        if (j1_final.pontos > j2_final.pontos)
        {
            cout << j1_final.nome << " venceu a rodada!\n";
            base_j1.selos_excelencia++;
        }
        else if (j2_final.pontos > j1_final.pontos)
        {
            cout << j2_final.nome << " venceu a rodada!\n";
            base_j2.selos_excelencia++;
        }
        else
        {
            cout << "Rodada empatada!\n";
        }

        if (base_j1.selos_excelencia == 2 || base_j2.selos_excelencia == 2)
        {
            jogoFinalizado = true;
        }
        else
        {
            cout << "\nPressione Enter para iniciar a próxima rodada...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }

    cout << "\n=== FIM DE JOGO ===\n";
    if (base_j1.selos_excelencia > base_j2.selos_excelencia)
    {
        cout << base_j1.nome << " venceu o jogo com 2 Selos de Excelencia!\n";
    }
    else
    {
        cout << base_j2.nome << " venceu o jogo com 2 Selos de Excelencia!\n";
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
    out << jogador1.selos_excelencia << '\n';
    out << jogador1.mao.size() << '\n';
    for (const auto &carta : jogador1.mao)
        out << carta.tipo << ' ';
    out << '\n';
    out << jogador1.camelo_count() << '\n';

    out << jogador2.nome << '\n';
    out << jogador2.pontos << '\n';
    out << jogador2.selos_excelencia << '\n';
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

void Jogo::carregar_jogo(const string &nome_arquivo, Jogador &jogador1, Jogador &jogador2, Baralho &baralho, Mercado &mercado, int &turno)
{
    ifstream in(nome_arquivo);
    if (!in)
    {
        cout << "Erro ao carregar o jogo." << endl;
        return;
    }

    string nome;
    int pontos, tamanho_mao, qtd_camelos, tamanho_baralho, tamanho_mercado, tipo;

    // Jogador 1
    getline(in, jogador1.nome);
    in >> jogador1.pontos;
    in >> jogador1.selos_excelencia;

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

    in.ignore();

    // Jogador 2
    getline(in, jogador2.nome);
    in >> jogador2.pontos;
    in >> jogador2.selos_excelencia;

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
    // in >> tamanho_mercado;
    // mercado.getCartas().clear();
    // for (int i = 0; i < tamanho_mercado; ++i)
    // {
    //     in >> tipo;
    //     mercado.getCartas().push_back(Carta(static_cast<TipoCarta>(tipo)));
    // }
    in >> tamanho_mercado;
    auto cartas_mercado = mercado.getCartas();
    cartas_mercado.clear();
    for (int i = 0; i < tamanho_mercado; ++i)
    {
        in >> tipo;
        cartas_mercado.push_back(Carta(static_cast<TipoCarta>(tipo)));
    }

    // Turno
    in >> turno;

    in.close();
    cout << "Jogo carregado com sucesso de '" << nome_arquivo << "'!\n";
}
