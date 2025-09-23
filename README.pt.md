<img src='https://sigarra.up.pt/feup/pt/imagens/LogotipoSI' width="30%"/>

<div align="center">
🌍 <a href="README.md">English</a> | 🇵🇹 <a href="README.pt.md">Português</a>
</div>

<h3 align="center">Licenciatura em Engenharia Informática e Computação<br>L.EIC018 - Laboratório de Computadores<br> 2024/2025 </h3>

---
<h3 align="center"> Colaboradores &#129309 </h2>

<div align="center">

| Nome              | Número      |
|------------------|-------------|
| Bárbara Gomes     | up202305089 |
| Francisco Leiras  | up202307823 |
| Tiago Yin         | up202306438 |
| Tomás Morais      | up202303834 |

Nota : 18,0

</div>

# Relatório do Projeto Wombat Maze

- [Qual foi o nosso objetivo? Qual é a nossa aplicação?](#objetivo)
- [Estrutura do Projeto](#estrutura)
- [Dispositivos Implementados](#dispositivos)  
   - [O Timer i8254](#timer)  
   - [O Teclado i8042](#teclado)  
   - [O Rato i8042](#rato)  
   - [Placa de Vídeo](#video)
- [Principais características diferenciadoras do projeto](#caracteristicas)

<a id="objetivo"></a>
## Qual foi o nosso objetivo? Qual é a nossa aplicação?

O objetivo do nosso projeto foi desenvolver um jogo interativo e visualmente apelativo
no ambiente `LCOM/Minix`, utilizando pelo menos quatro dispositivos de I/O diferentes
estudados ao longo do semestre, nomeadamente o `teclado`, `rato`, `timer` e `placa gráfica`.
O objetivo principal era criar uma aplicação completa que combinasse mecânicas de jogo,
interação do utilizador e integração de dispositivos de forma coesa e divertida.

O resultado é **Wombat Maze**, um jogo 2D dinâmico em que o jogador controla um wombat
que deve percorrer três mapas distintos: uma **floresta**, o **espaço** e a **praia**.
Cada mapa apresenta níveis de dificuldade crescente, com mais obstáculos e um inimigo mais
rápido e perigoso. O jogador navega pelos mapas utilizando o `teclado`, enquanto
interage e destrói obstáculos com o `rato`. Paralelamente, um `timer` conta regressivamente
60 unidades de tempo, e um inimigo perseguidor, representado por um monstro, persegue o
wombat sempre que este estiver na linha de visão.

O núcleo do jogo centra-se na **sobrevivência, navegação e tomada de decisões rápidas**.
O inimigo consegue atravessar obstáculos, ao contrário do wombat, aumentando a urgência e
estratégia na destruição de obstáculos. O jogo termina em **vitória**, se o wombat completar
os três mapas antes do tempo esgotar, ou **derrota**, caso o `dingoe` apanhe o wombat ou o
tempo acabe. A aplicação inclui também um **menu principal** com opções para iniciar o jogo,
ver instruções ou sair, melhorando a usabilidade e a experiência do utilizador.

Em resumo, **Wombat Maze** combina criatividade e execução técnica para entregar um jogo
divertido e responsivo, destacando a aplicação prática de conceitos de programação de sistemas,
enquanto mantém uma experiência agradável para o jogador.

<a id="estrutura"></a>
## Estrutura do Projeto

A estrutura do projeto foi organizada de forma modular para facilitar manutenção e
escalabilidade. O ficheiro `main.c` contém o ciclo principal da aplicação e é responsável
por controlar o ciclo de execução, processar entradas e atualizar o ecrã.

O módulo central `src` coordena os restantes componentes: `display` gere a renderização
gráfica com o suporte de `sprite.c`, `io` gere a comunicação com o hardware (como o
timer, teclado, rato e vídeo) e `game` integra a lógica principal do jogo.

A lógica do jogo está dividida em vários ficheiros, tais como `wombat.c`, `dingoe.c`,
`obstacle.c`, `maze.c` e `menu.c`, cada um responsável por diferentes elementos do jogo,
como personagens, colisões, obstáculos e menus.

<div align="center">
<img width="907" height="597" alt="Captura de ecrã" src="https://github.com/user-attachments/assets/e87bb972-b712-49de-bcd8-368b15885d91" />

***Diagrama de Arquitetura do Projeto***
</div>

<a id="dispositivos"></a>
## Dispositivos Implementados

<a id="timer"></a>
### O Timer i8254

O `Timer 0` foi usado para gerar interrupções periódicas a uma frequência de `30Hz`, regulando
e processando todos os eventos do jogo a este ritmo, como atualização do ecrã, desenho de
elementos, atualização de posições e verificação de colisões.

O timer do jogo incluía um temporizador interno nos níveis, programado para diminuir uma
unidade a cada dois segundos reais.

Para incluir com sucesso o Timer PC `i8254` no projeto, foram implementadas e utilizadas as funções  
`timer_subscribe_int`, `timer_unsubscribe_int`, `timer_set_frequency` e `timer_int_handler`.

<a id="teclado"></a>
### O Teclado i8042

O teclado foi implementado para processar pressionamentos e libertação de teclas, via interrupções.
Estas ações, dependendo do estado atual do jogo, permitem controlar o `Wombat`,
navegar pelo menu (usando `WASD` ou setas), selecionar opções e sair de ecrãs e mapas
(usando `ESC` ou `ENTER`).

Para incluir com sucesso o Teclado PC `i8042` no projeto, foram implementadas e utilizadas as funções  
`keyboard_subscribe_int`, `keyboard_unsubscribe_int` e `kbc_ih`.

<a id="rato"></a>
### O Rato i8042

O rato foi configurado para processar o pressionamento do botão esquerdo e movimento,
via interrupções.

Este dispositivo está presente durante todo o jogo, sendo apenas relevante dentro dos níveis,
onde o jogador pode fazer o Wombat destruir obstáculos movendo o cursor sobre o obstáculo
e pressionando o botão esquerdo do rato.

Para incluir com sucesso o Rato PC `i8042` no projeto, foram implementadas e utilizadas as funções  
`mouse_subscribe_int`, `mouse_unsubscribe_int` e `mouse_ih`.

<a id="video"></a>
### Placa de Vídeo

A placa de vídeo foi configurada para usar **modo de cor direta** com resolução de
`800x600` pixels e `8 bits` por componente RGB (`32 bits` no total). Para eliminar flickering,
foi implementado **Double Buffering**.

Para incluir com sucesso a Placa de Vídeo no projeto, foram implementadas e utilizadas as funções  
`set_mode_graphic`, `build_frame_buffer` e `swap_buffers`.

Para exibir elementos gráficos no ecrã, foi criada a classe `Sprite`, que define a
posição de desenho e armazena o bitmap do elemento. Os sprites são carregados a partir de
ficheiros **XPM** e convertidos para bitmap usando a função `xpm_load`, fornecida pelo `LCF`.
A cor `#FFFFFE` foi usada como chave de transparência e ignorada durante a renderização.

Para exibir corretamente os sprites no projeto, foram implementadas e utilizadas as funções  
`loadSprite`, `drawSprite`, `destroySprite` e `draw_pixel`.

<a id="caracteristicas"></a>
## Principais características diferenciadoras do projeto

O nosso jogo introduz várias inovações de jogabilidade. Uma das principais características é
o comportamento do inimigo: o monstro persegue o wombat sempre que este entra na linha de visão.
Isto adiciona uma camada estratégica, pois os jogadores devem considerar cuidadosamente
quando e como se mover, aproveitando obstáculos para cobertura ou para quebrar a linha de visão.

Outra mecânica única é o **sistema interativo de destruição de obstáculos**. Embora o wombat
não consiga atravessar obstáculos, os jogadores podem removê-los em tempo real com o rato.
Isto dá ao jogador um papel ativo na modelação do ambiente do jogo, combinando movimento
pelo `teclado` e destruição com o `rato`.

O jogo apresenta também **dificuldade progressiva** entre os níveis. À medida que os jogadores
avançam pelos mapas, o monstro move-se mais rápido e os obstáculos tornam-se mais frequentes,
exigindo planeamento estratégico. A inclusão de um temporizador de contagem decrescente
intensifica a experiência, colocando os jogadores sob pressão de tempo constante para
completar os objetivos antes que o monstro os apanhe ou o tempo acabe.

**Wombat Maze** oferece ainda **temas visuais e ambientes criativos**. Em vez de um único
mapa estático, o jogo leva o jogador por três cenários distintos — floresta, espaço e praia.
Cada ambiente tem identidade visual, design de layout e sensação de jogabilidade próprios,
proporcionando uma mudança de ritmo e atmosfera conforme o jogador progride.
