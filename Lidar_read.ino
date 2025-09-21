#include <HardwareSerial.h>

// Inicjalizacja UART (HardwareSerial) na pinach 35 (RX) i 36 (TX)
HardwareSerial sensorSerial(1); // UART1 (ESP32 obsługuje wiele portów UART)

void setup() {
  Serial.begin(115200);                  // UART0: komunikacja z komputerem
  sensorSerial.begin(115200, SERIAL_8N1, 35, 36); // UART1: komunikacja z dalmierzem

  Serial.println("ESP32: Program startuje...");
}

void loop() {
  // Komenda wyzwalająca pomiar odległości
  byte cmd[] = {0x55, 0x05, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xAA};
  sensorSerial.write(cmd, sizeof(cmd)); // Wysyłanie komendy
  Serial.println("Komenda wysłana do dalmierza");
  delay(50);

  // Sprawdzanie, czy jest dostępna odpowiedź
  if (sensorSerial.available() >= 8) {
    byte resp[8];
    for (int i = 0; i < 8; i++) {
      resp[i] = sensorSerial.read();
    }

    // Parsowanie i wyświetlanie odległości
    uint32_t distance = (resp[3] << 16) | (resp[4] << 8) | resp[5]; // Bajty 3, 4, 5
    Serial.print("Odczytana odległość: ");
    Serial.print(distance);
    Serial.println(" mm");
    //Serial.print("Odebrana ramka: ");
    //for (int i = 0; i < index; i++) {
    //  Serial.print(resp[i], HEX);
    //  Serial.print(" ");
    //}
  } else {
    Serial.println("Brak odpowiedzi od dalmierza.");
  }

  delay(1000); // Czekaj 1 sekundę przed kolejnym pomiarem
}