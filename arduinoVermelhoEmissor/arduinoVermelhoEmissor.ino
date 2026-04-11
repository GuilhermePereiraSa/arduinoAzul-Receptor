// --- CÓDIGO DO EMISSOR FAKE (ARDUINO VERMELHO) ---
#define PINO_CTS 9
#define PINO_RTS 12

void setup() {

  // config
  pinMode(PINO_RTS, OUTPUT);
  pinMode(PINO_CTS, INPUT); // ler o cts do arduino azul - ACM0
  digitalWrite(PINO_RTS, LOW); // começa abaixado, obvio, nao estamos prontos ainda pra enviar!
}

void loop() {
  // Simula que quer enviar um caractere
  digitalWrite(PINO_RTS, HIGH);
  delay(1000); // Fica com RTS alto por 1 segundo (tempo para você ler)
  // delay já está no handshake

  // so avisa que esta pronto pra enviar
  // o outro arduino, receptor, só precisa receber esse sinal!



  // Fim da transmissão
  digitalWrite(PINO_RTS, LOW);
  delay(3000); // Fica de boas por 3 segundos
}
