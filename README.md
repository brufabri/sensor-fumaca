# sensor-fumaca
ESP8266 com Sensor MQ-02

Este projeto monitora a presença de fumaça utilizando o sensor MQ-2 conectado ao ESP8266 NodeMCU, alimentado por um painel solar. O sistema coleta os dados do sensor e envia os valores para o ThingSpeak para análise e registro em tempo real. Caso o nível de fumaça ultrapasse um limite definido, o ESP8266 também envia um alerta ao bot do Telegram, notificando sobre um possível início de incêndio.
