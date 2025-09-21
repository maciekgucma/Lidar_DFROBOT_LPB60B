#include <SoftwareSerial.h>

// Definicja pinów dla SoftwareSerial
#define RX_PIN 2
#define TX_PIN 3

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Inicjalizacja SoftwareSerial na pinach D2 (RX) i D3 (TX)

// Ramka do pojedynczego pomiaru
uint8_t requestFrame[] = {0x55, 0x05, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xAA};

// Funkcja obliczająca CRC-8 dla odebranych danych (zgodnie z protokołem)
uint8_t calculateCRC(uint8_t *data, uint8_t len) {
  uint8_t crc = 0x00; // Początkowa wartość CRC
  for (uint8_t i = 0; i < len; i++) {
    crc ^= data[i]; // XOR z kolejnym bajtem danych
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31; // Jeśli najstarszy bit jest ustawiony, XOR z 0x31
      } else {
        crc = (crc << 1); // W przeciwnym razie przesunięcie w lewo
      }
    }
  }
  return crc;
}

void setup() {
  Serial.begin(115200); // Serial monitor do komunikacji z IDE
  mySerial.begin(115200); // SoftSerial dla komunikacji z urządzeniem

  Serial.println("Program startuje...");
}

void loop() {
  delay(500);
  Serial.println("Wysyłanie ramki...");

  // Wysyłanie ramki
  for (int i = 0; i < sizeof(requestFrame); i++) {
    mySerial.write(requestFrame[i]);
  }

  // Sprawdzanie, czy są dostępne dane od urządzenia
  if (mySerial.available()) {
    Serial.println("Odebrano dane od urządzenia:");

    uint8_t response[8]; // Bufor na ramkę
    int index = 0;

    // Odbiór pełnej ramki (8 bajtów)
    while (mySerial.available() && index < 8) {
      response[index++] = mySerial.read();
    }

    // Wyświetlenie odebranej ramki
    Serial.print("Odebrana ramka: ");
    for (int i = 0; i < index; i++) {
      Serial.print(response[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // Sprawdzenie poprawności ramki
    if (response[0] == 0x55 && response[7] == 0xAA) {
      // Obliczanie CRC dla bajtów 2-6 (zgodnie z protokołem)
      uint8_t calculatedCRC = calculateCRC(&response[1], 5);
      if (calculatedCRC == response[6]) {
        Serial.println("CRC poprawne!");

        // Sprawdzanie kodu błędu
        uint8_t errorCode = response[2];
        switch (errorCode) {
          case 0x00:
            Serial.println("System normalny.");
            // Dekodowanie odległości (3 bajty w polu Value)
            uint32_t distance = (response[3] << 16) | (response[4] << 8) | response[5];
            Serial.print("Odległość: ");
            Serial.print(distance);
            Serial.println(" mm");
            break;
          case 0x01:
            Serial.println("Błąd: Sygnał za słaby.");
            break;
          case 0x02:
            Serial.println("Błąd: Sygnał za silny.");
            break;
          case 0x03:
            Serial.println("Błąd: Poza zakresem.");
            break;
          case 0x04:
            Serial.println("Błąd: Błąd systemu.");
            break;
          default:
            Serial.println("Nieznany kod błędu.");
            break;
        }
      } else {
        Serial.println("CRC niepoprawne!");
      }
    } else {
      Serial.println("Niepoprawna ramka!");
    }

    delay(500); // Opóźnienie przed kolejnym odczytem
  }
}