# Jaipur com IA Minimax e Árvore de Estados

Este projeto é uma implementação completa em C++ do jogo de cartas para dois jogadores, Jaipur. O diferencial desta versão é a sua arquitetura, que inclui uma árvore de estados para rastreamento de jogadas e uma Inteligência Artificial baseada no algoritmo Minimax e também do mesmo com otimização de Poda Alfa-Beta.

##  Sobre o Jogo

> Jaipur é um jogo de cartas rápido e tático onde dois jogadores competem para se tornar o comerciante pessoal do Marajá. Para isso, eles devem coletar e vender mercadorias no mercado, ganhando pontos. Vendas maiores (de 3, 4 ou 5 cartas de uma vez) rendem bônus especiais. Os camelos servem como um recurso estratégico para realizar trocas. A partida é disputada em um formato de "melhor de três", e o primeiro jogador a conquistar dois "Selos de Excelência" é o vencedor.

### Funcionalidades
Este projeto vai além de uma simples implementação das regras, incluindo:

- Árvore de Estados: Cada partida é armazenada em uma estrutura de árvore, onde cada nó representa um estado completo do jogo. Isso permite funcionalidades como:
- Voltar Jogadas (Undo): Desfazer movimentos durante o jogo.
- Análise Pós-Jogo: Visualizar o fluxo completo da partida.
- Prevenção de Estados Repetidos: Um sistema de hash garante que estados idênticos não sejam duplicados na árvore, otimizando a memória.
- Inteligência Artificial (IA): O jogo conta com um oponente de IA configurável que utiliza algoritmos clássicos de teoria dos jogos para tomar decisões.
- Minimax Puro: Implementação do algoritmo Minimax com busca em profundidade limitada.
- Poda Alfa-Beta: Uma versão otimizada do Minimax que "poda" galhos irrelevantes da árvore de busca, permitindo uma análise mais profunda e rápida.
- Função Heurística: Uma função de avaliação detalhada que considera pontos, valor da mão, bônus por conjuntos, vantagem de camelos e desvantagens táticas para guiar as decisões da IA.
- Visualização da Árvore de Decisões: Ao final de cada rodada, o programa exporta a árvore de estados completa para um arquivo no formato .dot. Isso permite a criação de um grafo visual de todas as jogadas.

Caminhos Reais vs. Hipotéticos: É possível acionar uma análise que calcula todos os movimentos possíveis a partir de um estado. Na visualização, o caminho real do jogo é mostrado com linhas sólidas, enquanto as alternativas não exploradas são mostradas com linhas pontilhadas.
---

##  Compilação

Para compilar, utilize o `g++`:

```g++ arvore.cpp baralho.cpp carta.cpp estado.cpp ficha.cpp jogador.cpp jogo.cppmain.cpp mercado.cpp```
E para executar:
```
.\jaipur
```

# Visualizando a Árvore de Decisões com Graphviz
Após o término de cada rodada, o jogo automaticamente salvará um arquivo chamado arvore_rodada.dot. Para transformar este arquivo em uma imagem:
Instale o Graphviz: Baixe e instale a partir do site oficial do Graphviz.

Execute o Comando dot: Abra um terminal na pasta do seu projeto e execute o seguinte comando:
```
dot -Tpng arvore_rodada.dot -o arvore_rodada.png
```
