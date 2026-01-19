#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(10);

struct can_frame msgSpeedBrake;
struct can_frame msgRpm;
struct can_frame msgOilTemp;

// IDs
const canid_t ID_SPEED_BRAKE = 0x1;
const canid_t ID_RPM         = 0x2;  // <-- RPM ici
const canid_t ID_OIL_TEMP    = 0x3;

// Périodes d'envoi
const unsigned long SPEED_PERIOD_MS = 200;   // 5 Hz
const unsigned long RPM_PERIOD_MS   = 200;   // 5 Hz
const unsigned long TEMP_PERIOD_MS  = 1000;  // 1 Hz

unsigned long lastSpeedTx = 0;
unsigned long lastRpmTx   = 0;
unsigned long lastTempTx  = 0;

// Envoi générique avec gestion d'erreurs
void sendFrame(struct can_frame &f) {
  MCP2515::ERROR err = mcp2515.sendMessage(&f);

  if (err == MCP2515::ERROR_OK) {
    Serial.print("TX OK id=0x");
    Serial.println(f.can_id, HEX);
  } 
  else if (err == MCP2515::ERROR_ALLTXBUSY) {
    // code=2 : buffers TX pleins -> on drop cette trame et on réessaiera au prochain tick
    Serial.print("TX BUSY id=0x");
    Serial.println(f.can_id, HEX);
  } 
  else {
    Serial.print("CAN TX FAIL id=0x");
    Serial.print(f.can_id, HEX);
    Serial.print(" code=");
    Serial.println((int)err);
  }
}

void setup() {
  Serial.begin(250000);

  // Frein sur A3 : bouton vers GND, pull-up interne
  pinMode(A3, INPUT_PULLUP);

  // Init MCP2515 (bitrate/quartz inchangés)
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  // Message vitesse/frein
  msgSpeedBrake.can_id  = ID_SPEED_BRAKE;
  msgSpeedBrake.can_dlc = 8;

  // Message RPM
  msgRpm.can_id  = ID_RPM;
  msgRpm.can_dlc = 8;

  // Message température huile
  msgOilTemp.can_id  = ID_OIL_TEMP;
  msgOilTemp.can_dlc = 8;

  // Random seed
  randomSeed(analogRead(A1));

  unsigned long now = millis();
  lastSpeedTx = now;
  lastRpmTx   = now;
  lastTempTx  = now;
}

void loop() {
  unsigned long now = millis();

  // --- Envoi vitesse + frein (ID 0x1) ---
  if (now - lastSpeedTx >= SPEED_PERIOD_MS) {
    lastSpeedTx += SPEED_PERIOD_MS;

    uint16_t vitesse = (uint16_t)analogRead(A0);   // 0..1023
    bool freinAppuye = (digitalRead(A3) == LOW);   // INPUT_PULLUP => LOW si appuyé

    // Option : frein = vitesse forcée à 0
    if (freinAppuye) vitesse = 0;

    // Encodage :
    // data[0..1] = vitesse (uint16, little-endian)
    // data[2]    = frein (0/1)
    msgSpeedBrake.data[0] = (uint8_t)(vitesse & 0xFF);
    msgSpeedBrake.data[1] = (uint8_t)(vitesse >> 8);
    msgSpeedBrake.data[2] = freinAppuye ? 1 : 0;

    msgSpeedBrake.data[3] = 0;
    msgSpeedBrake.data[4] = 0;
    msgSpeedBrake.data[5] = 0;
    msgSpeedBrake.data[6] = 0;
    msgSpeedBrake.data[7] = 0;

    sendFrame(msgSpeedBrake);
  }

  // --- Envoi RPM (ID 0x2) ---
  if (now - lastRpmTx >= RPM_PERIOD_MS) {
    lastRpmTx += RPM_PERIOD_MS;

    uint16_t rpm_raw = (uint16_t)analogRead(A5);  // 0..1023

    // Option : conversion en RPM réalistes (0..8000)
    const uint16_t RPM_MAX = 8000;
    uint16_t rpm = (uint32_t)rpm_raw * RPM_MAX / 1023;

    // Encodage :
    // data[0..1] = rpm (uint16, little-endian)
    msgRpm.data[0] = (uint8_t)(rpm & 0xFF);
    msgRpm.data[1] = (uint8_t)(rpm >> 8);

    msgRpm.data[2] = 0;
    msgRpm.data[3] = 0;
    msgRpm.data[4] = 0;
    msgRpm.data[5] = 0;
    msgRpm.data[6] = 0;
    msgRpm.data[7] = 0;

    sendFrame(msgRpm);
  }

  // --- Envoi température huile (ID 0x3) ---
  if (now - lastTempTx >= TEMP_PERIOD_MS) {
    lastTempTx += TEMP_PERIOD_MS;

    uint8_t tempOilC = (uint8_t)random(70, 131); // 70..130 °C

    // Encodage :
    // data[0] = tempOilC
    msgOilTemp.data[0] = tempOilC;

    msgOilTemp.data[1] = 0;
    msgOilTemp.data[2] = 0;
    msgOilTemp.data[3] = 0;
    msgOilTemp.data[4] = 0;
    msgOilTemp.data[5] = 0;
    msgOilTemp.data[6] = 0;
    msgOilTemp.data[7] = 0;

    sendFrame(msgOilTemp);

    // Debug optionnel
    Serial.print("OilTemp=");
    Serial.print(tempOilC);
    Serial.println("C");
  }
}
