#include <Servo.h>

const int potNivel = A2;
const int potTempo = A1;
const int potAbertura = A0;
const int botaoNivel = 3;
const int botaoServo = 2;
const int ledInicio = 4;
const int pinServo = A5;

int valorpotNivel = 0;
int valorpotTempo = 0;
int valorPotAbertura = 0;
int estadoBotaoNivel = 0;
int estadoBotaoServo = 0;
int nivelConfirmado = 0;
int tempoConfirmado = 0;
int aberturaConfirmada = 0;

Servo meuServo;

void setup() {
  pinMode(botaoNivel, INPUT_PULLUP);
  pinMode(botaoServo, INPUT_PULLUP);
  for (int i = ledInicio; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
  meuServo.attach(pinServo);
  Serial.begin(9600);
}

void loop() {
  valorpotNivel = analogRead(potNivel);
  valorpotTempo = analogRead(potTempo);
  valorPotAbertura = analogRead(potAbertura);

  int nivelMapeado = map(valorpotNivel, 0, 1023, 0, 100);
  int tempoMapeado = map(valorpotTempo, 0, 1023, 0, 60);
  int aberturaMapeada = map(valorPotAbertura, 0, 1023, 0, 180);

  estadoBotaoNivel = digitalRead(botaoNivel);
  estadoBotaoServo = digitalRead(botaoServo);

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
  for (int i = ledInicio; i <= 13; i++) {
    digitalWrite(i, (i - ledInicio < ledsAcender) ? HIGH : LOW);
  }
}

int calcularReducaoContaminacao(int nivel, int abertura, int tempo) {
  // Calculo simplificado de redução de contaminação
  int reducao = (abertura / 18) * tempo;
  return reducao;
}
