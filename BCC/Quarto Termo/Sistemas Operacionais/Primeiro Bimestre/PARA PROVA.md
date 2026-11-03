#### Multiprocessamento
Sistemas com mais de um processador, que permitem executar tarefas em paralelo e distribuir a carga entre as CPUs.

> [!question] Por que isso importa?
> A forma como os processadores estão "ligados" entre si muda completamente como o balanceamento de carga e a reconfiguração do sistema acontecem.

- **Fortemente Acoplado (Tightly Coupled):**
    - Os processadores <mark style="background: #CACFD9A6;">compartilham a mesma memória física e o mesmo clock</mark>.
    - Controlados por um **único SO**.
    - **Balanceamento de carga:** fácil — o SO único enxerga todos os processadores e redistribui processos entre eles livremente.
    - **Reconfiguração:** simples — adicionar/remover um processador é uma mudança gerenciada por um só sistema.

- **Fracamente Acoplado (Loosely Coupled / Multicomputador):**
    - Cada processador tem <mark style="background: #CACFD9A6;">memória e SO próprios</mark>, e se comunicam por rede/troca de mensagens.
    - **Balanceamento de carga:** mais difícil — exige protocolos explícitos de distribuição de tarefas entre os nós.
    - **Reconfiguração:** mais robusta a falhas — a queda de um nó não derruba os outros, mas exige coordenação entre sistemas independentes.

- **Sistemas Distribuídos:**
    - Processadores **fisicamente separados** (às vezes geograficamente), cada um com seu próprio SO, conectados via rede.
    - **Balanceamento e reconfiguração:** mais complexos — dependem de protocolos de rede, consistência entre nós e coordenação distribuída.
    - Em troca, oferecem **alta escalabilidade** e **tolerância a falhas**.

> [!tip] Como lembrar
> Forte = 1 memória, 1 SO, fácil de gerenciar. Fraco/Distribuído = várias memórias, vários SOs, gerenciamento via rede — quanto mais "separado" fisicamente, mais complexo o balanceamento, mas maior a tolerância a falhas.

---

#### Ambiente de um Processo
O **ambiente de um processo** é o conjunto de tudo que o compõe enquanto ele existe no sistema — não é só o código, é a combinação de:

- **Espaço de endereçamento** (memória que o processo enxerga): seção de código/texto, seção de dados (variáveis globais), heap (alocação dinâmica) e pilha (variáveis locais, parâmetros, endereços de retorno).
- **PCB** (ver abaixo), que guarda o estado do processo quando ele não está executando: PC, registradores, prioridade, ponteiros de memória, arquivos abertos, dispositivos alocados.

> [!note] Por que separar "espaço de endereçamento" de "PCB"?
> O espaço de endereçamento é **o que o processo usa para rodar** (código + dados). O PCB é **o que o SO usa para gerenciar** o processo (metadados). Um processo "completo" = os dois juntos.


---

#### Tempo Real: Hard vs Soft

- **Hard Real-Time (Tempo Real Estrito):**
    - Perder um <mark style="background: #CACFD9A6;">deadline</mark> é considerado **falha do sistema** (ex: controle de usina nuclear, airbags).
    - <mark style="background: #CACFD9A6;">Evita dispositivos de memória secundária</mark> (discos, SSDs) para dados críticos, pois eles não garantem tempo de acesso limitado — usa memória rápida ou ROM.

- **Soft Real-Time (Tempo Real Flexível):**
    - Perder um deadline é **indesejável, mas tolerável** (ex: streaming de vídeo, jogos).
    - Pode conviver com dispositivos de memória secundária normalmente.

> [!warning] Pegadinha clássica de prova
> "Sistemas de tempo real usam SJF como escalonamento padrão" → **FALSO**. O padrão em tempo real são algoritmos baseados em **prioridades fixas** ou **EDF**, não SJF.

- **Rate Monotonic (RM):** atribui prioridade **fixa** com base no período — quanto **menor o período**, **maior a prioridade**. É o algoritmo por trás da fórmula de escalonabilidade $\sum C_i/P_i \leq 1$ vista acima.
- **EDF (Earliest Deadline First):** prioridade **dinâmica** — a cada instante, executa quem tem o **prazo mais próximo**. Consegue utilização de até 100% da CPU (mais eficiente que RM, porém mais complexo de implementar).

