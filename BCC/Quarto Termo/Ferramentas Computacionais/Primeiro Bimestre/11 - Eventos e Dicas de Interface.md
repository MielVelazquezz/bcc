## Como funciona o modelo de eventos, em resumo

Todo componente JavaFX pode "escutar" eventos através de métodos `setOnXxx(...)`, por exemplo:

- `setOnAction(...)` — clique de botão, Enter num TextField, seleção de item de menu.
- `setOnMouseClicked(...)`, `setOnMousePressed(...)`, `setOnMouseDragged(...)`, `setOnMouseReleased(...)` — eventos detalhados do mouse.
- `setOnKeyPressed(...)`, `setOnKeyTyped(...)` — eventos de teclado.

Cada um recebe um `EventHandler<TipoDoEvento>`, que pode ser implementado como classe anônima ou lambda (ver [[02 - Hello World e Estrutura de uma Aplicação]]).

## Janelas sem "Title Bar" (arrastar a janela manualmente)

Quando você usa `StageStyle.UNDECORATED` ou `TRANSPARENT` (ver [[05 - Componentes Básicos]]), a janela perde a barra de título — e com ela, a capacidade nativa de ser arrastada. É preciso implementar isso manualmente, capturando a posição do mouse.

### Passo a passo

1. Capture a posição do clique (`mousePressed`) em relação à tela.
2. Enquanto o mouse se move com o botão pressionado (`mouseDragged`), recalcule a posição da janela.

```java
@FXML
private void mousePressed(MouseEvent event) {
    Stage stage = ((Stage) ((Node) event.getSource()).getScene().getWindow());
    cx = stage.getX() - event.getScreenX();
    cy = stage.getY() - event.getScreenY();
}

@FXML
private void mouseDragged(MouseEvent event) {
    Stage stage = ((Stage) ((Node) event.getSource()).getScene().getWindow());
    stage.setX(event.getScreenX() + cx);
    stage.setY(event.getScreenY() + cy);
}
```

`cx` e `cy` são atributos de classe (`double`) que guardam o "deslocamento" entre o ponto onde o usuário clicou e o canto da janela — assim a janela não "salta" para debaixo do cursor ao começar a arrastar.

> Ligue `mousePressed` a `onMousePressed` e `mouseDragged` a `onMouseDragged` de algum painel que sirva de "barra de título falsa" (ex.: um `HBox` no topo da janela).

## Ícones em botões

1. Coloque o arquivo de ícone (PNG, por exemplo) na pasta `resources` do projeto.
2. No Scene Builder (ou CSS), configure a propriedade de estilo do botão para usar essa imagem como fundo/gráfico (ou, via Java, use `botao.setGraphic(new ImageView(new Image(...)))`).
3. Para deixar o botão "flutuando" (sem moldura, só o ícone), aplique:
   ```css
   -fx-background-color: transparent;
   ```