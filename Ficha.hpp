#ifndef FICHA_HPP
#define FICHA_HPP

class Ficha
{
public:
  int valor;
  Ficha(int v);
  int get_valor() const { return valor; }
};

#endif
