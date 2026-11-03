Documentação de referência: http://docs.oracle.com/javafx/2/ui_controls/table-view.htm

O `TableView` é o componente mais usado em sistemas de cadastro (CRUD). Aqui vamos ver duas versões do mesmo fluxo: **100% via código Java** e **via FXML + Controller** (a forma recomendada em projetos reais — ver [[09 - FXML e Scene Builder]]).

## Parte 1 — TableView 100% via código Java

### Passo 1 — Crie uma classe (Bean) com os atributos a serem mostrados na tabela

```java
public class Produto {
    private int codigo;
    private String descricao;

    public Produto(int codigo, String descricao) {
        this.codigo = codigo;
        this.descricao = descricao;
    }

    public int getCodigo() { return codigo; }
    public void setCodigo(int codigo) { this.codigo = codigo; }
    public String getDescricao() { return descricao; }
    public void setDescricao(String descricao) { this.descricao = descricao; }
}
```

> [!warning] IntelliJ + módulos (`module-info.java`)
> Se o seu projeto usa `module-info.java` (padrão do Java 9+), o JavaFX precisa acessar os atributos da sua classe "Bean" **por reflexão**. Abra o `module-info.java` e adicione `opens` (e `exports`, se for o caso) para o package onde está a classe `Produto`. Sem isso, o `PropertyValueFactory` (usado no Passo 2) lança uma exceção em tempo de execução.

### Passo 2 — Monte a tela, instancie o `TableView` e ligue com `TableColumn`

```java
public class JavaFXApplication extends Application {
    private TableView tabela;
    private TableColumn colCodigo, colDescricao;
    ObservableList<Produto> modelo;
    TextField tfCodigo, tfDescricao;

    public static void main(String[] args) { launch(args); }

    @Override
    public void start(Stage stage) throws Exception {
        stage.setTitle("Tabela");
        AnchorPane pane = new AnchorPane();

        tabela = new TableView();
        tabela.setLayoutX(10); tabela.setLayoutY(10);
        tabela.setPrefSize(620, 400);

        colCodigo = new TableColumn<>("Código");
        colDescricao = new TableColumn<>("Descrição do Produto");

        colCodigo.setCellValueFactory(new PropertyValueFactory<>("codigo"));
        colCodigo.setPrefWidth(100);
        colDescricao.setCellValueFactory(new PropertyValueFactory<>("descricao"));
        colDescricao.setPrefWidth(510);

        tabela.getColumns().addAll(colCodigo, colDescricao);

        modelo = FXCollections.observableArrayList();

        tfCodigo = new TextField();
        tfCodigo.setPromptText("Digite o código...");
        tfDescricao = new TextField();
        tfDescricao.setPromptText("Digite a descrição...");

        Button bAdiciona = new Button("Adiciona");
        bAdiciona.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) { adiciona(); }
        });
        Button bExclui = new Button("Exclui");
        bExclui.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) { exclui(); }
        });

        HBox hBox = new HBox();
        hBox.setLayoutX(10); hBox.setLayoutY(420);
        hBox.setPrefSize(620, 50);
        hBox.setPadding(new Insets(10, 10, 10, 10));
        hBox.setSpacing(10);
        hBox.setStyle("-fx-background-color: #336699;");
        hBox.getChildren().addAll(tfCodigo, tfDescricao, bAdiciona, bExclui);

        pane.getChildren().addAll(tabela, hBox);
        Scene scene = new Scene(pane, 640, 480);

        stage.setScene(scene);
        stage.show();
    }
```

> [!important] `PropertyValueFactory`
> O texto passado — `new PropertyValueFactory<>("codigo")` — precisa ser **exatamente** o nome do atributo na classe Bean (sem `get`/`is`), pois o JavaFX monta o nome do getter por convenção (`codigo` → `getCodigo()`).

### Passo 3 — Inserindo e removendo itens da tabela

```java
private void adiciona() {
    modelo.add(new Produto(Integer.parseInt(tfCodigo.getText()), tfDescricao.getText()));
    tabela.setItems(modelo);
    tfCodigo.clear();
    tfDescricao.clear();
}

private void exclui() {
    if (tabela.getSelectionModel().getSelectedIndex() >= 0) {
        tabela.getItems().remove(tabela.getSelectionModel().getSelectedIndex());
    }
}
```

### Passo 4 — Evento de clique duplo numa linha

```java
tabela.setOnMouseClicked(new EventHandler<MouseEvent>() {
    @Override
    public void handle(MouseEvent event) {
        if (event.getClickCount() == 2) {
            Produto p = (Produto) tabela.getSelectionModel().getSelectedItem();
            JOptionPane.showMessageDialog(null, p.getCodigo() + " - " + p.getDescricao());
        }
    }
});
```

## Parte 2 — TableView via FXML + Controller (recomendado)

Aqui a diferença é que o `TableView` e as `TableColumn` já existem no `.fxml` (desenhados no Scene Builder, com `fx:id`), e você só faz o "bind" no `initialize()`.

### Passo 1 — Bean (igual ao anterior)

```java
public class Produto {
    private int cod;
    private String descricao;

    public Produto(int cod, String descricao) {
        this.cod = cod;
        this.descricao = descricao;
    }
    public int getCod() { return cod; }
    public void setCod(int cod) { this.cod = cod; }
    public String getDescricao() { return descricao; }
    public void setDescricao(String descricao) { this.descricao = descricao; }
}
```

### Passo 2 — No Scene Builder, arraste o TableView + colunas para a tela

Dê `fx:id` à tabela (`tabela`) e a cada coluna (`col_cod`, `col_desc`).

### Passo 3 — No `initialize()` da controladora, faça as "amarrações"

```java
public class TabelaController implements Initializable {
    @FXML
    private TableView tabela;
    @FXML
    private TableColumn col_cod, col_desc;
    ObservableList<Produto> modelo;

    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // criando uma lista com os dados a serem expostos na tabela
        List dados = Arrays.asList(new Produto(1, "banana"), new Produto(2, "Ameixa"));

        // vinculando a coluna col_cod com o atributo "cod"
        col_cod.setCellValueFactory(new PropertyValueFactory<>("cod"));
        // vinculando a coluna col_desc com o atributo "descricao"
        col_desc.setCellValueFactory(new PropertyValueFactory<>("descricao"));

        // alimentando a tabela com a lista
        modelo = FXCollections.observableArrayList(dados);
        tabela.setItems(modelo);
    }
}
```

### Passo 4 — Controlando a tabela

```java
// nova linha
modelo.add(new Produto());

// remover a linha selecionada
modelo.remove(tabela.getSelectionModel().getSelectedItem());

// recuperar a linha selecionada
Produto p = tabela.getSelectionModel().getSelectedItem();
System.out.println(p.getDescricao());
```

### Passo 5 — Evento de clique duplo (versão FXML, com `@FXML`)

```java
@FXML
void clickTabela(MouseEvent event) {
    if (event.getClickCount() == 2) {
        Produto p = tabela.getSelectionModel().getSelectedItem();
        JOptionPane.showMessageDialog(null, p.getDescricao());
    }
}
```

(Lembre de ligar esse método ao `onMouseClicked` da tabela no Scene Builder, ou no atributo `onMouseClicked="#clickTabela"` do FXML.)

## Recursos avançados de TableView

### Formatando dados reais (ex.: 2 casas decimais)

Use `setCellFactory` para customizar **como o valor é exibido**, sem mudar o dado real:

```java
colAltura.setCellFactory(column -> new TableCell<>() {
    @Override
    public void updateItem(Double item, boolean empty) {
        super.updateItem(item, empty);
        if (!empty) {
            setText(String.format("%.2f", item));
        }
    }
});
```

### CheckBox dentro de uma coluna (coluna booleana)

**Passo 1** — crie um atributo `boolean` na classe Bean:

```java
public class Rifa {
    private String nome;
    private double valor;
    private boolean pago;
    // construtores, gets e sets
}
```

**Passo 2** — no controller:

```java
@FXML
private TableColumn<Rifa, Boolean> tcPago;

@Override
public void initialize(URL url, ResourceBundle rb) {
    tcPago.setCellFactory(CheckBoxTableCell.forTableColumn(tcPago));
    tabela.setEditable(true);
    // ...
}
```

Guia completo: https://medium.com/@antonio.gabriel/javafx-trabalhando-com-tableview-5cc1065babab

### Pré-selecionando (marcando) colunas booleanas automaticamente

Se você quer que a coluna já venha **marcada de acordo com o dado**, em vez de usar o atributo `boolean` direto, crie um **método que retorne `BooleanProperty`**:

```java
public class Arquivo {
    private String nome;
    private long tamanho;
    private boolean pasta;
    // construtores, gets e sets

    public BooleanProperty getPastaProperty() {
        return new SimpleBooleanProperty(pasta);
    }
}
```

No `initialize()`, vincule ao **método**, não ao atributo:

```java
colpasta.setCellValueFactory(cellData -> cellData.getValue().getPastaProperty());
// vincula o método (getPastaProperty) ao invés do atributo (pasta)

colpasta.setCellFactory(CheckBoxTableCell.forTableColumn(colpasta));
// visualiza em um checkbox
```

### Células editáveis (edição direta na tabela)

**Passo 1** — Bean igual ao de antes (classe `Rifa`).

**Passo 2** — no controller:

```java
@FXML
private TableColumn<Rifa, String> tcNome;

@Override
public void initialize(URL url, ResourceBundle rb) {
    tcNome.setCellFactory(TextFieldTableCell.forTableColumn());
    tabela.setEditable(true);
    // ...
}
```

### Objetos embutidos (mostrar atributo de um objeto dentro de outro objeto)

Quando um atributo da sua classe é, ele mesmo, outro objeto (ex.: `Rifa` tem um `Premio`), e você quer mostrar um campo do `Premio` numa coluna:

**Passo 1** — Beans:

```java
public class Rifa {
    private String nome;
    private double valor;
    private boolean pago;
    private Premio premio;
    // construtores, gets e sets
}

class Premio {
    private String produto;
    private double valor;
    // construtores, gets e sets
}
```

**Passo 2** — no controller, use um `Callback` customizado (o `PropertyValueFactory` sozinho não navega em objetos aninhados):

```java
@FXML
private TableColumn<Rifa, String> tcProduto;
@FXML
private TableColumn<Rifa, Double> tcVlrProd;

@Override
public void initialize(URL url, ResourceBundle rb) {
    // ... faça os bindings normais das outras colunas

    tcProduto.setCellValueFactory(
        new Callback<CellDataFeatures<Rifa, String>, ObservableValue<String>>() {
            @Override
            public ObservableValue<String> call(CellDataFeatures<Rifa, String> param) {
                return new SimpleStringProperty(param.getValue().getPremio().getProduto());
            }
        });

    tcVlrProd.setCellValueFactory(
        new Callback<CellDataFeatures<Rifa, Double>, ObservableValue<Double>>() {
            @Override
            public ObservableValue<Double> call(CellDataFeatures<Rifa, Double> param) {
                return new SimpleObjectProperty<>(param.getValue().getPremio().getValor());
            }
        });
}
```

### Colocando um componente (ex.: um ícone) numa coluna

```java
tcEntregue.setCellFactory(
    new Callback<TableColumn<Rifa, Boolean>, TableCell<Rifa, Boolean>>() {
        @Override
        public TableCell<Rifa, Boolean> call(TableColumn<Rifa, Boolean> param) {
            return new TableCell<Rifa, Boolean>() {
                @Override
                protected void updateItem(Boolean item, boolean empty) {
                    super.updateItem(item, empty);
                    ImageView imgcheck = new ImageView("icones/check.png");
                    if (!empty && item)
                        setGraphic(imgcheck);
                    else
                        setGraphic(null);
                }
            };
        }
    });
```

> Os ícones devem estar em uma pasta acessível pelo classpath, ex.: `src/icones`.

### Colorindo linhas inteiras conforme uma condição (`setRowFactory`)

```java
tabela.setRowFactory(tv -> new TableRow<Rifa>() {
    @Override
    public void updateItem(Rifa item, boolean empty) {
        super.updateItem(item, empty);
        if (item != null && item.isEntregue())
            setStyle("-fx-background-color: tomato;");
        else
            setStyle("");
    }
});
```

```java
public class Rifa {
    private String nome;
    private double valor;
    private boolean pago, entregue;
    private Premio premio;
    // construtores, gets e sets
}
```

Outro exemplo, colorindo baseado numa String de "situação":

```java
tab.setRowFactory(tv -> {
    return new TableRow<Cobranca>() {
        @Override
        public void updateItem(Cobranca item, boolean empty) {
            super.updateItem(item, empty);
            if (item == null) {
                setStyle("");
            } else if (item.getSituacao().getDescr().equals("Liquidado")) {
                setStyle("-fx-text-background-color: #AAA;");
            } else {
                setStyle("-fx-background-color: #0080FF;");
            }
        }
    };
});
```

> [!warning] Sempre chame `super.updateItem(item, empty)`
> Todo `updateItem` sobrescrito (em `TableCell` ou `TableRow`) precisa chamar `super.updateItem(...)` **primeiro** — é isso que atualiza corretamente o estado interno da célula/linha. Esquecer essa chamada é a causa mais comum de bugs estranhos de renderização (linhas "fantasmas", texto duplicado, etc.).

## Estilizando a tabela via CSS

Veja a seção "Estilizando TableView e ListView" em [[04 - CSS e Estilos]] para as classes `.table-view`, `.table-cell`, `.table-row-cell`, etc.