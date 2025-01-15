#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <WiFiClientSecure.h>

#define WLAN_SSID   "TP-Link_7FBA"  // nome da sua rede Wi-Fi
#define WLAN_PASS   "50804214"      // senha do Wi-Fi

WiFiClient client;
unsigned long myChannelNumber = 2665765;  // Número do canal ThingSpeak
const char * myWriteAPIKey = "EE58KA4XI5V1IXB2";  // API Key

const int sensorPin = A0;  // Pino analógico para ler o MQ-02
int initialReading;

String botToken = "7779591655:AAHOAr6Ne9b1IP_tm-Eusb4X8T1xyNdhQlQ"; // Token do seu bot Telegram
String chatID = "6920567636";  // ID do seu chat ou canal no Telegram

bool alertSent = false;  // Variável para controlar o envio do alerta

void setup() {
  Serial.begin(115200);
  
  // Conectar à rede Wi-Fi
  WiFi.mode(WIFI_STA);
  connectToWiFi();  // Conectar ao Wi-Fi

  ThingSpeak.begin(client);
  
  initialReading = analogRead(sensorPin);
  Serial.print("Leitura inicial do sensor: ");
  Serial.println(initialReading);
}

void loop() {
  // Garantir que o ESP8266 está conectado ao Wi-Fi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi desconectado. Tentando reconectar...");
    connectToWiFi();  // Tentar reconectar ao Wi-Fi
  }

  // Ler o valor do sensor MQ-02
  int sensorValue = analogRead(sensorPin);
  
  // Ajustar o valor para começar em 0
  int adjustedValue = sensorValue - initialReading;
  
  // Garantir que o valor não seja negativo
  if (adjustedValue < 0) {
    adjustedValue = 0;
  }

  // Converter o valor lido para voltagem
  float voltage = (sensorValue / 1023.0) * 3.3;

  // Verificar se o valor ultrapassou 70 e ainda não foi enviado o alerta
  if (adjustedValue > 70 && !alertSent) {
    String message = "INICIO DE INCÊNDIO! Valor do sensor MQ-02: " + String(adjustedValue);
    sendTelegramMessage(message);  // Enviar a mensagem via Telegram
    alertSent = true;  // Marcar que o alerta foi enviado
  }

  // Se o valor cair abaixo de 70, resetar a variável alertSent
  if (adjustedValue <= 70) {
    alertSent = false;
  }

  // Enviar a leitura padrão para o Telegram, caso não seja um alerta
  if (adjustedValue <= 70) {
    String message = "Valor do sensor MQ-02: " + String(adjustedValue) + "\nVoltagem: " + String(voltage);
    sendTelegramMessage(message);  // Enviar a mensagem normal
  }

  // Conectar ao ThingSpeak e enviar o valor do sensor
  if (client.connect("api.thingspeak.com", 80)) {
    ThingSpeak.setField(1, adjustedValue);
    int responseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (responseCode == 200) {
      Serial.println("Atualização bem-sucedida no ThingSpeak");
    } else {
      Serial.print("Falha na atualização. Código de resposta: ");
      Serial.println(responseCode);
    }
  } else {
    Serial.println("Falha na conexão com o ThingSpeak");
  }

  delay(30000);  // Espera 30 segundos antes de enviar novamente
}

void connectToWiFi() {
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado ao Wi-Fi! Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void sendTelegramMessage(String message) {
  String url = String("https://api.telegram.org/bot") + botToken + "/sendMessage?chat_id=" + chatID + "&text=" + message;
  Serial.println("Enviando para o Telegram: ");
  Serial.println(url);  // Para verificar se a URL está correta

  WiFiClientSecure secureClient;
  
  // Ignorar verificação do certificado SSL (temporário para testar)
  secureClient.setInsecure();
  
  // Conectar-se ao servidor do Telegram
  if (secureClient.connect("api.telegram.org", 443)) {
    Serial.println("Conectado ao servidor Telegram");
    
    // Enviar a requisição GET para o Telegram
    secureClient.print(String("GET ") + url + " HTTP/1.1\r\n" +
                      "Host: api.telegram.org\r\n" +
                      "Connection: close\r\n\r\n");
    delay(500);
    
    // Verificar se a requisição foi bem-sucedida
    while (secureClient.available()) {
      String response = secureClient.readString();
      Serial.println(response);  // Imprimir a resposta do servidor
    }
  } else {
    Serial.println("Falha na conexão com o Telegram");
  }
  secureClient.stop();
}