#include "Jogador.hpp"
#include <iostream>
#include <algorithm>

Jogador::Jogador(std::string nome)
    : nome(std::move(nome)), pontos(0), fichas_bonus(0), selos_excelencia(0) {}

void Jogador::pegar_carta(const Carta &carta)
{
    if (mao.size() >= 7)
    {
        std::cout << "Voce ja possui 7 cartas na mao! Nao pode pegar mais.\n";
        return;
    }

    if (carta.tipo != CAMELO)
    {
        mao.push_back(carta);
    }
    else
    {
        camelos.push_back(carta);
    }
}
void Jogador::zerar_pontos()
{
    pontos = 0;
    fichas_bonus = 0;
}

void Jogador::pegar_camelos_do_mercado(Mercado &mercado)
{
    for (auto it = mercado.cartas.begin(); it != mercado.cartas.end();)
    {
        if (it->tipo == CAMELO)
        {
            camelos.push_back(*it);
            it = mercado.cartas.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Jogador::mostrar_mao() const
{
    std::cout << "Mao de " << nome << ":\n";
    for (size_t i = 0; i < mao.size(); ++i)
    {
        std::cout << "[" << i << "] " << mao[i].nome() << "\n";
    }
}

void Jogador::mostrar_camelos() const
{
    std::cout << "Voce tem " << camelos.size() << " camelos.\n";
}

bool Jogador::excedeu_limite_mao() const
{
    return mao.size() > 7;
}
void Jogador::vender_mercadorias(Mercado &mercado, TipoCarta tipo, int quantidade)
{
    int disponiveis = 0;
    for (const auto &carta : mao)
    {
        if (carta.tipo == tipo)
            disponiveis++;
    }

    if (disponiveis < quantidade)
    {
        std::cout << "Voce nao possui cartas suficientes para vender.\n";
        return;
    }

    if ((tipo == OURO || tipo == PRATA || tipo == DIAMANTE) && quantidade < 2)
    {
        std::cout << "Voce deve vender pelo menos 2 cartas desse tipo.\n";
        return;
    }

    // Remover cartas mão
    int vendidos = 0;
    for (auto it = mao.begin(); it != mao.end() && vendidos < quantidade;)
    {
        if (it->tipo == tipo)
        {
            it = mao.erase(it);
            vendidos++;
        }
        else
        {
            ++it;
        }
    }

    // pontuacao por venda
    int pontos_ganhos = 0;
    for (int i = 0; i < quantidade && !mercado.fichas[tipo].empty(); ++i)
    {
        pontos_ganhos += mercado.fichas[tipo].back().get_valor();
        mercado.fichas[tipo].pop_back();
    }
    // bonus
    int bonus = 0;
    if (quantidade == 3 && !mercado.bonus3.empty())
    {
        bonus = mercado.bonus3.back().get_valor();
        mercado.bonus3.pop_back();
    }
    else if (quantidade == 4 && !mercado.bonus4.empty())
    {
        bonus = mercado.bonus4.back().get_valor();
        mercado.bonus4.pop_back();
    }
    else if (quantidade >= 5 && !mercado.bonus5.empty())
    {
        bonus = mercado.bonus5.back().get_valor();
        mercado.bonus5.pop_back();
    }

    pontos += pontos_ganhos + bonus;
    fichas_bonus += bonus;

    std::cout << "Voce vendeu " << quantidade << " cartas de " << Carta(tipo).nome()
              << " e ganhou " << pontos_ganhos << " pontos em fichas.\n";

    if (bonus > 0)
    {
        std::cout << "Bonus de " << bonus << " pontos por vender " << quantidade << " cartas!\n";
    }
}

bool Jogador::camelo_vazio() const
{
    return camelos.empty();
}
void Jogador::trocar_cartas(Mercado &mercado, const std::vector<int> &idx_mercado, const std::vector<int> &idx_mao)
{
    if (idx_mercado.size() != idx_mao.size() || idx_mao.empty())
    {
        std::cout << "Numero invalido de cartas para troca.\n";
        return;
    }

    if ((mao.size() - idx_mao.size() + idx_mercado.size()) > 7)
    {
        std::cout << "Essa troca excederia o limite de 7 cartas na mao.\n";
        return;
    }

    std::vector<Carta> cartas_para_trocar;
    for (int idx : idx_mao)
    {
        if (idx < 0 || idx >= (int)mao.size())
        {
            std::cout << "Indice invalido da mao.\n";
            return;
        }
        cartas_para_trocar.push_back(mao[idx]);
    }

    // Ordenar índices da mão
    std::vector<int> idx_mao_ordenado = idx_mao;
    std::sort(idx_mao_ordenado.begin(), idx_mao_ordenado.end(), std::greater<int>());

    for (int idx : idx_mao_ordenado)
    {
        mao.erase(mao.begin() + idx);
    }

    // Trocar cartas do mercado com as da mão
    for (size_t i = 0; i < idx_mercado.size(); ++i)
    {
        if (idx_mercado[i] < 0 || idx_mercado[i] >= (int)mercado.cartas.size())
        {
            std::cout << "Indice invalido do mercado.\n";
            return;
        }

        mao.push_back(mercado.cartas[idx_mercado[i]]);
        mercado.cartas[idx_mercado[i]] = cartas_para_trocar[i];
    }

    std::cout << "Troca realizada com sucesso.\n";
}

Carta Jogador::remover_carta_da_mao(int indice)
{
    if (indice >= 0 && indice < static_cast<int>(mao.size()))
    {
        Carta removida = mao[indice];
        mao.erase(mao.begin() + indice);
        return removida;
    }
    else
    {
        return Carta(VAZIO);
    }
}

Carta Jogador::remover_um_camelo()
{
    if (!camelos.empty())
    {
        Carta camelo = camelos.back();
        camelos.pop_back();
        return camelo;
    }
    else
    {
        std::cout << "Sem camelos para remover.\n";
        return Carta(CAMELO);
    }
}

int Jogador::camelo_count() const
{
    return camelos.size();
}
void Jogador::limpar_mao_e_camelos()
{
    mao.clear();
    camelos.clear();
}
