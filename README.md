# Afficheur EDF Tempo pour ESP32

Ce projet permet d'afficher les couleurs tarifaires EDF Tempo (aujourd'hui et demain) via des LEDs connectées à un ESP32.

## Composants nécessaires

### Option 1 - LEDs individuelles (économique ~8€)
- 1x ESP32 DevKit (~6€)
- 6x LEDs (2 bleues, 2 blanches, 2 rouges) (~1€)
- 6x Résistances 220Ω (~0.50€)
- Breadboard et fils de connexion (~0.50€)

### Option 2 - LEDs RGB (simple ~10€)
- 1x ESP32 DevKit (~6€)
- 2x LEDs RGB (~3€)
- 6x Résistances 220Ω (~0.50€)
- Breadboard et fils de connexion (~0.50€)

## Connexions

### Option 1 - LEDs individuelles
```
AUJOURD'HUI:
- LED Bleue  → GPIO 2  (+ résistance 220Ω)
- LED Blanche → GPIO 4  (+ résistance 220Ω)
- LED Rouge  → GPIO 5  (+ résistance 220Ω)

DEMAIN:
- LED Bleue  → GPIO 18 (+ résistance 220Ω)
- LED Blanche → GPIO 19 (+ résistance 220Ω)
- LED Rouge  → GPIO 21 (+ résistance 220Ω)

Toutes les cathodes → GND
```

### Option 2 - LEDs RGB
```
LED1 (AUJOURD'HUI):
- Pin Rouge → GPIO 2  (+ résistance 220Ω)
- Pin Vert  → GPIO 4  (+ résistance 220Ω)
- Pin Bleu  → GPIO 5  (+ résistance 220Ω)
- Cathode commune → GND

LED2 (DEMAIN):
- Pin Rouge → GPIO 18 (+ résistance 220Ω)
- Pin Vert  → GPIO 19 (+ résistance 220Ω)
- Pin Bleu  → GPIO 21 (+ résistance 220Ω)
- Cathode commune → GND
```

## Configuration

1. Installez l'environnement Arduino IDE
2. Ajoutez le support ESP32 : `https://dl.espressif.com/dl/package_esp32_index.json`
3. Installez la librairie `ArduinoJson` depuis le gestionnaire de bibliothèques
4. Modifiez les identifiants WiFi dans le code :
   ```cpp
   const char* ssid = "VOTRE_WIFI_SSID";
   const char* password = "VOTRE_WIFI_PASSWORD";
   ```

## Fonctionnement

- **Bleu** : Jour normal (tarif bas)
- **Blanc** : Jour intermédiaire (tarif moyen)
- **Rouge** : Jour de pointe (tarif élevé)

L'ESP32 récupère les données toutes les heures depuis l'API : https://www.api-couleur-tempo.fr/

## Codes disponibles

- `tempo_display.ino` : Version avec LEDs individuelles
- `tempo_display_rgb.ino` : Version avec LEDs RGB

## API utilisée

L'application utilise l'API gratuite https://www.api-couleur-tempo.fr/ qui fournit :
- `/api/jourTempo/today` : Couleur d'aujourd'hui
- `/api/jourTempo/tomorrow` : Couleur de demain

Les données proviennent de RTE (Réseau de Transport d'Électricité).