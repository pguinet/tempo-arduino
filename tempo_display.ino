#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "VOTRE_WIFI_SSID";
const char* password = "VOTRE_WIFI_PASSWORD";
const char* apiUrl = "https://www.api-couleur-tempo.fr/api/jourTempo";

// GPIO pour les LEDs (Option 1 - LEDs individuelles)
const int LED_AUJOURD_HUI_BLEU = 2;
const int LED_AUJOURD_HUI_BLANC = 4;
const int LED_AUJOURD_HUI_ROUGE = 5;

const int LED_DEMAIN_BLEU = 18;
const int LED_DEMAIN_BLANC = 19;
const int LED_DEMAIN_ROUGE = 21;

String couleurAujourdhui = "";
String couleurDemain = "";

void setup() {
  Serial.begin(115200);

  pinMode(LED_AUJOURD_HUI_BLEU, OUTPUT);
  pinMode(LED_AUJOURD_HUI_BLANC, OUTPUT);
  pinMode(LED_AUJOURD_HUI_ROUGE, OUTPUT);
  pinMode(LED_DEMAIN_BLEU, OUTPUT);
  pinMode(LED_DEMAIN_BLANC, OUTPUT);
  pinMode(LED_DEMAIN_ROUGE, OUTPUT);

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

  delay(10000); // Attendre 10 secondes avant la prochaine vérification
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

  // Afficher la couleur d'aujourd'hui
  if (couleurAujourdhui == "BLEU") {
    digitalWrite(LED_AUJOURD_HUI_BLEU, HIGH);
  } else if (couleurAujourdhui == "BLANC") {
    digitalWrite(LED_AUJOURD_HUI_BLANC, HIGH);
  } else if (couleurAujourdhui == "ROUGE") {
    digitalWrite(LED_AUJOURD_HUI_ROUGE, HIGH);
  }

  // Afficher la couleur de demain
  if (couleurDemain == "BLEU") {
    digitalWrite(LED_DEMAIN_BLEU, HIGH);
  } else if (couleurDemain == "BLANC") {
    digitalWrite(LED_DEMAIN_BLANC, HIGH);
  } else if (couleurDemain == "ROUGE") {
    digitalWrite(LED_DEMAIN_ROUGE, HIGH);
  }
}

void eteindreToutesLeds() {
  digitalWrite(LED_AUJOURD_HUI_BLEU, LOW);
  digitalWrite(LED_AUJOURD_HUI_BLANC, LOW);
  digitalWrite(LED_AUJOURD_HUI_ROUGE, LOW);
  digitalWrite(LED_DEMAIN_BLEU, LOW);
  digitalWrite(LED_DEMAIN_BLANC, LOW);
  digitalWrite(LED_DEMAIN_ROUGE, LOW);
}