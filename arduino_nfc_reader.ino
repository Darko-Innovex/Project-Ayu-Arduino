#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9
#define SS_PIN          10

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
}

void loop() {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    byte buffer[18];
    byte size = sizeof(buffer);

    // Authenticate with the default key (0xFFFFFFFFFFFF)
    byte sector = 1;  // Example: reading from sector 1
    byte block = 4;   // Example: block 4 within sector 1
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        return;
    }

    // Read the data from the block
    status = mfrc522.MIFARE_Read(block, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        return;
    }

    Serial.print("$");
    for (byte i = 0; i < 8; i++) {
        Serial.print(buffer[i], HEX);
    }
    Serial.print("#");

    // Halt PICC
    mfrc522.PICC_HaltA();
    
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();

    delay(100);
}
