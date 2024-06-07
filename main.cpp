#include <Servo.h>

const int pinoPotNivel = A2;
const int pinoPotTempo = A1;
const int pinoPotAbertura = A0;
const int pinoBotaoNivel = 3;
const int pinoBotaoServo = 2;
const int pinoLedInicio = 4;
const int pinoServo = A5;

int valorPotNivel = 0;
int valorPotTempo = 0;
int valorPotAbertura = 0;
int estadoBotaoNivel = 0;
int estadoBotaoServo = 0;
int nivelConfirmado = 0;
int tempoConfirmado = 0;
int aberturaConfirmada = 0;

Servo meuServo;

void setup() {
  pinMode(pinoBotaoNivel, INPUT_PULLUP);
  pinMode(pinoBotaoServo, INPUT_PULLUP);
  for (int i = pinoLedInicio; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
  meuServo.attach(pinoServo);
  Serial.begin(9600);
}

void loop() {
  valorPotNivel = analogRead(pinoPotNivel);
  valorPotTempo = analogRead(pinoPotTempo);
  valorPotAbertura = analogRead(pinoPotAbertura);

  int nivelMapeado = map(valorPotNivel, 0, 1023, 0, 100);
  int tempoMapeado = map(valorPotTempo, 0, 1023, 0, 60);
  int aberturaMapeada = map(valorPotAbertura, 0, 1023, 0, 180);

  estadoBotaoNivel = digitalRead(pinoBotaoNivel);
  estadoBotaoServo = digitalRead(pinoBotaoServo);

  if (estadoBotaoNivel == LOW) {
    nivelConfirmado = nivelMapeado;
    atualizarLedBarGraph(nivelConfirmado);
  }

  if (estadoBotaoServo == LOW) {
    tempoConfirmado = tempoMapeado;
    aberturaConfirmada = aberturaMapeada;

    meuServo.write(aberturaConfirmada);
    delay(tempoConfirmado * 1000);

    int reducaoContaminacao = calcularReducaoContaminacao(nivelConfirmado, aberturaConfirmada, tempoConfirmado);
    nivelConfirmado = max(0, nivelConfirmado - reducaoContaminacao);
    atualizarLedBarGraph(nivelConfirmado);

    meuServo.write(0);
  }

  Serial.print("Nível de Saúde: ");
  Serial.print(nivelMapeado);
  Serial.print(" | Nível Confirmado: ");
  Serial.print(nivelConfirmado);
  Serial.print(" | Tempo: ");
  Serial.print(tempoMapeado);
  Serial.print(" | Abertura: ");
  Serial.print(aberturaMapeada);
  Serial.println();

  delay(100);
}

void atualizarLedBarGraph(int nivel) {
  int ledsAcender = map(nivel, 0, 100, 0, 10);
  for (int i = pinoLedInicio; i <= 13; i++) {
    digitalWrite(i, (i - pinoLedInicio < ledsAcender) ? HIGH : LOW);
  }
}

int calcularReducaoContaminacao(int nivel, int abertura, int tempo) {
  // Calculo simplificado de redução de contaminação
  int reducao = (abertura / 18) * tempo;
  return reducao;
}
