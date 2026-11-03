
# Introdução e História do JavaFX

## O que é o JavaFX
JavaFX é uma tecnologia da **Oracle** para criar **RIAs** (*Rich Internet Applications*) e **GUIs** (interfaces gráficas de usuário) em Java — ou seja, aplicações desktop (e, com esforço extra, mobile) com telas modernas, efeitos visuais, animações e estilização via CSS.

Ele é o "sucessor espiritual" das bibliotecas gráficas antigas do Java:
- **1995** — Java é lançado com **AWT** (*Abstract Window Toolkit*), a primeira biblioteca gráfica.
- Depois — surge o **Swing**, uma extensão da AWT, mais rica em componentes.
- **JavaFX** vem depois do Swing, com um modelo mais moderno (CSS, FXML, propriedades observáveis, efeitos, transições).

## Linha do tempo

| Ano | Evento |
|---|---|
| 2007 | Primeira versão: **JavaFX Script** (rodava em Desktop e Browser, com planos futuros para mobile). Usava uma linguagem de script própria (não era Java puro). |
| 2010 | JavaFX Script é **descontinuado**. |
| 2011 | Oracle adquire a Sun Microsystems e lança o **JavaFX 2.0** — agora **código 100% em Java** (chega da linguagem de script separada) e com suporte a **CSS** em aplicações desktop. |
| — | Esforços da comunidade para portar o JavaFX para mobile, incluindo iOS e Android (ver [[16 - Referências e Links Úteis]]). |
| 2014 | JavaFX é **integrado ao JSE 8.0** (Java SE 8) — passa a vir "de fábrica" com o JDK. |

## Onde o JavaFX se encaixa entre as tecnologias RIA

Na época em que o JavaFX nasceu, existiam várias tecnologias concorrentes para aplicações ricas via internet/desktop:

- **Flex** — da Adobe.
- **Silverlight** — da Microsoft.
- **Ajax e frameworks derivados** — baseados em JavaScript.
- **JavaFX** — da Oracle (a que vamos aprender aqui).

## Por que aprender JavaFX hoje
- É a biblioteca gráfica **oficial e recomendada** pela Oracle para novos projetos desktop em Java (substitui o Swing).
- Separa **interface (FXML)** de **lógica (Controller)**, seguindo naturalmente o padrão **MVC**.
- Suporta **CSS** para estilização, então quem já mexeu com CSS web se sente em casa.
- Tem uma comunidade ativa com frameworks visuais extra (JFoenix, AtlantaFX, JMetro — ver [[14 - Frameworks Visuais (JFoenix, JMetro, FXRibbon)]]).