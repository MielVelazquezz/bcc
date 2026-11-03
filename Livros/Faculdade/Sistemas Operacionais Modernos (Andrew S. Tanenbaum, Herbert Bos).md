
## Capítulo 1.0
Modo Usuário (_User Mode_)
	É a camada visível ao usuário, onde rodam os aplicativos comuns — como navegadores, editores de texto e jogos. Nesse modo, os programas têm **privilégios restritos**; eles não podem acessar diretamente o hardware ou modificar a memória de outros processos, o que garante a segurança e impede que um erro em um aplicativo trave a máquina inteira. 

2. Modo Núcleo (_Kernel Mode_)
	É onde o **Sistema Operacional (SO)** atua diretamente sobre o hardware. Essa camada gerencia informações e instruções altamente sensíveis que o usuário comum não pode (e não deve) modificar. O modo núcleo possui **acesso total e irrestrito** aos componentes físicos do computador (como CPU, memória e discos). Se algo falhar aqui, o sistema inteiro pode colapsar.

![[Pasted image 20261011173435.png|center]]

Complexidade e Exemplos
	Por ser responsável por mediar tudo o que acontece no computador, o código-fonte de um sistema operacional moderno é extremamente extenso e complexo. Os exemplos mais clássicos e amplamente utilizados dessa arquitetura de duplo modo de operação são os sistemas **Windows, Linux e os derivados do Unix**

### Capítulo 1.1
Definir um sistema operacional não é fácil, pois ele atua em duas frentes: serve tanto como uma interface visual que traduz o hardware para o usuário quanto como um gerenciador de recursos que controla o funcionamento de todos os softwares.

#### Capítulo 1.1.1
A arquitura da maioria dos computadores é em nivel de linguagem de maquina, é primitiva e complicada de programar, especialmente para E/S.
Nenhum programador era doido de programar em nível de hardware e por isso foram feitos os drivers de disco, um software que traduz o hardware e deixa legível os blocos de código sem entrar muito em detalhes. Os sistemas operacionais tem vários drivers para controlar E/S.
Mesmo assim, o nível continua sendo baixo demais para os aplicativos e pra isso se tem os arquivos, mais um nivel abstrato. Com essa abstração é possivel criar, escrever, ler arquivos sem precisar entender o que ta acontecendo no hardware.
Essa abstração é a chave para gerenciar toda essa complexidade. Boas abstrações transformam uma tarefa praticamente impossível em duas tarefas gerenciáveis. A primeira é definir e implementar as abstrações, a outra é usar das abstrações para resolver problemas à mão.
Uma abstração que quase todo usuário de com- putadores compreende é o arquivo, como mencionado anteriormente. Trata-se de um fragmento de informação útil, como uma foto digital, uma mensagem de e-mail, música ou página da web salvas. É muito mais fácil li- dar com fotos, e-mails, músicas e páginas da web do que com detalhes de discos SATA (ou outros).
**A função dos sistemas operacionais é criar boas abstrações e então implementar e gerenciar os objetos abstratos criados desse modo.**

#### Capítulo 1.1.2
