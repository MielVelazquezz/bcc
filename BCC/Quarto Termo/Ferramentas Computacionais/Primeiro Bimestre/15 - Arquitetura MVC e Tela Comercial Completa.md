Esta nota junta tudo o que foi visto até aqui num padrão real de **tela comercial** (cadastro com pesquisa, listagem, inclusão, alteração e exclusão) — o "modelo" que se repete em praticamente todo sistema desktop feito em JavaFX.

## Estrutura de pastas recomendada do projeto

```
src/
 ├─ ui/            → telas .fxml e seus controllers
 ├─ db/            → conexão com banco de dados
 ├─ css/           → folhas de estilo
 ├─ icon/          → ícones usados nos botões/telas
 ├─ util/          → classes utilitárias (formatação, validação, etc.)
 ├─ dal/           → Data Access Layer (camada de acesso a dados)
 └─ entidade/      → classes Bean (ex.: Produto, Cliente...)
```

Essa organização implementa **3 camadas de persistência**:

1. **Entidade** — os Beans (ex.: `Produto`), como vimos em [[07 - TableView (Tabelas)]].
2. **DAL** (Data Access Layer) — classes responsáveis por conversar com o banco de dados (ex.: `DalProduto`, com métodos `Salvar`, `Alterar`, `Apagar`, `getProdutos`).
3. **UI/Controller** — a tela e sua lógica de interação, que **nunca** acessa o banco diretamente — sempre passa pela DAL.

```
Controller (tela)  →  DAL (regras de acesso a dados)  →  Banco de Dados
      ↑                        ↓
  Entidade (Bean) ←────────────┘
```

## Trocando o painel central (navegação entre telas/módulos)

Em vez de abrir uma janela nova para cada tela do sistema, um padrão muito comum é ter **um painel de conteúdo central** (dentro de um `BorderPane`, por exemplo) e trocar o que está dentro dele, conforme o usuário navega pelo menu.

```java
try {
    Parent root = FXMLLoader.load(getClass().getResource("ui/TelaCadEvento.fxml"));
    pndados.getChildren().clear();
    pndados.getChildren().add(root);
} catch (IOException ex) { }
```

`pndados` aqui é um painel (ex.: um `AnchorPane` ou `StackPane`) que fica na região `center` do layout principal (ver `top`/`left`/`center`/`bottom` em [[03 - Layouts (Painéis)]]).

### Centralizando o novo módulo dentro de um `ScrollPane`

Quando o painel de conteúdo é um `ScrollPane` (para permitir rolagem em telas grandes), pode ser necessário centralizar manualmente o conteúdo carregado:

```java
try {
    Parent tela = FXMLLoader.load(getClass().getResource("/sistema/ui/TelaHome.fxml"));
    painelcentral.setContent(tela);  // painelcentral é um ScrollPane

    // centralizando a tela no scrollpane
    this.center(painelcentral.getViewportBounds(), tela);
    painelcentral.viewportBoundsProperty().addListener((observable, oldValue, newValue) -> {
        this.center(newValue, tela);
    });
} catch (Exception e) {
    System.out.println(e);
}

private void center(Bounds viewPortBounds, Parent centeredNode) {
    double width = viewPortBounds.getWidth();
    double height = viewPortBounds.getHeight();
    double pwidth = ((Region) centeredNode).getPrefWidth();
    double pheight = ((Region) centeredNode).getPrefHeight();

    if (width > pwidth) {
        centeredNode.setTranslateX((width - pwidth) / 2);
    } else {
        centeredNode.setTranslateX(0);
    }
    if (height > pheight) {
        centeredNode.setTranslateY((height - pheight) / 2);
    } else {
        centeredNode.setTranslateY(0);
    }
}
```

O listener em `viewportBoundsProperty()` garante que, se o usuário **redimensionar a janela**, o conteúdo seja recentralizado automaticamente.

## Anatomia da tela comercial (padrão pesquisa + cadastro)

Layout típico de uma tela de cadastro completa:

```
┌───────────────────────────────────────────┐
│ <pnpesquisa>  txcod  txdescr  txpreco      │  ← campos de edição (topo)
├───────────────────────────────────────────┤
│ <pndados>                                  │
│   tabela (lista os registros)              │  ← TableView central
├───────────────────────────────────────────┤
│ btNovo  btAlterar  btApagar  btConfirmar   │  ← barra de ações
│ btCancelar                                 │
└───────────────────────────────────────────┘
```

- `<pnpesquisa>` — painel com os campos de edição do registro (código, descrição, preço).
- `<pndados>` — painel de "dados"/tabela.
- Botões: **Novo**, **Alterar**, **Apagar**, **Confirmar**, **Cancelar**.

### Passo 1 — Vinculando a tabela no `initialize()`

```java
@Override
public void initialize(URL url, ResourceBundle rb) {
    // vincular as colunas da tabela aos beans
    colCod.setCellValueFactory(new PropertyValueFactory("codigo"));
    // ... (uma linha dessas para cada coluna)
    estadoOriginal();
}
```

### Passo 2 — Definindo os "estados" da tela (o coração do padrão)

Uma tela comercial tem, no mínimo, **dois estados**: "consulta" (navegando/pesquisando registros) e "edição" (incluindo ou alterando um registro). Cada estado habilita/desabilita os componentes certos, evitando erros do usuário (ex.: tentar editar sem ter clicado em "Novo"/"Alterar").

**Estado original (consulta/navegação):**

```java
private void estadoOriginal() {
    pnpesquisa.setDisable(false);
    pndados.setDisable(true);

    btConfirmar.setDisable(true);
    btCancelar.setDisable(false);
    btApagar.setDisable(true);
    btAlterar.setDisable(true);
    btNovo.setDisable(false);

    // "limpa" os componentes de edição
    ObservableList<Node> componentes = pndados.getChildren();
    for (Node n : componentes) {
        if (n instanceof TextInputControl) // TextField, TextArea, HTMLEditor
            ((TextInputControl) n).setText("");
        if (n instanceof ComboBox)
            ((ComboBox) n).getItems().clear();
    }

    carregaTabela("");
}
```

> [!tip] O laço genérico de limpeza
> Repare que o laço `for (Node n : componentes)` **não precisa saber o nome de cada campo** — ele varre todos os componentes filhos do painel de dados e limpa qualquer `TextInputControl` ou `ComboBox` que encontrar. É uma forma elegante de "resetar" um formulário inteiro sem escrever `campoX.setText("")` um por um.

**Estado de edição (Novo/Alterar):**

```java
private void estadoEdicao() {
    // carregar os componentes da tela (listbox, combobox, ...), se necessário
    // ex.: carregaEstados();

    pnpesquisa.setDisable(true);
    pndados.setDisable(false);
    btConfirmar.setDisable(false);
    btApagar.setDisable(true);
    btAlterar.setDisable(true);
    txdescr.requestFocus();
}
```

`requestFocus()` — coloca o cursor automaticamente no primeiro campo a ser preenchido, poupando um clique do usuário.

### Passo 3 — Carregando (pesquisando) os dados da tabela

```java
private void carregaTabela(String filtro) {
    DalProduto dal = new DalProduto();
    ArrayList<Produto> res = dal.getProdutos(filtro, new Paciente());
    ObservableList<Produto> modelo;
    modelo = FXCollections.observableArrayList(res);
    tabela.setItems(modelo);
}
```

A tela **nunca monta SQL nem acessa o banco diretamente** — ela chama a DAL (`DalProduto`), que devolve uma lista já pronta de objetos `Produto`.

### Passo 4 — Eventos dos botões (o CRUD completo)

**Novo:**

```java
private void clkNovo(ActionEvent event) {
    estadoEdicao();
}
```

**Alterar** (carrega o registro selecionado nos campos de edição):

```java
private void clkAlterar(ActionEvent event) {
    Produto p = (Produto) tabela.getSelectionModel().getSelectedItem();
    txcod.setText("" + p.getCod());
    txdescr.setText(p.getDescricao());
    txpreco.setText("" + p.getPreco());
    estadoEdicao();
}
```

**Apagar** (com confirmação — ver [[08 - Janelas de Diálogo (Alert)]]):

```java
private void clkApagar(ActionEvent event) {
    Alert a = new Alert(Alert.AlertType.CONFIRMATION);
    a.setContentText("Confirma a exclusão");
    if (a.showAndWait().get() == ButtonType.OK) {
        DalProduto dal = new DalProduto();
        Produto p;
        p = tabela.getSelectionModel().getSelectedItem();
        dal.Apagar(p);
        carregaTabela("");
    }
}
```

**Confirmar** (grava inclusão ou alteração, dependendo se o código já existe):

```java
private void clkConfirmar(ActionEvent event) {
    int cod;
    try {
        cod = Integer.parseInt(txcod.getText());
    } catch (Exception e) {
        cod = 0;
    }

    Produto p = new Produto(cod, txdescr.getText(), Double.parseDouble(txpreco.getText()));
    DalProduto dal = new DalProduto();
    Alert a = new Alert(Alert.AlertType.INFORMATION);

    if (p.getCod() == 0) {          // código 0/vazio => é um NOVO cadastro
        if (dal.Salvar(p))
            a.setContentText("Gravado com Sucesso");
        else
            a.setContentText("Problemas ao Gravar");
    } else {                         // já tem código => é uma ALTERAÇÃO
        if (dal.Alterar(p))
            a.setContentText("Alterado com Sucesso");
        else
            a.setContentText("Problemas ao Alterar");
    }

    a.showAndWait();
    estadoOriginal();
}
```

> [!tip] O truque do "código 0"
> Esse padrão (`if (p.getCod() == 0) { ...salvar... } else { ...alterar... }`) é uma forma simples e muito comum de decidir, num único método `clkConfirmar`, se a operação é uma **inclusão** ou uma **alteração** — sem precisar de dois botões separados de "Salvar Novo" / "Salvar Alteração".

**Cancelar** (sai do modo edição, ou fecha a tela se já estiver em consulta):

```java
private void clkCancelar(ActionEvent event) {
    if (!pndados.isDisabled())     // está em estado de edição
        estadoOriginal();
    else
        TelaMenuController.cad1.close();
        // ou: TelaMenuController.painel.getChildren().clear();
}
```

### Passo 5 — Pesquisa e seleção na tabela

**Pesquisar** (filtra a tabela pelo texto digitado):

```java
private void clkPesquisar(ActionEvent event) {
    carregaTabela("upper(pro_descr) like '%" + txPesquisa.getText().toUpperCase() + "%'");
}
```

> [!danger] Cuidado com SQL Injection
> Esse exemplo monta o filtro **concatenando texto direto na query** — funciona para fins didáticos, mas em um sistema real **nunca** concatene entrada do usuário diretamente em SQL. Use `PreparedStatement` com parâmetros (`?`) na camada DAL, para evitar SQL Injection.

**Habilitando os botões conforme a seleção da tabela:**

```java
private void clkTabela(MouseEvent event) {
    if (tabela.getSelectionModel().getSelectedIndex() >= 0) {
        btAlterar.setDisable(false);
        btApagar.setDisable(false);
    }
}
```

## Estilizando a tela comercial (CSS de apoio)

Estilo de painel principal com gradiente:

```css
#pane {
    -fx-background-color: linear-gradient(from 0% 0% to 100% 100%, white 0%, silver 100%);
}
.button {
    -fx-background-color: orange;
    -fx-text-fill: black;
}
.button:hover {
    -fx-background-color: purple;
    -fx-text-fill: white;
}
```

Estilo de `ListView`/`ComboBox` usado em telas de navegação lateral (menus):

```css
.list-cell:filled:selected:focused, .list-cell:filled:selected {
    -fx-background-color: rgb(10,100,10);
    -fx-text-fill: white;
}
.list-cell {
    -fx-text-fill: black;
    -fx-cell-size: 50px;
    -fx-font-size: 16px;
}
.list-cell:odd {
    -fx-background-color: lightgray;
}
.list-cell:even {
    -fx-background-color: white;
}
.list-cell:filled:hover {
    -fx-background-color: rgb(10,100,10);
    -fx-text-fill: white;
}
.text-field {
    -fx-background-color: #ddd;
}
```

O curso original apresenta 3 variações visuais desse mesmo modelo de tela: **layout horizontal**, **layout vertical** e **layout vertical + JFoenix** (ver [[14 - Frameworks Visuais (JFoenix, JMetro, FXRibbon)]]) — a lógica do Controller (estados, DAL, eventos) é **a mesma** nas três; muda apenas a organização visual dos painéis.

## Checklist para replicar esse padrão no seu próprio CRUD

1. Crie a **Entidade** (Bean) com os atributos, construtores, getters/setters.
2. Crie a **DAL** com os métodos: `getXxx(filtro)`, `Salvar(obj)`, `Alterar(obj)`, `Apagar(obj)`.
3. Desenhe a **tela FXML**: painel de pesquisa (topo), tabela (centro), botões (rodapé).
4. No Controller: implemente `estadoOriginal()` e `estadoEdicao()`.
5. Implemente `carregaTabela(filtro)` chamando a DAL.
6. Implemente os 5 eventos de botão: `clkNovo`, `clkAlterar`, `clkApagar`, `clkConfirmar`, `clkCancelar`.
7. Ligue o clique na tabela (`clkTabela`) para habilitar Alterar/Apagar só quando algo estiver selecionado.
8. (Opcional) adicione a pesquisa (`clkPesquisar`) usando `PreparedStatement` na DAL.