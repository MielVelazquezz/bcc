Layouts são os "containers" que organizam automaticamente onde cada componente (botão, campo de texto, etc.) aparece na tela. Escolher o layout certo evita ter que calcular posição/tamanho manualmente.

Documentação oficial: http://docs.oracle.com/javafx/2/layout/builtin_layouts.htm

Layouts principais: `AnchorPane`, `BorderPane`, `FlowPane`, `GridPane`, `HBox`, `StackPane`, `TilePane`, `VBox`.

## AnchorPane — posicionamento livre (x, y)

O `AnchorPane` é o mais "manual": você posiciona cada componente informando `setLayoutX` e `setLayoutY` (coordenadas em pixels a partir do canto superior esquerdo). É o mais usado quando você desenha a tela junto com o Scene Builder (posição arrastar-e-soltar).

### Passo a passo

1. Crie o `AnchorPane`.
2. Crie cada componente (`Label`, `TextField`, `Button`...).
3. Posicione cada um com `setLayoutX(x)` / `setLayoutY(y)`.
4. Adicione todos ao painel com `getChildren().addAll(...)`.

```java
@Override
public void start(Stage stage) throws Exception {
    stage.setTitle("Login");
    AnchorPane pane = new AnchorPane();

    Label lUsuario = new Label("Usuario:");
    lUsuario.setLayoutX(10); lUsuario.setLayoutY(10);
    Label lSenha = new Label("Senha:");
    lSenha.setLayoutX(10); lSenha.setLayoutY(50);

    TextField tfUsuario = new TextField();
    tfUsuario.setPromptText("Entre com o usuário...");
    tfUsuario.setLayoutX(100); tfUsuario.setLayoutY(10);

    PasswordField pfSenha = new PasswordField();
    pfSenha.setPromptText("Entre com a senha...");
    pfSenha.setLayoutX(100); pfSenha.setLayoutY(50);

    Button bEntrar = new Button("Entrar...");
    bEntrar.setLayoutX(100); bEntrar.setLayoutY(100);

    Button bSair = new Button("Sair...");
    bSair.setLayoutX(180); bSair.setLayoutY(100);

    pane.getChildren().addAll(lUsuario, tfUsuario, lSenha, pfSenha, bEntrar, bSair);

    Scene scene = new Scene(pane, 300, 150);
    stage.setScene(scene);
    stage.show();
}
```

> [!tip] `PromptText`
> `setPromptText(...)` mostra um texto "fantasma" (placeholder) dentro do campo enquanto ele está vazio — some assim que o usuário digita algo.

## BorderPane — 5 regiões fixas

Divide a tela em 5 áreas: `top`, `bottom`, `left`, `right` e `center`. Cada área recebe **um único nó** (se quiser vários componentes numa área, coloque-os dentro de outro painel, como um `HBox`, e ponha esse `HBox` na região).

```java
BorderPane bPane = new BorderPane();

Button bLeft = new Button("Left");
bLeft.setPrefSize(150, 150);
bPane.setLeft(bLeft);

Button bRight = new Button("Right");
bRight.setPrefSize(150, 150);
bPane.setRight(bRight);

Button bCenter = new Button("Center");
bCenter.setPrefSize(150, 150);
bPane.setCenter(bCenter);

Button bTop = new Button("Top");
bTop.setPrefSize(450, 150);
bPane.setTop(bTop);

Button bBottom = new Button("Bottom");
bBottom.setPrefSize(450, 150);
bPane.setBottom(bBottom);

pane.getChildren().add(bPane);
```

Uso típico: `top` = menu/barra de ferramentas, `left` = menu lateral, `center` = conteúdo principal, `bottom` = barra de status.

## FlowPane — fluxo automático (quebra de linha)

Os componentes vão sendo posicionados em sequência e **quebram de linha automaticamente** quando não cabem mais na largura definida (`setPrefWrapLength`).

```java
FlowPane fPane = new FlowPane();
fPane.setPadding(new Insets(10, 10, 10, 10));
fPane.setVgap(4);
fPane.setHgap(4);
fPane.setPrefWrapLength(210);

Button botao;
for (int i = 0; i < 8; i++) {
    botao = new Button("Botão " + i);
    botao.setPrefSize(100, 50);
    fPane.getChildren().add(botao);
}

pane.getChildren().add(fPane);
```

- `setHgap` / `setVgap` — espaçamento horizontal/vertical entre os itens.
- `setPadding(new Insets(top, right, bottom, left))` — margem interna do painel.

## GridPane — grade de linhas e colunas

O layout mais usado para **formulários** (rótulo + campo, rótulo + campo...).

### Passo a passo

1. Crie o `GridPane`.
2. Configure espaçamento com `setHgap`/`setVgap`.
3. Adicione cada componente informando **coluna** e **linha**: `gPane.add(componente, coluna, linha)`.

```java
GridPane gPane = new GridPane();
gPane.setLayoutX(10);
gPane.setLayoutY(10);
gPane.setHgap(10);
gPane.setVgap(10);

Label lbLogin = new Label("Login: ");
TextField txLogin = new TextField();
txLogin.setPromptText("Digite o seu Login...");

Label lbSenha = new Label("Senha: ");
PasswordField txSenha = new PasswordField();
txSenha.setPromptText("Digite a sua Senha..");

Button btEntrar = new Button("Entrar");
Button btSair = new Button("Sair");

gPane.add(lbLogin, 0, 0);
gPane.add(txLogin, 1, 0);

gPane.add(lbSenha, 0, 1);
gPane.add(txSenha, 1, 1);

gPane.add(btEntrar, 0, 2);
gPane.add(btSair, 1, 2);

pane.getChildren().addAll(gPane);
```

> [!tip] Atalho `addRow`
> Em vez de adicionar célula por célula, dá para usar `gPane.addRow(0, lbLogin, txLogin);` para adicionar uma linha inteira de uma vez (comentado no exemplo original como alternativa).

### GridPane com laço duplo (matriz de botões)

Útil para criar coisas como tabuleiros (jogo da velha, campo minado, calendário):

```java
GridPane gPane = new GridPane();
Button[][] btn = new Button[5][5];

for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
        btn[i][j] = new Button(i + "," + j);
        btn[i][j].setPrefSize(60, 60);
        gPane.add(btn[i][j], i, j);
    }
}

pane.getChildren().addAll(gPane);
```

## HBox e VBox — caixas horizontal e vertical

`HBox` empilha os componentes **um ao lado do outro**; `VBox` empilha **um embaixo do outro**. São os layouts mais simples e mais usados dentro de outros painéis (por exemplo, uma barra de botões dentro de um `BorderPane`).

```java
HBox hBox = new HBox();
hBox.setLayoutX(10); hBox.setLayoutY(10);
hBox.setPadding(new Insets(10, 10, 10, 10));
hBox.setSpacing(10);
hBox.setStyle("-fx-background-color: #FF0000;");
Button botaoh1 = new Button("Estamos... ");
Button botaoh2 = new Button("na caixa... ");
Button botaoh3 = new Button("horizontal!!!");
hBox.getChildren().addAll(botaoh1, botaoh2, botaoh3);

VBox vBox = new VBox();
vBox.setLayoutX(10); vBox.setLayoutY(80);
vBox.setPadding(new Insets(10, 10, 10, 10));
vBox.setSpacing(10);
vBox.setStyle("-fx-background-color: #0000FF;");
Button botaov1 = new Button("Agora estamos... ");
Button botaov2 = new Button("na caixa... ");
Button botaov3 = new Button("vertical!!!");
vBox.getChildren().addAll(botaov1, botaov2, botaov3);

pane.getChildren().addAll(hBox, vBox);
```

- `setSpacing(n)` — espaço entre os componentes dentro da caixa.
- `setStyle("...")` — aplica CSS "inline" diretamente no componente (veja mais em [[04 - CSS e Estilos]]).

## TilePane — grade de "ladrilhos" de tamanho igual

Parecido com o `FlowPane`, mas todos os itens ficam com o **mesmo tamanho de célula** (como ícones num explorador de arquivos).

```java
TilePane tPane = new TilePane();
tPane.setPadding(new Insets(10, 10, 10, 10));
tPane.setVgap(4);
tPane.setHgap(4);

Button botao;
for (int i = 0; i < 8; i++) {
    botao = new Button("Botão " + i);
    botao.setPrefSize(100, 50);
    tPane.getChildren().add(botao);
}

pane.getChildren().addAll(tPane);
```

## StackPane — empilhamento (sobreposição)

Os componentes são **sobrepostos** na mesma posição, um em cima do outro (o último adicionado fica por cima). Útil para efeitos como texto sobre imagem, ou telas de "loading" sobre o conteúdo.

```java
StackPane sPane = new StackPane();

Label label = new Label("JavaFX 2 StackPane");
sPane.getChildren().add(label);
Button botao = new Button("Botão");
sPane.getChildren().add(botao);

pane.getChildren().add(sPane);
```

## Qual layout escolher?

| Preciso de... | Use |
|---|---|
| Posicionar componentes em coordenadas exatas (x,y) | `AnchorPane` |
| Cabeçalho / rodapé / menu lateral / conteúdo central | `BorderPane` |
| Formulário com rótulo + campo em linhas | `GridPane` |
| Fila de botões/itens lado a lado | `HBox` |
| Itens empilhados verticalmente | `VBox` |
| Itens que quebram linha ao encher o espaço | `FlowPane` |
| Grade de células do mesmo tamanho | `TilePane` |
| Componentes sobrepostos | `StackPane` |

> [!tip] Combine layouts
> É perfeitamente normal (e recomendado) **aninhar painéis**: por exemplo, um `BorderPane` como raiz, com um `HBox` no `top` (barra de botões) e um `GridPane` no `center` (formulário).
