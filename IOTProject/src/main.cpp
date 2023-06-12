#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <string.h>
#include <RTClib.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <time.h>

using namespace std;

// Definir as credenciais da rede Wi-Fi
const char *ssid = "Mateus Wifi";
const char *password = "MateusAbu";

// Definir as credenciais do broker MQTT
const char *mqtt_server = "f196f38f1bb3475dae36136af23cb2e3.s2.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char *mqtt_username = "admin";
const char *mqtt_password = "Admin123";

const char *root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "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\n"
    "-----END CERTIFICATE-----\n";

// Declaração das variáveis do motor, sensor e os horários que serão gravados atraves do hivemq
const int motorB1 = 26;
const int motorB2 = 27;
const int trigPin = 5;
const int echoPin = 18;

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

int horarios[7][4] = {
    {0, 0, 0, 0}, // Domingo: 0:00
    {0, 0, 0, 0}, // Segunda-feira: 0:00
    {0, 0, 0, 0}, // Terça-feira: 0:00
    {0, 0, 0, 0}, // Quarta-feira: 0:00
    {0, 0, 0, 0}, // Quinta-feira: 0:00
    {0, 0, 0, 0}, // Sexta-feira: 0:00
    {0, 0, 0, 0}  // Sábado: 0:00
};

String mensagem;

// Inicializar o cliente WiFi e o cliente MQTT
WiFiClientSecure espClient;
PubSubClient client(espClient);

// Função de callback chamada quando uma mensagem é recebida no tópico inscrito
void callback(char *topic, byte *payload, unsigned int length)
{
  mensagem = "";
  for (int i = 0; i < length; i++)
  {
    mensagem += (char)payload[i];
  }
  Serial.print("Mensagem recebida no tópico [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(mensagem);
  Serial.println();
}

void reconnect()
{
  // Loop até que o cliente MQTT esteja conectado ao broker
  while (!client.connected())
  {
    Serial.print("Conectando ao broker MQTT...");
    // Tente conectar o cliente MQTT
    if (client.connect("ESP32Client", mqtt_username, mqtt_password))
    {
      Serial.println("Conectado");
      // Inscreva-se em um tópico MQTT
      client.subscribe("messages");
    }
    else
    {
      Serial.print("Falha na conexão com o broker MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando se reconectar em 5 segundos...");
      // Aguarde 5 segundos antes de tentar novamente
      delay(5000);
    }
  }
}

void setup()
{
  // Inicializar a porta serial para depuração
  Serial.begin(9600);
  pinMode(motorB1, OUTPUT); 
  pinMode(motorB2, OUTPUT);   
  // Conectar à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando à rede Wi-Fi...");
  }

  // Exibir informações sobre a rede Wi-Fi conectada
  Serial.println("Conectado à rede Wi-Fi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Configurar o cliente MQTT
  espClient.setCACert(root_ca); // Onde root_ca é a string do certificado de autoridade de certificação do servidor MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  configTime(0, 0, "pool.ntp.org");

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
}

// Função de vetor para separar as strings recebidas atraves do hive a cada ':',
//  para pegar o dia da semana, hora e minuto dos dois horários
vector<string> tokenize(string s, string del = " ")
{
  vector<string> tokens;
  int start, end = -1 * del.size();
  do
  {
    start = end + del.size();
    end = s.find(del, start);
    tokens.push_back(s.substr(start, end - start));
  } while (end != -1);
  return tokens;
}

void loop()
{
  // Pega o horário de agora e salva nas variáveis declaradas a baixo para fim de comparação com a data/hora passada pela mensagem
  struct tm timeinfo;
  if (getLocalTime(&timeinfo))
  {
    int diaSemana = timeinfo.tm_wday;
    int hora = timeinfo.tm_hour;
    int minuto = timeinfo.tm_min;
    int segundo = timeinfo.tm_sec;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculate the distance
    distanceCm = duration * SOUND_SPEED / 2;

    // Convert to inches
    distanceInch = distanceCm * CM_TO_INCH;

    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(distanceCm);
    Serial.print("Distance (inch): ");
    Serial.println(distanceInch);

    // Aqui a baixo está a condição de rodar somente se o client do hive estiver conectado
    //  e se as mensagem for diferente de vazia, assim ele vai chamar o vector criado anteriormente
    //  para separar as strings recebidas e salvar os horários no lugar certo dentro da matriz horarios
    if (client.connected())
    {
      if (mensagem != "")
      {
        std::string mensagemStd = mensagem.c_str();
        vector<string> substrings = tokenize(mensagemStd, ":");
        std::string datastr = substrings[0];
        std::string horstr1 = substrings[1];
        std::string minstr1 = substrings[2];
        std::string horstr2 = substrings[3];
        std::string minstr2 = substrings[4];

        // Converte as substrings para inteiros
        int dataInt = std::stoi(datastr);
        int horaInt1 = std::stoi(horstr1);
        int minutoInt1 = std::stoi(minstr1);
        int horaInt2 = std::stoi(horstr2);
        int minutoInt2 = std::stoi(minstr2);

        // Defina os horários para cada dia da semana
        switch (dataInt)
        {
        case 0: // Domingo
          horarios[0][0] = horaInt1;
          horarios[0][1] = minutoInt1;
          horarios[0][2] = horaInt2;
          horarios[0][3] = minutoInt2;
          break;
        case 1: // Segunda-feira
          horarios[1][0] = horaInt1;
          horarios[1][1] = minutoInt1;
          horarios[1][2] = horaInt2;
          horarios[1][3] = minutoInt2;
          break;
        case 2: // Terça-feira
          horarios[2][0] = horaInt1;
          horarios[2][1] = minutoInt1;
          horarios[2][2] = horaInt2;
          horarios[2][3] = minutoInt2;
          break;
        case 3: // Quarta-feira
          horarios[3][0] = horaInt1;
          horarios[3][1] = minutoInt1;
          horarios[3][2] = horaInt2;
          horarios[3][3] = minutoInt2;
          break;
        case 4: // Quinta-feira
          horarios[4][0] = horaInt1;
          horarios[4][1] = minutoInt1;
          horarios[4][2] = horaInt2;
          horarios[4][3] = minutoInt2;
          break;
        case 5: // Sexta-feira
          horarios[5][0] = horaInt1;
          horarios[5][1] = minutoInt1;
          horarios[5][2] = horaInt2;
          horarios[5][3] = minutoInt2;
          break;
        case 6: // Sábado
          horarios[6][0] = horaInt1;
          horarios[6][1] = minutoInt1;
          horarios[6][2] = horaInt2;
          horarios[6][3] = minutoInt2;
          break;
        }

        // Verifique o horário para o dia da semana atual
        int horaEsperada1 = horarios[diaSemana][0];
        int minutoEsperado1 = horarios[diaSemana][1];
        int horaEsperada2 = horarios[diaSemana][2];
        int minutoEsperado2 = horarios[diaSemana][3];

        // Verifica o horário e o dia são os mesmos recebidos na mensagem
        if ((hora - 3) == horaEsperada1 && minuto == minutoEsperado1 && segundo == 0 && diaSemana == dataInt)
        {
          Serial.println("Funcionou");
          digitalWrite(motorB1, HIGH);
          digitalWrite(motorB2, LOW);
          delay(3000);
          digitalWrite(motorB1, LOW);
        }
        else if ((hora - 3) == horaEsperada2 && minuto == minutoEsperado2 && segundo == 0 && diaSemana == dataInt)
        {
          Serial.println("Funcionou");
          digitalWrite(motorB1, HIGH);
          digitalWrite(motorB2, LOW);
          delay(3000);
          digitalWrite(motorB1, LOW);
        }
      }
    }
  else
  {
    // Reconexão ao MQTT caso a conexão seja perdida
    reconnect();
  }
  // Mantenha o cliente MQTT conectado e processe as mensagens recebidas
  client.loop();
  }
}