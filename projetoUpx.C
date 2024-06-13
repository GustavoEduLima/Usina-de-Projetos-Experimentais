#include <WiFi.h>
#include <HTTPClient.h>

const int sensorPin = 15; // Pino do sensor de nível de água (GPIO 15)
const int ledPin = 2;     // Pino do LED (GPIO 2)

const char* ssid = "Nome do wiFi";
const char* password = "senha do Wifi;

// +international_country_code + phone number
// Brasil +55, example: +5511988888888
String phoneNumber = "+5515996537828";
String apiKey = "L8Mv7MV5o2Cf";

void sendMessage(String message) {
  // Converte a mensagem para URL-encoded
  message.replace("&", "%26");
  message.replace(" ", "%20");
  // Monta a URL completa
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + message;

  HTTPClient http;
  WiFiClient client;

  // Conecta-se à API
  if (http.begin(client, url)) {
    // Envia a requisição GET
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      Serial.println("Mensagem enviada com sucesso");
    } else {
      Serial.println("Erro no envio da mensagem");
      Serial.print("Código de resposta HTTP: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Falha na conexão com a API.");
  }
}

bool flag = true;

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial

  pinMode(sensorPin, INPUT); // Define o pino do sensor como entrada
  pinMode(ledPin, OUTPUT);   // Define o pino do LED como saída

  WiFi.begin(ssid, password);
  Serial.println("Conectando ao WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado ao WiFi neste IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int sensorValue = digitalRead(sensorPin); // Lê o valor do sensor

  if (sensorValue == HIGH) {
    Serial.println("Água detectada!");
    digitalWrite(ledPin, HIGH); // Liga o LED
    
    if (flag) {
      sendMessage("*ALERTA, RISCO DE ALAGAMENTO!!*");
      flag = false;
    }
  } else {
    Serial.println("Nível de água baixo.");
    digitalWrite(ledPin, LOW); // Desliga o LED
    flag = true;
  }

  delay(1000); // Aguarda 1 segundo antes de fazer a próxima leitura
}