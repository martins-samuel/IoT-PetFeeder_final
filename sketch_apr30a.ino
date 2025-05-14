#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
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

Stepper motor(2048, IN1, IN3, IN2, IN4); // sequência correta
ESP8266WebServer server(80);

// ====== HTML ESTILIZADO ======
const char* html_pagina = R"rawliteral(
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
      padding: 16px 32px;
      font-size: 18px;
      border: none;
      border-radius: 10px;
      cursor: pointer;
      box-shadow: 2px 2px 8px rgba(0,0,0,0.3);
    }
    button:hover {
      background-color: #45a049;
    }
  </style>
</head>
<body>
  <h1>Alimentador de Gatos</h1>
  <form action="/girar" method="POST">
    <button type="submit">Liberar Ração</button>
  </form>
</body>
</html>
)rawliteral";

// ====== AUTENTICAÇÃO ======
void autenticar() {
  if (!server.authenticate(username.c_str(), senha.c_str())) {
    return server.requestAuthentication();
  }
}

void setup() {
  Serial.begin(115200);
  motor.setSpeed(10); // Velocidade em RPM

  WiFi.begin(ssid, password);
  Serial.println("Conectando ao Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Página principal
  server.on("/", []() {
    autenticar();
    server.send(200, "text/html", html_pagina);
  });

  // Rota que gira o motor
  server.on("/girar", HTTP_POST, []() {
    autenticar();
    motor.step(2048); // 1 volta completa
    server.sendHeader("Location", "/");
    server.send(303); // Redireciona para a página principal
  });

  server.begin();
  Serial.println("Servidor web iniciado.");
}

void loop() {
  server.handleClient();
}
