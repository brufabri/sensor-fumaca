
 # **Projeto ESP8266 com Sensor MQ-02**

Este projeto usa um **ESP8266** com o **sensor de gás MQ-02** para monitorar a qualidade do ar e enviar os dados para **ThingSpeak**. Quando os níveis de gás ultrapassam um limite predefinido, o sistema envia uma notificação via **Telegram** alertando sobre o risco.

### -Componentes Necessários
#### Hardware

-ESP8266 NodeMCU  
-Sensor MQ-2  
-Jumpers e Protoboard  

#### Software

-Arduino IDE  
-Biblioteca para Telegram Bot  
-Biblioteca para comunicação com ThingSpeak   
 

### -Funcionamento do Sistema
O **ESP8266** lê os dados do **sensor MQ-2**.  
Envia os valores do sensor para o **ThingSpeak**, onde são armazenados e analisados.  
Se os níveis de fumaça ultrapassarem o limite configurado:  
O **ESP8266** envia um alerta ao **Telegram** via bot.  
Após cada ciclo, o sistema entra no modo Deep Sleep para economizar energia.  

### -Funções Principais:

**1. Conexão Wi-Fi:** O **ESP8266** se conecta à rede Wi-Fi definida pelas credenciais fornecidas.  
**2. Leitura do Sensor:** O valor analógico do **sensor MQ-02** é lido no **pino A0**. A leitura inicial é registrada para ajustar os valores subsequentes.  
**3. Envio ao ThingSpeak:** O valor ajustado do sensor é enviado para o canal **ThingSpeak**, permitindo a visualização em tempo real na plataforma.  
**4. Envio de Alertas via Telegram:** Caso o valor do sensor ultrapasse um limite de 70, uma notificação de alerta é enviada via **Telegram**, indicando possível risco de incêndio. Se o valor voltar a cair abaixo do limite, o alerta é resetado.  
**5. Verificação Contínua:** O código roda em um loop contínuo, enviando dados a cada 30 segundos e monitorando constantemente o valor do sensor.     

### -Conexão com WI-FI 

O código a seguir ilustra como é feita a conexão com WI-FI:

```cpp
#define WLAN_SSID   "REDE"  // nome da sua rede Wi-Fi
#define WLAN_PASS   "SENHA"      // senha do Wi-Fi
```

### -Exemplo de Envio de Dados

O código a seguir ilustra como o valor do sensor é lido e enviado:

```cpp
// Ler o valor do sensor MQ-02
int sensorValue = analogRead(A0); // Leitura do pino A0

// Enviar o valor do sensor para o campo 1 do ThingSpeak
ThingSpeak.setField(1, sensorValue);
int responseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

```

### -Conexão com ThingSpeak

O código a seguir ilustra como conectar ao canal do ThingSpeak:

```cpp
WiFiClient client;
unsigned long myChannelNumber = "NumberChannel";  // Número do canal ThingSpeak
const char * myWriteAPIKey = "WriteKey";  // API Key
```

### -Conexão com bot do Telegram

O código a seguir ilustra como conectar ao bot:

```cpp
String botToken = "Token"; // Token do seu bot Telegram
String chatID = "ID";  // ID do seu chat ou canal no Telegram
```

# Conclusão:
Este projeto oferece uma solução simples e eficiente para monitoramento de qualidade do ar com o uso do **ESP8266** e **MQ-02**, integrando com plataformas de análise **(ThingSpeak)** e envio de alertas em tempo real **(Telegram)**. É útil para detectar possíveis riscos de incêndio e realizar ações imediatas.









