#define LED_VERDE1 2
#define LED_VERMELHO 3
#define LED_AMARELO 4
#define LED_VERDE2 5

#define BOTAO_VERDE1 8
#define BOTAO_VERMELHO 9
#define BOTAO_AMARELO 10
#define BOTAO_VERDE2 11

#define BUZZER 7

#define INDEFINIDO -1

#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500

#define TAMANHO_SEQUENCIA 6

enum Estados {
  PRONTO_PARA_PROX_RODADA, //0
  USUARIO_RESPONDENDO, //1
  JOGO_FINALIZADO_SUCESSO, //2
  JOGO_FINALIZADO_FALHA //3
};

int sequenciaLuzes[TAMANHO_SEQUENCIA];
int rodada = 0;
int leds_respondidos = 0;

void setup() {
  Serial.begin(9600);
  iniciaPortas();
  iniciaJogo();
}

void iniciaJogo() {
  int jogo = analogRead(0); // pegando "sinal aleatório" - voltagem do ar
  randomSeed(jogo); // Função que vai randomizar o número da função random()

  for (int i = 0; i < TAMANHO_SEQUENCIA; i++) {
    sequenciaLuzes[i] = sorteiaCor(); // número randômico entre 2 e 5
  }
}

int sorteiaCor() {
  return random(LED_VERDE1, LED_VERDE2 + 1);
}

void iniciaPortas() {
  pinMode(LED_VERDE1, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE2, OUTPUT);

  pinMode(BOTAO_VERDE1, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_VERDE2, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);
}

void loop() {
  switch(estadoAtual()) {
    case PRONTO_PARA_PROX_RODADA:
      Serial.println("Pronto para próxima rodada");
      preparaNovaRodada();
      break;
    case USUARIO_RESPONDENDO:
      Serial.println("Usuário respondendo");
      processaRespostaUsuario();
      break;
    case JOGO_FINALIZADO_SUCESSO:
      Serial.println("Jogo finalizado com sucesso");
      jogoFinalizadoSucesso();
      break;
    case JOGO_FINALIZADO_FALHA:
      Serial.println("Jogo finalizado com falha");
      jogoFinalizadoFalha();
      break;
  }
  delay(500);
}

void preparaNovaRodada() {
  rodada++;
  leds_respondidos = 0;
  if (rodada <= TAMANHO_SEQUENCIA) {
    tocaLedsRodada();
  }
}
                                                                                                                                                                                                  
void processaRespostaUsuario() {
  int resposta = checaRespostaJogador();
  if (resposta == INDEFINIDO) {
    return;
  }

  if (resposta == sequenciaLuzes[leds_respondidos]) {
    leds_respondidos++;
  }
  else {
    Serial.println("Resposta errada");
    tone(BUZZER, 300, 5000);
    rodada = TAMANHO_SEQUENCIA + 2; // Quando isso acontecer, jogo finalizado
  }
}

int estadoAtual() {
  if (rodada <= TAMANHO_SEQUENCIA) {
    if (leds_respondidos == rodada) {
      return PRONTO_PARA_PROX_RODADA;
    } else {
      return USUARIO_RESPONDENDO;
    }
  } else if (rodada == TAMANHO_SEQUENCIA + 1) {
    // Computar estado atual
    return JOGO_FINALIZADO_SUCESSO;
  } else {
    return JOGO_FINALIZADO_FALHA;
  }
}

void tocaLedsRodada() {
  for (int i = 0; i < rodada; i++) {
    piscaLed(sequenciaLuzes[i]);
  }
}

int checaRespostaJogador() {
  if (digitalRead(BOTAO_VERDE1) == LOW) {
    return piscaLed(LED_VERDE1);
  }
  if (digitalRead(BOTAO_AMARELO) == LOW) {
    return piscaLed(LED_AMARELO);
  }
  if (digitalRead(BOTAO_VERMELHO) == LOW) {
    return piscaLed(LED_VERMELHO);
  }
  if (digitalRead(BOTAO_VERDE2) == LOW) {
    return piscaLed(LED_VERDE2);
  }
  return INDEFINIDO;
}

void jogoFinalizadoFalha() {  
  for (int i = 0; i < 3; i++) {
    piscaLedFinal(LED_VERDE1);
    piscaLedFinal(LED_VERMELHO);
    piscaLedFinal(LED_AMARELO);
    piscaLedFinal(LED_VERDE2);
  }
}

void jogoFinalizadoSucesso() {
  tocaSom(2500);
  digitalWrite(LED_VERDE1, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_VERDE2, HIGH);
  delay(MEIO_SEGUNDO);
  tocaSom(2500);
  digitalWrite(LED_VERDE1, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE2, LOW);
  delay(MEIO_SEGUNDO);
}

int piscaLed(int portaLed) {
  verificaSomDoLed(portaLed);

  digitalWrite(portaLed, HIGH);
  delay(MEIO_SEGUNDO);
  digitalWrite(portaLed, LOW);
  delay(MEIO_SEGUNDO);
  return portaLed;
}

int piscaLedFinal(int portaLed) {
  digitalWrite(portaLed, HIGH);
  delay(200);
  digitalWrite(portaLed, LOW);
  delay(200);
  return portaLed;
}

void tocaSom(int frequencia) {
  tone(BUZZER, frequencia, 100); // Recebe também o tempo de duração do som
}

void verificaSomDoLed(int portaLed) {
  switch (portaLed) {
    case LED_VERDE1:
      tocaSom(2000);
      break;
    case LED_AMARELO:
      tocaSom(2200);
      break;
    case LED_VERMELHO:
      tocaSom(2400);
      break;
    case LED_VERDE2:
      tocaSom(2500);
      break;
  }
} 
