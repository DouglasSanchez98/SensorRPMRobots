const byte sensorPin = 2;

volatile unsigned int contadorPulsos = 0;
volatile unsigned long tiempoUltimoPulso = 0;
volatile unsigned long periodo = 0;

const unsigned long tiempoDebounce = 100; // ms
const unsigned long intervaloMedicion = 1000; // ms
const unsigned long tiempoSinPulsos = 2000; // ms (si pasa esto sin pulsos = detenido)

unsigned long tiempoAnterior = 0;

float rpm = 0;
float rpmFiltrado = 0;
const float alpha = 0.7;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), contarPulsoDebounce, RISING);
}

void loop() {
  unsigned long tiempoActual = millis();

  // ¿Pasó el tiempo de medición?
  if (tiempoActual - tiempoAnterior >= intervaloMedicion) {
    float rpmNueva = 0;

    if (contadorPulsos >= 2) {
      rpmNueva = (contadorPulsos * 60.0) / (intervaloMedicion / 1000.0);
    }
    else if (periodo > 0 && (tiempoActual - tiempoUltimoPulso < tiempoSinPulsos)) {
      rpmNueva = (1.0 / (periodo / 1000.0)) * 60.0;
    }
    else {
      // Si no hubo pulsos por más de tiempoSinPulsos, motor detenido
      rpmNueva = 0;
    }

    // Suavizado
    rpmFiltrado = (alpha * rpmFiltrado) + ((1 - alpha) * rpmNueva);

    Serial.print("RPM: ");
    Serial.println(rpmFiltrado, 2);

    contadorPulsos = 0;
    tiempoAnterior = tiempoActual;
  }
}

void contarPulsoDebounce() {
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoUltimoPulso > tiempoDebounce) {
    periodo = tiempoActual - tiempoUltimoPulso;
    tiempoUltimoPulso = tiempoActual;
    contadorPulsos++;
  }
}
