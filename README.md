# 🐪 Jaipur - Jogo de Cartas em C++

Este é um projeto de implementação completa do jogo Jaipur em C++, jogável via terminal. Ele segue fielmente as regras oficiais do jogo de cartas para dois jogadores.

## 🎮 Sobre o Jogo

> Jaipur é um jogo competitivo onde dois comerciantes tentam ganhar mais riqueza negociando e vendendo mercadorias no mercado indiano. O jogo é disputado em várias rodadas, e o primeiro a ganhar 2 Selos de Excelência vence.

### Regras implementadas:
- Compra de cartas do mercado
- Coleta de todos os camelos
- Trocas com o mercado usando cartas da mão e/ou camelos
- Venda de mercadorias com regras específicas por tipo
- Fichas de bônus por venda de 3, 4 ou 5 cartas
- Encerramento da rodada por 3 pilhas de fichas vazias ou fim do baralho
- Bônus de camelos ao final da rodada
- Sistema de pontuação e vitória por 2 rodadas

---

## 🛠️ Compilação

Para compilar, utilize o `g++`:

```bash
g++ main.cpp Arvore.cpp Baralho.cpp Carta.cpp Estado.cpp Ficha.cpp Jogador.cpp Jogo.cpp Mercado.cpp -o jaipur 
```
E para executar:
```
.\jaipur
```

Já para o .dot
```
dot -Tpng arvore_rodada.dot -o arvore_rodada.png
```
