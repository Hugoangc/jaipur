#include "Carta.hpp"
using namespace std;

Carta::Carta(TipoCarta t) : tipo(t) {}

string Carta::nome() const
{
    switch (tipo)
    {
    case COURO:
        return "Couro";
    case TECIDO:
        return "Tecido";
    case ESPECIARIA:
        return "Especiaria";
    case OURO:
        return "Ouro";
    case PRATA:
        return "Prata";
    case DIAMANTE:
        return "Diamante";
    case CAMELO:
        return "Camelo";
    default:
        return "Desconhecido";
    }
}
