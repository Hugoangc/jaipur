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

    // 1. Gerar jogadas de "Comprar UMA carta" (não imprime nada, sem alterações)
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

    // 2. Gerar jogada de "Pegar TODOS os camelos" (não imprime nada, sem alterações)
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
            jogador_a_modificar.vender_mercadorias(novo_estado.mercado, tipo, qtd_vender, false);

            novo_estado.turno++;
            jogadas_possiveis.push_back(novo_estado);
        }
    }

    // 4. Gerar jogadas de "Trocar Cartas"
    vector<int> indices_mercado_disponiveis;
    for (int i = 0; i < estado_atual.mercado.getCartas().size(); ++i)
    {
        if (estado_atual.mercado.getCartas()[i].tipo != CAMELO)
        {
            indices_mercado_disponiveis.push_back(i);
        }
    }

    int total_cartas_jogador = jogador_da_vez.mao.size() + jogador_da_vez.camelo_count();

    for (int k = 2; k <= indices_mercado_disponiveis.size() && k <= total_cartas_jogador; k++)
    {
        vector<vector<int>> combos_mercado;
        vector<int> combo_mercado_atual;
        gerar_combinacoes_indices(0, k, combo_mercado_atual, combos_mercado, indices_mercado_disponiveis.size());

        vector<vector<int>> combos_jogador;
        vector<int> combo_jogador_atual;
        gerar_combinacoes_indices(0, k, combo_jogador_atual, combos_jogador, total_cartas_jogador);

        for (const auto &combo_mercado_indices_relativos : combos_mercado)
        {
            vector<int> combo_mercado_reais;
            for (int idx_relativo : combo_mercado_indices_relativos)
            {
                combo_mercado_reais.push_back(indices_mercado_disponiveis[idx_relativo]);
            }

            for (const auto &combo_jogador_indices : combos_jogador)
            {
                vector<int> indices_mao_para_dar;
                int camelos_para_dar = 0;
                int tamanho_mao = jogador_da_vez.mao.size();

                for (int idx_geral : combo_jogador_indices)
                {
                    if (idx_geral < tamanho_mao)
                    {
                        indices_mao_para_dar.push_back(idx_geral);
                    }
                    else
                    {
                        camelos_para_dar++;
                    }
                }

                if ((tamanho_mao - indices_mao_para_dar.size() + k) > 7)
                {
                    continue;
                }

                EstadoJogo novo_estado = estado_atual;
                Jogador &jogador_a_modificar = (turno_atual % 2 == 0) ? novo_estado.jogador1 : novo_estado.jogador2;
                jogador_a_modificar.realizar_troca_completa(novo_estado.mercado, combo_mercado_reais, indices_mao_para_dar, camelos_para_dar, false);

                novo_estado.turno++;
                jogadas_possiveis.push_back(novo_estado);
            }
        }
    }

    return jogadas_possiveis;
}

void gerar_combinacoes_indices(int start, int k, vector<int> &combo_atual, vector<vector<int>> &todas_as_combos, int n)
{
    if (k == 0)
    {
        todas_as_combos.push_back(combo_atual);
        return;
    }
    for (int i = start; i <= n - k; ++i)
    {
        combo_atual.push_back(i);
        gerar_combinacoes_indices(i + 1, k - 1, combo_atual, todas_as_combos, n);
        combo_atual.pop_back();
    }
}

// void Jogo::iniciar()
// {
//     cout << "Bem-vindo ao Jaipur!" << endl;
//     string nome1 = "MinMax", nome2 = "PodaAB";
//     // cout << "Digite o nome do Jogador 1: ";
//     getline(cin, nome1);
//     Jogador base_j1(nome1);
//     Jogador base_j2(nome2);

//     bool jogoFinalizado = false;
//     const int PROFUNDIDADE_DE_BUSCA = 2;
//     const int PROFUNDIDADE_ALFA_BETA = 4;

//     while (!jogoFinalizado)
//     {
//         Baralho baralho_inicial;
//         baralho_inicial.embaralhar();

//         Mercado mercado_inicial;
//         mercado_inicial.inicializar_fichas();
//         mercado_inicial.inicializar(baralho_inicial);

//         base_j1.limpar_mao_e_camelos();
//         base_j2.limpar_mao_e_camelos();
//         base_j1.zerar_pontos();
//         base_j2.zerar_pontos();

//         for (int i = 0; i < 5; ++i)
//         {
//             base_j1.pegar_carta(baralho_inicial.comprar());
//             base_j2.pegar_carta(baralho_inicial.comprar());
//         }

//         EstadoJogo estado_inicial(base_j1, base_j2, baralho_inicial, mercado_inicial, 0);
//         unique_ptr<ArvoreEstados> arvore = make_unique<ArvoreEstados>(estado_inicial);

//         bool rodada_ativa = true;

//         while (rodada_ativa)
//         {

//             const EstadoJogo &estado_atual = arvore->get_estado_atual();

//             Jogador j1 = estado_atual.jogador1;
//             Jogador j2 = estado_atual.jogador2;
//             Mercado m = estado_atual.mercado;
//             int turno_atual = estado_atual.turno;

//             Jogador &jogador_da_vez = (turno_atual % 2 == 0) ? j1 : j2;

//             cout << "\n-------------------------------\n";
//             cout << "Turno de " << jogador_da_vez.nome << " (Selos: " << jogador_da_vez.selos_excelencia << ")" << endl;
//             m.mostrar();
//             jogador_da_vez.mostrar_mao();
//             jogador_da_vez.mostrar_camelos();
//             if (turno_atual % 2 == 0)
//             {
//                 cout << "Escolha uma acao:\n";
//                 cout << "1. Comprar UMA carta do mercado\n";
//                 cout << "2. Pegar TODOS os camelos\n";
//                 cout << "3. Trocar cartas com o mercado\n";
//                 cout << "4. Vender mercadorias\n";
//                 cout << "5. Encerrar partida atual (termina a rodada)\n";
//                 // cout << "6. Salvar Jogo\n"; // Salvar/Carregar desativado na versão com árvore
//                 // cout << "7. Carregar Jogo\n";
//                 cout << "8. Voltar jogada\n";
//                 cout << "9. Explorar caminhos alternativos\n";

//                 int opcao;
//                 cin >> opcao;

//                 if (cin.fail())
//                 {
//                     cin.clear();
//                     cin.ignore(numeric_limits<streamsize>::max(), '\n');
//                     cout << "Entrada invalida! Por favor, digite um numero.\n";
//                     continue;
//                 }

//                 if (opcao == 8)
//                 {
//                     arvore->voltar_jogada();
//                     continue;
//                 }

//                 if (opcao == 5)
//                 {
//                     rodada_ativa = false;
//                     continue;
//                 }

//                 EstadoJogo proximo_estado = estado_atual;
//                 bool jogada_realizada = false;

//                 // Obtém referências aos componentes DENTRO do 'proximo_estado' para modificá-los
//                 Jogador &jogador_a_modificar = (proximo_estado.turno % 2 == 0) ? proximo_estado.jogador1 : proximo_estado.jogador2;
//                 Baralho &baralho_a_modificar = proximo_estado.baralho;
//                 Mercado &mercado_a_modificar = proximo_estado.mercado;

//                 if (opcao == 1)
//                 {
//                     int idx;
//                     cout << "Escolha a carta do mercado (0 a " << mercado_a_modificar.getCartas().size() - 1 << "): ";
//                     cin >> idx;
//                     if (!cin.fail() && idx >= 0 && idx < mercado_a_modificar.getCartas().size())
//                     {
//                         Carta escolhida = mercado_a_modificar.getCartas()[idx];
//                         if (escolhida.tipo == CAMELO)
//                         {
//                             cout << "Use a opcao 2 para pegar camelos.\n";
//                         }
//                         else if (jogador_a_modificar.mao.size() >= 7)
//                         {
//                             cout << "Voce ja possui 7 cartas na mao.\n";
//                         }
//                         else
//                         {
//                             jogador_a_modificar.pegar_carta(escolhida);
//                             mercado_a_modificar.remover_carta(idx);
//                             mercado_a_modificar.repor(baralho_a_modificar);
//                             jogada_realizada = true;
//                         }
//                     }
//                     else
//                     {
//                         cout << "Entrada ou indice invalido!\n";
//                         cin.clear();
//                         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//                     }
//                 }
//                 else if (opcao == 2)
//                 {
//                     if (mercado_a_modificar.tem_camelo())
//                     {
//                         jogador_a_modificar.pegar_camelos_do_mercado(mercado_a_modificar);
//                         mercado_a_modificar.repor(baralho_a_modificar);
//                         jogada_realizada = true;
//                     }
//                     else
//                     {
//                         cout << "Nao ha camelos no mercado.\n";
//                     }
//                 }

//                 else if (opcao == 3)
//                 {

//                     int qtd;
//                     cout << "Quantas cartas deseja trocar? (minimo 2): ";
//                     cin >> qtd;

//                     if (cin.fail() || qtd < 2)
//                     {
//                         cout << "Quantidade inválida. A troca deve ser de pelo menos 2 cartas.\n";
//                         cin.clear();
//                         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//                         continue;
//                     }

//                     // Valida se a troca excede o limite da mão
//                     if ((jogador_da_vez.mao.size() - qtd + qtd) > 7)
//                     {
//                         cout << "Essa troca excederia o limite de 7 cartas na mao.\n";
//                         continue;
//                     }

//                     vector<int> indicesMercado;
//                     cout << "Digite os " << qtd << " indices das cartas do mercado para PEGAR (ex: 1 3 4): ";
//                     for (int i = 0; i < qtd; ++i)
//                     {
//                         int idx;
//                         cin >> idx;
//                         if (cin.fail() || idx < 0 || idx >= m.getCartas().size() || m.getCartas()[idx].tipo == CAMELO)
//                         {
//                             cout << "Indice '" << idx << "' invalido ou é um camelo. Tente novamente.\n";
//                             cin.clear();
//                             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//                             indicesMercado.clear();
//                             i = -1; // Reseta o loop
//                             continue;
//                         }
//                         indicesMercado.push_back(idx);
//                     }

//                     // --- Coleta de cartas do jogador para a troca ---
//                     vector<int> indicesMao;
//                     int camelosParaTrocar = 0;
//                     cout << "Agora, escolha as " << qtd << " cartas para DAR em troca.\n";
//                     for (int i = 0; i < qtd; ++i)
//                     {
//                         cout << "Troca " << i + 1 << "/" << qtd << ": carta da mao (m) ou camelo (c)? ";
//                         char tipo;
//                         cin >> tipo;

//                         if (tipo == 'm')
//                         {
//                             jogador_da_vez.mostrar_mao();
//                             cout << "Digite o indice da carta da mao: ";
//                             int idx_mao;
//                             cin >> idx_mao;
//                             if (cin.fail() || idx_mao < 0 || idx_mao >= jogador_da_vez.mao.size())
//                             {
//                                 cout << "Indice da mao inválido. Tente novamente.\n";
//                                 cin.clear();
//                                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
//                                 i--;
//                                 continue;
//                             }
//                             indicesMao.push_back(idx_mao);
//                         }
//                         else if (tipo == 'c')
//                         {
//                             if (camelosParaTrocar >= jogador_da_vez.camelo_count())
//                             {
//                                 cout << "Você não tem camelos suficientes. Tente novamente.\n";
//                                 i--;
//                                 continue;
//                             }
//                             camelosParaTrocar++;
//                         }
//                         else
//                         {
//                             cout << "Opção inválida. Tente 'm' ou 'c'.\n";
//                             i--;
//                             continue;
//                         }
//                     }

//                     // o número de cartas a dar deve ser igual ao número de cartas a pegar
//                     if (indicesMao.size() + camelosParaTrocar != qtd)
//                     {
//                         cout << "Erro: o número de cartas oferecidas não corresponde ao número de cartas a pegar. A troca foi cancelada.\n";
//                         continue;
//                     }

//                     jogador_a_modificar.realizar_troca_completa(mercado_a_modificar, indicesMercado, indicesMao, camelosParaTrocar);
//                     jogada_realizada = true;
//                 }
//                 else if (opcao == 4) // Vender
//                 {
//                     jogador_a_modificar.mostrar_mao();
//                     cout << "Digite o tipo de carta a vender (0 a 6): ";
//                     mostrar_tipos_de_carta();
//                     int tipo_int;
//                     cin >> tipo_int;
//                     cout << "Quantas cartas deseja vender?: ";
//                     int qtd;
//                     cin >> qtd;

//                     if (!cin.fail() && tipo_int >= 0 && tipo_int <= 5 && qtd > 0)
//                     {
//                         // Guardar pontos antes de vender para verificar se a venda foi bem-sucedida
//                         int pontos_antes = jogador_a_modificar.pontos;
//                         jogador_a_modificar.vender_mercadorias(mercado_a_modificar, static_cast<TipoCarta>(tipo_int), qtd);
//                         if (jogador_a_modificar.pontos > pontos_antes)
//                         {
//                             jogada_realizada = true;
//                         }
//                     }
//                     else
//                     {
//                         cout << "Entrada inválida!\n";
//                         cin.clear();
//                         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//                     }
//                 }
//                 else if (opcao == 9)
//                 {
//                     cout << "Gerando jogadas alternativas a partir do estado atual... Isso pode levar um momento.\n";
//                     vector<EstadoJogo> alternativas = gerar_jogadas_possiveis(estado_atual);
//                     arvore->adicionar_filhos_hipoteticos(alternativas);
//                     continue;
//                 }
//                 else
//                 {
//                     cout << "Opcao invalida!\n";
//                 }

//                 if (jogada_realizada)
//                 {
//                     proximo_estado.turno++;
//                     if (!arvore->adicionar_jogada(proximo_estado))
//                     {
//                         cout << "Esta jogada levou a um estado já conhecido. Tente outra ação.\n";
//                     }
//                 }
//             }
//             else
//             {
//                 // A IA não vê um menu. Ela "pensa" e toma uma decisão.
//                 EstadoJogo jogada_da_ia = encontrar_melhor_jogada_minmax(estado_atual, PROFUNDIDADE_DE_BUSCA);

//                 // Adiciona a jogada escolhida pela IA diretamente à árvore
//                 arvore->adicionar_jogada(jogada_da_ia);
//             }

//             const EstadoJogo &estado_para_verificacao = arvore->get_estado_atual();
//             if (estado_para_verificacao.mercado.tres_pilhas_vazias() || estado_para_verificacao.baralho.vazio())
//             {
//                 rodada_ativa = false;
//             }
//         }

//         cout << "\n=== FIM DA RODADA ===\n";
//         const EstadoJogo &estado_final = arvore->get_estado_atual();
//         arvore->exportar_para_dot("arvore_rodada.dot");
//         Jogador j1_final = estado_final.jogador1;
//         Jogador j2_final = estado_final.jogador2;

//         if (j1_final.camelo_count() > j2_final.camelo_count())
//         {
//             cout << j1_final.nome << " ganha o bonus de 5 pontos por ter mais camelos!\n";
//             j1_final.pontos += 5;
//         }
//         else if (j2_final.camelo_count() > j1_final.camelo_count())
//         {
//             cout << j2_final.nome << " ganha o bonus de 5 pontos por ter mais camelos!\n";
//             j2_final.pontos += 5;
//         }

//         cout << "\nPontuacao da rodada:\n";
//         cout << j1_final.nome << ": " << j1_final.pontos << " pontos\n";
//         cout << j2_final.nome << ": " << j2_final.pontos << " pontos\n";

//         if (j1_final.pontos > j2_final.pontos)
//         {
//             cout << j1_final.nome << " venceu a rodada!\n";
//             base_j1.selos_excelencia++;
//         }
//         else if (j2_final.pontos > j1_final.pontos)
//         {
//             cout << j2_final.nome << " venceu a rodada!\n";
//             base_j2.selos_excelencia++;
//         }
//         else
//         {
//             cout << "Rodada empatada!\n";
//         }

//         if (base_j1.selos_excelencia == 2 || base_j2.selos_excelencia == 2)
//         {
//             jogoFinalizado = true;
//         }
//         else
//         {
//             cout << "\nPressione Enter para iniciar a próxima rodada...";
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//             cin.get();
//         }
//     }

//     cout << "\n=== FIM DE JOGO ===\n";
//     if (base_j1.selos_excelencia > base_j2.selos_excelencia)
//     {
//         cout << base_j1.nome << " venceu o jogo com 2 Selos de Excelencia!\n";
//     }
//     else
//     {
//         cout << base_j2.nome << " venceu o jogo com 2 Selos de Excelencia!\n";
//     }
// }

void Jogo::iniciar()
{
    cout << "Bem-vindo ao Jaipur!" << endl;

    string nome1 = "IA (Min-max)";
    string nome2 = "IA (Alfa-Beta)";
    cout << "Iniciando partida: " << nome1 << " vs. " << nome2 << endl;

    Jogador base_j1(nome1);
    Jogador base_j2(nome2);

    const int PROFUNDIDADE_MINIMAX_PURO = 2;
    const int PROFUNDIDADE_ALFA_BETA = 4;

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
            int turno_atual = estado_atual.turno;

            cout << "\n-------------------------------\n";
            cout << "Turno de " << ((turno_atual % 2 == 0) ? estado_atual.jogador1.nome : estado_atual.jogador2.nome) << endl;
            estado_atual.mercado.mostrar();
            ((turno_atual % 2 == 0) ? estado_atual.jogador1 : estado_atual.jogador2).mostrar_mao();
            // Minmax
            if (turno_atual % 2 == 0)
            {
                EstadoJogo jogada = encontrar_melhor_jogada_minmax(estado_atual, PROFUNDIDADE_MINIMAX_PURO);
                arvore->adicionar_jogada(jogada);
            }
            // poda a b
            else
            {
                EstadoJogo jogada = encontrar_melhor_jogada_podaAB(estado_atual, PROFUNDIDADE_ALFA_BETA);
                arvore->adicionar_jogada(jogada);
            }

            if (arvore->get_estado_atual().mercado.tres_pilhas_vazias() || arvore->get_estado_atual().baralho.vazio())
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
            cout << "\nPressione Enter para iniciar a proxima rodada...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }

    cout << "\n=== FIM DE JOGO ===\n";
    if (base_j1.selos_excelencia > base_j2.selos_excelencia)
    {
        cout << base_j1.nome << " venceu o jogo com " << base_j1.selos_excelencia << " Selos de Excelencia!\n";
    }
    else
    {
        cout << base_j2.nome << " venceu o jogo com " << base_j2.selos_excelencia << " Selos de Excelencia!\n";
    }
}
int calcular_pontuacao_heuristica(const Jogador &jogador, const EstadoJogo &estado)
{
    // Pesos
    const int W_PONTOS = 10;
    const int W_CAMELOS = 3;
    const int W_MAO = 5;
    const int W_CONJUNTO = 4;
    const int W_MAO_CHEIA = -2;

    int pontuacao_heuristica = 0;

    // Componente 1: Pontuação Atual
    pontuacao_heuristica += jogador.pontos * W_PONTOS;

    // Componente 2: Vantagem de Camelos
    pontuacao_heuristica += jogador.camelo_count() * W_CAMELOS;

    // Componente 3: Valor Potencial da Mão
    int valor_potencial_mao = 0;
    map<TipoCarta, int> contagem_mao;
    for (const auto &carta : jogador.mao)
    {
        contagem_mao[carta.tipo]++;
        if (!estado.mercado.fichas.at(carta.tipo).empty())
        {
            valor_potencial_mao += estado.mercado.fichas.at(carta.tipo).back().get_valor();
        }
    }
    pontuacao_heuristica += valor_potencial_mao * W_MAO;

    // Componente 4: Bônus por Vendas
    int bonus_potencial = 0;
    for (auto const &[tipo, qtd] : contagem_mao)
    {
        if (qtd == 3 && !estado.mercado.bonus3.empty())
            bonus_potencial += estado.mercado.bonus3.back().get_valor();
        if (qtd == 4 && !estado.mercado.bonus4.empty())
            bonus_potencial += estado.mercado.bonus4.back().get_valor();
        if (qtd >= 5 && !estado.mercado.bonus5.empty())
            bonus_potencial += estado.mercado.bonus5.back().get_valor();
    }
    pontuacao_heuristica += bonus_potencial * W_CONJUNTO;

    // Componente 5: Penalidade por Mão Cheia
    if (jogador.mao.size() >= 7)
    {
        pontuacao_heuristica += W_MAO_CHEIA;
    }

    return pontuacao_heuristica;
}

int avaliar_estado(const EstadoJogo &estado)
{

    const Jogador &jogador_max = (estado.turno % 2 == 0) ? estado.jogador1 : estado.jogador2;
    const Jogador &jogador_min = (estado.turno % 2 == 0) ? estado.jogador2 : estado.jogador1;

    int pontuacao_max = calcular_pontuacao_heuristica(jogador_max, estado);
    int pontuacao_min = calcular_pontuacao_heuristica(jogador_min, estado);
    return pontuacao_max - pontuacao_min;
}

EstadoJogo encontrar_melhor_jogada_minmax(const EstadoJogo &estado_atual, int profundidade)
{
    vector<EstadoJogo> proximas_jogadas = gerar_jogadas_possiveis(estado_atual);

    if (proximas_jogadas.empty())
    {
        cout << "IA: Nenhuma jogada possivel." << endl;
        return estado_atual;
    }

    EstadoJogo melhor_jogada = proximas_jogadas[0];
    int melhor_valor = numeric_limits<int>::min(); // pior valor possível para o MAX, -infinito no caso

    cout << "IA esta analisando " << endl;

    // Itera sobre cada jogada possível a partir do estado atual
    for (const auto &jogada : proximas_jogadas)
    {
        // Para cada jogada, chama o minimax para ver qual seria o resultado
        int valor_da_jogada = minimax(jogada, profundidade - 1, false);

        if (valor_da_jogada > melhor_valor)
        {
            melhor_valor = valor_da_jogada;
            melhor_jogada = jogada;
        }
    }

    cout << "IA tomou a decisao com valor " << melhor_valor << endl;
    return melhor_jogada;
}

int minimax(const EstadoJogo &estado_node, int profundidade, bool eh_jogador_max)
{

    // Se atingir a profundidade máxima ou o jogo acabar neste nó, retorna
    if (profundidade == 0 || estado_node.mercado.tres_pilhas_vazias() || estado_node.baralho.vazio())
    {
        return avaliar_estado(estado_node);
    }

    vector<EstadoJogo> proximas_jogadas = gerar_jogadas_possiveis(estado_node);

    if (eh_jogador_max)
    {
        int melhor_valor = numeric_limits<int>::min();
        for (const auto &filho : proximas_jogadas)
        {
            int valor = minimax(filho, profundidade - 1, false); // Chama para o MIN
            melhor_valor = max(melhor_valor, valor);             // Pega o MÁX valor entre as jogadas
        }
        return melhor_valor;
    }
    // Se for o turno do jogador MIN
    else
    {
        int pior_valor = numeric_limits<int>::max();
        for (const auto &filho : proximas_jogadas)
        {
            int valor = minimax(filho, profundidade - 1, true); // Chama para o MAX
            pior_valor = min(pior_valor, valor);                // Pega o MÍNIMO valor entre as jogadas
        }
        return pior_valor;
    }
}

int minimax_alfabeta(const EstadoJogo &estado_node, int profundidade, int alpha, int beta, bool eh_jogador_max)
{
    if (profundidade == 0 || estado_node.mercado.tres_pilhas_vazias() || estado_node.baralho.vazio())
    {
        return avaliar_estado(estado_node);
    }

    vector<EstadoJogo> proximas_jogadas = gerar_jogadas_possiveis(estado_node);

    if (eh_jogador_max)
    {
        int melhor_valor = numeric_limits<int>::min();
        for (const auto &filho : proximas_jogadas)
        {
            int valor = minimax_alfabeta(filho, profundidade - 1, alpha, beta, false);
            melhor_valor = max(melhor_valor, valor);
            alpha = max(alpha, valor); // Atualiza o melhor valor que MAX pode garantir
            if (beta <= alpha)
            {
                break; // MIN não deixaria MAX chegar aqui, pois MIN já tem uma opção melhor
            }
        }
        return melhor_valor;
    }
    // Se for o turno do jogador MIN
    else
    {
        int pior_valor = numeric_limits<int>::max();
        for (const auto &filho : proximas_jogadas)
        {
            int valor = minimax_alfabeta(filho, profundidade - 1, alpha, beta, true);
            pior_valor = min(pior_valor, valor);
            beta = min(beta, valor); // Atualiza o melhor valor que MIN pode garantir
            if (beta <= alpha)
            {
                break; // MAX não escolheria este caminho, pois já tem uma opção melhor (alpha).
            }
        }
        return pior_valor;
    }
}

EstadoJogo encontrar_melhor_jogada_podaAB(const EstadoJogo &estado_atual, int profundidade)
{
    vector<EstadoJogo> proximas_jogadas = gerar_jogadas_possiveis(estado_atual);

    if (proximas_jogadas.empty())
    {
        cout << "IA: Nenhuma jogada possivel." << endl;
        return estado_atual;
    }

    EstadoJogo melhor_jogada = proximas_jogadas[0];
    int melhor_valor = numeric_limits<int>::min();

    // Valores iniciais de alpha e beta
    int alpha = numeric_limits<int>::min();
    int beta = numeric_limits<int>::max();

    cout << "IA (AB) esta analisando " << endl;

    for (const auto &jogada : proximas_jogadas)
    {
        int valor_da_jogada = minimax_alfabeta(jogada, profundidade - 1, alpha, beta, false);

        if (valor_da_jogada > melhor_valor)
        {
            melhor_valor = valor_da_jogada;
            melhor_jogada = jogada;
        }
        // Atualiza o alpha no nível da raiz também
        alpha = max(alpha, melhor_valor);
    }

    cout << "IA decidiu pela jogada com avaliacao de: " << melhor_valor << endl;
    return melhor_jogada;
}