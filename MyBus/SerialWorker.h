#pragma once
#include <windows.h>
#include <vector>
#include <string>



class SerialWorker {

private:

	HANDLE hComm;
	double t_35 = 0;



protected:

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

public:


	//конструктор подключения
	SerialWorker(std::string portName, int speed = 9600) {
		std::wstring stemp = std::wstring(portName.begin(), portName.end());
		LPCWSTR sw = stemp.c_str();
		hComm = CreateFile(sw,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		DCB dcbSerialParams = { 0 };
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

		if (!GetCommState(hComm, &dcbSerialParams)) {
			throw;
		}

		//t_35 = 35000000 / 115200;
		if (speed > 19200)
		{
			t_35 = 1750;
		}
		else {
			t_35 = 35000000 / speed;
		}

		switch (speed)
		{
		case 115200: {
			dcbSerialParams.BaudRate = CBR_115200;
			break;
		}
		case 9600: {
			dcbSerialParams.BaudRate = CBR_9600;
			break;
		}
		default:
			dcbSerialParams.BaudRate = CBR_9600;
			break;
		}


		//dcbSerialParams.BaudRate = CBR_115200;
		dcbSerialParams.ByteSize = 8;
		dcbSerialParams.StopBits = ONESTOPBIT;
		dcbSerialParams.Parity = NOPARITY;

		if (!SetCommState(hComm, &dcbSerialParams)) {
			throw;
		}


	}


	//Чтение регистров
	std::vector<uint8_t>readRegisters(uint8_t slaveID, uint16_t startAddress, uint16_t quantity = 1) {

		if (!hComm)
			throw;

		//формирование пакета на чтение 
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


		//отправка запроса на чтение
		DWORD bytesWritten;
		BOOL result = WriteFile(hComm, packet.data(), packet.size(), &bytesWritten, NULL);




		if (!result || bytesWritten != packet.size()) {

		}





	}


	//Запись в регистр
	std::vector<uint8_t>writeRegisters() {






	}


};