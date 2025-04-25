#include <WiFi.h>

// Pines de dirección
#define ADELANTE    12   
#define ATRAS       14      
#define DERECHA     27    
#define IZQUIERDA   26 

// Red Wi-Fi
const char *SSID = "Alternet_27_2_52";
const char *PASSWORD = "012345C5B0";

// Servidor
WiFiServer server(80);

void setup()
{
  Serial.begin(115200);

  pinMode(ADELANTE, OUTPUT);
  pinMode(ATRAS, OUTPUT);
  pinMode(DERECHA, OUTPUT);
  pinMode(IZQUIERDA, OUTPUT);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP());

  server.begin();
}

void detenerTodo()
{
  digitalWrite(ADELANTE, LOW);
  digitalWrite(ATRAS, LOW);
  digitalWrite(DERECHA, LOW);
  digitalWrite(IZQUIERDA, LOW);
}

void controlarMotores(const String &cmd)
{
  detenerTodo();
  if (cmd == "F")
    digitalWrite(ADELANTE, HIGH);
  else if (cmd == "B")
    digitalWrite(ATRAS, HIGH);
  else if (cmd == "L")
    digitalWrite(IZQUIERDA, HIGH);
  else if (cmd == "R")
    digitalWrite(DERECHA, HIGH);

  Serial.println("Comando recibido: " + cmd);
}

void loop()
{
  WiFiClient client = server.available();
  if (!client)
    return;

  Serial.println("Nuevo cliente");

  String request = client.readStringUntil('\r');
  client.read(); // Leer '\n'

  if (request.startsWith("GET /") && !request.startsWith("GET /favicon.ico"))
  {
    int cmdIndex = request.indexOf("GET /") + 5;
    String cmd = request.substring(cmdIndex, cmdIndex + 1);
    controlarMotores(cmd);
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body><h1>Comando recibido</h1></body></html>");
  client.println();

  client.stop();
  Serial.println("Cliente desconectado\n");
}
