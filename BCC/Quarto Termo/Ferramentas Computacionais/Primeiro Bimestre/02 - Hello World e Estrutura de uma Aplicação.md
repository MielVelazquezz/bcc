## Os 3 conceitos fundamentais

Toda aplicação JavaFX é composta por 3 "camadas" que você precisa entender antes de escrever qualquer código:
1. **`Stage`** (Palco) — é a **janela** do sistema operacional (a borda, o título, os botões minimizar/fechar). Toda aplicação recebe automaticamente um `Stage` chamado "primary stage".
2. **`Scene`** (Cenário) — é o **conteúdo** que fica dentro do palco: um objeto `Scene` guarda a árvore de componentes visuais e tem uma largura/altura.
3. **`Pane`** (Painel) e seus componentes (`Button`, `Label`, etc.) — são os elementos visuais propriamente ditos, organizados dentro da `Scene` usando algum **layout** (ver [[03 - Layouts (Painéis)]]).

Resumindo a hierarquia:

```
Stage (janela)
 └─ Scene (cenário)
     └─ Pane / layout (painel raiz)
         └─ Button, Label, TextField... (componentes)
```

## Passo a passo: criando o "Hello World"

### Passo 1 — Crie a classe principal

A classe principal precisa **estender `Application`** e sobrescrever o método `start(Stage stage)`. É esse método que o JavaFX chama automaticamente para você montar a tela.

### Passo 2 — Configure o `main`

No `main`, chame `launch(args)` — esse método interno da classe `Application` é quem inicializa toda a engine gráfica do JavaFX e, por baixo dos panos, chama o `start()`.

### Passo 3 — Dentro do `start`, monte a interface

1. Dê um título ao palco: `stage.setTitle(...)`.
2. Crie um painel (`Pane`) — o "container" raiz.
3. Crie e configure os componentes (ex.: um `Button`).
4. Associe uma ação ao componente (o que acontece quando o usuário interage).
5. Adicione o(s) componente(s) ao painel.
6. Crie um `Scene`, passando o painel e um tamanho (largura, altura).
7. Associe a `Scene` ao `Stage` com `stage.setScene(...)`.
8. Mostre a janela com `stage.show()`.

### Exemplo completo (usando classe anônima, sintaxe "clássica")

```java
public class JavaFXApplication1 extends Application {

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage stage) {
        // 1) configuração do palco
        stage.setTitle("Hello World");

        // 2) criando um painel
        Pane painel = new Pane();

        // 3) criando e configurando um botão
        Button botao = new Button();
        botao.setText("Clique aqui...");

        // 4) associando uma ação ao botão
        botao.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                System.out.println("Hello world!!!");
            }
        });

        // 5) adicionando o botão no painel
        painel.getChildren().add(botao);

        // 6-7-8) cenário, palco e exibição
        Scene cenario = new Scene(painel, 640, 480);
        stage.setScene(cenario);
        stage.show();
    }
}
```

## O mesmo exemplo com Expressão Lambda (forma moderna e recomendada)

Desde o Java 8, `EventHandler` é uma interface funcional, então podemos trocar a classe anônima por uma **lambda**, muito mais curta:

```java
botao.setOnAction(e -> {
    System.out.println("Hello world!");
});
```

> [!tip] Regra prática
> Sempre que você ver `new AlgumaInterface() { @Override public void metodo(...) {...} }` com **um único método abstrato**, dá para trocar por uma lambda `(parametros) -> { corpo }`.

## Delegando o evento para um método separado

Para deixar o código mais organizado (principalmente quando a lógica do clique cresce), extraia a lambda para um **método próprio**:

```java
botao.setOnAction(e -> evento(e));

// ...

void evento(ActionEvent e) {
    System.out.println("Hello world!!!");
}
```

Isso funciona porque `evento` tem a mesma assinatura exigida pelo método `handle` da interface `EventHandler<ActionEvent>` (recebe um `ActionEvent` e não retorna nada).

> [!warning] Cuidado com referência de método
> Como `evento` já bate com a assinatura esperada, você também pode simplificar ainda mais usando **referência de método**: `botao.setOnAction(this::evento);` — funciona igual, é só outra sintaxe.

## Exercício sugerido

1. Crie um novo projeto JavaFX no seu IDE.
2. Reproduza o exemplo acima com lambda.
3. Troque o texto do botão e a mensagem impressa no console.
4. Adicione um segundo botão que muda o título da janela (`stage.setTitle(...)`) quando clicado.