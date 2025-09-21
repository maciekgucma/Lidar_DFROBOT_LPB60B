#include <SoftwareSerial.h>

// Definicja pinów dla SoftwareSerial
#define RX_PIN 36
#define TX_PIN 35

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Inicjalizacja SoftwareSerial na pinach D2 (RX) i D3 (TX)


// Ramka do odczytu adresu urządzenia
uint8_t requestFrame[] = {0x55, 0x05, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xAA};

void setup() {
  Serial.begin(115200); // Serial monitor do komunikacji z IDE
  mySerial.begin(115200); // SoftSerial dla komunikacji z urządzeniem

  Serial.println("Program startuje...");
  //Serial.println("Wysyłanie ramki do odczytu adresu urządzenia...");
  
  // Wysyłanie ramki
  //for (int i = 0; i < sizeof(requestFrame); i++) {
  //  mySerial.write(requestFrame[i]);
  //}
}

void loop() {
  delay(2000);
  Serial.println("Send Rqr");
  
  // Wysyłanie ramki
  for (int i = 0; i < sizeof(requestFrame); i++) {
    mySerial.write(requestFrame[i]);
  }

  // Sprawdzanie, czy są dostępne dane od urządzenia
  if (mySerial.available()) {
    Serial.println("Odebrano dane od urządzenia:");
    
    // Odbiór danych
    uint8_t response[8]; // Ramka odpowiedzi
    int index = 0;

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
/*
    // Sprawdzenie, czy ramka jest poprawna (nagłówek i stopka)
    if (response[0] == 0x55 && response[7] == 0xAA) {
      uint8_t deviceAddress = response[5]; // Adres urządzenia znajduje się w 6. bajcie ramki
      Serial.print("Adres urządzenia: ");
      Serial.println(deviceAddress, HEX);
    } else {
      Serial.println("Niepoprawna ramka!");
    }
*/
    delay(1000); // Opóźnienie przed kolejnym odczytem
  }
  
}