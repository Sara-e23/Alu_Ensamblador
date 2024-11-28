char data[26];
bool modoSuma = false, modoResta = false, modoMultiplicacion = false, modoDivision = false;
int estadoA;
int estadoB;

void setup() {
  Serial.begin(9600);
  Serial.println("En línea, ingrese comandos...");

  pinMode(2, INPUT_PULLUP);//estadoA pull-up
  pinMode(3, INPUT_PULLUP);//estadoB

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(11, OUTPUT);//led azul(Infinito)
  pinMode(12, OUTPUT);//led rojo (Indefinido)

  low();
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
}

void loop() {
  estadoA = digitalRead(2);
  estadoB = digitalRead(3);

  if (Serial.available()) {
    int bytesRecibidos = Serial.readBytesUntil(13, data, 25);
    data[bytesRecibidos] = '\0';
    String comando = String(data);
    comando.trim();

    Serial.print("Comando recibido: ");
    Serial.println(comando);

    //modos
    if (comando == "sum") {
      Serial.println("Modo: Suma activado");
      activarModo(&modoSuma);
    } else if (comando == "res") {
      Serial.println("Modo: Resta activado");
      activarModo(&modoResta);
    } else if (comando == "mul") {
      Serial.println("Modo: Multiplicación activado");
      activarModo(&modoMultiplicacion);
    } else if (comando == "div") {
      Serial.println("Modo: División activado");
      activarModo(&modoDivision);
    } else {
      Serial.println("Comando no reconocido");
      desactivarTodosLosModos();
    }
  }
    if (modoSuma) {
    if (estadoA ^ estadoB) uno(); // 0+1 o 1+0
    else if (estadoA && estadoB) dos(); // 1+1 estado inicial (debería ser dos xd)
    else cero(); // 0 + 0
  } else if (modoResta) {
    if (estadoA && !estadoB) uno(); //1-0
    else if (!estadoA && estadoB) menos();//0-1
    else cero(); // 1-1 o 0-0
  } else if (modoMultiplicacion) {
    if (estadoA && estadoB) uno();//1*1
    else cero(); // 0 * 1, 1 * 0, 0 * 0
  } else if (modoDivision) {
    if (!estadoA && !estadoB) cero(); // 0 / 0
    else if (estadoA && !estadoB) indefinido(); //1/0
    else if (!estadoA && estadoB) infinito(); // 0/1
    else uno();//1/1
  }
}
void activarModo(bool *modo) {
  desactivarTodosLosModos();
  *modo = true;
  low();
}

void desactivarTodosLosModos() {
  modoSuma = false;
  modoResta = false;
  modoMultiplicacion = false;
  modoDivision = false;
  low();
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
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

void menos() {
  digitalWrite(5, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(9, HIGH);
}

void indefinido() {
  digitalWrite(12, HIGH);//enciende rojo
  digitalWrite(12, LOW);//se apaga estando inactivo
  low(); //apaga el display
}

void infinito() {
  digitalWrite(11, HIGH);//enciende azul
  digitalWrite(11, LOW);//se apaga estando inactivo
  low(); //apaga el display
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
