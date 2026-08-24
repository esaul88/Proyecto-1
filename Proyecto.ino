int time; //Contador de tiempo restante.
int score; //Contador de aciertos.
int t = 30; //Usado de referencia para saber cuándo ha pasado la mitad del tiempo límite.

int p; //Variable que evita que se presiona el botón correspondiente más de una vez por ronda.

int r = 8; //Pin digital 8 enciende rojo.
int g = 9; //Pin digital 9 enciende verde.
int b = 10; //Pin digital 10 enciende azul.

int b1 = 4; //Pin digital que recibe señal del botón correspondiente al rojo.
int b2 = 3; //Pin digital que recibe señal del botón correspondiente al verde.
int b3 = 2; //Pin digital que recibe señal del botón correspondiente al azul.
int s; //Variable que define qué botón se espera que sea presionado al momento.

int buzz = 11; //Pin 11 asignado al buzzer pasivo.

void victory(); //Función que realiza sonido de victoria.

void defeat(); //Función que realiza sonido de derrota.

void countdown(); //Función que realiza sonido de reinicio de juego.

void warning(); //Función que se encarga de alertar al usuario cuando haya pasado la mitad del tiempo límite y cuando queden 5 o menos segundos.

void setup() {
  Serial.begin(9600); //Iniciar comunicaciones para monitoreo de funcionalidad.

  randomSeed(analogRead(A0)); //Inicializa el generador de números aleatorios para evitar que sea predecible.

  //Se definen a los pines que activan el LED como OUTPUTS.
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);

  //Se definie el pin utilizado para el buzzer pasivo como OUTPUT.
  pinMODE(buzz, OUTPUT);

  //Se definen a los pines que detectan activaciones de botones como INPUTS.
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
}

void loop() {
  time = t+1; //Se reinicia el tiempo límite para cada partida. Se toma en cuenta fenómeno observado donde se le resta 1 al tiempo asignado.
  score = 0; //Reinicia número de aciertos para cada partida.
  while (time > 0) {
    //Reinicia el LED para evitar mezclas de colores.
    digitalWrite(r, LOW);
    digitalWrite(g, LOW);
    digitalWrite(b, LOW);

    //Se asigna de manera aleatoria un color al LED y el botón del color correspondiente.
    switch (random(3)) { //La función random(3) genera un número entero aleatorio entre 0 y 2.
      case 0: //Cada caso está asignado a un posible valor de random(3).
        digitalWrite(r, HIGH); //Asigna color al LED
        s = b1; //Se asigna el botón (correspondiente al color asignado() que debe ser presionado.
        break; //Evita que se ejecuten los otros casos.
      case 1:
        digitalWrite(g, HIGH);
        s = b2;
        break;
      case 2:
        digitalWrite(b, HIGH);
        s = b3;
        break;
    }

    //Evita que el programa continúe sin que se haya presionado el botón correspondiente al color del LED.
    while (digitalRead(s) != HIGH) {
      p = 0; //Permite que el botón pueda volver a ser presionado.
      warning(); //Ver descripción al inicio del código.
      time--; //Va reduciendo el contador de tiempo restante.
      Serial.println(time); //Permite monitorear el tiempo desde el ordenador.

      if (time == 0) { //Evita que el programa se atore en caso de que, mientras se espere que se presione el botón, el tiempo se acabe.
        break;
      } 
    }

    //Si el tiempo ya se acabó o ya se presionó el botón antes de que quede registrado en el Arduino, se previene que se asigne un acierto adicional indebido.
    if (time != 0 && p == 0) 
    {
      score++; //Incrementa el registro de aciertos por uno.
      p = 1; //Evita que se registre más de una presionada de botón por ronda.
    }

    //Mantiene al LED apagado mientras se tenga presionado el botón, haciendo que el tiempo de la partida continúe sin interrupciones.
    while (digitalRead(s) == HIGH) {
      warning(); 
      time--; 
      Serial.println(time); 
      digitalWrite(r, LOW);
      digitalWrite(g, LOW);
      digitalWrite(b, LOW);
      if (time == 0) { 
        break;
      } 
    }
  }

  //Permite conocer cantidad de aciertos totales hechos durante la partida.
  Serial.print("score = ");
  Serial.println(score); 

  //Apaga foco LED para evitar que el usuario piense que la partida continúa cuando en realidad ya se acabó.
  digitalWrite(r, LOW);
  digitalWrite(g, LOW);
  digitalWrite(b, LOW);

  //Da tiempo suficiente para crear un espacio entre el sonido hecho cuando quedan 5 o menos segundos y el sonido ya sea de victoria o derrota.
  delay(3000);

  //Revisa la cantidad de aciertos finales hechos en la partida y, con base en eso, emite el sonido ya sea de victoria (10 o más aciertos) o de derrota (menos de 10 aciertos).
  if (score >= 10) {
    victory(); //Ver descripción al inicio del código.
  }
  else {
    defeat(); //Ver descripción al inicio del código.
  }

  //Permite al jugador prepararse para la siguiente ronda.
  delay(3000);
  countdown(); //Ver descripción al inicio del código.
}

void victory() {
  tone(buzz, 392, 500);
  delay(200);
  noTone(buzz);
  tone(buzz, 392, 500);
  delay(200);
  noTone(buzz);
  tone(buzz, 392, 500);
  delay(200);
  noTone(buzz);
  tone(buzz, 311, 1000);
  delay(500);
  tone(buzz, 349, 500);
  delay(200);
  tone(buzz, 392, 1500);
  delay(200);
}

void defeat() {
  tone(buzz, 330, 500);
  delay(200);
  tone(buzz, 311, 500);
  delay(200);
  tone(buzz, 294, 500);
  delay(200);
  tone(buzz, 277, 1000);
}

void countdown() {
  //El sonido se va repitiendo cada vez más rápido a lo largo de 10 repeticiones.
  for (int i = 10; i > 0; i--) {
    tone(buzz, 392, 500);
    delay(i*100);
    noTone(buzz);
  }
}

void warning() {
  //Manda señal al buzzer para que suene.
  if (time == round(t/2) || time <= 5) {
    tone(buzz, 392, 500);
    delay(200);
  }

  //Toma en cuenta el tiempo que toma hacer sonar el buzzer (200 ms) cuando ha pasado la mitad del tiempo límite o queden 5 o menos segundos.
  if(time <= 5 || time == round(t/2)) {
    delay(800); //Junto con el tiempo que suena el buzzer llega a 1000 ms.
  }
  else {
    delay(1000);
  }
}





