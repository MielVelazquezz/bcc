## Abrindo uma janela modal a partir de um FXML

Uma janela **modal** trava a interação com a janela anterior até ser fechada — muito usada em telas de cadastro, confirmação e formulários secundários.

```java
Stage stage = new Stage();
Scene scene = new Scene(FXMLLoader.load(getClass().getResource("FXMLDocument.fxml")));
stage.setScene(scene);
stage.setTitle("Título da Janela");
stage.setResizable(false);
stage.initModality(Modality.APPLICATION_MODAL);
stage.showAndWait();
```

- `initModality(Modality.APPLICATION_MODAL)` — bloqueia toda a aplicação até essa janela fechar.
- `showAndWait()` — em vez de `show()`, **pausa** a execução do código que abriu a janela até ela ser fechada (útil quando você precisa saber o resultado assim que a janela fecha).

## Passando parâmetros entre janelas

Cenário comum: você abre uma segunda tela e precisa "avisá-la" de algo (ex.: o número de uma comanda, o ID de um registro selecionado).

### Passo a passo

1. **Na controladora da tela de destino**, crie um método público para receber o parâmetro.
2. **Na tela de origem**, use um `FXMLLoader` "manual" (em vez do atalho `FXMLLoader.load(...)`) para conseguir acesso à instância do controller **antes** de mostrar a janela.
3. Chame o método criado no passo 1, passando o valor.
4. Só então mostre a janela.

```java
// 1) Na controladora ALVO (da nova janela):
public class FXMLComandaController implements Initializable {
    // ...
    public void setNumeroComanda(int num) {
        comanda = num;
        // ... atualizar a tela com esse valor, se necessário
    }
    // ...
}

// 2) Na tela de ORIGEM (quem abre a nova janela):

// cria um loader para a janela alvo
FXMLLoader loader = new FXMLLoader(getClass().getResource("FXMLComanda.fxml"));
// carrega o painel (isso já instancia o controller internamente)
Parent root = (Parent) loader.load();
// pega a instância do controller que o loader acabou de criar
FXMLComandaController ctr = loader.getController();
// chama o método para passar o parâmetro
ctr.setNumeroComanda(i);

// abre a nova janela
Stage stage = new Stage();
stage.setScene(new Scene(root));
stage.showAndWait();
```

> [!important] Por que não usar `FXMLLoader.load(...)` direto?
> O atalho estático `FXMLLoader.load(getClass().getResource(...))` carrega e descarta a referência ao `FXMLLoader`, então você **não consegue** chamar `getController()` depois. Por isso, quando você precisa acessar o controller da nova janela (para passar parâmetros, chamar métodos, etc.), sempre **instancie o `FXMLLoader` manualmente** (`new FXMLLoader(...)`), chame `.load()` nele, e só depois `.getController()`.

## Acessando o `Stage` a partir de um componente

Às vezes você precisa fechar/mover a janela atual, mas só tem acesso a um componente (ex.: um botão) — não tem uma referência direta ao `Stage`. A solução é "subir" pela árvore de nós: componente → cena → janela.

```java
mybutton.getScene().getWindow().hide();

// ou, se precisar do objeto Stage especificamente (para chamar métodos como setTitle):
((Stage) (mybutton.getScene().getWindow())).close();
```

> [!warning] Não funciona dentro do `initialize()`
> Esse truque **não funciona** dentro do método `public void initialize(URL url, ResourceBundle rb)` da controladora — nesse momento a tela ainda não foi totalmente associada a uma `Scene`/`Stage`, então `getScene()` retorna `null`. Use essa técnica **dentro de um evento** (clique de botão, etc.), não na inicialização.

## Fechando a janela a partir do evento de um componente

```java
private void clickBFechar(ActionEvent event) {
    ((Stage) ((Node) event.getSource()).getScene().getWindow()).close();
}
```

Aqui usamos `event.getSource()` (o componente que disparou o evento) em vez de uma referência fixa — funciona para **qualquer** botão, sem precisar saber o nome dele.

> [!danger] Não funciona para itens de menu
> Essa técnica **não funciona para itens de `Menu`/`MenuItem`** — o `getSource()` de um evento de menu não tem o mesmo caminho até a `Scene` que um `Button` normal tem. Para menus, guarde uma referência ao `Stage` de outra forma (por exemplo, como atributo estático ou passado via `setStage(...)`, como fizemos com parâmetros na seção anterior).

