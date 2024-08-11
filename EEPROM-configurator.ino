#include <EEPROM.h>

const int MAX_ADDRESSES = 512;  // Adjust as needed
int notEmptyAddressCount = 0;
int notEmptyAddressesArr[MAX_ADDRESSES];  // Array to store non-empty addresses

void setup() {
  Serial.begin(9600);

  // Wait for the serial connection to establish
  while (!Serial) {}

  Serial.print("EEPROM Size: ");
  Serial.println(EEPROM.length());

  // Read and print all address-value pairs
  for (int i = 0; i < EEPROM.length(); i++) {
    byte value = EEPROM.read(i);  // Read as byte
    Serial.print("Address ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(value, HEX);  // Print in hexadecimal format

    if (value != 0xFF) {
      if (notEmptyAddressCount < MAX_ADDRESSES) {
        notEmptyAddressesArr[notEmptyAddressCount++] = i;  // Store the address
      }
    }
  }

  // Print the count of non-empty addresses
  if (notEmptyAddressCount != 0) {
    Serial.print("Not empty address count: ");
    Serial.println(notEmptyAddressCount);

    // Print all non-empty addresses
    Serial.print("Not empty addresses: ");
    for (int i = 0; i < notEmptyAddressCount; i++) {
      Serial.print(notEmptyAddressesArr[i]);
      if (i < notEmptyAddressCount - 1) {
        Serial.print(", ");
      }
    }
    Serial.println();
    Serial.println("Choose an option:\nDelete all non-empty values - A\nDelete a value based on its address - Y\nDo nothing - N");
  } else {
    Serial.println("No non-empty addresses found.");
  }
}

void deleteAllData() {
  for (int i = 0; i < notEmptyAddressCount; i++) {
    EEPROM.write(notEmptyAddressesArr[i], 0xFF);
  }
  Serial.println("All non-empty values deleted");
}

void deleteDataByAddress() {
  Serial.println("Press enter after each address!\nInput 000 to quit deletion!");

  while (true) {
    Serial.println("Address to delete its value --> ");

    while (Serial.available() == 0) {
      delay(100);
    }
    String userAddress = Serial.readStringUntil('\n');
    userAddress.trim();

    if (userAddress == "000") {
      Serial.println("Exiting...");
      break;
    }
    
    int address = userAddress.toInt();
    if (address >= 0 && address < EEPROM.length()) {
      EEPROM.write(address, 0xFF);
    } else {
      Serial.print("Invalid address. Please enter a number between 0 and ");
      Serial.print(EEPROM.length() - 1);
      Serial.println(".");
    }
    Serial.println("Address to delete its value --> ");
  }
}

void loop() {
  if (Serial.available() > 0) {
    String userChoice = Serial.readStringUntil('\n');
    userChoice.trim();
    userChoice.toUpperCase();

    if (userChoice == "A") {
      deleteAllData();
    } else if (userChoice == "Y") {
      deleteDataByAddress();
    } else if (userChoice == "N") {
      Serial.println("No action taken.");
    } else {
      Serial.println("Invalid choice. Please enter A, Y, or N.");
    }
  }
}
