/*
  Serial:

  - Configura clock - usado para mandar-receber bits 
  - Configura leds para quando a passagem dos dados ocorre

  - no arduino.ino chega os bits -> chama funcao do arduino.ino para transformar em char
  -> chama a funcao de bitParidade do Serial.ino para verificar se o dado (char + bit de paridade)
    XOR dos bits do char tem que ser o mesmo do bit de paridade!  

*/
#define PINO_RX 13 
#define PINO_TX 13
#define BAUD_RATE 1 
#define HALF_BAUD 1000/(2*BAUD_RATE)

#include "Temporizador.h"

// Calcula bit de paridade - Par!

// Aqui o bit ja vem como char, de alguma forma ele ja foi montado
bool bitParidade(char dado){
    int count = 0;

    while(dado > 0) {
        count += dado & 1;   // Verifica se o último bit é 1
        dado = dado >> 1;    // Desloca para a direita, eliminando o último bit de dado
    }

    return count % 2 == 0; // 0 par, 1 ímpar
}

// Rotina de interrupcao do timer1
// O que fazer toda vez que 1s passou? 
// Toda vez que passar 1 segundo eu tenho que ler (ou escrever, se emissor) o bit
// transmitido para formacao do char (VERIFICA-SE bits de inicio e fim*)

// * bit de inicio e fim tem que ser verificados por uma funcao que interrompa tbm
// ou ja utilizamos no proprio ISR** 

// - usar volaties aqui
ISR(TIMER1_COMPA_vect){

  // se "conseguir" ler o bit

  // se necessario verifica - ou verifica no loop do arduino - quando recebe os bits** 

  // else para temporizador - pode ler o proximo
  paraTemporizador();
  // desce o pino de CTS? - nao faz sentido ficar ligado
}

// Executada uma vez quando o Arduino reseta
void setup(){
  //desabilita interrupcoes
  noInterrupts();
  // Configura porta serial (Serial Monitor - Ctrl + Shift + M)
  Serial.begin(9600);

  // Inicializa TX ou RX // EMITIR SINAIS DE CLOCK? -> OUTPUT
  pinMode(PINO_RX, OUTPUT);
  
  // Configura timer -- pq baud_rate * n? pra ter n ticks, n interações (interrupções sequentes) a se fazer 
  configuraTemporizador(BAUD_RATE);

  iniciaTemporizador();

  // habilita interrupcoes
  interrupts();
}

// O loop() eh executado continuamente (como um while(true))
void loop ( ) {
  // para cada bit espera-se
  digitalWrite(PINO_RX, HIGH);

  delay(HALF_BAUD);
  
  digitalWrite(PINO_RX, LOW);

  delay(HALF_BAUD);
}
