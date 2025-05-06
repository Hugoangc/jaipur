#ifndef CARTA_HPP
#define CARTA_HPP
#include <string>
#include <iostream>

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

    std::string nome() const; 
    Carta(TipoCarta t);

    std::string toString() const
    {
        std::string tipo_str;
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

        return tipo_str; // + " (Valor: " + std::to_string(valor) + ")";
    }
};
#endif
