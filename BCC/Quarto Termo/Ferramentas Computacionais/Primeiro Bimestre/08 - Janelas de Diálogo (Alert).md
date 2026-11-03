Desde o **JDK 8u40**, o JavaFX tem diálogos nativos oficiais (`Alert`, `TextInputDialog`) — não é mais necessário usar `JOptionPane` do Swing.

Referências: http://code.makery.ch/blog/javafx-dialogs-official/ e http://aprendendo-javafx.blogspot.com.br/

## Diálogo informativo simples

```java
Alert alert = new Alert(AlertType.INFORMATION);
alert.setTitle("Information Dialog");
alert.setHeaderText("Look, an Information Dialog");
alert.setContentText("I have a great message for you!");

alert.showAndWait();
```

- `setTitle` — título da janela do diálogo.
- `setHeaderText` — texto em destaque, logo abaixo do título (pode ser `null` para omitir).
- `setContentText` — a mensagem principal.
- `showAndWait()` — exibe o diálogo e **pausa** a execução até o usuário fechá-lo (diferente de `show()`, que não espera).

## Diálogo de confirmação (Sim/Não, OK/Cancelar)

```java
Alert alert = new Alert(AlertType.CONFIRMATION);
alert.setTitle("Confirmation Dialog");
alert.setHeaderText("Look, a Confirmation Dialog");
alert.setContentText("Are you ok with this?");

Optional<ButtonType> result = alert.showAndWait();
if (result.get() == ButtonType.OK) {
    // ... usuário escolheu OK
} else {
    // ... usuário escolheu CANCEL ou fechou o diálogo
}
```

> [!tip] Por que `Optional<ButtonType>`?
> O retorno é um `Optional` porque o usuário pode **fechar a janela sem escolher nada** (clicando no X). Sempre trate esse caso — em produção, prefira `result.isPresent() && result.get() == ButtonType.OK` em vez de `result.get()` direto, para evitar `NoSuchElementException`.

## Capturando um texto digitado pelo usuário

```java
TextInputDialog dialog = new TextInputDialog("walter");  // valor padrão/inicial
dialog.setTitle("Text Input Dialog");
dialog.setHeaderText("Look, a Text Input Dialog");
dialog.setContentText("Please enter your name:");

Optional<String> result = dialog.showAndWait();
System.out.println("Your name: " + result.get());
```

## Outros tipos de `AlertType`

Além de `INFORMATION` e `CONFIRMATION`, existem também `WARNING`, `ERROR` e `NONE` — a diferença entre eles é basicamente o **ícone** exibido (informação, alerta amarelo, erro vermelho) e os botões padrão.

```java
Alert erro = new Alert(Alert.AlertType.ERROR);
erro.setTitle("Erro");
erro.setHeaderText(null);
erro.setContentText("Não foi possível salvar o registro.");
erro.showAndWait();
```

## Exercício sugerido

1. Crie um botão "Excluir" que mostra um `Alert` de `CONFIRMATION`.
2. Se o usuário confirmar (`ButtonType.OK`), mostre um segundo `Alert` de `INFORMATION` dizendo "Excluído com sucesso".
3. Se cancelar, não faça nada.

Esse é exatamente o padrão usado depois na tela comercial completa (ver [[15 - Arquitetura MVC e Tela Comercial Completa]], método `clkApagar`).

## Próximo passo

Vá para [[09 - FXML e Scene Builder]] — a partir daqui, o curso passa a usar **FXML** como forma principal de montar telas (em vez de montar tudo via código Java, como fizemos até aqui).
