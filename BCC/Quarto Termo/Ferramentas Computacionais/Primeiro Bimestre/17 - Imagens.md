## Índice

- [[#1. Duas formas de armazenar uma imagem]]
- [[#2. Imagens Vetoriais (aprofundando)]]
- [[#3. Imagens Bitmap (Raster)]]
- [[#4. Como calcular o tamanho de uma imagem bitmap]]
- [[#5. Curiosidades sobre pixels]]
- [[#6. Resoluções de tela padrão]]
- [[#7. Formatos de arquivo de imagem]]
- [[#8. Comparações práticas entre formatos]]
- [[#9. Qual formato usar? (guia rápido)]]


---
## 1. Duas formas de armazenar uma imagem
Toda imagem digital, no fundo, é guardada de um destes dois jeitos:
- **Bitmap (Raster)**: guarda a cor de cada pixel, ponto a ponto, não cresce a qualidade ao ampliar, seu tamanho depende da resolução x cores, é bom para fotografias e imagens complexas e é possivel de editar utilizando ferramentas como Photoshop e GIMP.
- **Vetorial**: guarda fórmulas matemáticas / comandos geométricos, cresce ao ampliar, seu tamanho geralmente é pequeno, é bom para logotipos, desenhos tecnicos e icones, e é possivel editar utilizando CorelDraw, AutoCAD e Illustrator.
  
****bitmap é uma "foto pixel a pixel"**; **vetorial é uma "receita de como desenhar"***

---
## 2. Imagens Vetoriais (aprofundando)

### O que são
Uma imagem vetorial não guarda cor de pixel nenhuma. Ela guarda **comandos gráficos** baseados em coordenadas cartesianas (aquele sistema de eixos X/Y que você usa em matemática) que descrevem cada elemento da imagem — um retângulo, um círculo, uma linha.

### Exemplo do slide
O material dá este exemplo de "registro" (comando) que descreveria um retângulo:

```

RECT 100,100, 220,150, RED, BLUE

```  

Decompondo esse comando:
- `RECT` → tipo de forma (retângulo)
- `100,100` → coordenada inicial (canto, ponto de partida)
- `220,150` → coordenada final (ou largura/altura, dependendo da convenção usada)
- `RED` → cor da borda
- `BLUE` → cor de preenchimento

  ![[Pasted image 20260903084756.png]]

****isso é texto/números, não uma grade de pixels**. Por isso o arquivo fica pequeno -> é só uma "receita", não uma foto.

> [!tip] Por que isso importa
> Se você pegar esse retângulo vetorial e aumentar 500x, o programa só recalcula a fórmula com o novo tamanho -> a borda continua perfeitamente nítida. Se fizesse isso com uma foto (bitmap), se veria os quadradinhos (pixels) aparecendo.
### Características principais

- **Redimensionamento sem perda de qualidade** -> porque a imagem é recalculada matematicamente a cada novo tamanho, não "esticada".
- **Tamanho de arquivo pequeno** -> armazena bem menos informação que um bitmap equivalente.
- **Uso típico:** softwares CAD (AutoCAD) e de desenho vetorial (CorelDraw, Illustrator, Inkscape).

### Vetorial x Bitmap 

**Vantagens do vetorial:**
- Ocupa menos espaço em memória
- Ideal pra imagens técnicas (plantas, diagramas, ícones)
- Fácil de modificar sem perder qualidade/resolução
- Fácil de converter **para** bitmap (o caminho inverso é difícil)

**Vantagens/limitações do bitmap:**
- Representa imagens "complexas" (como fotos) com perfeição -> coisa que vetorial não consegue bem
- O espaço de armazenamento depende do tamanho da imagem, quantidade de cores, resolução e método de compressão usado
- Só é alterado com técnicas de manipulação de imagem (editores de foto)
- Converter de bitmap pra vetorial exige software especializado (é um processo chamado *vetorização*, e nunca é 100% automático/perfeito)

### Formatos vetoriais mais usados

| Extensão | Uso comum                                                                            |
| -------- | ------------------------------------------------------------------------------------ |
| **CDR**  | CorelDraw                                                                            |
| **DXF**  | AutoCAD (intercâmbio de desenhos técnicos)                                           |
| **EPS**  | Encapsulated PostScript, usado em editoração/impressão                               |
| **HGL**  | Plotters (HP Graphics Language)                                                      |
| **SWF**  | Flash (hoje praticamente obsoleto)                                                   |
| **SVG**  | Padrão aberto pra web — o único da lista realmente vivo e usado hoje em sites/ícones |
  
---
## 3. Imagens Bitmap (Raster)
  
### O que são
Uma **matriz de informações** onde cada elemento da matriz descreve um ponto (pixel) da imagem. Cada pixel ocupa de **1 a 24 bits**, dependendo de quantas cores diferentes a imagem precisa representar.
### A relação entre bits e quantidade de cores  
Quantos bits você reserva por pixel determina **quantas cores diferentes** aquele pixel pode assumir. A fórmula é: **quantidade de cores = 2^(quantidade de bits)**.

| Qtde de bits      | Qtde máxima de cores | Espaço p/ 1024 pontos* |
| ----------------- | -------------------- | ---------------------- |
| 1 bit             | 2 (Preto/Branco)     | 128 Bytes              |
| 2 bits            | 4                    | 256 Bytes              |
| 4 bits (½ byte)   | 16                   | 512 Bytes              |
| 8 bits (1 byte)   | 256                  | 1024 Bytes             |
| 16 bits (2 bytes) | 65.536               | 2048 Bytes             |
| 24 bits (3 bytes) | ~16.777.216          | 3072 Bytes             |
  ***sem considerar compactação nem cabeçalhos do arquivo.
  
### Exemplos visuais do material (como os bits viram cor)
**16 cores (4 bits):** cada *byte* (8 bits) guarda **dois pixels**, já que cada pixel usa só metade de um byte (4 bits). Exemplo de sequência de bits do slide: `0 0 1 0 1 0 1 0` -> isso descreve 2 pixels (cada par de 4 bits é um pixel).

**256 cores (8 bits):** cada byte guarda **exatamente 1 pixel** -> é o caso mais direto de entender, 1 byte = 1 pixel = 1 cor entre 256 possíveis.
  
**True Color / 16 milhões de cores (24 bits):** cada pixel usa **3 bytes** -> um byte pra intensidade de **R**ed (vermelho), um pra **G**reen (verde), um pra **B**lue (azul). É o famoso RGB. Cada canal (R, G ou B) vai de 0 a 255 (8 bits cada), e a combinação dos três dá a cor final. Por isso 256 × 256 × 256 = 16.777.216 combinações possíveis.

---  
## 4. Como calcular o tamanho de uma imagem bitmap

### A fórmula

```

Tamanho em bytes = (largura × altura × qtde de bits por pixel) / 8

```  

O `/ 8` está ali porque o resultado de `largura × altura × bits` te dá o tamanho em **bits**, e queremos o tamanho em **bytes** (1 byte = 8 bits).

### Exemplo resolvido passo a passo
Vamos calcular o tamanho (sem compressão) de uma imagem de **800 × 600 pixels**, em **True Color (24 bits)**:


**Passo 1** — Multiplique largura × altura (total de pixels):

```

800 × 600 = 480.000 pixels

```


**Passo 2** — Multiplique pela quantidade de bits por pixel (24, nesse caso):

```

480.000 × 24 = 11.520.000 bits

```
  

**Passo 3** — Divida por 8 pra converter bits em bytes:

```

11.520.000 / 8 = 1.440.000 bytes

```
  

**Passo 4** — Se quiser em KB ou MB, divida por 1024 sucessivamente:

```

1.440.000 / 1024 ≈ 1.406 KB

1.406 / 1024 ≈ 1,37 MB

```
  

**Resultado: uma imagem 800×600 em 24 bits ocupa cerca de 1,37 MB sem compressão.**

  

> [!important] Isso explica por que existem formatos comprimidos
> Uma foto tirada com celular hoje facilmente tem 4000×3000 pixels em 24 bits — isso daria **~34 MB sem compressão**! É exatamente por isso que formatos como JPEG existem: eles comprimem essa informação pra caber em poucos MB (ou até KB) sem o usuário perceber muita diferença visual. Isso conecta direto com a seção de formatos mais abaixo.

  

### Pratique você mesmo

Tente calcular: qual o tamanho (sem compressão) de uma imagem **1920×1080** (resolução Full HD) em **8 bits (256 cores)**? Aplique os mesmos 3 passos da fórmula acima.

  

(Resposta pra conferir depois: 1920×1080 = 2.073.600 pixels × 8 bits = 16.588.800 bits ÷ 8 = 2.073.600 bytes ≈ 2 MB)

  

---
## 5. Curiosidades sobre pixels
- A palavra **"pixel"** é um acrônimo de **Pic**ture **El**ement (elemento de figura/imagem).
- Aproximadamente **117 pixels equivalem a 1 centímetro** (em referências de resolução gráfica padrão).
- A visão humana tem uma resolução aproximada de **3000 × 3000 pixels**.
- Em um monitor colorido comum, **cada pixel é formado por 3 subpontos**: um vermelho, um verde e um azul (RGB) — a mesma lógica dos 24 bits/3 bytes explicada acima. Cada subponto exibe 256 tonalidades (8 bits), e a combinação dos três dá as ~16 milhões de cores possíveis.
- Contagem de pixels totais em resoluções comuns:
  - 640×480 → 307 mil pixels
  - 800×600 → 480 mil pixels
  - 1024×768 → 786 mil pixels

---
## 6. Resoluções de tela padrão
| Nome          | Megapixels | Resolução |
| ------------- | ---------- | --------- |
| VGA           | 0,3 MP     | 640×480   |
| SVGA          | 0,5 MP     | 800×600   |
| XGA (ou XVGA) | 0,8 MP     | 1024×768  |
| SXGA          | 1,3 MP     | 1280×1024 |
| UXGA          | 1,9 MP     | 1600×1200 |
| QXGA          | 3,1 MP     | 2048×1536 |
| QSXGA         | 5,2 MP     | 2560×2048 |
| WQSXGA        | 6,6 MP     | 3200×2048 |
| QUXGA         | 7,7 MP     | 3200×2400 |
| WQUXGA        | 9,2 MP     | 3840×2400 |

---
## 7. Formatos de arquivo de imagem
Como os conceitos acima (bitmap, bits por pixel, compressão) viram **formatos de arquivo reais** que você usa no dia a dia (`.jpg`, `.png`, `.gif`...).

### Os 5 formatos bitmap mais populares
BMP, GIF, JPEG, PNG e WEBP -> sendo que **GIF, JPEG, PNG e WEBP são "web compatible"** (todo navegador moderno exibe eles nativamente). BMP é o único da lista que não é totalmente seguro pra web (nem todo navegador exibe, e quando exibe, o arquivo costuma ser gigante).

### 7.1 GIF (Graphics Interchange Format)
**História:**
- Criado em **1987** pela Compuserve, com o objetivo de dar alta resolução às imagens da internet -> importante lembrar que **na época a internet era muito lenta**, então um formato compacto era essencial.
- Em **1989**, ganhou recursos novos: **transparência** e **animação** (o famoso GIF animado).
  

**Características técnicas:**
- Usa o algoritmo de compressão **LZW**, que é **sem perdas** (*lossless* — a imagem não perde qualidade ao comprimir).
- Tem opção de **interlacing** (entrelaçamento): a imagem carrega em "camadas" progressivas — você vê uma versão borrada primeiro, que vai ficando nítida enquanto carrega. Bom pra conexões lentas.
- **Limitação séria: máximo de 256 cores.** Por isso GIF é ruim pra fotos (que têm milhões de tons de cor) e ótimo pra desenhos com cores sólidas (logos, ícones, ilustrações simples).
- O algoritmo LZW foi **patenteado pela Unisys** — isso gerou uma polêmica nos anos 90 (empresas tinham que pagar royalties pra usar GIF), e foi justamente esse problema que motivou a criação do **PNG** (veja a seção 7.3).


**Compressão do GIF na prática (exemplo do slide):**
O material mostra imagens de 100×100 pixels, 256 cores, comparando tamanhos:
- O **BMP** (sem compressão) é sempre o maior.
- Entre os GIFs, imagens com **grandes áreas de linhas horizontais/cores sólidas repetidas** comprimem melhor — faz sentido, porque LZW funciona detectando **padrões repetidos** (repetições horizontais na varredura da imagem).

**Reduzindo cores pra economizar espaço:**

Outra tática pra deixar um GIF menor é reduzir a paleta de cores. O slide mostra a mesma imagem em:

| Cores     | Tamanho |
| --------- | ------- |
| 256 cores | 57 KB   |
| 64 cores  | 42 KB   |
| 16 cores  | 26 KB   |
| 8 cores   | 17 KB   |
  O trade-off é claro: **menos cores = arquivo menor, mas qualidade visual pior** (perceba como isso conecta direto com a tabela de bits x cores da seção 3 — 8 cores = 3 bits, 16 cores = 4 bits, etc.)


**Quando usar GIF (segundo o material):**
- Imagens com **poucas cores/camadas de cor** (gráficos, ícones, texto estilizado)
- **Nunca** pra fotos (a limitação de 256 cores fica muito visível)
- Quando você precisa de **animação** ou **transparência** simples (sem gradiente de transparência — é tudo ou nada)


### 7.2 JPEG (Joint Photographic Experts Group)

**Características principais:**
- Desenvolvido **especificamente pra comprimir fotografias**.
- Taxa de compressão altíssima: **até 20:1**, com degradação mínima perceptível ao olho.
- Suporta até **16.772.210 cores** (praticamente o "True Color" de 24 bits que vimos na seção 3 — repare que esse número também tem uma pequena inconsistência com o 2²⁴ exato, mas é o valor que o material usa).
- Existe uma variante **progressiva** (Progressive JPEG), onde o número de "passadas" de renderização pode ser especificado — a imagem também carrega em etapas, ficando nítida aos poucos (parecido com o interlacing do GIF, mas pra fotos).

**A compressão do JPEG é do tipo *lossy* (com perdas) e ajustável.** O material mostra o mesmo exemplo em qualidades diferentes (imagem 313×234, 16M cores):

| Qualidade     | Tamanho |
| ------------- | ------- |
| 100% (máxima) | 83 KB   |
| 50% (média)   | 15 KB   |
| 25%           | 9 KB    |
| 10% (baixa)   | 4,6 KB  |
| 1% (mínima)   | 1,5 KB  |
  
Repare o quanto o tamanho despenca — de 83 KB pra 1,5 KB é uma redução de mais de 50x — só ajustando o "slider de qualidade" que qualquer editor de imagem oferece ao exportar um `.jpg`.

**Quando usar JPEG:**
- **Ideal pra fotos** e imagens com muitas cores/gradientes suaves.
- **Ruim pra imagens com poucas cores** (texto, ícones, desenhos com bordas nítidas) — porque o algoritmo de compressão do JPEG cria "artefatos" (borrões, blocos) justamente em transições bruscas de cor, que é exatamente o que acontece nas bordas de texto/desenho.

  

### 7.3 PNG (Portable Network Graphics)
**Por que foi criado:**
- Em **1994**, a Unisys anunciou que passaria a **cobrar royalties** de quem usasse a compressão LZW do GIF (lembra da patente mencionada acima?).
- Isso motivou **Thomas Boutell** a reunir um grupo de profissionais de internet pra criar um formato **livre de patentes** e melhor que o GIF.
- Em **julho de 1996**, o **W3C** (o mesmo órgão que padroniza HTML/CSS) adotou oficialmente a especificação PNG.

  
**PNG x GIF — vantagens do PNG:**
- **Transparência superior** — PNG tem canal alfa (transparência gradual, de 0% a 100%), enquanto GIF só tem transparência "tudo ou nada" (um pixel é 100% transparente ou 100% opaco, sem meio-termo).
- **Muito mais cores** — suporta True Color (16 milhões de cores), contra o limite de 256 do GIF.
- Consegue **embutir texto** (metadados) dentro do próprio arquivo.
- **Maior compressão, e ainda sem perdas** (lossless) — ou seja, é melhor que GIF em praticamente todos os aspectos técnicos.

**Quando usar PNG:**
- Substitui o GIF em quase todos os casos hoje em dia.
- Ideal quando você precisa de **transparência de qualidade** (logos sobre fundos variados, ícones de interface).
- Não é o melhor pra fotos "cruas" (arquivo fica maior que um JPEG equivalente), mas é o padrão certo quando qualidade/transparência importa mais que tamanho de arquivo.

### 7.4 WebP
**História:**
- Desenvolvido pelo **Google em 2010**, mas só se tornou um padrão **estável em 2018**.
- É um padrão **web, open source**, suportado pelos navegadores mais recentes a partir de **2020**.

**Vantagens:**
- Suporta **compressão lossy e lossless** (você escolhe) — é o único formato da lista que faz as duas coisas.
- **25% a 34% menor que JPEG**, e **26% menor que PNG**, mantendo qualidade visual equivalente.
- Suporta **transparência e animação** — ou seja, ele tenta ser "o melhor dos três mundos" (GIF + JPEG + PNG) num só formato.

**Exemplo comparativo do material:**

| Imagem             | Original | WebP     |
| ------------------ | -------- | -------- |
| JPEG de referência | 43,84 KB | 29,61 KB |
| PNG de referência  | 38,1 KB  | 27 KB    |
  
**Limitação prática:** nem todos os softwares/sistemas mais antigos aceitam `.webp` — é o formato mais moderno da lista, então a compatibilidade "legado" ainda é um ponto de atenção (embora hoje, em 2026, isso já seja bem raro de dar problema).

  

### 7.5 BMP (Bitmap)
- Suporta até **16.772.216 cores** (True Color).
- Usa compressão **RLE** (*Run-Length Encoding*) — uma taxa de compressão baixa, mas **sem degradação** de qualidade.
- Arquitetura **simples e independente** de hardware/sistema operacional.
- É o **formato padrão histórico do Windows** (por isso o nome "Bitmap" ficou tão associado ao Paint/Windows).
- Na prática moderna: raramente usado pra web (arquivo grande demais) — sobrevive mais em contextos internos do Windows.

---
## 8. Comparações práticas entre formatos
### Exemplo 1 — Imagem de scanner, 125×190 pixels

| Formato  | Cores | Tamanho  | Compressão     |
| -------- | ----- | -------- | -------------- |
| BMP      | 16M   | 70 KB    | Sem compressão |
| GIF      | 256   | 20 KB    | LZW            |
| **JPEG** | 16M   | **3 KB** | JPEG (lossy)   |
| PCX      | 16M   | 84 KB    | RLE            |
| PNG      | 16M   | 62 KB    | LZ77 + Huffman |

  Repare como o **JPEG é disparado o menor** (3 KB!) mesmo suportando 16 milhões de cores — é a natureza fotográfica da imagem (vinda de scanner) favorecendo o algoritmo do JPEG, que foi desenhado exatamente pra esse tipo de conteúdo.

### Exemplo 2 — Imagem vetorial de origem (Corel Draw), 154×221 pixels

| Formato             | Cores | Tamanho   |
| ------------------- | ----- | --------- |
| BMP                 | 256   | 35 KB     |
| GIF                 | 256   | 13 KB     |
| JPEG (qualidade 10) | 16M   | 11 KB     |
| PCX                 | 256   | 15 KB     |
| **PNG**             | 256   | **12 KB** |
  
  Aqui a diferença entre formatos é bem menor — porque a imagem, tendo vindo de um programa vetorial (Corel Draw), tem **cores mais sólidas e menos gradientes**, um cenário onde GIF/PNG competem de igual pra igual com o JPEG (que, lembra, é otimizado pra gradientes/fotos, não pra cores chapadas).

  
> [!tip] O padrão que aparece nos dois exemplos
> Qual formato "vence" depende muito do **tipo de conteúdo da imagem** (foto vs. desenho de cores sólidas), não existe um "melhor formato" universal — é sempre uma decisão de contexto.


---
## 9. Qual formato usar? (guia rápido)
Resumo prático de quando escolher cada um, juntando tudo o que foi explicado:  

| Situação                                         | Formato recomendado                  | Por quê                                      |
| ------------------------------------------------ | ------------------------------------ | -------------------------------------------- |
| Foto/fotografia                                  | **JPEG** ou **WebP**                 | Alta compressão, feito pra gradientes de cor |
| Logo, ícone, desenho de cores sólidas            | **PNG** ou **SVG** (se for vetorial) | Sem perda de qualidade nas bordas nítidas    |
| Precisa de transparência                         | **PNG** ou **WebP**                  | Canal alfa (transparência gradual)           |
| Precisa de animação                              | **GIF** ou **WebP**                  | WebP é mais moderno e menor                  |
| Ícone/logo que será redimensionado bastante      | **SVG** (vetorial)                   | Nunca perde nitidez, não importa o tamanho   |
| Uso interno no Windows / edição rápida sem perda | **BMP**                              | Simplicidade, mas arquivo grande             |
| Quer o menor arquivo possível mantendo qualidade | **WebP**                             | Combina o melhor de JPEG/PNG/GIF             |
