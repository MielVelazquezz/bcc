## Por que isso importa

O JavaFX (assim como o Swing) tem **uma única thread responsável por desenhar e atualizar a interface** — a chamada *JavaFX Application Thread*. Qualquer código demorado (um laço grande, uma chamada de rede, acesso a banco de dados, abrir uma câmera) executado **diretamente** nessa thread **trava a janela inteira** — ela para de responder até o código terminar.

A solução é rodar esse código pesado em uma **Thread separada**, e só "avisar" a interface quando precisar atualizar algo nela — usando `Platform.runLater(...)`.

## `Thread` + `Runnable` — a forma "clássica" do Java

### Exemplo: um relógio que atualiza um `Label` a cada segundo

```java
public class ThRelogio implements Runnable {
    private TelaPrincipalController tela;
    private String hora;

    public ThRelogio(TelaPrincipalController tela) {
        this.tela = tela;
    }

    @Override
    public void run() {
        while (tela.getAtivo()) {
            try {
                Thread.sleep(1000);
            } catch (Exception e) { }

            hora = LocalTime.now().format(DateTimeFormatter.ofPattern("hh:mm:ss"));

            Platform.runLater(() -> {
                tela.getDisplay().setText(hora);
            });
        }
    }
}
```

> [!important] `Platform.runLater` é obrigatório aqui
> `tela.getDisplay().setText(hora)` mexe num componente da interface (`Label`). Como esse código roda **dentro da Thread separada** (`ThRelogio`), ele **precisa** ser embrulhado em `Platform.runLater(...)` — senão o JavaFX lança uma exceção (`IllegalStateException: Not on FX application thread`) ou, na melhor das hipóteses, tem comportamento instável.

Controller que dispara essa thread:

```java
public class TelaPrincipalController implements Initializable {
    @FXML
    private Label display;
    @FXML
    private Button btstart;
    private boolean ativo = false;

    @FXML
    private void handleButtonAction(ActionEvent event) {
        if (!ativo) {
            Thread th = new Thread(new ThRelogio(this));
            th.start();
            btstart.setText("Parar");
        } else {
            btstart.setText("Iniciar");
        }
        ativo = !ativo;
    }

    public Label getDisplay() { return display; }
    public boolean getAtivo() { return ativo; }

    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
    }
}
```

Note o padrão: o `Controller` expõe `getDisplay()` e `getAtivo()` para que a classe `ThRelogio` (que roda em outra thread) consiga ler o estado atual e atualizar a tela sem acoplamento direto excessivo.

## `Timer`/`TimerTask` — alternativa mais simples para tarefas periódicas

Quando você só precisa repetir uma ação em intervalos regulares (sem um laço manual com `sleep`), o `java.util.Timer` é mais direto:

```java
public class TelaPrincipalController implements Initializable {
    @FXML
    private Label display;
    @FXML
    private Button btstart;

    @FXML
    private void handleButtonAction(ActionEvent event) {
        new Timer().schedule(new TimerTask() {
            @Override
            public void run() {
                // para acessar componentes da interface (GUI)
                Platform.runLater(() -> {
                    display.setText(LocalTime.now()
                            .format(DateTimeFormatter.ofPattern("hh:mm:ss")));
                });
            }
        }, 0, 5000); // começa imediatamente (0) e repete a cada 5s (5000ms)
    }

    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
    }
}
```

`schedule(task, delay, period)` — `delay` é quanto tempo esperar antes da primeira execução (em ms); `period` é o intervalo entre repetições (em ms).

## `Task<T>` — a forma "oficial" do JavaFX para trabalho em segundo plano

`Task` (pacote `javafx.concurrent`) é pensada especificamente para JavaFX: já vem com propriedades observáveis (`progressProperty`, `messageProperty`) que você pode **vincular (bind)** direto a componentes como `ProgressIndicator` (ver exemplo em [[05 - Componentes Básicos]]).

```java
Task<Void> task = new Task<Void>() {
    @Override
    protected Void call() throws Exception {
        // código demorado aqui, rodando fora da thread da interface
        return null;
    }
};
new Thread(task).start();
```

## Encerrando threads/tasks junto com a aplicação

Se sua aplicação tem threads em segundo plano rodando (relógios, monitoramento, câmera aberta), é importante garantir que elas parem quando o usuário fechar a janela — senão o processo Java pode continuar "vivo" mesmo depois de fechar a janela.

```java
public class MyAPP extends Application {
    // ...

    // sobrescreva o método stop()
    @Override
    public void stop() throws Exception {
        super.stop();
        Platform.exit();
        System.exit(0);
    }
    // ...
}
```

O método `stop()` da classe `Application` é chamado automaticamente pelo JavaFX quando a última janela é fechada — é o lugar certo para "limpar a casa" (fechar conexões, parar threads, liberar a webcam, etc.).

## Resumo — quando usar cada abordagem

| Preciso de...                                                                             | Use                                       |
| ----------------------------------------------------------------------------------------- | ----------------------------------------- |
| Loop contínuo customizado (ex.: relógio com `while`)                                      | `Thread` + `Runnable`                     |
| Repetir uma ação em intervalos fixos                                                      | `Timer` / `TimerTask`                     |
| Tarefa única, demorada, com progresso reportável (ex.: importação de dados, abrir câmera) | `Task<T>`                                 |
| Atualizar QUALQUER componente da interface, de dentro de uma thread                       | sempre `Platform.runLater(() -> { ... })` |
