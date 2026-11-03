## Abertura

Bom, pessoal, boa noite. Hoje a gente fecha o bloco de processos com o tema **concorrência entre processos**. Olhem o sumário: a gente já viu introdução, modelos do processo, estados e mudanças, tipos de processos, comunicação entre processos e escalonamento. Agora entram duas coisas: **concorrência** (problemas de sincronização e soluções de hardware e software) e depois **deadlock** e **threads**.

---

## Condições de corrida

Pensa numa cozinha pequena com vários cozinheiros: todo mundo quer o mesmo fogão e a mesma faca ao mesmo tempo. No computador é assim. **Condição de corrida** é a situação em que dois ou mais processos acessam dados compartilhados e **o resultado final depende de quem roda quando**.

### Exemplo 1: spool de impressão

Sempre que um processo quer imprimir um arquivo, ele coloca o nome do arquivo num diretório de spool. Esse diretório tem `n` entradas, e cada uma guarda um nome de arquivo. O servidor de impressão fica olhando: se tem arquivo, imprime e remove o nome do diretório.

Para isso usamos duas variáveis compartilhadas:

- `in`: aponta para a **próxima entrada livre**;
- `out`: aponta para o **próximo arquivo a ser impresso**.

Agora o problema:

1. O processo A lê `in == 7`, guarda o 7 na variável interna dele, e acontece a **mudança de contexto**.
2. O processo B lê `in == 7`, coloca o arquivo dele na posição 7 e incrementa `in = 8`.
3. O A volta a executar. Ele não olha de novo, usa o 7 que guardou, coloca o arquivo na posição 7 e faz `in = 8`.
4. **Sobrescreveu o arquivo de B.**

E o pior: para o gerenciador do spool, **está tudo certo**. Ninguém avisa nada.

### Exemplo 2: o `echo()`

```pascal
Procedure echo();
var out, in: character;
begin
    input (in, keyboard);
    out := in;
    output (out, display)
end.
```

- P1 chama `echo()` e é interrompido logo depois do `input()`. Digitaram `x`, que está em `in`.
- P2 é despachado, também chama `echo()`. Digitaram `y`, `in` recebe `y` e ele é exibido.
- P1 volta. O caractere exibido não é o `x` que foi digitado, porque o `y` passou por cima.
- **Conclusão:** o `y` aparece duas vezes.

Qual é a essência do problema? O **compartilhamento da variável global `in`**. Anotem isso.

---

## Dificuldades e ação necessária

Concorrência traz três dificuldades:

- **compartilhamento de recursos globais**;
- **gerência da alocação de recursos**;
- **localização de erros de programação**, a famosa depuração. O erro depende de "quem rodou quando", então você roda dez vezes e ele só aparece na sétima.

A ação necessária é:

- **proteger os dados compartilhados** (variáveis, arquivos, outros recursos globais);
- promover o **acesso ordenado**, o controle de acesso, e isso é a **sincronização de processos**.

Quando o que um processo faz **depende ou afeta** o outro, um espera o outro. Aí já é síncrono, tá? E repara: quem decide a ordem de execução é o **escalonador do sistema operacional**, não o programador. Ou eu controlo o código ou não tenho controle nenhum: você controla **o que** o programa faz, mas **não quando** ele roda. Então o seu programa **não pode depender de uma ordem específica**.

---

## Regiões críticas e exclusão mútua

Como evitar condições de corrida? **Proibindo que mais de um processo acesse o recurso compartilhado ao mesmo tempo.** Isso é implementar **exclusão mútua**. É isso que eu chamei de "bloquear". Dois detalhes:

- a exclusão mútua deve afetar **apenas os processos concorrentes** que disputam aquele recurso;
- a parte do programa que acessa o recurso compartilhado é a **região crítica (R.C.)**. Sempre que o processo quiser entrar ou sair dela, ele verifica se pode entrar.

Mas atenção: só isso **não basta** para os processos cooperarem direito. Quatro condições precisam ser garantidas:

1. Dois ou mais processos **não podem estar simultaneamente** na R.C.
2. Considerações sobre a **velocidade** dos processos devem ser ignoradas.
3. Processos **fora** da R.C. **não podem bloquear** outro processo.
4. Nenhum processo pode ficar **esperando indefinidamente** para entrar na sua R.C.

---

## Tipos de soluções

Três famílias:

- **Hardware:** inibição de interrupções; instrução TSL (que apresenta busy wait).
- **Software com busy wait:** variável de bloqueio, alternância estrita, algoritmo de Dekker (no slide está escrito "Decker", mas o certo é **Dekker**), algoritmo de Peterson.
- **Software com bloqueio:** sleep/wakeup, semáforos, monitores.

Lembrem da minha correção: "bloquear a variável" é a **ideia**. Ela pode ser feita por hardware **ou** por software.

---

## Soluções de hardware

### Inibição de interrupções

Cada processo **inibe as interrupções** logo depois de entrar na R.C. e **habilita de novo** logo antes de sair. `DI` = _disable interrupt_, `EI` = _enable interrupt_. Sem interrupção, o processador não faz mudança de contexto, então o processo que está rodando não é trocado.

Vejam o exemplo do produtor-consumidor, com a variável `N` indicando quantos itens ainda cabem no buffer. Em linguagem de máquina:

```
Produtor            Consumidor
DI                  DI
LDA N               LDA N      ; AC <- Mem[N]
DCR AC              INC AC
STA N               STA N      ; Mem[N] <- AC
EI                  EI
```

Cada um desliga as interrupções, lê `N`, ajusta e grava, e religa. Parece ótimo, mas tem problema. Pergunta que está no slide: **se o processo não habilitar as interrupções de volta, o que acontece?** Trava o sistema. Os outros problemas:

- **é desaconselhável** dar a processos de usuário o poder de desabilitar interrupções;
- **não funciona com vários processadores**: a inibição só ocorre no processador onde o processo está executando;
- **conflito com o kernel**: ele pode estar fazendo uma operação crítica justo quando o processo inibe as interrupções;
- inibir por **muito tempo** é perigoso, por exemplo, você perde a sincronização com os periféricos.

Observação: para o **kernel**, inibir por **algumas poucas instruções** é conveniente, por exemplo, para atualizar uma estrutura de controle.

### Instrução TSL (Test and Set Lock)

É uma instrução básica disponível em computadores multiprocessados. Funciona assim:

- transfere o conteúdo de um endereço de memória para um **registrador** e depois armazena um **valor não nulo** nesse endereço;
- essa transferência é **indivisível** (bloqueia o barramento de memória);
- usa uma variável compartilhada `FLAG`: se `FLAG = 0`, qualquer processo pode setar para 1 e ler ou escrever na memória compartilhada; ao terminar, `FLAG = 0` (com uma instrução `MOVE`).

**Vantagens:**

- simples de usar (embora a implementação em hardware não seja trivial);
- não dá ao processo de usuário o poder de desabilitar interrupções;
- presente em quase todos os processadores atuais;
- funciona com vários processadores.

**Desvantagens:**

- **espera ocupada** (_busy wait_);
- possibilidade de **postergação infinita** (_starvation_): o "processo azarado" sempre pega a variável `lock` com valor 1.

---

## Soluções de software com busy wait

_Busy wait_ é espera ativa ou espera ocupada. O que essas soluções fazem: quando o processo quer entrar na R.C., ele verifica se pode. Se não puder, **espera num laço improdutivo** até liberar:

```
while (vez == OUTRO) do { nothing };
```

Consequência: **desperdício de CPU**. É como ficar apertando o botão do elevador sem parar.

E tem o **problema da inversão de prioridade**: o processo `LowPriority` está na R.C. e é interrompido. O `HighPriority` é selecionado, mas entra em espera ativa. Nessa situação o `LowPriority` **nunca vai ter a chance de sair** da R.C.

### Variável de bloqueio (1ª tentativa)

É uma **única variável compartilhada**, que começa em 0. O processo testa antes de entrar: se for 0, muda para 1 e entra; se for 1, espera voltar a 0. `turn = 0` significa R.C. livre; `turn = 1`, R.C. em uso.

```
var turn: 0..1
turn := 0

Process Pi:
...
while turn == 1 do {nothing};
turn := 1;
< critical section >
turn := 0;
...
```

**Problema:** a proposta **não está correta**, porque dois processos podem concluir "simultaneamente" que a R.C. está livre. Os dois testam `turn` antes de um deles mudar para 1. É o **mesmo problema do diretório de spool**.

No slide seguinte tem um código em C usando memória compartilhada (`shmget`, `shmat`) e `fork()` para demonstrar isso: o pai e o filho testam `turn[0] == 1`, imprimem "estou bloqueado" se for o caso, setam `turn[0] = 1`, imprimem "critical section" e voltam para 0. Rodem e vejam o problema acontecendo.

### Alternância estrita (2ª tentativa)

Teste contínuo de uma variável, esperando que ela assuma um valor. `TURN = 0` → o processo A entra; `TURN = 1` → o B entra. Para 2 processos, P1 e P2:

```
while (TRUE) {                    while (TRUE) {
    while (turn != 0) // espera       while (turn != 1) // espera
    critical_region();                critical_region();
    turn = 1;                         turn = 0;
    noncritical_region();             noncritical_region();
}                                 }
```

**Problemas:**

- garante exclusão mútua, mas **obriga a alternância**;
- um mesmo processo **não pode entrar duas vezes seguidas** na R.C., então a velocidade é ditada pelo **processo mais lento**;
- se um processo **falhar ou terminar**, o outro nunca mais entra, **bloqueado permanentemente**. **Não garante progresso**;
- consome muito tempo de processamento.

Exemplo no slide: o processo A é mais longo que o B.

- A sai da R.C., faz `TURN = 1` e continua executando (fora da R.C.);
- B executa rápido, faz `TURN = 0` e volta ao início do laço;
- B não consegue entrar, porque `TURN = 0` e o A **ainda não mudou** `TURN = 1`, pois continua rodando fora da R.C.

Isso **viola a condição 3**: um processo fora da R.C. bloqueando outro.

### Vetor de intenção (3ª tentativa)

O problema da anterior é que ela guarda **a identificação de quem pode entrar**. O que a gente precisa é da **informação de estado**: se o processo **quer** entrar. Então cada processo tem a sua "chave de intenção", num array.

```
var flag: array[0..1] of boolean;
flag[0] := false; flag[1] := false;

Process P0:                       Process P1:
while flag[1] do {nothing};       while flag[0] do {nothing};
flag[0] := true;                  flag[1] := true;
< critical section >              < critical section >
flag[0] := false;                 flag[1] := false;
```

Antes de entrar, o processo olha o array. Se ninguém manifestou interesse, ele liga o bit dele e segue.

**Problemas:**

- se um processo falha **fora** da R.C., ninguém fica bloqueado. Mas se falhar **dentro**, dá problema;
- **não assegura exclusão mútua**: cada um pode concluir que o outro não quer entrar. Isso acontece porque os dois podem **testar** (o `while`) **antes** de um deles **marcar** a intenção.

### 4ª tentativa

A ideia é **marcar a intenção antes de testar** a do outro. É o mesmo algoritmo, só com uma **troca de linha**:

```
Process P0:                  Process P1:
flag[0] := true;             flag[1] := true;
while flag[1] do {nothing};  while flag[0] do {nothing};
< critical section >         < critical section >
flag[0] := false;            flag[1] := false;
```

**Problemas:**

- garante exclusão mútua, mas se um processo falhar dentro da R.C. (ou mesmo depois de setar o flag), o outro fica **bloqueado eternamente**;
- o "problemão": todos ligam o flag ao mesmo tempo e ficam **presos no `while` num loop eterno**. Isso é **deadlock**.

### 5ª tentativa

Na anterior, o processo marcava a intenção **sem saber da intenção do outro**, e não tinha chance de mudar de ideia. Agora, depois de testar no laço, se o outro também quer entrar, o processo **declina** da intenção e dá a vez ao parceiro:

```
Process P0:                      Process P1:
flag[0] = 1;                     flag[1] = 1;
while (flag[1]) {                while (flag[0]) {
    flag[0] = 0;                     flag[1] = 0;
    <delay for a short time>         <delay for a short time>
    flag[0] = 1;                     flag[1] = 1;
}                                }
< critical section >             < critical section >
flag[0] = 0;                     flag[1] = 0;
```

Essa solução é **quase correta**. O pequeno problema é a possibilidade de os processos ficarem **cedendo a vez um para o outro indefinidamente**, o problema da "**mútua cortesia**". Isso é **livelock**. Na prática é difícil sustentar isso por muito tempo, por causa das velocidades relativas dos processos. Mas é uma possibilidade **teórica**, e isso **invalida** a proposta como solução geral.

O exemplo do slide mostra a sequência: P0 seta `flag[0]` true; P1 seta `flag[1]` true; P0 testa `flag[1]`; P1 testa `flag[0]`; P0 seta `flag[0]` false; P1 seta `flag[1]` false; P0 seta true; P1 seta true... e assim vai.

> **Entrega:** vocês vão implementar o **vetor de intenção** (essa 5ª tentativa). Vou falar da entrega completa mais abaixo.

---

## Solução de Dekker

Matemático holandês, proposta na década de 60. É a **primeira solução correta** para o problema da exclusão mútua de **dois processos**. O algoritmo **combina** a ideia da variável de bloqueio com a do array de intenção. É parecido com o anterior, mas usa uma **variável adicional** (`turn`, a vez) para fazer o **desempate** quando os dois entram no loop de mútua cortesia.

```
var flag: array[0..1] of boolean;
    turn: 0..1; // quem tem a prioridade

flag[0] := false
flag[1] := false
turn := 0   // ou 1

Process p0:                       Process p1:
  flag[0] := true                   flag[1] := true
  while flag[1] {                   while flag[0] {
      if turn ≠ 0 {                     if turn ≠ 1 {
          flag[0] := false                  flag[1] := false
          while turn ≠ 0 {}                 while turn ≠ 1 {}
          flag[0] := true                   flag[1] := true
      }                                 }
  }                                 }

  // critical section               // critical section
  turn := 1                         turn := 0
  flag[0] := false                  flag[1] := false
```

Lendo para o P0:

- ele coloca `flag[0]` em true e checa o flag do P1;
- se o flag do P1 for false, o P0 **entra imediatamente**;
- do contrário, consulta `turn`. Se `turn = 0`, é a vez dele **insistir**, e fica em busy wait testando o P1;
- em algum momento o P1 vai perceber que é a vez dele **declinar**, e isso deixa o P0 prosseguir;
- depois da R.C., o P0 coloca o flag em false para liberar e faz `turn = 1`, **passando o direito para o P1**.

**Limitação:** uma solução assim só é aceitável se houver **número de CPUs igual (ou maior) ao número de processos**. Por quê? Porque a gente só poderia se dar "ao luxo" de gastar ciclos de CPU nessa situação, e isso é raro na prática (em geral há mais processos que CPUs). Por isso o Dekker é **pouco usado**.

**Importância:** o Dekker mostrou que dá para resolver o problema **inteiramente por software**, sem instruções de máquina especiais.

Para estender de 2 para **N processos**, precisa de uma modificação **significativa**: `flag[]` com N posições, `turn` assumindo valores de 1 a N, e alteração das condições de teste em todos os processos.

### Entrega (atenção!)

Vocês vão entregar:

- a implementação do **algoritmo de vetor de intenção (5ª tentativa)** e do **Dekker**;
- usando **`fork` e ponteiro** para a exclusão mútua;
- **código documentado** e **teste de mesa**;
- e mais: o **Dekker para 3 processos**.

---

## Solução de Peterson

Usa a variável compartilhada `TURN` e dois procedimentos:

- `enter_region`: verifica se é seguro entrar na R.C.; se não, fica esperando;
- `leave_region`: libera a entrada da R.C. para outro processo.

Funcionamento:

- o processo 0 chama `enter_region` e seta `interested[0] = TRUE`;
- seta `TURN = 0`;
- se o processo 1 chamar `enter_region`, ele fica **preso no laço** até `interested[0] = FALSE`, o que só acontece quando o processo 0 chamar `leave_region`;
- mesmo que os processos 0 e 1 chamem `enter_region` **ao mesmo tempo**, **somente um** entra na R.C. e o outro fica bloqueado até quem entrou sair.

Versão 2, com `flag` e `turn`. O **truque** é: ao marcar a intenção de entrar, o processo já indica (para o caso de empate) que **a vez é do outro**.

```
flag[0] := false
flag[1] := false
turn := 0

Process P0:                             Process P1:
    flag[0] := true                         flag[1] := true
    turn := 1                               turn := 0
    while (flag[1] && turn == 1) { }        while (flag[0] && turn == 0) { }
    // critical section                     // critical section
    flag[0] := false                        flag[1] := false
```

**Por que funciona:**

- **Exclusão mútua:** depois que P0 faz `flag[0] = true`, P1 não consegue entrar. E se P1 já estiver na R.C., então `flag[1] = true` e o P0 fica impedido.
- **Deadlock evitado:** suponha o P0 bloqueado no `while`. Isso significa `flag[1] = true` e `turn = 1`. Mas se é assim, o **P1 é quem entra** na seção crítica. O P0 só entra quando `flag[1]` virar false ou `turn` passar a ser 0.

---

## Soluções de software com bloqueio

### Sleep / Wakeup

Uma solução nova para o problema da espera ocupada. A espera ocupada **gasta muito processamento**, porque fica testando a variável. E ainda tem a **inversão de prioridade**: um processo de maior prioridade fica impedido de entrar porque o de menor prioridade, que está dentro da R.C., não consegue executar para sair.

O `Sleep/Wakeup` **bloqueia** o processo **sem gastar tempo de execução**.

- `SLEEP`: chamada de sistema que **bloqueia** o processo que a chamou;
- `WAKEUP`: "acorda" o processo suspenso pelo `SLEEP`. Tem um parâmetro que identifica **qual processo** acordar.

**Exemplo clássico: produtor-consumidor.**

- Dois processos compartilham um **buffer de tamanho fixo**;
- o **produtor** coloca informações; o **consumidor** retira;
- buffer cheio → produtor dorme; buffer vazio → consumidor dorme.

O problema: **condição de corrida**, parecida com o spool, só que no buffer. Usa-se a variável `COUNT` para o número de itens. `COUNT = 0` → consumidor dorme. `COUNT = N` → produtor dorme. Situação do problema, com buffer vazio:

1. O consumidor lê `COUNT` para testar se é 0 e acontece a **mudança de contexto**;
2. o produtor executa, coloca um item, `COUNT = 1`;
3. o produtor tenta acordar o consumidor com `WAKEUP`, mas **a chamada é perdida**, porque o consumidor ainda não estava dormindo;
4. quando o consumidor volta, ele vai dormir, pois o `COUNT` que ele leu era 0;
5. o produtor, quando chegar em `COUNT = N`, **também dorme**;
6. **ambos dormem eternamente.**

### Semáforos

Variável **inteira, não negativa**, manipulada por duas operações **atômicas** (indivisíveis):

- **`DOWN`**, `Wait` ou **P** (_proberen_, "testar"): verifica se o valor é maior que 0; se for, **decrementa** 1 e o processo continua; se for zero, o processo é **posto para dormir na fila** do semáforo;
- **`UP`**, `Signal` ou **V** (_verhogen_, "incrementar"): incrementa o valor e **acorda** um processo que esteja esperando na fila.

As chamadas trabalham **em pares**: para cada `DOWN`, um `UP`. Pode haver vários processos esperando num semáforo, e um deles é escolhido de alguma forma (aleatoriamente, por exemplo).

Semáforos que só assumem **0 e 1** são **binários**, ou **mutex**. Nesse caso, P e V se chamam **`LOCK`** e **`UNLOCK`**. É o **lock** que eu citei.

```
Down(S):                         UP(S):
  If S > 0                         If algum processo dorme na fila de S
     Then S := S - 1                  Then acorda processo
  Else bloqueia processo           S := S + 1
       (coloca na fila de S)
```

Em Java já existe essa estrutura: `java.util.concurrent.Semaphore`.

**Usos de semáforos:**

**1) Exclusão mútua** (semáforo binário):

```
Semaphore mutex = 1;

Processo P1       Processo P2       ...    Processo Pn
Down(mutex)       Down(mutex)              Down(mutex)
// R.C.           // R.C.                  // R.C.
UP(mutex)         UP(mutex)                UP(mutex)
```

**2) Alocação de recursos** (semáforo contador, iniciado com qualquer inteiro):

```
Semaphore S := 3;

Processo P1       Processo P2       Processo P3
Down(S)           Down(S)           Down(S)
// usa recurso    // usa recurso     // usa recurso
UP(S)             UP(S)             UP(S)
```

**3) Relação de precedência** (executar `p1_rot2` somente depois de `p0_rot1`):

```
semaphore S = 0;
parbegin
  begin  /* processo P0 */          begin  /* processo P1 */
     p0_rot1()                          p1_rot1()
     UP(S)                              Down(S)
     p0_rot2()                          p1_rot2()
  end                               end
parend
```

**4) Barreira** (os n-1 processos esperam o n-ésimo para todos prosseguirem):

```
Y, Z: semaphore initial 0;

P1:            P2:
  UP(Z);         UP(Y);
  Down(Y);       Down(Z);
  A;             B;
```

Na figura: (a) processos se aproximando da barreira; (b) todos, menos um, bloqueados; (c) o último chega e **todos passam**.

**Produtor-consumidor com semáforos e buffer limitado.** Usa três semáforos:

- `FULL = 0`: conta as posições do buffer **preenchidas**;
- `EMPTY = N`: conta as posições **ainda vazias**;
- `MUTEX = 1`: garante que **só um** processo acesse o buffer por vez (leitura ou escrita), gerenciando a exclusão mútua.

**Deficiências dos semáforos:**

1. **Erro de programação** gera resultado imprevisível. Exemplo: se os dois `Down` do produtor estiverem **invertidos**, o `mutex` é decrementado **antes** do `empty`. Se o buffer estiver cheio, o produtor bloqueia **com `mutex = 0`**. Da próxima vez que o consumidor tentar acessar o buffer, ele faz `Down` no `mutex` (agora zero) e **também bloqueia**. Os dois ficam bloqueados para sempre. **Deadlock.**
2. Semáforo é uma abstração de alto nível, baseada em primitivas de baixo nível (atomicidade, bloqueio, filas de espera e escalonamento). Isso o torna **lento**. Para alguns recursos tudo bem; para outros esse tempo é **inaceitável**. O slide dá o exemplo do Unix: se o bloco desejado é achado no buffer cache, `getblk()` tenta reservá-lo com `P()`. Se o buffer já estiver reservado, **não há garantia** de que ele conterá o mesmo bloco que tinha originalmente.

### Monitores

Sugeridos por **Dijkstra (1971)** e desenvolvidos por **Hoare (1974)** e **Brinch Hansen (1975)**. São estruturas de sincronização de **alto nível**, para **impor** uma boa estruturação aos programas concorrentes.

**Motivação:** sistemas baseados em exclusão mútua ou semáforos estão sujeitos a erros de programação. Esses mecanismos precisam estar no código do processo, mas **não existe nenhuma exigência formal** da presença deles. Então erros e omissões (deliberados ou não) podem existir e a exclusão mútua pode não ser atingida. Ou seja, a ideia é **facilitar a vida do programador**: implementar semáforos exige muito cuidado.

**O que é:** um conjunto de **procedimentos, variáveis e estruturas de dados**, agrupados num **módulo especial**. Os processos podem **chamar os procedimentos** do monitor quando quiserem, mas **não podem acessar diretamente** as estruturas de dados e variáveis internas.

**Solução:** tornar a exclusão mútua **obrigatória**, colocando as seções críticas numa área acessível a **um processo de cada vez**.

**Ideia central:** em vez de codificar a região crítica dentro de cada processo, a gente a codifica como **procedimentos (_procedure entries_) do monitor**. Quando o processo precisa mexer em dado compartilhado, ele **chama o procedimento do monitor**. Resultado: o código da R.C. **não se duplica** em cada processo.

**Propriedade importante:** **somente um processo pode estar ativo dentro do monitor** num dado instante. Quem implementa essa exclusão mútua é o **compilador**. Normalmente as regiões críticas ficam dentro dos procedimentos do monitor.

**Variáveis de condição:** são variáveis associadas a condições que provocam a **suspensão e a reativação** de processos. Permitem sincronizações do tipo sleep/wakeup. Só podem ser declaradas **dentro do monitor** e sempre usadas como argumento de dois comandos:

- **`WAIT`** (ou `Delay`): o monitor **suspende** o processo que chamou e guarda as informações dele numa **fila** associada à variável de condição;
- **`SIGNAL`** (ou `Continue`): o monitor **reativa UM** dos processos suspensos na fila.

**Regras:**

- o processo acordado **ganha o direito de executar**, suspendendo o processo que o acordou;
- o processo que executar um `SIGNAL` é **obrigado a deixar o monitor** logo em seguida;
- por isso, o `SIGNAL` aparece como a **última instrução** do procedimento do monitor.

### Troca de mensagens

É outra forma de comunicação entre processos. O slide mostra o **problema do produtor-consumidor com N mensagens**.

---

## Problemas clássicos de comunicação entre processos

### Jantar dos filósofos (filósofos glutões)

- 5 filósofos ao redor de uma mesa;
- cada um tem à frente um prato de macarrão;
- para comer, precisa de **dois garfos**;
- entre cada prato existe **um garfo**;
- cada um pega **um garfo por vez**.

A pergunta é: **como prevenir deadlock?** Se todo mundo pegar o garfo da esquerda ao mesmo tempo, ninguém consegue o segundo. Os slides mostram uma **solução errada** e depois uma solução correta (em duas partes).

### Leitores e escritores

- processos acessando uma **base de dados** ao mesmo tempo;
- durante uma **escrita**, nenhum processo pode **ler nem escrever**;
- problema: **como programar** essa ideia? Tem uma solução no slide.

### Barbeiro dorminhoco (atividade em grupo)

- um barbeiro, uma cadeira e vários lugares de espera;
- se houver lugar, o cliente espera; se não, **vai embora**;
- se não houver clientes, o barbeiro senta na cadeira e **cai no sono**;
- problema: **programar o barbeiro e os clientes** sem condição de corrida.

Os slides finais trazem uma solução para cada um desses problemas.

---

## Fechamento

Resumindo para a prova:

- **condição de corrida**: resultado depende de quem roda quando;
- **região crítica**: trecho que mexe no recurso compartilhado; **exclusão mútua**: só um por vez;
- as **4 condições** de uma boa solução;
- **hardware**: DI/EI e TSL; **software com busy wait**: variável de bloqueio, alternância estrita, vetor de intenção, Dekker e Peterson; **software com bloqueio**: sleep/wakeup, semáforos e monitores;
- **semáforo**: contador com `Down`/`Up` atômicos; binário = mutex/lock;
- **monitor**: módulo com exclusão mútua garantida pelo compilador, com variáveis de condição `WAIT` e `SIGNAL`;
- **deadlock**: bloqueio eterno; **livelock**: ficam cedendo a vez sem avançar; **starvation**: um processo nunca consegue entrar.

E não esqueçam da **entrega**: vetor de intenção (5ª tentativa) e Dekker com `fork` e ponteiro, código documentado, teste de mesa e Dekker para 3 processos.

**Referências:** Tanenbaum (_Sistemas Operacionais Modernos_, seção 2.3 até 2.3.3, e 2.3.7), Silberschatz (_Fundamentos de Sistemas Operacionais_, cap. 7), Deitel (_Sistemas Operacionais_, cap. 5 e seções 6.2 e 6.3), além do Machado e Maia (_Arquitetura de Sistemas Operacionais_). Para monitores em Java, tem dois links nos slides.

Na próxima aula vamos falar de **deadlock** em detalhe e depois **threads**. Dúvidas?