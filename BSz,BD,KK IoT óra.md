> **ESP32 + Home Assistant + MQTT Integráció**
>
> **DS18B20 & DHT11 Szenzor Teljes Dokumentáció**
>
> **Elkészült:** 2025. november 29.
>
> **Szerző:** Barna Szilveszter, Beráth Dávid, Kovács Kornél
>
> **Verzió:** 1.0
>
> **Tartalomjegyzék**

1.  Bevezetés

2.  Rendszer Áttekintése

3.  Hardver Bekötés

4.  Szoftver Telepítés

5.  Arduino Kód

6.  Home Assistant Beállítás

7.  MQTT Konfigurálás

8.  Tesztelés

9.  Hibaelhárítás

10. Végszó

> **Bevezetés**
>
> Ez a dokumentáció teljes útmutatót nyújt ahhoz, hogy egy **Raspberry
> Pi 3B**-n futó **Home Assistant** szerverhez csatlakoztassunk egy
> **ESP32** mikrokontrollert két hőmérséklet és páratartalom mérő
> szenszorral, és az adatokat **MQTT** protokollon keresztül elküldjük
> az otthonautomatizálási rendszerbe.
>
> **A projekt célja**

- Műhely hőmérsékletének és páratartalmának monitorozása (DHT11)

- Vegyestüzelésű kazán vízhomérsékletének figyelése (DS18B20)

- Adatok tárolása és megjelenítése a Home Assistant felületén

- Statisztika állítása a szenzoradatok alapján

> **Rendszer Áttekintése**
>
> **Hardver komponensek**

1

<table style="width:100%;">
<colgroup>
<col style="width: 28%" />
<col style="width: 25%" />
<col style="width: 44%" />
<col style="width: 1%" />
</colgroup>
<tbody>
<tr class="odd">
<td><blockquote>
<p>Komponens</p>
</blockquote></td>
<td><blockquote>
<p>Leírás</p>
</blockquote></td>
<td colspan="2"><blockquote>
<p>Funkció</p>
</blockquote></td>
</tr>
<tr class="even">
<td></td>
<td></td>
<td></td>
<td></td>
</tr>
<tr class="odd">
<td><strong>Raspberry Pi 3B</strong></td>
<td>Egylapos ARM processzoros Linux optimalizát miniszámítógép</td>
<td colspan="2" rowspan="2"><blockquote>
<p>Home</p>
<p>Assistant OS +</p>
<p>Mosquitto</p>
<p>MQTT broker futtatása</p>
</blockquote></td>
</tr>
<tr class="even">
<td rowspan="2"><strong>ESP32 mikrokontroller</strong></td>
<td rowspan="2"><blockquote>
<p>WiFi modullal szerelt egylapos mikrokontroller digitális és analóg
portokkal</p>
</blockquote></td>
</tr>
<tr class="odd">
<td><blockquote>
<p>Szenzor adatok</p>
<p>gyűjtése és</p>
<p>MQTT-n</p>
</blockquote></td>
<td></td>
</tr>
<tr class="even">
<td><strong>DS18B20</strong></td>
<td><blockquote>
<p>OneWire digitális</p>
<p>hőmérő</p>
</blockquote></td>
<td><blockquote>
<p>Kazán</p>
<p>vízhőmérséklet mérése</p>
</blockquote></td>
<td></td>
</tr>
<tr class="odd">
<td><strong>DHT11</strong></td>
<td><blockquote>
<p>Digitális hőmérséklet és páratartalom</p>
<p>szenzor</p>
</blockquote></td>
<td><blockquote>
<p>Műhely</p>
<p>hőmérséklet és</p>
<p>Páratartalom mérése</p>
</blockquote></td>
<td></td>
</tr>
<tr class="even">
<td><strong>WiFi hálózat</strong></td>
<td><blockquote>
<p>2.4 GHz-es vezeték nélküli kommunikációs</p>
</blockquote></td>
<td><blockquote>
<p>ESP32 és</p>
<p>Home</p>
<p>Assistant</p>
<p>közötti</p>
<p>kommunikáció</p>
</blockquote></td>
<td></td>
</tr>
<tr class="odd">
<td></td>
<td></td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

> **Szoftver komponensek**

- **Home Assistant OS**: Otthonautomatizálási operációs rendszer

- **Mosquitto broker**: MQTT szerver (add-on formájában)

- **Arduino IDE**: ESP32 programozási környezet

- **PubSubClient**: MQTT kliens könyvtár

> **Adatáramlás**

DS18B20/DHT11 \> ESP32 (WiFi) \> Mosquitto MQTT \> Home Assistant
(192.168.1.34:1883)

> **Hardver Bekötés**
>
> **DS18B20 (OneWire Digitális Hőmérő)**
>
> A DS18B20 egy háromvezetékes szenzor, amely az OneWire protokollt
> használja.
>
> A bekötés egyszerű, de a felhúzó ellenállás **kötelező**!
>
> **Lábkiosztás**

2

<table>
<colgroup>
<col style="width: 38%" />
<col style="width: 23%" />
<col style="width: 13%" />
<col style="width: 24%" />
</colgroup>
<tbody>
<tr class="odd">
<td><blockquote>
<p>DS18B20 Láb</p>
</blockquote></td>
<td><blockquote>
<p>Szín</p>
</blockquote></td>
<td><blockquote>
<p>ESP32 Pin</p>
</blockquote></td>
<td><blockquote>
<p>Megjegyzés</p>
</blockquote></td>
</tr>
<tr class="even">
<td></td>
<td></td>
<td></td>
<td></td>
</tr>
<tr class="odd">
<td>GND</td>
<td><blockquote>
<p>Fekete</p>
</blockquote></td>
<td><blockquote>
<p>GND</p>
</blockquote></td>
<td><blockquote>
<p>Föld</p>
</blockquote></td>
</tr>
<tr class="even">
<td>DQ (adat)</td>
<td><blockquote>
<p>Sárga/szürke</p>
</blockquote></td>
<td><blockquote>
<p>GPIO 4</p>
</blockquote></td>
<td><blockquote>
<p>Adat + 4.7 kΩ felhúzó VCC-hez</p>
</blockquote></td>
</tr>
<tr class="odd">
<td>VCC</td>
<td><blockquote>
<p>Piros</p>
</blockquote></td>
<td><blockquote>
<p>3.3 V</p>
</blockquote></td>
<td><blockquote>
<p>Tápfeszültség</p>
</blockquote></td>
</tr>
<tr class="even">
<td></td>
<td></td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

> **Felhúzó ellenállás** A DQ pin-hez **4.7 kΩ-os ellenállást** kell
> csatlakoztatni a VCC és DQ között. Ez biztosítja a szenzor és a
> mikrokontroller közti kommunikáció megbízhatóságát.
>
> +3.3V
>
> \|
>
> \[4.7kΩ\]
>
> \|
>
> \[DS18B20\]
>
> \|\_\_\_\_\_ GPIO 4 (ESP32)
>
> GND
>
> **DHT11 (Hőmérséklet/Páratartalom Szenzor)**
>
> A DHT11 egy kész modul, amely már tartalmazza a szükséges felhúzó
> ellenállásokat. Három lábú csatlakozó:

<table>
<colgroup>
<col style="width: 48%" />
<col style="width: 17%" />
<col style="width: 12%" />
<col style="width: 21%" />
</colgroup>
<tbody>
<tr class="odd">
<td><blockquote>
<p>DHT11 Pin</p>
</blockquote></td>
<td><blockquote>
<p>Elnevezés</p>
</blockquote></td>
<td><blockquote>
<p>ESP32 Pin</p>
</blockquote></td>
<td><blockquote>
<p>Megjegyzés</p>
</blockquote></td>
</tr>
<tr class="even">
<td></td>
<td></td>
<td></td>
<td></td>
</tr>
<tr class="odd">
<td>S</td>
<td><blockquote>
<p>Signal (adat)</p>
</blockquote></td>
<td><blockquote>
<p>GPIO 5</p>
</blockquote></td>
<td><blockquote>
<p>Adat</p>
</blockquote></td>
</tr>
<tr class="even">
<td>Középső</td>
<td><blockquote>
<p>VCC</p>
</blockquote></td>
<td><blockquote>
<p>3.3 V</p>
</blockquote></td>
<td><blockquote>
<p>Tápfeszültség</p>
</blockquote></td>
</tr>
<tr class="odd">
<td>–</td>
<td><blockquote>
<p>GND</p>
</blockquote></td>
<td><blockquote>
<p>GND</p>
</blockquote></td>
<td><blockquote>
<p>Föld</p>
</blockquote></td>
</tr>
<tr class="even">
<td></td>
<td></td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

> **Bekötési séma**
>
> DHT11 modul
>
> S pin \> GPIO 5 (ESP32) VCC \> 3.3 V (ESP32) GND \> GND (ESP32)
>
> **Szoftver Telepítés**
>
> **Arduino IDE Beállítása**

1.  **Arduino IDE Letöltése** Töltsük le az Arduino IDE-t az
    > [Arduino.cc](https://www.arduino.cc/en/software) oldal-ról.
    > Telepítsük az operációs rendszerhez megfelelő verziót.

3

2.  **ESP32 Board Hozzáadása**

    1.  Nyissuk meg az Arduino IDE-t

    2.  Menünkj a **File \> Preferences** menübe

    3.  Az **Additional Boards Manager URLs** mezőbe illesszük be:
        > https://dl.espressif.com/dl/package_esp32_index.json

    4.  Kattintsunk az OK gombra

    5.  Menjünk a **Tools \> Board \> Boards Manager...** menübe

    6.  Keressük meg az: esp32 menüpontot és kattintsunk az **Install**
        > gombra

    <!-- -->

    2.  **Board Kiválasztása** A feltöltés után: 1. **Tools \> Board \>
        > ESP32 Arduino \> DOIT ESP32 DEVKIT V1** (vagy az általunk
        > használt variáns)

    <!-- -->

    2.  **Tools \> Port** \> válasszuk ki az ESP32 USB portját
        > (általában COM3, COM4 vagy /dev/ttyUSB0 Linux-on)

> **Szükséges Library-k Telepítése**
>
> Az Arduino IDE-ben nyissuk meg: **Sketch \> Include Library \>
> Manage**
>
> **Libraries...**
>
> Telepítsük az alábbi library-kat:

1.  **PubSubClient (Nick O'Leary)**

    - Könyvtárnév: pubsubclient

    - Verzió: 2.8.0+

    - Funkcó: MQTT kliens

2.  **OneWire**

    - Könyvtárnév: onewire

    - Verzió: 2.3.5+

    - Función: DS18B20 kommunikáció

3.  **DallasTemperature**

    - Könyvtárnév: dallastemperature

    - Verzió: 3.9.1+

    - Función: DS18B20 kezelés

4.  **DHT Sensor Library (Adafruit)**

    - Könyvtárnév: dht sensor library

    - Verzió: 1.4.4+

    - Función: DHT szenzor kezelés

4

> **EESP-32D Kód**
>
> **Teljes Kód Listája**
>
> \#include **\<WiFi.h\>**
>
> \#include **\<PubSubClient.h\>**
>
> \#include **\<OneWire.h\>**
>
> \#include **\<DallasTemperature.h\>**
>
> \#include **\<DHT.h\>**

- *============================================================*

- *SETUPS (ÁLLÍTHATÓ ADATOK)*

- *============================================================*

- *WiFi adatok*

> const char\* WIFI_SSID = \"ide a wifi SSID jön\";
>
> const char\* WIFI_PASSWORD = \"ide meg a wifi kód\";
>
> *// MQTT adatok*
>
> const char\* MQTT_SERVER = \"MQTT/HA szerver ip címe\";
>
> const uint16_t MQTT_PORT = 1883;
>
> const char\* MQTT_USER = \"esp32mqtt\";
>
> const char\* MQTT_PASS = \"esp32mqtt\";
>
> *// MQTT Topic-ok*
>
> const char\* TOPIC_DS18B20_TEMP = \"home/esp32/temperature_ds18b20\";
>
> const char\* TOPIC_DHT_TEMP = \"home/esp32/temperature_dht\";
>
> const char\* TOPIC_DHT_HUM = \"home/esp32/humidity_dht\";
>
> *// GPIO Pin-ek*
>
> \#define DS18B20_PIN 4 *// OneWire adat pin*
>
> \#define DHT_PIN 5 *// DHT adat pin*
>
> \#define DHT_TYPE DHT11 *// DHT11 típus*
>
> *// Mérési időköz*
>
> const unsigned long MEASURE_INTERVAL = 10000; *// 10 másodperc*

- *============================================================*

- *OBJEKTUMOK ÉS GLOBÁLIS VÁLTOZÓK*

- *============================================================*

> WiFiClient espClient;
>
> PubSubClient client(espClient);

5

> OneWire oneWire(DS18B20_PIN);
>
> DallasTemperature ds18b20(&oneWire);
>
> DHT dht(DHT_PIN, DHT_TYPE);
>
> unsigned long lastMeasure = 0;
>
> bool wifiConnected = **false**;
>
> bool mqttConnected = **false**;

- *============================================================*

- *WIFI KAPCSOLÓDÁS*

- *============================================================*

> void connectWiFi() {
>
> Serial.println(\"WiFi csatlakozasi kiserlet elindult\...\");
> WiFi.mode(WIFI_STA);
>
> WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
>
> unsigned long start = millis();
>
> **while** (WiFi.status() != WL_CONNECTED && millis() - start \< 10000)
> { delay(500);
>
> Serial.print(\".\");
>
> }
>
> Serial.println();
>
> **if** (WiFi.status() == WL_CONNECTED) {
>
> wifiConnected = **true**;
>
> Serial.print(\"WiFi kapcsolat sikeresen felult, IP: \");
> Serial.println(WiFi.localIP());
>
> } **else** {
>
> wifiConnected = **false**;
>
> Serial.println(\"WiFi NINCS csatlakozva, offline mod (csak
> Serial).\");
>
> }
>
> }

- *============================================================*

- *MQTT KAPCSOLÓDÁS*

- *============================================================*

> void connectMQTT() {
>
> **if** (!wifiConnected) {
>
> mqttConnected = **false**;
>
> **return**;
>
> }
>
> Serial.println(\"MQTT csatlakozasi kiserlet elindult\...\");

6

> client.setServer(MQTT_SERVER, MQTT_PORT);
>
> **if** (client.connect(\"esp32-sensors\", MQTT_USER, MQTT_PASS)) {
> mqttConnected = **true**;
>
> Serial.println(\"MQTT OK\");
>
> } **else** {
>
> mqttConnected = **false**;
>
> Serial.print(\"MQTT hiba, rc=\");
>
> Serial.println(client.state());
>
> }
>
> }

- *============================================================*

- *SETUP FUNKCIÓ*

- *============================================================*

> void setup() {
>
> Serial.begin(115200);
>
> delay(1000);
>
> Serial.println();
>
> Serial.println(\"========================================\");
> Serial.println(\"ESP32 DS18B20 + DHT11 + MQTT projekt\");
> Serial.println(\"========================================\");

- *Szenzorok inicializálása* ds18b20.begin(); dht.begin();

- *WiFi és MQTT kapcsolódás* connectWiFi(); connectMQTT();

> }

- *============================================================*

- *MAIN LOOP*

- *============================================================*

> void loop() {
>
> *// MQTT kapcsolat fenntartása*
>
> **if** (wifiConnected && mqttConnected) {
>
> **if** (!client.connected()) {
>
> connectMQTT();
>
> }
>
> client.loop();
>
> }

7

> *// Mérési logika*
>
> unsigned long now = millis();
>
> **if** (now - lastMeasure \>= MEASURE_INTERVAL) {
>
> lastMeasure = now;

- *\-\-\-\-\-\-\-- DS18B20 Mérés \-\-\-\-\-\-\--*

> ds18b20.requestTemperatures();
>
> float t_ds = ds18b20.getTempCByIndex(0);

- *\-\-\-\-\-\-\-- DHT11 Mérés \-\-\-\-\-\-\--*

> float t_dht = dht.readTemperature();
>
> float h_dht = dht.readHumidity();

- *\-\-\-\-\-\-\-- Kiírás Serialra \-\-\-\-\-\-\--*

> Serial.println(\"\-\-\-- Uj meres \-\-\--\");
>
> **if** (!isnan(t_ds)) {
>
> Serial.print(\"DS18B20: \");
>
> Serial.print(t_ds, 2);
>
> Serial.println(\" °C\");
>
> } **else** {
>
> Serial.println(\"DS18B20: hibas meres\");
>
> }
>
> **if** (!isnan(t_dht) && !isnan(h_dht)) {
>
> Serial.print(\"DHT11 homérséklet: \");
>
> Serial.print(t_dht, 1);
>
> Serial.println(\" °C\");
>
> Serial.print(\"DHT11 páratartalom: \");
>
> Serial.print(h_dht, 1);
>
> Serial.println(\" %\");
>
> } **else** {
>
> Serial.println(\"DHT11: hibas meres\");
>
> }

- *\-\-\-\-\-\-\-- MQTT Publikálás \-\-\-\-\-\-\--*

> **if** (wifiConnected && mqttConnected) { **if** (!isnan(t_ds)) {
>
> String payload = String(t_ds, 2);
>
> client.publish(TOPIC_DS18B20_TEMP, payload.c_str(), **true**);
>
> }
>
> **if** (!isnan(t_dht) && !isnan(h_dht)) { String pt = String(t_dht,
> 1); String ph = String(h_dht, 1);
>
> client.publish(TOPIC_DHT_TEMP, pt.c_str(), **true**);
> client.publish(TOPIC_DHT_HUM, ph.c_str(), **true**);

8

> }
>
> } **else** {
>
> Serial.println(\"MQTT nincs, adatok csak Serialon.\");
>
> }
>
> Serial.println(\"\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\--\");
>
> }
>
> }
>
> **Kód Magyarázata**
>
> **Globális Konstansok**
>
> const char\* WIFI_SSID = \"wifi SSID\";
>
> const char\* WIFI_PASSWORD = \"wifi jelszo\";

A WiFi hálózat neve és jelszava. **Ezeket meg kell a sajátodra
változtatni!**

<table>
<colgroup>
<col style="width: 43%" />
<col style="width: 32%" />
<col style="width: 2%" />
<col style="width: 21%" />
</colgroup>
<tbody>
<tr class="odd">
<td><blockquote>
<p>const</p>
</blockquote></td>
<td><blockquote>
<p>char* MQTT_SERVER</p>
</blockquote></td>
<td>=</td>
<td><blockquote>
<p>"Szerver ip";</p>
</blockquote></td>
</tr>
<tr class="even">
<td>const</td>
<td><blockquote>
<p>uint16_t MQTT_PORT</p>
</blockquote></td>
<td>=</td>
<td><blockquote>
<p>1883;</p>
</blockquote></td>
</tr>
</tbody>
</table>

> Az MQTT broker (Mosquitto) IP címe és portja. A Pi3 IP-je.
>
> const char\* MQTT_USER = \"esp32mqtt\";
>
> const char\* MQTT_PASS = \"esp32mqtt\";
>
> Az MQTT felhasználónév és jelszó. Ezt meg kell a Home Assistantban
> létre-hozni.
>
> **GPIO Pin-ek**
>
> \#define DS18B20_PIN 4

<table>
<colgroup>
<col style="width: 94%" />
<col style="width: 5%" />
</colgroup>
<tbody>
<tr class="odd">
<td><blockquote>
<p>#define DHT_PIN</p>
</blockquote></td>
<td>5</td>
</tr>
</tbody>
</table>

> A szenzorok GPIO pin-jei. Ha máshova kötötted, ezt módosítsd.
>
> **connectWiFi() Funkció**
>
> void connectWiFi() {
>
> Serial.println(\"WiFi csatlakozasi kiserlet elindult\...\");
> WiFi.mode(WIFI_STA);
>
> WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
>
> *// \... max 10 másodpercig próbálkozik*
>
> Csatlakozik a WiFi hálózathoz. Ha nem sikerül 10 másodperc alatt,
> offline módban működik.
>
> **connectMQTT() Funkció**
>
> **if** (client.connect(\"esp32-sensors\", MQTT_USER, MQTT_PASS)) {
> mqttConnected = **true**;

9

> Serial.println(\"MQTT OK\");
>
> }
>
> Az MQTT brokerhez csatlakozik. Az esp32-sensors az eszköz neve a
> broker-ben.

**loop() - Mérési Logika** 10 másodpercenként: 1. Olvassa a DS18B20
értékét

2.  Olvassa a DHT11 hőmérsékletet és páratartalmát 3. Kiírja a Serial
    > monitorra

<!-- -->

4.  Publikálja az MQTT topicokra (ha van MQTT kapcsolat)

> **Home Assistant Beállítás**
>
> **1. Mosquitto Add-on Telepítése**
>
> A Home Assistant már telepítve van a Pi3-on, de a Mosquitto MQTT
> brokert még hozzá kell adni.
>
> **Lépések:**

1.  Nyissuk meg a Home Assistant webes felületét

2.  Menjünk a **Beállítások \> Kiegészítők \> Kiegészítő bolt** menübe

3.  Keresük ezt: mosquitto

4.  Válasszuk ki ezt: **Mosquitto broker** (az első)

5.  Kattintsunk az **Telepítés** gombra

6.  Várjuk meg, hogy befejeződik

7.  Kapcsoljuk be: kattintsunk az **Indítás** gombra

8.  Pipáljuk be az **Indítás rendszerrel** opciót

<!-- -->

2.  **MQTT Felhasználó Létrehozása**

> Az MQTT brokerhez csatlakozáshoz egy felhasználó szükséges.
>
> **Lépések:**

1.  Home Assistant: **Beállítások \> Személyek \> Felhasználók**

2.  Kattintsunk az **Felhasználó hozzáadása** gombra

3.  Kitöltés:

    - **Név**: MQTT ESP32 (tetszőleges)

    - **Felhasználónév**: esp32mqtt

    - **Jelszó**: esp32mqtt (vagy másik)

    - **Bejelentkezhet**: (pipa)

    - **Admin**: (nincs pipa)

4.  Kattintsunk a **Létrehozás** gombra

> **Fontos**: A felhasználónévnek és jelszónak **pontosan ugyanennek**
> kell lennie a kódban!

10

> **3. MQTT Integráció Hozzáadása**
>
> Mostantól a Home Assistantnak tudnia kell, hogy van MQTT broker.
>
> **Lépések:**

1.  **Beállítások \> Eszközök és szolgáltatások**

2.  Jobb alsó sarokban: **+ Integráció hozzáadása** gomb

3.  Keressük meg ezt: MQTT

4.  A varázsló automatikusan kitölti:

    - **Broker**: core-mosquitto

    - **Port**: 1883

5.  Kattintsunk a **Küldés** gombra

6.  Befejezés

> Mostantól az MQTT integráció aktív!
>
> **4. Szenzorok Hozzáadása**
>
> Most a szenzoradatokat szenzor entitásként kell felvenni a HA-be.
>
> **File Editor Telepítése**

1.  **Beállítások \> Kiegészítők \> Kiegészítő bolt**

2.  Keressük meg ezt: file editor

3.  Telepítés és indítás

4.  Ha sikeresen települt a bal oldali menüben megjelenik a **File
    > editor** ikon

> **configuration.yaml Szerkesztése**

1.  A File editorban nyissuk meg ezt: configuration.yaml

2.  Menjünk a fájl végére

3.  Adjuk hozzá az alábbiakat (vigyázzunk az indentálásra!):

> mqtt**:**
>
> sensor**:**

- name**:** \"Muhely homerseklet (DHT)\"

> state_topic**:** \"home/esp32/temperature_dht\"
>
> unit_of_measurement**:** \"°C\"

- name**:** \"Muhely paratartalom (DHT)\"

> state_topic**:** \"home/esp32/humidity_dht\"
>
> unit_of_measurement**:** \"%\"

- name**:** \"Kazan vizhofoky (DS18B20)\"

> state_topic**:** \"home/esp32/temperature_ds18b20\"
>
> unit_of_measurement**:** \"°C\"

3.  Mentés (Ctrl+S)

11

5.  **Beállítások \> Rendszer \> Beállítás ellenőrzése**

    - Ha zöld checkmark: OK

    - Ha hiba: az editor azt jelzi

6.  Ha OK: **Beállítások \> Rendszer \> Újraindítás**

<!-- -->

5.  **Szenzor Entitások Ellenőrzése**

> Az újraindítás után az entitások megjelennek:

1.  **Beállítások \> Eszközök és szolgáltatások \> Entitások**

2.  Keressük meg: muhely vagy kazan

3.  Ez kell, hogy megjelenjen:

    - sensor.muhely_homerseklet_dht

    - sensor.muhely_paratartalom_dht

    - sensor.kazan_vizhofoky_ds18b20

> Ezek használhatók dashboardon, automatizálásban, stb.
>
> **MQTT Konfigurálás**
>
> **MQTT Topic Struktúra**
>
> Az ESP32 három topicra publikál:
>
> home/esp32/temperature_ds18b20 → DS18B20 hőmérséklet (kazánvíz)
>
> home/esp32/temperature_dht → DHT11 hőmérséklet (műhely)
>
> home/esp32/humidity_dht → DHT11 páratartalom (műhely)
>
> **Tesztelés: Topic Figyelése**
>
> Az MQTT topicok élőben figyelhetők:

1.  Home Assistant: **Beállítások \> MQTT** (az MQTT integráció
    > fogaskerékje)

2.  **Figyelj egy témára** (Listen to a topic) input mező

3.  Írjuk be: home/esp32/#

4.  Kapcsoljuk be a figyelést

5.  Az ESP32 bekapcsolt állapotában 10 másodpercenként jönnek az értékek

> **Tesztelés**
>
> **1. Serial Monitor Ellenőrzés**
>
> Az ESP32 indulása után könnyen ellenőrizhető, hogy működik-e:

1.  Arduino IDE: **Tools \> Serial Monitor**

2.  Baud rate: **115200** (nagyon fontos!)

12

> 3\. Line ending: **Both NL & CR**
>
> **Várt kimenet** Indulás után 1-2 másodpercen belül:
>
> ========================================
>
> ESP32 DS18B20 + DHT11 + MQTT projekt
>
> ========================================
>
> WiFi csatlakozasi kiserlet elindult\...
>
> \.....
>
> WiFi kapcsolat sikeresen felult, IP: 192.168.1.118 MQTT csatlakozasi
> kiserlet elindult\... MQTT OK
>
> \-\-\-- Uj meres \-\-\--
>
> DS18B20: 65.12 °C
>
> DHT11 homérséklet: 24.0 °C
>
> DHT11 páratartalom: 45.0 %
>
> \-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\--
>
> \-\-\-- Uj meres \-\-\--
>
> DS18B20: 65.10 °C
>
> DHT11 homérséklet: 24.1 °C
>
> DHT11 páratartalom: 44.9 %
>
> \-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\--

2.  **MQTT Topic Ellenőrzés**

    1.  Home Assistant: **Beállítások \> MQTT**

    2.  Listen to a topic: home/esp32/#

    3.  Indítsuk el az ESP32-t

    4.  10 másodpercen belül jöjjenek az üzenetek:

> Topic: home/esp32/temperature_ds18b20
>
> Payload: 65.12
>
> Topic: home/esp32/temperature_dht
>
> Payload: 24.0
>
> Topic: home/esp32/humidity_dht
>
> Payload: 45.0
>
> **Paraméterek Testreszabása**
>
> **Mérési Intervallum Módosítása**
>
> Alapból 10 másodpercenként méri. Ha másra akarjuk változtatni:

14

> const unsigned long MEASURE_INTERVAL = 30000; *// 30 másodperc* const
> unsigned long MEASURE_INTERVAL = 5000; *// 5 másodperc*
>
> **Topic Nevek Módosítása**
>
> Ha más topicneveket szeretnénk:
>
> const char\* TOPIC_DS18B20_TEMP = \"otthon/sensor/kazan\";
>
> const char\* TOPIC_DHT_TEMP = \"otthon/sensor/muhely/temp\";
>
> const char\* TOPIC_DHT_HUM = \"otthon/sensor/muhely/hum\";
>
> **Fontos**: Ezt a configuration.yaml-ban is frissítened kell az új
> nevekre!
>
> **GPIO Pin-ek Módosítása**
>
> Ha más pin-ekre kötnénk:

<table>
<colgroup>
<col style="width: 40%" />
<col style="width: 26%" />
<col style="width: 6%" />
<col style="width: 4%" />
<col style="width: 12%" />
<col style="width: 8%" />
</colgroup>
<tbody>
<tr class="odd">
<td><blockquote>
<p>#define</p>
</blockquote></td>
<td><blockquote>
<p>DS18B20_PIN 12</p>
</blockquote></td>
<td colspan="2"><em>//</em></td>
<td><blockquote>
<p><em>másik</em></p>
</blockquote></td>
<td><blockquote>
<p><em>pin</em></p>
</blockquote></td>
</tr>
<tr class="even">
<td>#define</td>
<td><blockquote>
<p>DHT_PIN</p>
</blockquote></td>
<td>14</td>
<td><em>//</em></td>
<td><blockquote>
<p><em>másik</em></p>
</blockquote></td>
<td><blockquote>
<p><em>pin</em></p>
</blockquote></td>
</tr>
<tr class="odd">
<td></td>
<td></td>
<td></td>
<td></td>
<td></td>
<td></td>
</tr>
</tbody>
</table>

> **Végszó**
>
> Ezzel a beállítással az ESP32 szenzor csomópont mostantól folyamatosan
> figyeli a műhelyhőmérsékletet és a kazánvízet, és az adatokat a Home
> Assistantba küldi.
>
> **Lehetőségek az adatokkal**

- **Automatizálások**: Hőmérséklet eléri az X°C-ot →
  > szivattyú/ventilátor bekapcs

- **Értesítések**: Ha a víz túl forró → push notification

- **Előzmények**: Grafikonok a hőmérsékleti görbékről

- **Energia**: Kazán fűtési ciklus nyomon követése

- **Okos otthon**: Fűtési rendszer teljes automatizálása

> **Hasznos linkek**

- [Home Assistant Dokumentáció](https://www.home-assistant.io/)

- [MQTT Protokoll](https://mqtt.org/)

- [ESP32
  > Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)

- [Arduino IDE](https://www.arduino.cc/)

> *2025. november 29.*

15
