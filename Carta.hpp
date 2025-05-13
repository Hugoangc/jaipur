#ifndef CARTA_HPP
#define CARTA_HPP
#include <string>
#include <iostream>
using namespace std;
enum TipoCarta
{
    DIAMANTE,
    OURO,
    PRATA,
    TECIDO,
    ESPECIARIA,
    COURO,
    CAMELO,
    VAZIO
};

class Carta
{
public:
    TipoCarta tipo;
    int valor;

    string nome() const;
    Carta(TipoCarta t);

    string toString() const
    {
        string tipo_str;
        switch (tipo)
        {
        case DIAMANTE:
            tipo_str = "Diamante";
            break;
        case OURO:
            tipo_str = "Ouro";
            break;
        case COURO:
            tipo_str = "Couro";
            break;
        case PRATA:
            tipo_str = "Prata";
            break;
        case TECIDO:
            tipo_str = "Tecido";
            break;
        case ESPECIARIA:
            tipo_str = "Especiaria";
            break;
        case CAMELO:
            tipo_str = "Camelo";
            break;
        case VAZIO:
            tipo_str = "Vazio";
            break;
        }

        return tipo_str; // + " (Valor: " + to_string(valor) + ")";
    }
};
inline void mostrar_tipos_de_carta()
{
    cout << "Tipos de mercadoria:\n";
    cout << "0 - Diamante\n";
    cout << "1 - Ouro\n";
    cout << "2 - Prata\n";
    cout << "3 - Tecido\n";
    cout << "4 - Especiaria\n";
    cout << "5 - Couro\n";
    cout << "6 - Camelo (nao pode ser vendido)\n";
}
#endif
