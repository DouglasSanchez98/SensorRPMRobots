const byte sensorPin = 2;
volatile unsigned int contadorPulsos = 0;
volatile unsigned long tiempoUltimoPulso = 0;

const unsigned long tiempoDebounce = 100; // milisegundos (ajustar según necesidad)

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), contarPulsoDebounce, RISING);
}

void loop() {
  delay(5000);
  Serial.print("Pulsos válidos en 5 segundos: ");
  Serial.println(contadorPulsos);
  contadorPulsos = 0;
}

void contarPulsoDebounce() {
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoUltimoPulso > tiempoDebounce) {
    contadorPulsos++;
    tiempoUltimoPulso = tiempoActual;
  }
}
