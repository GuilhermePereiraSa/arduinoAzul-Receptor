#include "Temporizador.h"

#define BAUD_RATE 1 
#define HALF_BAUD 1000/(2*BAUD_RATE)

#define PINO_CTS 9 // indicar para o outro arduino que estamos prontos
#define PINO_DADOS 10 // passagem dos dados, frame inteiro!
#define PINO_CLOCK 11 // configurado no serial -> passado para aqui 
#define PINO_RTS 12 // do outro arduino - entrada para este de recepção 
#define PINO_LED 13 // pino de LED (RX - request para confirmar prontidão para send de dados)

// Prototipo das funções 
void loop();
void processaChar();
bool bitParidade(char dado);


volatile uint8_t charAtual = 0;
volatile uint8_t indexBit = 0;
volatile bool recebendoDado = false; // esta no ISR - para leitura
volatile bool charPronto = 0; // loop

volatile bool bitParidadeRecebido = 0;

volatile int lastClock = 0;


/*
  Foi definido paridade par

  FRAME: | START | b7 b6 b5 b4 b3 b2 b1 b0 | PAR | STOP | 
*/

// Função para calcular bit de paridade par
bool bitParidade(char dado){
    int count = 0;

    unsigned char uData = (unsigned char)dado;

    while(uData > 0) {
        count += uData & 1;   // Verifica se o último bit é 1
        uData = uData >> 1;    // Desloca para a direita, eliminando o último bit de dado
    }

    return count % 2 == 0; // 0 par, 1 ímpar
}


// Função para processar o charAtual
void processaChar(){
  // Verifica paridade

  if(bitParidade(charAtual) != bitParidadeRecebido){
    Serial.println("Erro de paridade!");
    return;
  }

  // Sucesso
  Serial.println("Recebido: ", (char)charAtual);
}


// Função loop que serve para receber os dados
void loop() {
  // Fica lendo o pino RTS do emissor
  int estadoRTS = digitalRead(PINO_RTS);

  // Se o emissor pediu para enviar (RTS em HIGH)...
  // e se for high, fica preso aqui
  if (estadoRTS == HIGH) {
    Serial.println("RTS Detectado!");
    
    // config tudo
    recebendoDado = true;
    charPronto = false;
    indexBit = 0; // 0 a 7
    charAtual = 0; // 1 char é 8 bits.

    lastClock = digitalRead(PINO_CLOCK);

    // Levanta o CTS 
    digitalWrite(PINO_CTS, HIGH);

    // oversampling
    configuraTemporizador(10*BAUD_RATE);

    iniciaTemporizador();
  }

  if(charPronto){    
    // nao necessario mais o clock!
    paraTemporizador();

    while(digitalRead(PINO_RTS) == HIGH) {
      // polling ate o emissor abaixar o rts
    }

    // Emissor baixou o RTS (terminou)
    Serial.println("Emissor finalizou. Baixando CTS...");
  
    // ENCERRA HANDSHAKE
    digitalWrite(PINO_CTS, LOW); // Baixa o CTS para encerrar - Assim como de acordo com o pdf
    digitalWrite(PINO_LED, LOW);
    Serial.println("-----------------------------------"); 

    // valida e imprime
    processaChar();

    Serial.println("-----------------------------------");
    
    // reseta
    charPronto = false;
    recebendoDado = false;
  }
}

