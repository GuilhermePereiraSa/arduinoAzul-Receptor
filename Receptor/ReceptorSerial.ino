#include "Temporizador.h"

// Prototipos das funções
void setup();
ISR(TIMER1_COMPA_vect); // Interrupt Service Routine

// Função de setup
void setup() {
  Serial.begin(9600);
  
  // Configura os pinos
  pinMode(PINO_RTS, INPUT);
  pinMode(PINO_CLOCK, INPUT);
  pinMode(PINO_DADOS, INPUT);
  pinMode(PINO_CTS, OUTPUT);
  pinMode(PINO_LED, OUTPUT);

  digitalWrite(PINO_CTS, LOW);
  digitalWrite(PINO_LED, LOW);

  Serial.println("Receptor ligado. Aguardando sinal RTS do Emissor...");
}

// Se variavel é compartilhada com o loop principal <=> volatile


// Rotina de interrupcao do timer1
// O que fazer toda vez que 1s passou? - LER O BIT 
ISR(TIMER1_COMPA_vect){
  if(!recebendoDado || charPronto) return;
  
  int clockAtual = digitalRead(PINO_CLOCK);

  // atual e ultimo clock -> subida
  if(clockAtual == HIGH && lastClock == LOW){
    
    int bitAtual = (PINB >> PB2) & 1;

    digitalWrite(PINO_LED, bitAtual); // LED RX

    // bits do dado
    if(indexBit < 8){
      bitWrite(charAtual, indexBit, bitAtual);
    }

    if(indexBit == 8){
      // bit de paridade
      bitParidadeRecebido = bitAtual;
      charPronto = true;
    }

    indexBit++;
  }

  lastClock = clockAtual;
}