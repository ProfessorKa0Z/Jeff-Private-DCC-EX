/*
  This is a library to read/write to external I2C EEPROMs.
  It uses the same template system found in the Arduino
  EEPROM library so you can use the same get() and put() functions.

  https://github.com/sparkfun/SparkFun_External_EEPROM_Arduino_Library
  Best used with the Qwiic EEPROM: https://www.sparkfun.com/products/18355

  Various external EEPROMs have various interface specs
  (overall size, page size, write times, etc). This library works with
  all types and allows the various settings to be set at runtime.

  All read and write restrictions associated with pages are taken care of.
  You can access the external memory as if it was contiguous.

  Development environment specifics:
  Arduino IDE 1.8.x

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*/

#include "exEEPROM.h"

ExternalEEPROM EEPROM;

bool ExternalEEPROM::begin(uint8_t deviceAddress)
{
   
    settings.deviceAddress = deviceAddress;

    if (isConnected() == false)
    {
        return false;
    }
    return true;
}


uint32_t ExternalEEPROM::length()
{
    return settings.memorySize_bytes;
}

// Returns true if device is detected
bool ExternalEEPROM::isConnected()
{
   return I2CManager.exists(settings.deviceAddress);
}

// Returns true if device is not answering (currently writing)
// Caller can pass in an I2C address. This is helpful for larger EEPROMs that have two addresses (see block bit 2).
bool ExternalEEPROM::isBusy()
{
    if (isConnected())
        return (false);
    return (true);
}

void ExternalEEPROM::setAddressBytes(uint8_t addressBytes)
{
    settings.addressSize_bytes = addressBytes;
}

void ExternalEEPROM::setMemorySizeBytes(uint32_t memSize)
{
    settings.memorySize_bytes = memSize;

    //Try to identify this memory size settings
    switch (memSize)
    {
    default:
        // Unknown memory size
        break;
    case (16):
        setAddressBytes(1);
        setPageSizeBytes(1);
        break;
    case (128):
        setAddressBytes(1);
        setPageSizeBytes(8);
        break;
    case (256):
        setAddressBytes(1);
        setPageSizeBytes(8);
        break;
    case (512):
        setAddressBytes(1);
        setPageSizeBytes(16);
        break;
    case (1024):
        setAddressBytes(1);
        setPageSizeBytes(16);
        break;
    case (2048):
        setAddressBytes(1);
        setPageSizeBytes(16);
        break;
    case (4096):
        setAddressBytes(2);
        setPageSizeBytes(32);
        break;
    case (8192):
        setAddressBytes(2);
        setPageSizeBytes(32);
        break;
    case (16384):
        setAddressBytes(2);
        setPageSizeBytes(64);
        break;
    case (32768):
        setAddressBytes(2);
        setPageSizeBytes(64);
        break;
    case (65536):
        setAddressBytes(2);
        setPageSizeBytes(128);
        break;
    case (128000):
        setAddressBytes(2);
        setPageSizeBytes(128);
        break;
    case (262144):
        setAddressBytes(2);
        setPageSizeBytes(256);
        break;
    }
}

void ExternalEEPROM::setMemoryType(uint16_t typeNumber)
{
    //Set settings based on known memory types
    switch (typeNumber)
    {
    default:
        // Unknown type number
        break;
    case (0):
        setMemorySizeBytes(16);
        break;
    case (1):
        setMemorySizeBytes(128 * (uint32_t)typeNumber); //128
        break;
    case (2):
        setMemorySizeBytes(128 * (uint32_t)typeNumber); //256
        break;
    case (4):
        setMemorySizeBytes(128 * (uint32_t)typeNumber); //512
        break;
    case (8):
        setMemorySizeBytes(128 * (uint32_t)typeNumber); //1024
        break;
    case (16):
        setMemorySizeBytes(128 * (uint32_t)typeNumber); //2048
        break;
    case (32):
        setMemorySizeBytes(128 * (uint32_t)typeNumber); //4096
        break;
    case (64):
        setMemorySizeBytes(128 * (uint32_t)typeNumber); //8192
        break;
    case (128):
        setMemorySizeBytes(128 * (uint32_t)typeNumber); //16384
        break;
    case (256):
        setMemorySizeBytes(128 * (uint32_t)typeNumber); //32768
        break;
    case (512):
        setMemorySizeBytes(128 * (uint32_t)typeNumber); //65536
        break;
    case (1025):
        setMemorySizeBytes(128000); //128000
        break;
    case (2048):
        setMemorySizeBytes(262144); //262144
        break;
    }
}

void ExternalEEPROM::setPageSizeBytes(uint16_t pageSize) 
{
    settings.pageSize_bytes = pageSize;
}


// Read a byte from a given location
uint8_t ExternalEEPROM::read(uint32_t eepromLocation)
{
    uint8_t tempByte;
    read(eepromLocation, &tempByte, 1);
    return tempByte;
}

// Bulk read from EEPROM
// Handles breaking up read amt into 32 byte chunks (can be overriden with setI2CBufferSize)
// Handles a read that straddles the 512kbit barrier
int ExternalEEPROM::read(uint32_t eepromLocation, uint8_t *buff, uint16_t bufferSize)
{
    int result = 0;

    uint16_t received = 0;
    while (received < bufferSize)
    {
        // Limit the amount to write to a page size
        uint16_t amtToRead = bufferSize - received;
        if (amtToRead > I2C_BUFFER_LENGTH_RX) // Arduino I2C buffer size limit
            amtToRead = I2C_BUFFER_LENGTH_RX;

        // Check if we are dealing with large (>512kbit) EEPROMs
        uint8_t i2cAddress = settings.deviceAddress;
        if (settings.memorySize_bytes > 0xFFFF)
        {
            // Figure out if we are going to cross the barrier with this read
            if (eepromLocation + received < 0xFFFF)
            {
                if (0xFFFF - (eepromLocation + received) < amtToRead) // 0xFFFF - 0xFFFA < 32
                    amtToRead =
                        0xFFFF - (eepromLocation + received); // Limit the read amt to go right up to edge of barrier
            }

            // Figure out if we are accessing the lower half or the upper half
            if (eepromLocation + received > 0xFFFF)
                i2cAddress |= 0b100; // Set the block bit to 1
        }
        // Check if we are dealing with 24LC04/08/16 (512, 1024, and 2048 bytes)
        // These use a single address byte but change the I2C address
        else if (settings.memorySize_bytes >= 512 && settings.memorySize_bytes <= 2048)
        {
            // Set I2C Address bits (A2/A1/A0) accordingly
            i2cAddress |= ((eepromLocation + received) >> 8);
        }

        // See if EEPROM is available or still writing a previous request
        while (isBusy() == true) // Poll device's original address, not the modified one
            delay(1); // changed from 100 microseconds to 1 millisecond to reduce I2C bus load

        uint8_t readBuffer[amtToRead];
        
        result = I2CManager.read(i2cAddress, readBuffer, amtToRead, 2, (eepromLocation + received) >> 8, (eepromLocation + received) & 0xFF);

/*
        settings.i2cPort->beginTransmission(i2cAddress);
        if (settings.addressSize_bytes > 1)
            settings.i2cPort->write((uint8_t)((eepromLocation + received) >> 8)); // MSB
        settings.i2cPort->write((uint8_t)((eepromLocation + received) & 0xFF));   // LSB

        result = settings.i2cPort->endTransmission();
*/

       // settings.i2cPort->requestFrom((uint8_t)i2cAddress, (size_t)amtToRead);

        for (uint16_t x = 0; x < amtToRead; x++)
            buff[received + x] = readBuffer[x];

        received += amtToRead;
    }

    return (result);
}

// Write a byte to a given location
int ExternalEEPROM::write(uint32_t eepromLocation, uint8_t dataToWrite)
{
    if (read(eepromLocation) != dataToWrite) // Update only if data is new
        return (write(eepromLocation, &dataToWrite, 1));
    return (0);
}

// Write large bulk amounts
// Limits writes to the I2C buffer size (default is 32 bytes)
// Returns the result of the I2C endTransmission
int ExternalEEPROM::write(uint32_t eepromLocation, const uint8_t *dataToWrite, uint16_t bufferSize)
{
    int result = 0;

    // Error check
    if (eepromLocation + bufferSize >= settings.memorySize_bytes)
        bufferSize = settings.memorySize_bytes - eepromLocation;

    // Serial.print("bufferSize: ");
    // Serial.println(bufferSize);

    int16_t maxWriteSize = settings.pageSize_bytes;
    if (maxWriteSize > I2C_BUFFER_LENGTH_TX - settings.addressSize_bytes)
        maxWriteSize =
            I2C_BUFFER_LENGTH_TX -
            settings.addressSize_bytes; // Arduino has 32 byte limit. We loose 1 or 2 bytes to the EEPROM address

    // Serial.print("maxWriteSize: ");
    // Serial.println(maxWriteSize);

    // Break the buffer into page sized chunks
    uint16_t recorded = 0;
    while (recorded < bufferSize)
    {
        // Limit the amount to write to either the page size or the Arduino limit of 30
        int amtToWrite = bufferSize - recorded;

        // Serial.print("amtToWrite: ");
        // Serial.println(amtToWrite);

        if (amtToWrite > maxWriteSize)
            amtToWrite = maxWriteSize;

        // Serial.print("amtToWrite: ");
        // Serial.println(amtToWrite);

        if (amtToWrite > 1)
        {
            // Check for crossing of a page line. Writes cannot cross a page line.
            uint16_t pageNumber1 = (eepromLocation + recorded) / settings.pageSize_bytes;
            uint16_t pageNumber2 = (eepromLocation + recorded + amtToWrite - 1) / settings.pageSize_bytes;
            if (pageNumber2 > pageNumber1)
                amtToWrite = ((pageNumber1 + 1) * settings.pageSize_bytes) -
                             (eepromLocation + recorded); // Limit the write amt to go right up to edge of page barrier
        }

        uint8_t i2cAddress = settings.deviceAddress;
        // Check if we are dealing with large (>512kbit) EEPROMs
        // These use two address bytes and a B0 'block' bit
        if (settings.memorySize_bytes > 0xFFFF)
        {
            // Figure out if we are accessing the lower half or the upper half
            if (eepromLocation + recorded > 0xFFFF)
                i2cAddress |= 0b100; // Set the block bit to 1
        }
        // Check if we are dealing with 24LC04/08/16 (512, 1024, and 2048 bytes)
        // These use a single address byte but change the I2C address
        else if (settings.memorySize_bytes >= 512 && settings.memorySize_bytes <= 2048)
        {
            // Set I2C Address bits (A2/A1/A0) accordingly
            i2cAddress |= ((eepromLocation + recorded) >> 8);
        }

      

        // See if EEPROM is available or still writing a previous request
        while (isBusy() == true) // Poll device's original address, not the modified one
            delay(1); // changed from 100 microseconds to 1 millisecond to reduce I2C bus load

        uint8_t writeBuffer[2 + amtToWrite]; 
        
        writeBuffer[0] = (uint8_t)((eepromLocation + recorded) >> 8); // MSB
        writeBuffer[1] = (uint8_t)((eepromLocation + recorded) & 0xFF);   // LSB
        for (uint16_t x = 0; x < amtToWrite; x++) 
            writeBuffer[2 + x] = dataToWrite[recorded + x];
        result = I2CManager.write(i2cAddress, writeBuffer, 2 + amtToWrite);
/*
        settings.i2cPort->beginTransmission(i2cAddress);
        if (settings.addressSize_bytes > 1) // Device larger than 16,384 bits have two byte addresses
            settings.i2cPort->write((uint8_t)((eepromLocation + recorded) >> 8)); // MSB
        settings.i2cPort->write((uint8_t)((eepromLocation + recorded) & 0xFF));   // LSB
*/
        

        recorded += amtToWrite;
    }

    return (result);
}