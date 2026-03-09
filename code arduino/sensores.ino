#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Configuração do display I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definição dos pinos dos sensores e buzzer
const int mq2Pin = A0;        // Pino do MQ-2
const int flameSensorPin = 2; // Pino do sensor de chama
const int dhtPin = 4;         // Pino de dados do DHT11
const int buzzerPin = 3;      // Pino do buzzer (PWM)

// Limites de alerta
const int gasThreshold = 400;   // Limite do MQ-2 para acionar alerta
const float tempThreshold = 30; // Temperatura limite em °C para acionar alerta

// Configuração do DHT
#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);

void setup() {
  Serial.begin(9600);

  // Configuração dos pinos
  pinMode(mq2Pin, INPUT);
  pinMode(flameSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Inicialização do display LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.init();
  lcd.print("Iniciando...");
  delay(2000);
  lcd.clear();

  // Inicialização do sensor DHT11
  dht.begin();
}

void loop() {
  // Leitura do MQ-2
  int mq2Value = analogRead(mq2Pin);

  // Leitura de chama
  int flameValue = digitalRead(flameSensorPin);

  // Leitura do DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Verifica se os dados do DHT11 são válidos
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Erro ao ler o DHT11");
    lcd.setCursor(0, 0);
    lcd.print("Erro no DHT11");
  } else {
    // Exibe temperatura e umidade no Serial Monitor
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Umidade: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Exibe temperatura e umidade no LCD
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temperature);
    lcd.print("C/");
    lcd.print("U:");
    lcd.print(humidity);
    lcd.print("%");

    // Alerta de temperatura alta
    if (temperature > tempThreshold) {
      tone(buzzerPin, 2000); // Ativa o buzzer com frequência de 2000 Hz
      Serial.println("Alerta: Temperatura alta!");
    }
  }

  // Exibe o valor do gás no monitor serial
  Serial.print("MQ-2: ");
  Serial.println(mq2Value);

  // Exibe o valor do MQ-2 no LCD
  lcd.setCursor(0, 1);
  lcd.print("MQ2:");
  lcd.print(mq2Value);

  // Alerta de gás
  if (mq2Value > gasThreshold) {
    tone(buzzerPin, 1000); // Ativa o buzzer com frequência de 1000 Hz
    lcd.setCursor(12, 1);
    lcd.print("Gas!");
    Serial.println("Alerta: Nivel de gas alto!");
  } else {
    lcd.setCursor(12, 1);
    lcd.print("    "); // Limpa o aviso de gás
  }

  // Alerta de chama
  if (flameValue == LOW) {
    tone(buzzerPin, 1500); // Ativa o buzzer com frequência de 1500 Hz
    lcd.setCursor(12, 1);
    lcd.print("Fogo!");
    Serial.println("Alerta: Chama detectada!");
  }

  // Desativa o buzzer se nenhum alerta estiver ativo
  if (mq2Value <= gasThreshold && temperature <= tempThreshold && flameValue != LOW) {
    noTone(buzzerPin); // Desliga o buzzer
  }

  delay(2000); // Atraso antes da próxima leitura
}
