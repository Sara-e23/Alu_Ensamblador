char data[26];
String operacion = "";
bool esperandoOperacion = true;

void setup() {
  Serial.begin(9600);
  Serial.println("Seleccione tipo de operación (sum, res, mul, div, and, or, not, xnor, nand, xor, nor):");

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(11, OUTPUT);//led azul (Infinito)
  pinMode(12, OUTPUT);//led rojo (Indefinido)

  low();//inicia apagado
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
}

void loop() {
  if (esperandoOperacion) {
    if (Serial.available()) {
      int bytesRecibidos = Serial.readBytesUntil(13, data, 25);
      data[bytesRecibidos] = '\0';
      String entrada = String(data);
      entrada.trim();

      if (entrada == "sum" || entrada == "res" || entrada == "mul" || entrada == "div" ||
          entrada == "and" || entrada == "or" || entrada == "not" || entrada == "xnor" ||
          entrada == "nand" || entrada == "xor" || entrada == "nor") {
        operacion = entrada;
        Serial.print("Operación seleccionada: ");
        Serial.println(operacion);
        Serial.println("Ingrese los valores (00, 10, 01, 11):");
        esperandoOperacion = false;
      } else {
        Serial.println("Operación no reconocida. Seleccione una válida.");
        low();
      }
    }
  } else {
    if (Serial.available()) {
      int bytesRecibidos = Serial.readBytesUntil(13, data, 25);
      data[bytesRecibidos] = '\0';
      String entrada = String(data);
      entrada.trim();

      int resultado = procesarOperacion(entrada, operacion);
      if (resultado == -999) {
        Serial.println("Entrada inválida. Apagando display.");
        low();
      } else if (resultado == -998) {
        Serial.println("División indefinida o infinita.");
      } else if (resultado == -997) {
        mostrarMenos();
      } else {
        mostrarResultado(resultado);
      }
      Serial.println("Seleccione la operación");
      esperandoOperacion = true;
    }
  }
}
//operacion desde teclado
int procesarOperacion(String entrada, String operacion) {
  if (entrada != "00" && entrada != "10" && entrada != "01" && entrada != "11") {
    return -999;
  }

  int estadoA = entrada[0] - '0'; //Convierte el primer carácter a número (0 o 1)
  int estadoB = entrada[1] - '0'; //Convierte el segundo carácter a número (0 o 1)

  // Realizar la operación según el modo
  if (operacion == "sum") {
    return estadoA + estadoB;
  } else if (operacion == "res") {
    if (estadoA == 0 && estadoB == 1) return -997;//signo -
    return estadoA - estadoB;
  } else if (operacion == "mul") {
    return estadoA * estadoB;
  } else if (operacion == "div") {
    if (estadoA == 1 && estadoB == 0) {
      digitalWrite(12, HIGH);//led rojo (Indefinido)
      delay(500);//tiempo en que enciende led
      digitalWrite(12, LOW);
      return -998;//op indefinida
    } else if (estadoA == 0 && estadoB == 1) {
      digitalWrite(11, HIGH);//led azul (Infinito)
      delay(500);//tiempo en que enciende led
      digitalWrite(11, LOW);
      return -998;//op infinita
    }
    return estadoA / estadoB;
  } else if (operacion == "and") {
    return estadoA & estadoB;
  } else if (operacion == "or") {
    return estadoA | estadoB;
  } else if (operacion == "not") {
    return !estadoA;//NOT solo usa estadoA
  } else if (operacion == "xnor") {
    return !(estadoA ^ estadoB);
  } else if (operacion == "nand") {
    return !(estadoA & estadoB);
  } else if (operacion == "xor") {
    return estadoA ^ estadoB;
  } else if (operacion == "nor") {
    return !(estadoA | estadoB);
  }
  return -999;//op no reconocida
}
void mostrarResultado(int resultado) {
  if (resultado == 0) cero();
  else if (resultado == 1) uno();
  else if (resultado == 2) dos();
  else low();//se apaga si es otro numero
}

//signo (-) funcion
void mostrarMenos() {
  low();//apaga todo
  digitalWrite(6, LOW);//Enciende solo el segmento para el signo menos
}

void cero() {
  digitalWrite(5, LOW);
  digitalWrite(10, LOW);
  digitalWrite(8, LOW);
  digitalWrite(6, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(9, HIGH);
}

void uno() {
  digitalWrite(8, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
}

void dos() {
  digitalWrite(5, LOW);
  digitalWrite(10, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(4, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(9, LOW);
}

void low() {
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
}
