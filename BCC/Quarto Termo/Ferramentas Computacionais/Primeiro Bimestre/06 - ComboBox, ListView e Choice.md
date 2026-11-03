Documentação de referência: http://docs.oracle.com/javafx/2/ui_controls/table-view.htm

## `ComboBox` com itens simples (String)

```java
ComboBox<String> cbDados = new ComboBox<>();
cbDados.setLayoutX(10); cbDados.setLayoutY(10);
cbDados.getItems().add("C");
cbDados.getItems().add("Java");
cbDados.getItems().add("Cobol");
cbDados.getItems().add("Pascal");
cbDados.getSelectionModel().select(1);   // seleciona pelo índice (0-based) -> "Java"

pane.getChildren().add(cbDados);

// lendo o valor selecionado
int index = cbDados.getSelectionModel().getSelectedIndex();
if (index >= 0) {
    System.out.println(cbDados.getItems().get(index));
}
```

## `ComboBox` com objetos (o caso mais comum em sistemas reais)

Quando você quer que o `ComboBox` mostre, por exemplo, uma lista de categorias/clientes/produtos (objetos, não texto puro), siga este passo a passo.

### Passo 1 — Crie a classe (Bean) e sobrescreva `toString()`

O `toString()` é **essencial**: é o texto que o `ComboBox` vai exibir para cada item da lista.

```java
public class Dados {
    private int codigo;
    private String descricao;

    public Dados(int codigo, String descricao) {
        this.codigo = codigo;
        this.descricao = descricao;
    }

    public int getCodigo() { return codigo; }
    public void setCodigo(int codigo) { this.codigo = codigo; }
    public String getDescricao() { return descricao; }
    public void setDescricao(String descricao) { this.descricao = descricao; }

    @Override
    public String toString() {
        return descricao;   // <- é isso que aparece no ComboBox
    }
}
```

### Passo 2 — Popule e leia o ComboBox normalmente

```java
ComboBox<Dados> cbDados = new ComboBox<>();
cbDados.setLayoutX(10); cbDados.setLayoutY(10);
cbDados.getItems().add(new Dados(1, "C"));
cbDados.getItems().add(new Dados(2, "Java"));
cbDados.getItems().add(new Dados(3, "Cobol"));
cbDados.getItems().add(new Dados(4, "Pascal"));
cbDados.getSelectionModel().select(1);

pane.getChildren().add(cbDados);

int index = cbDados.getSelectionModel().getSelectedIndex();
if (index >= 0) {
    Dados d = cbDados.getItems().get(index);
    System.out.println("código: " + d.getCodigo() + ", Descrição: " + d.getDescricao());
}
```

> [!tip] Ganho real
> Diferente do ComboBox de `String`, aqui você tem acesso ao **objeto inteiro** selecionado (`d.getCodigo()`, etc.), não só ao texto — é assim que se faz em sistemas reais (ex.: ComboBox de Categoria dentro de uma tela de Produto).

## ComboBox/ChoiceBox usando FXML + Controller (fluxo completo em telas reais)

Quando a tela é feita em FXML (ver [[09 - FXML e Scene Builder]]), o carregamento dos dados normalmente acontece no `initialize()` da controladora.

### Passo 1 — Crie a classe Bean

```java
public class Categoria {
    private int cod;
    private String nome;

    public Categoria(int cod, String nome) {
        this.cod = cod;
        this.nome = nome;
    }
    public Categoria() { this(0, ""); }

    public int getCod() { return cod; }
    public void setCod(int cod) { this.cod = cod; }
    public String getNome() { return nome; }
    public void setNome(String nome) { this.nome = nome; }

    @Override
    public String toString() {
        return nome;
    }
}
```

### Passo 2 — No Scene Builder (ou FXML), arraste um `ComboBox`/`ChoiceBox` para a tela

Dê um `fx:id` a ele (ex.: `cbTodos`) para poder referenciá-lo no controller.

### Passo 3 — No `initialize()` do controller, carregue o componente

```java
public class TelaCadCategoriaController implements Initializable {

    @FXML
    private ComboBox<Categoria> cbTodos;

    @Override
    public void initialize(URL url, ResourceBundle rb) {
        ArrayList<Categoria> dados = new ArrayList<>();
        dados.add(new Categoria(1, "categoria 1"));
        dados.add(new Categoria(2, "categoria 2"));

        ObservableList<Categoria> modelo = FXCollections.observableArrayList(dados);

        cbTodos.setItems(modelo);
    }
}
```

> [!note] `ObservableList`
> É uma lista "observável": componentes como `ComboBox`/`TableView`/`ListView` conseguem **detectar automaticamente** quando você adiciona/remove itens dela e se atualizam sozinhos na tela — por isso usamos `FXCollections.observableArrayList(...)` em vez de uma `ArrayList` comum como fonte de dados desses componentes.

### Passo 4 — Controlando o componente (adicionar / ler seleção)

```java
// adicionar novo elemento
cbTodos.getItems().add(new Categoria(3, "categoria 3"));

// recuperar o valor selecionado
Categoria cat = cbTodos.getSelectionModel().getSelectedItem();
System.out.println(cat.getCod());
```

## ComboBox com autocomplete

O JavaFX não traz autocomplete pronto — é preciso implementar um `EventHandler` que filtra a lista conforme o usuário digita. Guia completo (com código): http://aprendendo-javafx.blogspot.com/2016/03/completar-automaticamente-combobox.html