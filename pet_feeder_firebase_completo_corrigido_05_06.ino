
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// ======== CONFIGURAÇÕES ======== //
// Wi-Fi
#define WIFI_SSID "..."
#define WIFI_PASSWORD "..."

// Firebase
#define API_KEY ""
#define DATABASE_URL ""
#define FIREBASE_AUTH ""

// Sensor ultrassônico
#define TRIG_PIN D5
#define ECHO_PIN D6

// Motor de passo
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3 * 3600, 60000); // Fuso horário Brasil

String horaAtual() {
  timeClient.update();
  return timeClient.getFormattedTime().substring(0, 5); // HH:MM
}

String dataHoraLog() {
  timeClient.update();
  time_t rawTime = timeClient.getEpochTime();
  struct tm *timeinfo = localtime(&rawTime);
  char buffer[20];
  sprintf(buffer, "%04d-%02d-%02d_%02d:%02d", timeinfo->tm_year + 1900,
          timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min);
  return String(buffer);
}

void alimentarGato() {
  for (int i = 0; i < 512; i++) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); delay(2);
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); delay(2);
    digitalWrite(IN2, LOW);  digitalWrite(IN3, HIGH); delay(2);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); delay(2);
  }
}

float medirNivel() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duracao = pulseIn(ECHO_PIN, HIGH);
  float distancia_cm = duracao * 0.034 / 2;
  return distancia_cm;
}

void conectarWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado! IP: " + WiFi.localIP().toString());
}

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);

  conectarWiFi();
  timeClient.begin();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  String hora = horaAtual();

  if (Firebase.RTDB.getBool(&fbdo, "/alimentar") && fbdo.boolData()) {
    Serial.println(">> Comando remoto recebido. Liberando ração...");
    alimentarGato();
    Firebase.RTDB.setBool(&fbdo, "/alimentar", false);
    Firebase.RTDB.setString(&fbdo, "/status", "Alimentado em: " + hora);
    Firebase.RTDB.setString(&fbdo, "/historico/" + dataHoraLog(), "ração liberada");
  }

  if (Firebase.RTDB.getJSON(&fbdo, "/agendamentos")) {
    FirebaseJson &json = fbdo.jsonObject();

    FirebaseJsonData dataHora1, dataHora2;
    String h1 = "", h2 = "";

    json.get(dataHora1, "hora1");
    json.get(dataHora2, "hora2");

    if (dataHora1.success) h1 = dataHora1.to<String>();
    if (dataHora2.success) h2 = dataHora2.to<String>();

    static bool agendado1 = false, agendado2 = false;

    if (hora == h1 && !agendado1) {
      alimentarGato();
      Firebase.RTDB.setString(&fbdo, "/historico/" + dataHoraLog(), "ração liberada (agendada)");
      agendado1 = true;
    } else if (hora != h1) {
      agendado1 = false;
    }

    if (hora == h2 && !agendado2) {
      alimentarGato();
      Firebase.RTDB.setString(&fbdo, "/historico/" + dataHoraLog(), "ração liberada (agendada)");
      agendado2 = true;
    } else if (hora != h2) {
      agendado2 = false;
    }
  }

  float nivel = medirNivel();
  Firebase.RTDB.setFloat(&fbdo, "/nivel_racao_cm", nivel);
  Serial.println(">> Nível de ração: " + String(nivel) + " cm");

  delay(60000); // Aguarda 1 min
}
