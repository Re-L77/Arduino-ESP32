#include <WiFi.h>

// Pines de dirección
#define ADELANTE 12
#define ATRAS 14
#define DERECHA 27
#define IZQUIERDA 26
#define LED_2 2

#define GIRO 800 // Tiempo de giro en ms

const char *SSID = "Alternet_27_2_52";
const char *PASSWORD = "012345C5B0";

WiFiServer server(80);

// Variables de control
String comandoPendiente = "";
unsigned long tiempoGiroInicio = 0;
bool girando = false;

void setup()
{
  Serial.begin(115200);

  pinMode(ADELANTE, OUTPUT);
  pinMode(ATRAS, OUTPUT);
  pinMode(DERECHA, OUTPUT);
  pinMode(IZQUIERDA, OUTPUT);
  pinMode(LED_2, OUTPUT);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP());
  server.begin();
  digitalWrite(LED_2, HIGH);
}

void detenerTodo()
{
  digitalWrite(ADELANTE, LOW);
  digitalWrite(ATRAS, LOW);
  digitalWrite(DERECHA, LOW);
  digitalWrite(IZQUIERDA, LOW);
}

void girarIzquierda()
{
  digitalWrite(ADELANTE, LOW);
  digitalWrite(ATRAS, LOW);
  digitalWrite(DERECHA, HIGH);
  digitalWrite(IZQUIERDA, LOW);
}

void girarDerecha()
{
  digitalWrite(ADELANTE, HIGH);
  digitalWrite(ATRAS, LOW);
  digitalWrite(DERECHA, LOW);
  digitalWrite(IZQUIERDA, LOW);
}

void avanzar()
{
  digitalWrite(ADELANTE, HIGH);
  digitalWrite(ATRAS, LOW);
  digitalWrite(DERECHA, HIGH);
  digitalWrite(IZQUIERDA, LOW);
}

void retroceder()
{
  digitalWrite(ADELANTE, LOW);
  digitalWrite(ATRAS, HIGH);
  digitalWrite(DERECHA, LOW);
  digitalWrite(IZQUIERDA, HIGH);
}

void controlarMotores(const String &cmd)
{
  detenerTodo();

  if (cmd == "W")
  {
    avanzar();
  }
  else if (cmd == "S")
  {
    retroceder();
  }
  else if (cmd == "A")
  {
    girarIzquierda();
  }
  else if (cmd == "D")
  {
    girarDerecha();
  }
  else if (cmd == "Q" || cmd == "E" || cmd == "Z" || cmd == "X")
  {
    // Comandos con giro + avance/retroceso
    if (!girando)
    {
      if (cmd == "Q" || cmd == "Z")
      {
        girarIzquierda();
      }
      else
      {
        girarDerecha();
      }
      girando = true;
      tiempoGiroInicio = millis();
      comandoPendiente = cmd;
    }
  }
}

void loop()
{
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("Nuevo cliente");

    String request = client.readStringUntil('\r');
    client.read(); // Leer '\n'

    if (request.startsWith("GET /") && !request.startsWith("GET /favicon.ico"))
    {
      int cmdIndex = request.indexOf("GET /") + 5;
      String cmd = request.substring(cmdIndex, cmdIndex + 1);
      controlarMotores(cmd);
      Serial.println("Comando recibido: " + cmd);
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<style>");
    client.println("body { text-align: center; font-family: Arial; }");
    client.println(".button { width: 80px; height: 80px; font-size: 30px; margin: 10px; }");
    client.println("</style></head><body>");
    client.println("<h1>Control del Carrito</h1>");

    client.println("<div>");
    client.println("<button class=\"button\" onclick=\"location.href='/Q'\">Q</button>");
    client.println("<button class=\"button\" onclick=\"location.href='/W'\">W</button>");
    client.println("<button class=\"button\" onclick=\"location.href='/E'\">E</button><br>");
    client.println("<button class=\"button\" onclick=\"location.href='/A'\">A</button>");
    client.println("<button class=\"button\" onclick=\"location.href='/S'\">S</button>");
    client.println("<button class=\"button\" onclick=\"location.href='/D'\">D</button><br>");
    client.println("<button class=\"button\" onclick=\"location.href='/Z'\">Z</button>");
    client.println("<button class=\"button\" onclick=\"location.href='/X'\">X</button><br>");
    client.println("</div>");

    client.println("</body></html>");
    client.println();

    client.stop();
    Serial.println("Cliente desconectado\n");
  }

  // Verificar si terminó el giro
  if (girando && (millis() - tiempoGiroInicio >= GIRO))
  {
    girando = false;

    if (comandoPendiente == "Q" || comandoPendiente == "E")
    {
      avanzar();
    }
    else if (comandoPendiente == "Z" || comandoPendiente == "X")
    {
      retroceder();
    }

    comandoPendiente = "";
  }
}
