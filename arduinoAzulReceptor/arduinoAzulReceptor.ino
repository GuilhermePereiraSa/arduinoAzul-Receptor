#define PINO_CTS 9
#define PINO_DADOS 10
#define PINO_CLOCK 11
#define PINO_RTS 12

// Receptor e Emissor tem mesma config de PINOS!

// metodo de config
void setup() {
  // baud rate - preconfig - ser o mesmo do grupo de receptores
  Serial.begin(9600);
  
  // Configura os pinos
  // so enviamos que estamos prontos! - unico de output aqui
  pinMode(PINO_RTS, INPUT);
  pinMode(PINO_CLOCK, INPUT);
  pinMode(PINO_DADOS, INPUT);
  pinMode(PINO_CTS, OUTPUT);


  digitalWrite(PINO_CTS, LOW); // Começa com CTS baixo - de acordo com o pdf
  

  // O emissor, ele está pronto pra enviar?
  Serial.println("Receptor ligado. Aguardando sinal RTS do Emissor...");
}

void loop() {

  // Fica lendo o pino RTS do emissor
  int estadoRTS = digitalRead(PINO_RTS);

  // Se o emissor pediu para enviar (RTS em HIGH)...
  if (estadoRTS == HIGH) {
    Serial.println("RTS Detectado! Avisando que estou pronto (CTS = HIGH)...");

    // Levanta o CTS 
    digitalWrite(PINO_CTS, HIGH);
    

    // EM LOOP! - Ler todo o frame aqui!
    // teste vamos só segurar enquanto o RTS estiver alto
    while(digitalRead(PINO_RTS) == HIGH) {
      // Não faz nada, só espera o emissor terminar (baixar o RTS)
    }

    // Emissor baixou o RTS (terminou)
    Serial.println("Emissor finalizou. Baixando CTS...");
    digitalWrite(PINO_CTS, LOW); // Baixa o CTS para encerrar - Assim como de acordo com o pdf
    
    // ENCERRA HANDSHAKE
    Serial.println("-----------------------------------"); 


  }
}