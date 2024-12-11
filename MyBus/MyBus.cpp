// MyBus.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
//
//
//

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdint>

#include "ModbusRegisterList.h"
#include "RegisterTypes.h"


//функция записи в опредленный 1 регистр данных
void writeRegisters(HANDLE hComm);



//функция чтения 1 определённого регистра
void readRegisters(HANDLE hComm, uint8_t slaveID, uint16_t startAddress, uint16_t quantity);


//CRC
uint16_t calculateCRC(const std::vector<uint8_t>& data);


std::vector<unsigned char> readFromDevice(HANDLE hComm, size_t bytesToRead);


std::vector<uint16_t> convertToUint16(const std::vector<unsigned char>& input);


void printHex(const std::vector<unsigned char>& bytes);


int main()
{
	setlocale(LC_ALL, "ru");



	ModbusRegisterList registerList; // Создаем список регистров



	// Добавляем регистры разных типов
	registerList.addRegister(std::make_shared<UInt16Register>(1, 12)); // Целое число
	registerList.addRegister(std::make_shared<UInt8Register>(3, 8));
	registerList.addRegister(std::make_shared<UInt16Register>(2, 721)); // Целое число
	// Целое число
	//registerList.addRegister(std::make_shared<UInt8Register>(3, 8)); // Целое число

	// Вещественное число

	   // Печатаем регистры
	registerList.printRegisters();

	registerList.generateBody(1, 3);


	return 0;
	//HANDLE hComm = CreateFileA("COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	//if (hComm == INVALID_HANDLE_VALUE) {
	//	std::cerr << "Ошибка открытия порта" << std::endl;
	//	return 1;
	//}

	//// Настройка параметров порта
	//DCB dcbSerialParams = { 0 };
	//dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	//if (!GetCommState(hComm, &dcbSerialParams)) {
	//	std::cerr << "Ошибка получения состояния порта" << std::endl;
	//	CloseHandle(hComm);
	//	return 1;
	//}

	//dcbSerialParams.BaudRate = CBR_115200;
	//dcbSerialParams.ByteSize = 8;
	//dcbSerialParams.StopBits = ONESTOPBIT;
	//dcbSerialParams.Parity = NOPARITY;

	//if (!SetCommState(hComm, &dcbSerialParams)) {
	//	std::cerr << "Ошибка установки параметров порта" << std::endl;
	//	CloseHandle(hComm);
	//	return 1;
	//}

	//readRegisters(hComm, 84, 1, 1);
}


void readRegisters(HANDLE hComm, uint8_t slaveID, uint16_t startAddress, uint16_t quantity = 1)
{
	std::vector<uint8_t> packet;
	packet.push_back(slaveID);
	packet.push_back(3);
	packet.push_back((startAddress >> 8) & 0xFF);
	packet.push_back(startAddress & 0xFF);
	packet.push_back((quantity >> 8) & 0xFF);
	packet.push_back(quantity & 0xFF);
	uint16_t crc = calculateCRC(packet);
	packet.push_back(crc & 0xFF);
	packet.push_back((crc >> 8) & 0xFF);

	DWORD bytesWritten;
	BOOL result = WriteFile(hComm, packet.data(), packet.size(), &bytesWritten, NULL);
	if (!result || bytesWritten != packet.size()) {
		std::cerr << "Ошибка отправки данных" << std::endl;
	}
	std::vector<uint8_t> dts;

	const size_t bytesToRead = 7;
	auto receivedData = readFromDevice(hComm, bytesToRead);
	printHex(receivedData);



	//while (true) {
	//	// Чтение 13 байт (105 бит) из COM-порта
	//	const size_t bytesToRead = 1;
	//	auto receivedData = readFromDevice(hComm, bytesToRead);

	//	if (receivedData.size()) {
	//		dts.push_back(receivedData[0]);
	//	}
	//	else {


	//		
	//		std::cout << "Полученные данные: ";

	//		printHex(dts);
	//		std::vector<uint16_t> uint16Array = convertToUint16(receivedData);

	//		// Вывод массива uint16_t
	//	   /* std::cout << "Преобразованные данные (uint16_t): ";
	//		for (uint16_t val : uint16Array) {
	//			std::cout << std::dec << val << " ";
	//		}*/
	//		std::cout << std::endl;

	//		dts.clear();
	//	}
	//	// Преобразование в массив uint16_t

	//}

	//return packet;
}


uint16_t calculateCRC(const std::vector<uint8_t>& data) {
	uint16_t crc = 0xFFFF;
	for (auto byte : data) {
		crc ^= byte;
		for (int i = 0; i < 8; ++i) {
			if (crc & 0x0001) {
				crc = (crc >> 1) ^ 0xA001;
			}
			else {
				crc >>= 1;
			}
		}
	}
	return crc;
}


std::vector<unsigned char> readFromDevice(HANDLE hComm, size_t bytesToRead) {
	DWORD bytesRead;
	std::vector<unsigned char> buffer(bytesToRead);
	if (ReadFile(hComm, buffer.data(), bytesToRead, &bytesRead, NULL)) {

		buffer.resize(bytesRead); // Уменьшаем до фактического количества прочитанных байт

	}
	else {
		std::cerr << "Ошибка чтения из COM-порта" << std::endl;
	}
	return buffer;
}


void printHex(const std::vector<unsigned char>& bytes) {
	for (unsigned char byte : bytes) {
		std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << " ";
	}
	std::cout << std::dec << std::endl; // Сбрасываем формат на десятичный
}

std::vector<uint16_t> convertToUint16(const std::vector<unsigned char>& input) {
	std::vector<uint16_t> output;
	size_t count = input.size() / sizeof(uint16_t);
	output.resize(count);

	for (size_t i = 0; i < count; ++i) {
		// Объединяем два байта в uint16_t
		output[i] = static_cast<uint16_t>(input[2 * i]) | (static_cast<uint16_t>(input[2 * i + 1]) << 8);
	}

	return output;
}
