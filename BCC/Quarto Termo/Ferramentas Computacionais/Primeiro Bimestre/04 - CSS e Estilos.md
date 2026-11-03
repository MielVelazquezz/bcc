O JavaFX permite estilizar componentes com **CSS**, mas é um CSS **próprio** (parecido com o da web, porém com propriedades específicas prefixadas com `-fx-`). Documentação oficial de referência: http://docs.oracle.com/javafx/2/api/javafx/scene/doc-files/cssref.html

Existem duas formas de aplicar estilo: **inline** (direto no Java, via `setStyle`) e **por arquivo `.css` vinculado** (recomendado para projetos reais).

## Forma 1 — Estilo inline (`setStyle`)

Você escreve o CSS como uma `String` diretamente no componente. Bom para testes rápidos ou estilos únicos/dinâmicos.

```java
@Override
public void start(Stage stage) throws Exception {
    stage.setTitle("Login");
    AnchorPane pane = new AnchorPane();

    // efeito de gradiente branco e azul
    pane.setStyle("-fx-background-color: linear-gradient(" +
                   "from 0% 0% to 100% 100%, white 0%, blue 100%);");

    // ... resto dos componentes
}
```

## Forma 2 — Arquivo `.css` vinculado (recomendado)

### Passo a passo

1. Crie um arquivo `estilos.css` e salve dentro do **package** do seu projeto (ex.: junto dos `.fxml`).
2. Escreva as regras usando os seletores do JavaFX:
   - `.root` — aplica ao painel/cenário principal.
   - `.nomedaclasse` (ex.: `.button`) — aplica a **todos** os componentes daquele tipo (minúsculo, sem `.class` Java — é o "estilo CSS" do componente).
   - `#idDoComponente` — aplica a **um componente específico**, pelo ID que você definiu com `setId(...)`.
   - `:hover`, `:focused`, `:selected`, `:odd`, `:even` — **pseudo-classes**, aplicam o estilo só em determinado estado.
3. No Java, vincule o arquivo à `Scene`: `scene.getStylesheets().add("caminho/estilos.css");`
4. Para aplicar um estilo específico a um componente, use **classe de estilo** (`getStyleClass().add(...)`) ou **ID** (`setId(...)`).

### Exemplo de `estilos.css`

```css
.root {  /* aplica para o painel principal */
    -fx-font-size: 16pt;
    -fx-font-family: "Calibri";
    -fx-base: rgb(0, 0, 0);  /* cor base para os componentes */
    -fx-background-color: linear-gradient(from 0% 0% to 100% 100%, white 0%, silver 100%);
}

.button {  /* todos os buttons */
    -fx-text-fill: rgb(255, 255, 255);
    -fx-border-color: rgb(49, 89, 23);
    -fx-border-radius: 5;
    -fx-padding: 3 6 6 6;
}

#redbutton {  /* usa o ID do componente */
    -fx-background-color: red;
}
#redbutton:hover {  /* pseudo-classe */
    -fx-background-color: lightcoral;
}

.bluebutton {  /* cria uma classe de estilo customizada */
    -fx-background-color: blue;
    -fx-cursor: hand;
}
```

> [!tip] Outros seletores prontos
> Existem seletores para praticamente todo componente: `.button`, `.check-box`, `.scroll-bar`, `.table-view`, `.list-view`, etc. — a lista completa está na documentação oficial da Oracle.

### Aplicando o CSS pelo Java

```java
// 1) vinculando a folha de estilos à Scene
scene.getStylesheets().add("nome do package/estilos.css");

// 2) aplicando uma classe de estilo a um componente específico
bluebutton.getStyleClass().add("bluebutton");

// 3) aplicando um ID a um componente (para usar #id no CSS)
bluebutton.setId("redbutton");
```

## Trocando de estilo em tempo de execução

Você pode ter **vários arquivos `.css`** (temas diferentes) e trocar qual está aplicado a partir de um evento — por exemplo, um botão "Modo escuro":

```java
// aplicando o estilo inicial, ao abrir a janela
public void start(Stage stage) throws Exception {
    Parent root = FXMLLoader.load(getClass().getResource("FXMLDocument.fxml"));
    Scene scene = new Scene(root);
    stage.setScene(scene);
    scene.getStylesheets().add("javafxcss/estilo.css");
    stage.show();
}

// trocando o estilo a partir de um evento (ex.: clique de um botão)
@FXML
private void mudaLayout(ActionEvent event) {
    pane.getScene().getStylesheets().add("javafxcss/estilo2.css");
}
```

> [!warning] Adicionar x Substituir
> `getStylesheets().add(...)` **acrescenta** uma folha de estilo à lista (as regras se somam/sobrescrevem por ordem). Se você quiser **trocar completamente** o tema, dê `clear()` na lista antes de adicionar a nova: `scene.getStylesheets().clear(); scene.getStylesheets().add(novoCss);`

## Exemplo de tema completo com `:hover`

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

## Temas prontos da própria JavaFX

O JavaFX já vem com 2 folhas de estilo padrão que você pode aplicar globalmente na aplicação inteira, antes mesmo de montar a `Scene`:

```java
public class Main extends Application {
    @Override
    public void start(Stage stage) throws IOException {
        Application.setUserAgentStylesheet(Application.STYLESHEET_CASPIAN);
        // ou:
        // Application.setUserAgentStylesheet(Application.STYLESHEET_MODENA);
        ...
    }

    public static void main(String[] args) { ... }
}
```

- `STYLESHEET_MODENA` — tema padrão desde o Java 8 (visual "flat" moderno).
- `STYLESHEET_CASPIAN` — tema antigo (visual do Java 7 / anterior).

## Temas de terceiros (ex.: AtlantaFX)

Bibliotecas externas oferecem temas prontos (dark/light) bem mais elaborados. Exemplo com **AtlantaFX**:

**1) Adicione a dependência no `pom.xml` (Maven):**

```xml
<dependency>
    <groupId>io.github.mkpaz</groupId>
    <artifactId>atlantafx-base</artifactId>
    <version>2.0.1</version>
</dependency>
```

**2) Aplique o tema no código:**

```java
Application.setUserAgentStylesheet(new PrimerDark().getUserAgentStylesheet());
// ou
Application.setUserAgentStylesheet(new PrimerLight().getUserAgentStylesheet());
```

## Estilizando `TableView` e `ListView` (CSS avançado)

Esses componentes têm muitas partes internas (cabeçalho, células, linhas) — cada uma com seu próprio seletor. Guia completo: https://edencoding.com/style-tableview-javafx/

### TableView

```css
.table-view .column-header,
.table-view .column-header .filler,
.table-view .column-header-background .filler {
    -fx-background-color: #A0A0A0;
    -fx-background-radius: 0;
    -fx-background-insets: 0;
    -fx-text-background-color: white;
}

.table-view .table-cell {
    -fx-border-color: transparent;
    -fx-padding: 2 0 2 10px;
}

.table-row-cell:hover {
    -fx-background-color: #93f9b911;
    -fx-text-background-color: red;
}

.table-row-cell:odd {
    -fx-background-color: #93f9b911;
    -fx-background-insets: 0, 0 0 1 0;
    -fx-padding: 0.0em;
}

.table-row-cell:even {
    -fx-background-color: #1dbbdd11;
    -fx-background-insets: 0, 0 0 1 0;
    -fx-padding: 0.0em;
}
```

### ListView (e ComboBox, que usa a mesma célula)

```css
.list-view {
    -fx-border-color: black;
}

.list-cell:odd {
    -fx-background-color: #FFCCDD;
}

.list-cell:filled:hover {
    -fx-background-color: #0093ff;
    -fx-text-fill: white;
}

.list-cell:empty {
    -fx-background-color: #FFCCAA;
}

.list-cell {
    -fx-cell-size: 50px;
}
```

### ListView/ComboBox com item selecionado destacado

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

## Ícones em botões e botão transparente

Para colocar um ícone num `Button`, use as propriedades de estilo `-fx-graphic` (ou defina via `setGraphic(new ImageView(...))` no Java, que é mais comum). Os arquivos de ícone devem estar na pasta `resources` do projeto.

Para deixar um botão sem fundo (só o ícone/texto aparecem):

```css
.button-transparente {
    -fx-background-color: transparent;
}
```

## Próximo passo

Vá para [[05 - Componentes Básicos]] para conhecer os principais componentes prontos do JavaFX (Accordion, TreeView, WebView, CheckBox, RadioButton, Spinner, etc.).
