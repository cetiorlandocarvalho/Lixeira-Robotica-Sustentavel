#include <Servo.h> // Inclui a biblioteca para controle de servos

// Criação dos objetos servo
Servo servoTampa;       // Servo que controla a abertura da tampa
Servo servoEsquerda;    // Servo que gira para a esquerda
Servo servoDireita;     // Servo que gira para a direita (sentido oposto)

// Definição dos pinos dos componentes
const int trigPin = 10;         // Pino TRIG do sensor ultrassônico
const int echoPin = 9;          // Pino ECHO do sensor ultrassônico

const int servoTampaPin = 3;    // Pino de controle do servo da tampa
const int servoEsquerdaPin = 7; // Pino de controle do servo esquerdo
const int servoDireitaPin = 8;  // Pino de controle do servo direito

const int buzzerPin = 4;        // Pino do buzzer
const int ledVerdePin = 5;      // Pino do LED verde
const int ledVermelhoPin = 6;   // Pino do LED vermelho

long duration;    // Armazena o tempo do pulso ultrassônico
int distance;     // Distância calculada em centímetros

void setup() {
  // Associa cada objeto servo ao seu pino correspondente
  servoTampa.attach(servoTampaPin);
  servoEsquerda.attach(servoEsquerdaPin);
  servoDireita.attach(servoDireitaPin);

  // Define os modos dos pinos
  pinMode(trigPin, OUTPUT);        
  pinMode(echoPin, INPUT);         
  pinMode(buzzerPin, OUTPUT);      
  pinMode(ledVerdePin, OUTPUT);    
  pinMode(ledVermelhoPin, OUTPUT); 

  // Estado inicial: tampa fechada, LEDs e servos no estado de espera
  servoTampa.write(0);         // Tampa fechada (0°)
  servoEsquerda.write(0);      // Servo esquerdo no início
  servoDireita.write(180);     // Servo direito no fim (movimento oposto)

  digitalWrite(ledVerdePin, LOW);    
  digitalWrite(ledVermelhoPin, HIGH); // LED vermelho aceso (estado normal)

  Serial.begin(9600); // Inicia comunicação serial para debug
}

void loop() {
  // Envia pulso de 10 µs para o TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede o tempo de retorno do pulso no ECHO
  duration = pulseIn(echoPin, HIGH);

  // Calcula a distância com base no tempo (velocidade do som: 0.034 cm/µs)
  distance = duration * 0.034 / 2;

  // Exibe a distância no monitor serial
  Serial.print("Distancia: ");
  Serial.println(distance);

  // Verifica se objeto está próximo (≤ 15 cm)
  if(distance <= 15){
    // Ação de abertura
    servoTampa.write(180);  // Abre a tampa
    delay(500);             // Aguarda o servo abrir

    digitalWrite(ledVerdePin, HIGH);   // LED verde acende
    digitalWrite(ledVermelhoPin, LOW); // LED vermelho apaga

    // Buzzer bipando 4 vezes
    for(int i = 0; i < 4; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(500);
      digitalWrite(buzzerPin, LOW);
      delay(500);
    }

    // Gira os dois servos em sentidos opostos
    servoEsquerda.write(180); // Vai para 180°
    servoDireita.write(0);    // Vai para 0° (oposto)

    delay(1000); // Mantém os servos abertos por 1 segundo
  } else {
    // Ação de fechamento
    servoTampa.write(0); // Fecha a tampa
    delay(500);

    // LEDs e buzzer em estado normal
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledVerdePin, LOW);
    digitalWrite(ledVermelhoPin, HIGH);

    // Retorna os dois servos à posição original
    servoEsquerda.write(0);     // Servo esquerdo volta ao início
    servoDireita.write(180);    // Servo direito volta ao fim
  }

  delay(300); // Pequeno atraso antes da próxima medição
}
