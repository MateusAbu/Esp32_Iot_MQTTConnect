#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
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

const int ledPin = 18;
const int motorB1 = 26;
const int motorB2 = 27;

int Hor;
int Min;
int Sec;
int Data;

struct tm data;

const bool ledState = false;
String mensagem;

// Inicializar o cliente WiFi e o cliente MQTT
WiFiClientSecure espClient;
PubSubClient client(espClient);

RTC_DS3231 rtc;

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
  pinMode(ledPin, OUTPUT);
  pinMode(motorB1, OUTPUT); 
  pinMode(motorB2, OUTPUT);   
  // Conectar à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando à rede Wi-Fi...");
  }

  // Inicia o RTC
  //  rtc.begin();

  // Exibir informações sobre a rede Wi-Fi conectada
  Serial.println("Conectado à rede Wi-Fi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Configurar o cliente MQTT
  espClient.setCACert(root_ca); // Onde root_ca é a string do certificado de autoridade de certificação do servidor MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

   timeval tv;
   tv.tv_sec = 1683998960;
   settimeofday(&tv, NULL);
}

vector<string> tokenize(string s, string del = " ")
{
    vector<string> tokens;
    int start, end = -1 * del.size();
    do {
        start = end + del.size();
        end = s.find(del, start);
        tokens.push_back(s.substr(start, end - start));
    } while (end != -1);
    return tokens;
}

void loop()
{

  vTaskDelay(pdMS_TO_TICKS(1000));
  time_t tt = time(NULL);
  data = *gmtime(&tt);
  char data_formatada[64];
  strftime(data_formatada, 64, "%d/%m/%Y %H:%M:%S", &data);
  // printf("\nUnix Time: %d\n", int32_t(tt));//Mostra na Serial o Unix time
  // printf("Data formatada: %s\n", data_formatada);//Mostra na Serial a data formatada

  if(mensagem == "on") {
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    delay(3000);
    digitalWrite(motorB1, LOW);
  } else {
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
  }

  // Publicação de mensagem no tópico
  if (client.connected())
  {
    // client.publish("message", "jadshgdiuaudniajdioasdfsa");
    // DateTime now = rtc.now();
    // Hor = now.hour();
    // Min = now.minute();     
    // Sec = now.second();     
    // Data = now.dayOfTheWeek();
      // ostringstream oss;  // fluxo de saída de string
      // oss << Hor;         // insere o valor do número no fluxo
      // string Horstr = oss.str();  // extrai o valor do fluxo para uma string
      // oss.str("");        // limpa o conteúdo do fluxo

      // oss << Min;         // insere o valor do número no fluxo
      // string Minstr = oss.str();  // extrai o valor do fluxo para uma string
      // oss.str("");        // limpa o conteúdo do fluxo

      // oss << Data;         // insere o valor do número no fluxo
      // string Datastr = oss.str();  // extrai o valor do fluxo para uma string
      // oss.str(""); 

      // Serial.println(Hor);
      // Serial.println(Min);
      // Serial.println(Data);

      // if(mensagem != "") {
      // vector<string> substrings = tokenize(mensagem, ":");

      //   for (int i = 0; i < substrings.size(); i++) {
      //     cout << "Substring " << i << ": " << substrings[i] << endl;

        // Verifica o horário e o dia são os mesmos recebidos na mensagem
        // if ( Horstr == substrings[1] &&  Minstr == substrings[2] && Sec == 00 && Datastr == substrings[0] ) {
        //   analogWrite(motorB1, 255);
        //   analogWrite(motorB2, 0);
        //   delay(3000);
        //   analogWrite(motorB1, 0);
        //   analogWrite(motorB2, 255);
        //   delay(2000);
        //   analogWrite(motorB1, 255);
        //   analogWrite(motorB2, 0);
        //   delay(3000);
        //   analogWrite(motorB1, 0);
        //   analogWrite(motorB2, 0);
        // }
      //   }
      // }
      
  }
  else
  {
    // Reconexão ao MQTT caso a conexão seja perdida
    reconnect();
  }
  // Mantenha o cliente MQTT conectado e processe as mensagens recebidas
  client.loop();
}