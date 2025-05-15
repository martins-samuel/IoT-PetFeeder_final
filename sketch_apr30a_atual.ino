#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Stepper.h>

// ====== CONFIGURAÇÃO DO WIFI ======
const char* ssid = "Nomedarede";
const char* password = "senhadarede";

// ====== USUÁRIO E SENHA PARA ACESSO ======
String username = "admin";
String senha = "1234";

// ====== CONFIGURAÇÃO DO MOTOR ======
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4

Stepper motor(2048, IN1, IN3, IN2, IN4);
ESP8266WebServer server(80);

// ====== NTP ======
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3 * 3600, 60000);  // Horário de Brasília

// ====== VARIÁVEIS DE HORÁRIO ======
String hora1 = "08:00";
String hora2 = "18:00";
bool executadoHoje[2] = {false, false};

// ====== PÁGINA HTML ======
String html_pagina = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <title>Alimentador de Gatos</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      background-color: #f4f4f4;
      font-family: Arial, sans-serif;
      text-align: center;
      padding-top: 50px;
    }
    h1 {
      color: #333;
    }
    button {
      background-color: #4CAF50;
      color: white;
      padding: 12px 24px;
      font-size: 16px;
      border: none;
      border-radius: 10px;
      cursor: pointer;
    }
    button:hover {
      background-color: #45a049;
    }
    input[type=time] {
      padding: 10px;
      font-size: 16px;
      margin: 10px;
    }
  </style>
</head>
<body>
  <h1>Alimentador de Gatos</h1>
  <form action="/girar" method="POST">
    <button type="submit">Liberar Ração</button>
  </form>
  <br>
  <h2>Agendar Alimentação</h2>
  <form action="/agendar" method="POST">
    <label>Horário 1: <input type="time" name="hora1" required></label><br>
    <label>Horário 2: <input type="time" name="hora2" required></label><br><br>
    <button type="submit">Salvar Horários</button>
  </form>
  <p>Horários atuais: %HORARIOS%</p>
</body>
</html>
)rawliteral";

// ====== AUTENTICAÇÃO ======
void autenticar() {
  if (!server.authenticate(username.c_str(), senha.c_str())) {
    return server.requestAuthentication();
  }
}

// ====== FUNÇÃO PARA LIBERAR RAÇÃO ======
void alimentar() {
  Serial.println("Liberando ração...");
  motor.step(2048); // 1 volta completa
}

// ====== SETUP ======
void setup() {
  Serial.begin(115200);
  motor.setSpeed(10);

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado. IP: " + WiFi.localIP().toString());

  timeClient.begin();

  // Rota principal com página HTML
  server.on("/", []() {
    autenticar();
    String horariosStr = hora1 + " e " + hora2;
    String pagina = html_pagina;
    pagina.replace("%HORARIOS%", horariosStr);
    server.send(200, "text/html", pagina);
  });

  // Rota para girar manualmente
  server.on("/girar", HTTP_POST, []() {
    autenticar();
    alimentar();
    server.sendHeader("Location", "/");
    server.send(303);
  });

  // Rota para agendamento de horários
  server.on("/agendar", HTTP_POST, []() {
    autenticar();
    if (server.hasArg("hora1") && server.hasArg("hora2")) {
      hora1 = server.arg("hora1");
      hora2 = server.arg("hora2");
      Serial.println("Horários atualizados: " + hora1 + " e " + hora2);
    }
    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.begin();
  Serial.println("Servidor iniciado.");
}

// ====== LOOP PRINCIPAL ======
void loop() {
  server.handleClient();
  timeClient.update();

  int horaAtual = timeClient.getHours();
  int minutoAtual = timeClient.getMinutes();

  String horaStr = (horaAtual < 10 ? "0" : "") + String(horaAtual) + ":" +
                   (minutoAtual < 10 ? "0" : "") + String(minutoAtual);

  String agendados[] = {hora1, hora2};

  for (int i = 0; i < 2; i++) {
    if (horaStr == agendados[i]) {
      if (!executadoHoje[i]) {
        alimentar();
        executadoHoje[i] = true;
      }
    } else {
      executadoHoje[i] = false;
    }
  }
}
