#ifndef FICHA_HPP
#define FICHA_HPP

class Ficha
{
public:
  int valor;
  Ficha(int v);
  int get_valor() const { return valor; }
  bool operator==(const Ficha &outra) const
  {
    return valor == outra.valor;
  }
};

#endif
