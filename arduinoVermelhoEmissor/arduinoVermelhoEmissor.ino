// Pinos no Arduino Emissor (conectados aos pinos equivalentes no Receptor)
#define PINO_RTS 12 
#define PINO_CTS 9  
#define PINO_CLOCK 11 
#define PINO_DADOS 10 

// Frequência de 1Hz (1 segundo por ciclo completo)
#define HALF_BAUD 500 // 500ms em LOW, 500ms em HIGH

void setup() {
  Serial.begin(9600);
  pinMode(PINO_RTS, OUTPUT);
  pinMode(PINO_CTS, INPUT);
  pinMode(PINO_CLOCK, OUTPUT);
  pinMode(PINO_DADOS, OUTPUT);

  // Estado inicial ocioso
  digitalWrite(PINO_RTS, LOW);
  digitalWrite(PINO_CLOCK, LOW);
  digitalWrite(PINO_DADOS, LOW);
  
  Serial.println("Mock Emissor Iniciado.");
}

void loop() {
  delay(5000); // Espera 5 segundos entre cada envio para facilitar a visualização no monitor serial
  
  Serial.println("\nIniciando transmissao da letra 'o'...");
  
  // 1. Levanta RTS e espera CTS
  digitalWrite(PINO_RTS, HIGH);
  Serial.println("RTS em HIGH. Aguardando CTS...");
  while(digitalRead(PINO_CTS) == LOW) {
    // Fica preso aqui até o receptor responder
  }
  Serial.println("CTS detectado! Comecando a enviar o clock e os dados...");

  // Letra 'o' (0x6F) -> Binário invertido (LSB primeiro): 1, 1, 1, 1, 0, 1, 1, 0
  char caractere = 'o';
  bool bitParidadePar = 0; // Para 'o' (que tem 6 uns), a paridade par é 0

  // 2. Envia os 8 bits de dados
  for(int i = 0; i < 8; i++) {
    int bitAtual = bitRead(caractere, i);
    
    // Escreve o dado com o clock em LOW (Borda de descida)
    digitalWrite(PINO_DADOS, bitAtual); 
    delay(HALF_BAUD); 
    
    // Borda de subida (Aqui o seu Receptor vai disparar a interrupção e ler o pino!)
    digitalWrite(PINO_CLOCK, HIGH);
    delay(HALF_BAUD);
    
    // Volta o clock para LOW
    digitalWrite(PINO_CLOCK, LOW);
  }

  // 3. Envia o bit de paridade
  digitalWrite(PINO_DADOS, bitParidadePar);
  delay(HALF_BAUD);
  digitalWrite(PINO_CLOCK, HIGH); // Receptor lê a paridade aqui
  delay(HALF_BAUD);
  digitalWrite(PINO_CLOCK, LOW);

  // 4. Finaliza a transmissão
  digitalWrite(PINO_RTS, LOW);
  digitalWrite(PINO_DADOS, LOW);
  Serial.println("Transmissao concluida. RTS baixado. Aguardando Receptor baixar o CTS...");
  
  while(digitalRead(PINO_CTS) == HIGH) {
    // Espera o receptor confirmar que finalizou
  }
  
  Serial.println("Handshake encerrado com sucesso.");
}