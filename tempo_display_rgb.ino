#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "VOTRE_WIFI_SSID";
const char* password = "VOTRE_WIFI_PASSWORD";
const char* apiUrl = "https://www.api-couleur-tempo.fr/api/jourTempo";

// GPIO pour les LEDs RGB
// LED1 (Aujourd'hui)
const int LED1_ROUGE = 2;
const int LED1_VERT = 4;
const int LED1_BLEU = 5;

// LED2 (Demain)
const int LED2_ROUGE = 18;
const int LED2_VERT = 19;
const int LED2_BLEU = 21;

String couleurAujourdhui = "";
String couleurDemain = "";

void setup() {
  Serial.begin(115200);

  pinMode(LED1_ROUGE, OUTPUT);
  pinMode(LED1_VERT, OUTPUT);
  pinMode(LED1_BLEU, OUTPUT);
  pinMode(LED2_ROUGE, OUTPUT);
  pinMode(LED2_VERT, OUTPUT);
  pinMode(LED2_BLEU, OUTPUT);

  eteindreToutesLeds();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion WiFi en cours...");
  }
  Serial.println("WiFi connecté !");

  recupererDonneesTempo();
}

void loop() {
  static unsigned long derniereMiseAJour = 0;
  unsigned long maintenant = millis();

  // Mise à jour toutes les heures (3600000 ms)
  if (maintenant - derniereMiseAJour >= 3600000 || derniereMiseAJour == 0) {
    recupererDonneesTempo();
    derniereMiseAJour = maintenant;
  }

  delay(10000);
}

void recupererDonneesTempo() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Récupérer les données d'aujourd'hui
    http.begin(String(apiUrl) + "/today");
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      couleurAujourdhui = doc["codeJour"].as<String>();
      Serial.println("Aujourd'hui: " + couleurAujourdhui);
    }
    http.end();

    // Récupérer les données de demain
    http.begin(String(apiUrl) + "/tomorrow");
    httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      couleurDemain = doc["codeJour"].as<String>();
      Serial.println("Demain: " + couleurDemain);
    }
    http.end();

    afficherCouleurs();
  }
}

void afficherCouleurs() {
  eteindreToutesLeds();

  // Afficher la couleur d'aujourd'hui sur LED1
  afficherCouleurRGB(1, couleurAujourdhui);

  // Afficher la couleur de demain sur LED2
  afficherCouleurRGB(2, couleurDemain);
}

void afficherCouleurRGB(int numLed, String couleur) {
  int pinRouge, pinVert, pinBleu;

  if (numLed == 1) {
    pinRouge = LED1_ROUGE;
    pinVert = LED1_VERT;
    pinBleu = LED1_BLEU;
  } else {
    pinRouge = LED2_ROUGE;
    pinVert = LED2_VERT;
    pinBleu = LED2_BLEU;
  }

  if (couleur == "BLEU") {
    digitalWrite(pinBleu, HIGH);
    digitalWrite(pinRouge, LOW);
    digitalWrite(pinVert, LOW);
  } else if (couleur == "BLANC") {
    digitalWrite(pinBleu, HIGH);
    digitalWrite(pinRouge, HIGH);
    digitalWrite(pinVert, HIGH);
  } else if (couleur == "ROUGE") {
    digitalWrite(pinRouge, HIGH);
    digitalWrite(pinBleu, LOW);
    digitalWrite(pinVert, LOW);
  }
}

void eteindreToutesLeds() {
  digitalWrite(LED1_ROUGE, LOW);
  digitalWrite(LED1_VERT, LOW);
  digitalWrite(LED1_BLEU, LOW);
  digitalWrite(LED2_ROUGE, LOW);
  digitalWrite(LED2_VERT, LOW);
  digitalWrite(LED2_BLEU, LOW);
}