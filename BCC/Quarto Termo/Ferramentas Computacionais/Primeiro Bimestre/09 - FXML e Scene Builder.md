## O que é FXML e por que usar
Até agora, montamos as telas **inteiramente em código Java** (`new Button()`, `setLayoutX()`, etc.). Isso funciona, mas mistura "desenho da tela" com "lógica do sistema" no mesmo arquivo — fica difícil de manter conforme o projeto cresce.

**FXML** é um formato **XML** que descreve a interface (layout + componentes), separado da lógica. A lógica fica numa classe **Controller** separada. Isso é, na prática, aplicar o padrão **MVC** (Model-View-Controller) dentro do próprio JavaFX:

- **View** → o arquivo `.fxml` (desenhado visualmente no **Scene Builder**, um editor gráfico tipo "arrastar e soltar").
- **Controller** → uma classe Java `TelaController.java`, ligada ao FXML, que recebe os eventos dos componentes.
- **Application** → a classe principal `FXApplication.java`, que carrega o FXML e abre a janela.

```
FXApplication.java  →  carrega  →  Tela.fxml  →  aponta para  →  TelaController.java
     (main/start)                  (a "View")            (fx:controller="...")
```

## Scene Builder — o editor visual

Baixe em: http://gluonhq.com/open-source/scene-builder/

No Scene Builder você arrasta componentes para a tela, ajusta layout/propriedades visualmente, e o próprio editor gera o XML do `.fxml` para você. Você pode abrir o `.fxml` direto pelo IDE (IntelliJ/NetBeans) com clique direito → "Open in SceneBuilder".

## Passo a passo: criando uma tela do zero com FXML

### Passo 1 — Crie o arquivo `.fxml` (via Scene Builder ou manualmente)

Todo `.fxml` tem uma estrutura parecida:

```xml
<?xml version="1.0" encoding="UTF-8"?>

<?import javafx.scene.text.*?>
<?import java.lang.*?>
<?import java.util.*?>
<?import javafx.scene.*?>
<?import javafx.scene.control.*?>
<?import javafx.scene.layout.*?>

<AnchorPane id="AnchorPane" prefHeight="204.0" prefWidth="447.0"
            xmlns:fx="http://javafx.com/fxml/1"
            xmlns="http://javafx.com/javafx/8"
            fx:controller="fxapplication.TelaController">
   <children>
      <Label layoutX="126.0" layoutY="14.0" text="JavaFX">
         <font>
            <Font size="48.0" />
         </font>
      </Label>
      <Button fx:id="bFechar" layoutX="198.0" layoutY="148.0"
              mnemonicParsing="false" onAction="#clickBotaoFechar" text="Fechar" />
   </children>
</AnchorPane>
```

Pontos-chave para entender essa estrutura:

- **`fx:controller="fxapplication.TelaController"`** — diz ao JavaFX **qual classe Java** vai controlar essa tela (deve ser o caminho completo, com package).
- **`fx:id="bFechar"`** — dá um "apelido" ao componente, para que o Controller consiga referenciá-lo via `@FXML`.
- **`onAction="#clickBotaoFechar"`** — liga o evento de clique ao **método** `clickBotaoFechar` do Controller (repare o `#` antes do nome).
- As tags `<?import .../?>` no topo são os "imports" — cada tipo de componente usado no FXML precisa do seu import correspondente (o Scene Builder cuida disso sozinho).

### Passo 2 — Crie a classe Controller

A classe Controller normalmente implementa `Initializable` (para ter um lugar certo — `initialize()` — de rodar código assim que a tela é carregada) e usa a anotação `@FXML` para "capturar" os componentes e métodos referenciados no `.fxml`.

```java
package fxapplication;

import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;

public class TelaController implements Initializable {

    @FXML
    private Button bFechar;

    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO: código a executar assim que a tela é carregada
    }

    @FXML
    private void clickBotaoFechar(ActionEvent event) {
        System.exit(0);
    }
}
```

> [!important] O `fx:id` do FXML precisa bater com o nome do atributo
> `fx:id="bFechar"` no FXML deve corresponder **exatamente** ao nome do atributo `private Button bFechar;` no Controller (nome sensível a maiúsculas/minúsculas). É assim que o JavaFX sabe "encaixar" um no outro.

### Passo 3 — Crie a classe principal (`Application`) que carrega o FXML

```java
package fxapplication;

import java.io.IOException;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class FXApplication extends Application {

    @Override
    public void start(Stage stage) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource("Tela.fxml"));
        Scene scene = new Scene(root);
        stage.setScene(scene);
        stage.setTitle("Tela JavaFX");
        stage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
```

`FXMLLoader.load(...)` faz todo o trabalho pesado: lê o `.fxml`, instancia os componentes, instancia a classe Controller (indicada em `fx:controller`), injeta os componentes anotados com `@FXML` e chama `initialize()`.

## Resumindo o fluxo completo (os 3 arquivos)

| Arquivo | Papel |
|---|---|
| `Tela.fxml` | Desenho da tela (View) |
| `TelaController.java` | Lógica/eventos da tela (Controller) |
| `FXApplication.java` | Ponto de entrada da aplicação — carrega o FXML e mostra a janela |

## Próximo passo

Agora que você sabe criar uma tela com FXML, vá para [[10 - Múltiplas Janelas e Stage]] para aprender a abrir novas janelas, passar parâmetros entre elas e fechá-las corretamente.
