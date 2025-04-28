#define IN1 32  // Pin de control para IN1
#define IN2 33  // Pin de control para IN2

void setup() {
  // Configurar los pines como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  // Gira el motor en una dirección (adelante)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(2000);  // Motor girando durante 2 segundos
  
  // Detiene el motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(1000);  // Pausa de 1 segundo
  
  // Gira el motor en la otra dirección (atrás)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(2000);  // Motor girando durante 2 segundos
  
  // Detiene el motor nuevamente
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(1000);  // Pausa de 1 segundo
}
