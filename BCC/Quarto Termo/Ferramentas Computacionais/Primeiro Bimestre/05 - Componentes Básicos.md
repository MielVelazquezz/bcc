## Ações úteis rápidas

### Fechar a aplicação

```java
Platform.exit();
```

### Aplicar efeitos visuais a um painel/componente

```java
// borrar (blur)
Effect frostEffect = new BoxBlur(4, 4, 1);
painel.setEffect(frostEffect);

// clarear/ajustar cor
ColorAdjust claro = new ColorAdjust(0, 0, .85, 0);
painel.setEffect(claro);
```

`ColorAdjust(hue, saturation, brightness, contrast)` — os 4 parâmetros vão de -1.0 a 1.0.

### Transições (animações)

```java
// fade in/out
FadeTransition ft = new FadeTransition(Duration.millis(2000), root);
ft.setFromValue(0.0);
ft.setToValue(1.0);
ft.play();

// translação (mover um nó de um ponto a outro)
TranslateTransition translateTransition =
        new TranslateTransition(Duration.millis(500), root);
translateTransition.setFromX(500);
translateTransition.setToX(0);
translateTransition.play();
```

`root` aqui é **qualquer painel/nó** que você queira animar. Mais exemplos de transições: http://www.java2s.com/Tutorials/Java/JavaFX/1000__JavaFX_Transitions.htm

### Executando janelas modais (uma janela "trava" a outra)

Uma janela **modal** impede que o usuário interaja com a janela anterior até fechar a atual (comum em telas de cadastro/confirmação).

**Passo a passo:**

1. Crie outra classe (a "classe principal" da nova janela — outra `Application`/tela com seu próprio `start`).
2. No `start` dessa nova janela, adicione:
   ```java
   primaryStage.initModality(Modality.APPLICATION_MODAL);
   ```
3. Troque `primaryStage.show()` por `primaryStage.showAndWait()` — isso faz o código **esperar** até a janela fechar antes de continuar.
4. Na janela principal, no evento que deve abrir a nova tela:
   ```java
   NovaTela nova = new NovaTela();
   nova.start(new Stage());
   ```

> Veja também [[10 - Múltiplas Janelas e Stage]] para o jeito mais comum de fazer isso usando **FXML**.

### Configurações de palco (`Stage`)

```java
// remove os botões Maximizar e Minimizar (mantém fechar e borda)
stage.initStyle(StageStyle.UTILITY);

// remove os 3 botões (min/max/fechar) e a borda da janela
stage.initStyle(StageStyle.UNDECORATED);

// desativa o redimensionamento da janela
stage.setResizable(false);

// tela cheia
stage.setFullScreen(true);
```

### Janela transparente (sem fundo/cenário visível)

```java
stage.initStyle(StageStyle.TRANSPARENT);
scene.setFill(Color.TRANSPARENT);
```

Útil para criar janelas com formato customizado (cantos arredondados, formas irregulares) usando CSS.

---

## Componentes prontos do JavaFX

Lista completa de componentes: http://tutorials.jenkov.com/javafx/index.html

Os principais: `Accordion`, `Hyperlink`, `ProgressIndicator`, `TreeView`, `WebView`, `ContextMenu`, `Tooltip`, `RadioButton`, `ComboBox`, `HTMLEditor`, `TableView` (este último tem nota própria: [[07 - TableView (Tabelas)]]).

### Accordion — painéis expansíveis/retráteis

Cada seção (`TitledPane`) pode ser aberta clicando no título; só uma fica aberta por vez (por padrão).

```java
Label label = new Label("Primeiro painel aberto!");
label.setLayoutX(10); label.setLayoutY(10);
Button botao = new Button("Clique aqui...");
botao.setLayoutX(10); botao.setLayoutY(30);

AnchorPane paneInterno = new AnchorPane();
paneInterno.getChildren().addAll(label, botao);

Accordion accordion = new Accordion();
TitledPane tPane1 = new TitledPane("Primeiro Painel", paneInterno);
TitledPane tPane2 = new TitledPane("Segundo Painel", new Label("Segundo painel aberto!"));
TitledPane tPane3 = new TitledPane("Terceiro Painel", new Label("Terceiro painel aberto!"));
accordion.getPanes().addAll(tPane1, tPane2, tPane3);

painelPrincipal.getChildren().addAll(accordion);
```

Note que o conteúdo de cada `TitledPane` pode ser **qualquer nó** — de um simples `Label` até um painel inteiro (`AnchorPane` com vários componentes dentro, como no primeiro painel do exemplo).

### Hyperlink — link clicável que abre o navegador

```java
Hyperlink hlink = new Hyperlink("Clique aqui...");
hlink.setLayoutX(10); hlink.setLayoutY(10);
hlink.setOnAction(new EventHandler<ActionEvent>() {
    @Override
    public void handle(ActionEvent event) {
        try {
            Desktop.getDesktop().browse(new URI("http://www.unoeste.br"));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
});

pane.getChildren().addAll(hlink);
```

`Desktop.getDesktop().browse(uri)` é do pacote `java.awt.Desktop` — abre o navegador padrão do sistema operacional na URL informada.

### ProgressIndicator — indicador de progresso vinculado a uma tarefa

Combina com `Task` (ver [[13 - Threads e Task]]) para mostrar o andamento de uma operação demorada sem travar a interface.

```java
ProgressIndicator progress = new ProgressIndicator(0.0);
Task<Void> task = new Task<Void>() {
    @Override
    protected Void call() throws Exception {
        final int max = 100;
        for (int i = 1; i <= max; i++) {
            updateProgress(i, max);
            try {
                Thread.sleep(200);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return null;
    }
};
progress.progressProperty().bind(task.progressProperty());
new Thread(task).start();

pane.getChildren().add(progress);
```

`progress.progressProperty().bind(task.progressProperty())` é o que faz a barra "andar sozinha" conforme o valor de `updateProgress` muda dentro da `Task`.

### TreeView — árvore hierárquica (pastas, categorias, menus)

**Passo a passo:**

1. Crie o `TreeView<TipoDoItem>`.
2. Crie os `TreeItem` (cada um pode ter uma imagem/ícone).
3. Monte a hierarquia com `getChildren().addAll(...)`/`add(...)`.
4. Defina o item raiz com `treeView.setRoot(...)`.
5. (Opcional) capture o clique duplo para saber qual item foi selecionado.

```java
Image rootIcon = new Image(getClass().getResourceAsStream("folder.png"));
TreeView<String> treeView = new TreeView<>();
TreeItem<String> item1 = new TreeItem<>("Tópico 1", new ImageView(rootIcon));
TreeItem<String> subitem1 = new TreeItem<>("Tópico 1.1", new ImageView(rootIcon));
TreeItem<String> subitem1_1 = new TreeItem<>("Tópico 1.1.1");
TreeItem<String> subitem1_2 = new TreeItem<>("Tópico 1.1.2");
TreeItem<String> subitem2 = new TreeItem<>("Tópico 1.2", new ImageView(rootIcon));
TreeItem<String> subitem3 = new TreeItem<>("Tópico 1.3", new ImageView(rootIcon));
TreeItem<String> subitem3_1 = new TreeItem<>("Tópico 1.3.1");

item1.getChildren().addAll(subitem1, subitem2, subitem3);
subitem1.getChildren().addAll(subitem1_1, subitem1_2);
subitem3.getChildren().add(subitem3_1);
treeView.setRoot(item1);

treeView.setOnMouseClicked(new EventHandler<MouseEvent>() {
    @Override
    public void handle(MouseEvent mouseEvent) {
        if (mouseEvent.getClickCount() == 2) {
            TreeItem<String> item = treeView.getSelectionModel().getSelectedItem();
            System.out.println("Item Selecionado : " + item.getValue());
        }
    }
});

pane.getChildren().addAll(treeView);
```

### WebView — navegador embutido dentro da aplicação

Permite abrir páginas web (ou HTML local) dentro da própria janela JavaFX.

**Passo a passo:**

1. Adicione a dependência do módulo web ao `pom.xml`:
   ```xml
   <dependency>
       <groupId>org.openjfx</groupId>
       <artifactId>javafx-web</artifactId>
       <version>22</version>
   </dependency>
   ```
2. No `module-info.java`, adicione: `requires javafx.web;`
3. Crie o `WebView` e carregue uma URL:

```java
WebView webView = new WebView();
webView.setPrefSize(1024, 768);
webView.getEngine().load("http://www.unoeste.br");

pane.getChildren().add(webView);
```

**Para abrir um HTML local** (arquivo de ajuda, por exemplo), duas formas:

```java
// caminho absoluto direto
webView.getEngine().load("file:///D:/pasta/ajuda.html");

// ou a partir de uma pasta dentro do projeto
File file = new File("help/ajuda.html");
webView.getEngine().load(file.toURI().toString());
```

### ContextMenu — menu de clique direito

```java
final ListView<String> listView = new ListView<>(
        FXCollections.observableArrayList("Primeiro item", "Segundo item", "Terceiro item"));

ContextMenu contextMenu = new ContextMenu();
MenuItem item1 = new MenuItem("Mostrar valor");
item1.setOnAction(new EventHandler<ActionEvent>() {
    @Override
    public void handle(ActionEvent event) {
        JOptionPane.showMessageDialog(null,
                listView.getSelectionModel().getSelectedItem());
    }
});
MenuItem item2 = new MenuItem("Trocar valor");
item2.setOnAction(new EventHandler<ActionEvent>() {
    @Override
    public void handle(ActionEvent event) {
        listView.getItems().set(
                listView.getSelectionModel().getSelectedIndex(), "Valor Trocado");
    }
});
contextMenu.getItems().addAll(item1, item2);
listView.setContextMenu(contextMenu);

pane.getChildren().add(listView);
```

> [!note] `JOptionPane`
> Esse componente é do **Swing** (`javax.swing`), não do JavaFX — o material usa ele como atalho rápido para mostrar mensagens. Em um projeto JavaFX "puro" prefira `Alert` (ver [[08 - Janelas de Diálogo (Alert)]]).

### Tooltip — dica ao passar o mouse

```java
Label lbNome = new Label("Nome: ");
TextField txNome = new TextField();
txNome.setLayoutX(50);
Tooltip.install(txNome, new Tooltip("Digite aqui seu nome completo..."));

pane.getChildren().addAll(lbNome, txNome);
```

### CheckBox — múltipla escolha independente

```java
CheckBox cb1 = new CheckBox("Alternativa 1");
cb1.setLayoutX(10); cb1.setLayoutY(10);
CheckBox cb2 = new CheckBox("Alternativa 2");
cb2.setLayoutX(10); cb2.setLayoutY(60);
CheckBox cb3 = new CheckBox("Alternativa 3");
cb3.setLayoutX(10); cb3.setLayoutY(110);

pane.getChildren().addAll(cb1, cb2, cb3);

// verificando quais alternativas foram marcadas
if (cb1.isSelected()) System.out.println("CheckBox 1");
if (cb2.isSelected()) System.out.println("CheckBox 2");
if (cb3.isSelected()) System.out.println("CheckBox 3");
```

### RadioButton — escolha única, agrupada por `ToggleGroup`

Sem um `ToggleGroup`, cada `RadioButton` funciona de forma independente (não se excluem). Para que só um possa ficar marcado por vez, todos precisam compartilhar o **mesmo** `ToggleGroup`.

```java
RadioButton rb1 = new RadioButton("Opção 1");
rb1.setLayoutX(10); rb1.setLayoutY(10);
RadioButton rb2 = new RadioButton("Opção 2");
rb2.setLayoutX(10); rb2.setLayoutY(60);
RadioButton rb3 = new RadioButton("Opção 3");
rb3.setLayoutX(10); rb3.setLayoutY(110);

ToggleGroup tg = new ToggleGroup();
rb1.setToggleGroup(tg);
rb1.setSelected(true);   // marca a primeira como padrão
rb2.setToggleGroup(tg);
rb3.setToggleGroup(tg);

pane.getChildren().addAll(rb1, rb2, rb3);

if (rb1.isSelected())
    System.out.println("RadioButton 1");
else if (rb2.isSelected())
    System.out.println("RadioButton 2");
else if (rb3.isSelected())
    System.out.println("RadioButton 3");
```

### Spinner — campo numérico (ou de lista) com setas de incremento

```java
// Spinner para inteiros
private Spinner<Integer> spAno;
...
// configurando min, max e valor padrão
spAno.setValueFactory(
        new SpinnerValueFactory.IntegerSpinnerValueFactory(1900, 2030, 2015));

// alterando o valor manualmente
spAno.getEditor().textProperty().set("2000");

// recuperando o valor atual
Integer ano = spAno.getValue();


// Spinner para uma lista de itens (String, objetos, etc.)
private Spinner<String> spTimes;
...
ObservableList<String> spItems = FXCollections.observableArrayList();
spItems.add("Corinthians");
spItems.add("Santos");
spItems.add("São Paulo");
spTimes.valueFactoryProperty().set(new SpinnerValueFactory.ListSpinnerValueFactory<>(spItems));
```

## Calendário "aberto" (sem o ComboBox do DatePicker)

Truque para mostrar só o calendário do `DatePicker`, sem a caixinha de texto/combo:

```java
DatePicker cal = new DatePicker();
DatePickerSkin skin = new DatePickerSkin(cal);
Node calendar = skin.getPopupContent(); // obtém somente o calendário
painellateral.getChildren().add(calendar);
```