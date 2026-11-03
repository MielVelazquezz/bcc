O visual "padrão" do JavaFX (tema Modena) é funcional, mas simples. Existem bibliotecas de terceiros que trazem componentes com visual mais moderno, prontos para usar no lugar dos componentes padrão.

## JFoenix — componentes estilo Material Design

Site oficial: http://www.jfoenix.com/
Repositório: https://github.com/jfoenixadmin/JFoenix
Videoaulas: https://www.youtube.com/watch?v=22QlOj6JVe4&list=PLhs1urmduZ29LNYi_MaoU60JemQ6Aei6A

### Passo a passo de instalação

1. Baixe o `jfoenix.jar` a partir do repositório no GitHub (ou adicione como dependência Maven/Gradle, se o seu ambiente permitir).
2. **Instale o pacote no Scene Builder** — assim os componentes JFoenix (como `JFXButton`, `JFXComboBox`, `JFXTextField`) aparecem na paleta de componentes do editor visual, e você pode arrastá-los normalmente para a tela.
3. **Adicione o pacote ao projeto** (NetBeans ou o IDE que estiver usando), da mesma forma que qualquer outra biblioteca externa (adicionar `.jar` às dependências do projeto).

Depois de instalado, os componentes JFoenix substituem os componentes padrão (ex.: `JFXButton` no lugar de `Button`) tanto no FXML quanto no código Java, com efeitos visuais como *ripple* (ondulação ao clicar) já embutidos.

### Dica prática: selecionando um item num `JFXComboBox`

Diferente do `ComboBox` padrão, o `JFXComboBox` exige alguns passos extras para funcionar corretamente:

1. Carregue o combobox com os objetos (igual ao `ComboBox` normal — ver [[06 - ComboBox, ListView e Choice]]).
2. Habilite o componente: `cbbox.setDisable(false);`
3. Posicione no primeiro item (se necessário): `cbbox.getSelectionModel().select(0);`
4. Selecione o objeto desejado: `cbbox.getSelectionModel().select(objeto);`

> [!warning] Ordem importa
> Pular o passo de habilitar o componente (`setDisable(false)`) antes de selecionar um item é uma causa comum de o `JFXComboBox` "não responder" como esperado no JFoenix.

## JMetro — tema visual estilo Windows Metro/Fluent

Site: https://www.pixelduke.com/jmetro

Diferente do JFoenix (que troca os componentes), o **JMetro** é mais parecido com os temas `MODENA`/`CASPIAN` vistos em [[04 - CSS e Estilos]]: aplica um **tema visual completo** por cima dos componentes padrão do JavaFX, deixando a aplicação com a estética "Metro" (usada em versões antigas do Windows/Office).

## FXRibbon — barra de ferramentas estilo "Ribbon" (Office)

Site: https://www.pixelduke.com/fxribbon

Componente para criar aquela barra de ferramentas em abas, com grupos de botões grandes, no estilo do Microsoft Office (Word, Excel) — útil para aplicações desktop mais "corporativas".

## Quando vale a pena usar um framework visual?

| Cenário | Recomendação |
|---|---|
| Protótipo rápido, aprendizado, projeto pequeno | Componentes padrão do JavaFX + CSS próprio (ver [[04 - CSS e Estilos]]) |
| Quer visual "Material Design" pronto (efeitos, sombras) | JFoenix |
| Quer visual estilo Windows/Office sem trocar componentes | JMetro |
| Aplicação com muitas ferramentas/comandos organizados por aba | FXRibbon |

