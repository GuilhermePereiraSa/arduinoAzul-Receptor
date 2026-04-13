#define PINO_CTS 9 // indicar para o outro arduino que estamos prontos
#define PINO_DADOS 10 // passagem dos dados, frame inteiro!
#define PINO_CLOCK 11 // configurado no serial -> passado para aqui 
#define PINO_RTS 12 // do outro arduino - entrada para este de recepção 

// Receptor e Emissor tem mesma config de PINOS!

// metodo de preconfig para tudo rodar direito
void setup() {
  // baud rate arduino config
  Serial.begin(9600);
  
  // Configura os pinos
  pinMode(PINO_RTS, INPUT);
  pinMode(PINO_CLOCK, INPUT);
  pinMode(PINO_DADOS, INPUT);

  // so enviamos que estamos prontos! - unico de output aqui
  pinMode(PINO_CTS, OUTPUT);


  // ja coloca o de recebimento como não-pronto
  digitalWrite(PINO_CTS, LOW); // Começa com CTS baixo - de acordo com o pdf

  // O emissor, ele está pronto pra enviar?
  Serial.println("Receptor ligado. Aguardando sinal RTS do Emissor...");
}


// loop serve para receber os dados
void loop() {

  // Fica lendo o pino RTS do emissor
  int estadoRTS = digitalRead(PINO_RTS);

  // Se o emissor pediu para enviar (RTS em HIGH)...
  if (estadoRTS == HIGH) {
    Serial.println("RTS Detectado! Avisando que estou pronto (CTS = HIGH)...");

    // Levanta o CTS 
    digitalWrite(PINO_CTS, HIGH);
    

    // INTERROMPER PARA LER O BIT - ARDUINO É MAIS RAPIDO
    // utilizar a funcao de Serial.ino


    // so fazer um for para receber os bits?
    // usar volatile 
    // usar operadores logicos de bit - bit a bit, como o (& AND bitwise), ou o | (ou) 
    // usar vector de booleans para os bits - 0 false, 1 true


    // Emissor baixou o RTS (terminou)
    Serial.println("Emissor finalizou. Baixando CTS...");
    digitalWrite(PINO_CTS, LOW); // Baixa o CTS para encerrar - Assim como de acordo com o pdf
    
    // ENCERRA HANDSHAKE
    Serial.println("-----------------------------------"); 


  }else {
    digitalWrite(PINO_CTS, LOW);
  }
}
