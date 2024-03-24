#include <Ethernet.h>
#include <MQTT.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

EthernetClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("Conectando ao servidor MQTT...");
  while (!client.connect("arduino")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConectado!");

  client.subscribe("/vgarden");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("Mensagem recebida: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(9600);
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha ao obter o endereço IP via DHCP. Verifique a conexão do cabo e reinicie o Arduino.");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("--------------------");
  Serial.print("Endereço IP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("Gateway: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("DNS: ");
  Serial.println(Ethernet.dnsServerIP());
  Serial.println("--------------------");

  client.begin("broker.hivemq.com", 1883, net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }
}