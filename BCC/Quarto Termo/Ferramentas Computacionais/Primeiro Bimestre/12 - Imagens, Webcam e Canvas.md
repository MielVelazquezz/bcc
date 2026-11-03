## A classe `Image` do JavaFX
`Image` (pacote `javafx.scene.image`) é a representação de uma imagem dentro do JavaFX — é ela que você associa a um `ImageView` para exibir na tela.

### Abrindo uma imagem escolhida pelo usuário

```java
Image img = null; // javafx.scene.image.Image

FileChooser fc = new FileChooser();
File arq = fc.showOpenDialog(null);
if (arq != null) {
    img = new Image(arq.toURI().toString()); // nome do arquivo: JPEG, GIF ou PNG
}

img.getWidth();   // largura da imagem
img.getHeight();  // altura da imagem

// adequando as dimensões do componente à imagem
imageview.setFitWidth(img.getWidth());
imageview.setFitHeight(img.getHeight());
imageview.setImage(img);
```

Mais sobre `FileChooser`: http://docs.oracle.com/javafx/2/ui_controls/file-chooser.htm

### Gravando (salvando) uma imagem

```java
FileChooser fc = new FileChooser();
File arq = fc.showSaveDialog(null);
if (arq != null) {
    BufferedImage bimg;
    bimg = SwingFXUtils.fromFXImage(img, null);
    try {
        ImageIO.write(bimg, "jpg", arq);
    } catch (Exception e) {
        propriedades.setText(e.getMessage());
    }
}
```

## Transformações com imagens — a ponte `Image` ↔ `BufferedImage`

O JavaFX **não** tem uma API própria para manipular pixel a pixel; para isso, convertemos a `Image` do JavaFX para um `BufferedImage` (classe do AWT/Swing, `java.awt.image`), manipulamos, e convertemos de volta.

```java
// converte um Image (JavaFX) em BufferedImage (AWT)
BufferedImage bimg;
bimg = SwingFXUtils.fromFXImage(img, null);

// capturando/alterando pixels manualmente
              // Red, Green, Blue, Alpha
int pixel[] = { 0, 0, 0, 0 };
WritableRaster raster = bimg.getRaster();
raster.getPixel(i, j, pixel);   // obtém um pixel na posição (i,j)
// ... transforme o array "pixel" (ex.: inverter cores, aplicar filtro)
raster.setPixel(i, j, pixel);   // reaplica o pixel modificado

// se necessário, volta para Image do JavaFX
img = SwingFXUtils.toFXImage(bimg, null);
```

`SwingFXUtils` é a classe utilitária (pacote `javafx.embed.swing`) responsável por essa conversão nos dois sentidos.

## Processamento avançado de imagens com ImageJ

Para filtros mais avançados (detecção de bordas, erosão, dilatação, etc.), o material recomenda usar a biblioteca **ImageJ**.

### Passo a passo de instalação (IntelliJ)

1. Baixe o `ij.jar` (sem JRE): https://imagej.nih.gov/ij/download.html
2. Descompacte e copie `ij.jar` para uma pasta conhecida do seu projeto.
3. No IntelliJ: `File > Project Structure > Modules > aba Dependencies`.
4. Clique no `+` → **"1 JARs or Directories"** → selecione o `ij.jar` (ou a pasta).
5. Clique OK.
6. Abra o `module-info.java` e adicione:
   ```java
   requires java.desktop;
   requires javafx.swing;
   requires ij;
   ```

**Alternativa (Maven):**

```xml
<dependency>
    <groupId>org.openjfx</groupId>
    <artifactId>javafx-swing</artifactId>
    <version>20.0.2</version>
</dependency>
```

(procure também por uma dependência Maven equivalente para `ij`, se disponível no seu repositório).

### Usando o ImageJ (`ImagePlus` / `ImageProcessor`)

```java
// cria um ImagePlus vazio
ImagePlus imgplus = new ImagePlus();

// cria um BufferedImage a partir de um ImageView
BufferedImage bimg;
bimg = SwingFXUtils.fromFXImage(imageView.getImage(), null);

// converte o BufferedImage em ImagePlus
imgplus.setImage(bimg);

// cria o "processador" de imagem
ImageProcessor ipr = imgplus.getProcessor();

ipr.findEdges();               // detecta bordas
ipr.erode();                   // aplica efeito de erosão
ipr.drawRect(10, 10, 200, 150); // desenha um retângulo na imagem
ipr.dilate();                  // aplica efeito de dilatação

// devolve a imagem modificada ao ImageView
imageView.setImage(SwingFXUtils.toFXImage(imgplus.getBufferedImage(), null));

// para abrir uma imagem diretamente de um arquivo, sem passar pelo JavaFX
ImagePlus imagem = IJ.openImage("c:\\teste123.jpg");
```

## Webcam — capturando imagens da câmera

Biblioteca usada: **Webcam Capture** — http://webcam-capture.sarxos.pl/

### Passo a passo de instalação

1. Baixe o pacote **Webcam Capture**.
2. Adicione ao projeto os jars: `webcam-capture-0.3.10.jar`, `bridj-0.6.2.jar` e `slf4j-api-1.7.2.jar`.
3. Crie uma tela com um botão (para conectar) e um `ImageView` (para exibir o vídeo/foto).

### Iniciando a câmera e capturando uma imagem (versão simples)

```java
// iniciando a câmera
webcam = Webcam.getDefault();
webcam.setViewSize(new Dimension(320, 240));
webcam.open();
// -- em projetos reais, faça isso dentro de uma Task (câmera demora a abrir)

// no handler do botão de captura:
BufferedImage bimage;
bimage = webcam.getImage();

// opcionalmente, salvar em arquivo
ImageIO.write(bimage, "JPG", new File("test.jpg"));

// convertendo para Image do JavaFX
WritableImage wimg;
wimg = SwingFXUtils.toFXImage(bimage, null);

// aplicando no ImageView
imageview.setImage(wimg);
```

### Versão completa com Thread (conectar/desconectar sem travar a interface)

Abrir a câmera é uma operação **lenta** — se feita na thread principal (a que desenha a interface), a janela "trava" (fica sem responder) até a câmera abrir. Por isso usamos uma `Thread`/`Task` (ver [[13 - Threads e Task]] para entender por quê).

```java
// evento do botão conectar/desconectar
if (btconectar.getText().equals("Conectar")) {
    new Thread(new Task<Void>() {
        @Override
        protected Void call() throws Exception {
            ativarCamera();
            return null;
        }
    }).start();
} else {
    webcam.close();
    btconectar.setText("Conectar");
    btcapturar.setDisable(true);
}

// evento do botão capturar
if (webcam.isOpen()) {
    BufferedImage bimage;
    bimage = webcam.getImage();
    WritableImage wimg;
    wimg = SwingFXUtils.toFXImage(bimage, null);
    imageview.setImage(wimg);
}
```

```java
// método que efetivamente abre a câmera, chamado de dentro da Thread
private void ativarCamera() {
    Platform.runLater(() -> {
        btcapturar.getScene().setCursor(Cursor.WAIT);
    });

    webcam = Webcam.getDefault();
    webcam.setViewSize(new Dimension(320, 240));
    webcam.open();

    Platform.runLater(() -> {
        btcapturar.setDisable(false);
        btconectar.setText("Desconectar");
        btcapturar.getScene().setCursor(Cursor.DEFAULT);
    });
}
```

> [!important] `Platform.runLater(...)`
> Qualquer código que **toque em um componente da interface** (mudar texto, cursor, habilitar/desabilitar botão) só pode rodar na **thread da interface gráfica (JavaFX Application Thread)**. Como `ativarCamera()` roda numa `Thread` separada, tudo que mexe na tela precisa ser "empacotado" dentro de `Platform.runLater(() -> { ... })`. Veja [[13 - Threads e Task]] para o motivo completo.

## Canvas — desenho livre (formas geométricas e linhas)

O `Canvas` é uma "tela em branco" onde você desenha usando um `GraphicsContext` — útil para gráficos, assinaturas, desenho livre, jogos simples.

### Passo a passo

1. Tenha um `Canvas` na tela (via FXML, com `fx:id`, ou criado via código).
2. Pegue o `GraphicsContext` dele: `canvas.getGraphicsContext2D()`.
3. Use os métodos de desenho (`fillOval`, `strokeRect`, `fillPolygon`, etc.).

### Exemplo: formas geométricas

```java
@FXML
private Canvas desenho;

@Override
public void initialize(URL url, ResourceBundle rb) {
    drawShapes(desenho.getGraphicsContext2D());
}

private void drawShapes(GraphicsContext gc) {
    gc.setFill(Color.GREEN);
    gc.setStroke(Color.BLUE);
    gc.setLineWidth(2);

    gc.strokeLine(40, 10, 10, 40);

    gc.fillOval(10, 60, 30, 30);
    gc.strokeOval(60, 60, 30, 30);

    gc.fillRoundRect(110, 60, 30, 30, 10, 10);
    gc.strokeRoundRect(160, 60, 30, 30, 10, 10);

    gc.fillArc(10, 110, 30, 30, 45, 240, ArcType.OPEN);
    gc.fillArc(60, 110, 30, 30, 45, 240, ArcType.CHORD);
    gc.fillArc(110, 110, 30, 30, 45, 240, ArcType.ROUND);
    gc.strokeArc(10, 160, 30, 30, 45, 240, ArcType.OPEN);
    gc.strokeArc(60, 160, 30, 30, 45, 240, ArcType.CHORD);
    gc.strokeArc(110, 160, 30, 30, 45, 240, ArcType.ROUND);

    gc.fillPolygon(new double[]{10, 40, 10, 40},
                    new double[]{210, 210, 240, 240}, 4);
    gc.strokePolygon(new double[]{60, 90, 60, 90},
                      new double[]{210, 210, 240, 240}, 4);
    gc.strokePolyline(new double[]{110, 140, 110, 140},
                       new double[]{210, 210, 240, 240}, 4);
}
```

- `fillX(...)` — desenha a forma **preenchida** (cor definida por `setFill`).
- `strokeX(...)` — desenha só o **contorno** (cor definida por `setStroke`, espessura por `setLineWidth`).

### Exemplo: desenho livre com "caneta" (moveTo / lineTo / stroke)

```java
private void drawShapes(GraphicsContext gc) {
    gc.moveTo(0, 0);      // coloca a "caneta" na posição inicial
    gc.lineTo(100, 100);  // risca até a próxima posição
    gc.lineTo(150, 200);
    gc.lineTo(250, 280);
    gc.stroke();          // efetivamente desenha o traço acumulado
}
```