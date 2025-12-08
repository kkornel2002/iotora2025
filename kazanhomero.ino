#include <WiFi.h>
#include <PubSubClient.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <DHT.h>

// -------------------- ÁLLÍTHATÓ ADATOK --------------------
const char* WIFI_SSID     = "MamaNet_IoT";
const char* WIFI_PASSWORD = "PannaReka2019";

const char* MQTT_SERVER   = "192.168.1.34";
const uint16_t MQTT_PORT  = 1883;

const char* MQTT_USER     = "esp32mqtt";
const char* MQTT_PASS     = "esp32mqtt";

const char* TOPIC_DS18B20_TEMP = "home/esp32/temperature_ds18b20";
const char* TOPIC_DHT_TEMP     = "home/esp32/temperature_dht";
const char* TOPIC_DHT_HUM      = "home/esp32/humidity_dht";

// GPIO-k
#define DS18B20_PIN 4
#define DHT_PIN     5
#define DHT_TYPE    DHT11

const unsigned long MEASURE_INTERVAL = 10000;  // 10 s
// ----------------------------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);

OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);

DHT dht(DHT_PIN, DHT_TYPE);

unsigned long lastMeasure = 0;
bool wifiConnected = false;
bool mqttConnected = false;

void connectWiFi() {
  Serial.println("WiFi csatlakozasi kiserlet elindult...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) { // max 10 s
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.print("WiFi kapcsolat sikeresen felepult, IP: ");
    Serial.println(WiFi.localIP());
  } else {
    wifiConnected = false;
    Serial.println("WiFi NINCS csatlakozva, offline mod (csak Serial).");
  }
}

void connectMQTT() {
  if (!wifiConnected) {
    mqttConnected = false;
    return;
  }

  Serial.println("MQTT csatlakozasi kiserlet eliindult...");
  client.setServer(MQTT_SERVER, MQTT_PORT);

  if (client.connect("esp32-sensors", MQTT_USER, MQTT_PASS)) {
    mqttConnected = true;
    Serial.println("MQTT OK");
  } else {
    mqttConnected = false;
    Serial.print("MQTT hiba, rc=");
    Serial.println(client.state());
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("ESP32 DS18B20 + DHT11 teszt indul");

  ds18b20.begin();
  dht.begin();

  connectWiFi();
  connectMQTT();
}

void loop() {
  // MQTT kapcsolat életben tartása, ha van
  if (wifiConnected && mqttConnected) {
    if (!client.connected()) {
      connectMQTT();
    }
    client.loop();
  }

  unsigned long now = millis();
  if (now - lastMeasure >= MEASURE_INTERVAL) {
    lastMeasure = now;

    // DS18B20 mérés
    ds18b20.requestTemperatures();
    float t_ds = ds18b20.getTempCByIndex(0);

    // DHT11 mérés
    float t_dht = dht.readTemperature();
    float h_dht = dht.readHumidity();

    // Kiírás Serialra MINDENKÉPP
    Serial.println("---- Uj meres ----");
    if (!isnan(t_ds)) {
      Serial.print("DS18B20: ");
      Serial.print(t_ds, 2);
      Serial.println(" °C");
    } else {
      Serial.println("DS18B20: hibas meres");
    }

    if (!isnan(t_dht) && !isnan(h_dht)) {
      Serial.print("DHT11 ho: ");
      Serial.print(t_dht, 1);
      Serial.println(" °C");
      Serial.print("DHT11 para: ");
      Serial.print(h_dht, 1);
      Serial.println(" %");
    } else {
      Serial.println("DHT11: hibas meres");
    }

    // Ha van MQTT kapcsolat, akkor küldjük is
    if (wifiConnected && mqttConnected) {
      if (!isnan(t_ds)) {
        String payload = String(t_ds, 2);
        client.publish(TOPIC_DS18B20_TEMP, payload.c_str(), true);
      }
      if (!isnan(t_dht) && !isnan(h_dht)) {
        String pt = String(t_dht, 1);
        String ph = String(h_dht, 1);
        client.publish(TOPIC_DHT_TEMP, pt.c_str(), true);
        client.publish(TOPIC_DHT_HUM,  ph.c_str(), true);
      }
    } else {
      Serial.println("MQTT nincs, adatok csak Serialon.");
    }
    Serial.println("-------------------");
  }
}
