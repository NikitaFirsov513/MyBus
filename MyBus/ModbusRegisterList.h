#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <typeinfo>


#include "ModbusRegister.h"
#include "RegisterTypes.h"


#pragma once

class ModbusRegisterList {
private:
	std::vector<std::shared_ptr<ModbusRegister>> registers; // ¬ектор дл€ хранени€ регистров

public:

	void addRegister(std::shared_ptr<ModbusRegister> reg) {

		bool isAddresFind = false;
		auto newAddres = reg.get()->address;
		for (auto var : registers) {
			auto a = var.get()->address;
			if (a == newAddres)
				isAddresFind = true;
		}

		//такой адрес недопустим
		if (isAddresFind) {
			throw;
		}

		bool isInsert = false;

		if (registers.size() > 0)
			for (int i = 0; i < registers.size(); i++) {

				ModbusRegister* elem = dynamic_cast<ModbusRegister*>(registers[i].get());
				unsigned int addres = elem->address;
				if (newAddres < addres) {

					registers.insert(registers.begin() + i, reg);
					return;
				}

			}


		if (!isInsert)
			registers.push_back(reg);

	}

	void printRegisters() const {
		for (const auto& reg : registers) {
			std::cout << "Address: " << reg->address << ", Value: ";
			reg->printValue(); // ¬ызов функции дл€ вывода значени€
			std::cout << std::endl;
		}
	}


	std::vector<uint8_t> generateBody(uint16_t startAddress, uint16_t quantity = 1) {

		std::vector<uint8_t> body;


		if (quantity < 1 || startAddress < 1) {
			throw;
		}

		bool isFindIndex = false;
		size_t indexRegister{};

		for (int i = 0; i < registers.size(); i++) {
			UInt16Register* uint16Reg = dynamic_cast<UInt16Register*>(registers[i].get());
			unsigned int addres = uint16Reg->address;

			if (addres == startAddress) {
				indexRegister = i;
				isFindIndex = true;
				break;
			}

		}


		if (!isFindIndex || indexRegister + quantity - 1 > registers.size()) {
			throw;
		}


		for (size_t i = indexRegister; i <= indexRegister + quantity - 1; i++) {


			auto reg = registers[i].get();



			if (UInt16Register* uint16Reg = dynamic_cast<UInt16Register*>(reg)) {
				uint16_t value = uint16Reg->value;
				body.push_back((value >> 8) & 0xFF);
				body.push_back(value & 0xFF);
				continue;
			}

			if (UInt8Register* uint8Reg = dynamic_cast<UInt8Register*>(reg)) {
				uint8_t value = uint8Reg->value;
				body.push_back(value & 0xFF);
				continue;
			}

		}


		return body;
	}

	void changeList(uint16_t startAddress, uint16_t quantity = 1, std::vector<uint8_t> data = {}) {





		//начальные проверки
		if (startAddress < 0 || quantity == 0) {
			return;
		}


		//

		bool isFindIndex = false;
		size_t indexRegister{};

		for (int i = 0; i < registers.size(); i++) {
			UInt16Register* uint16Reg = dynamic_cast<UInt16Register*>(registers[i].get());
			unsigned int addres = uint16Reg->address;

			if (addres == startAddress) {
				indexRegister = i;
				isFindIndex = true;
				break;
			}

		}


		if (!isFindIndex) {
			return;
		}

		//начинаем перебирать массив


		//смотрим на тип регистра и достаем из вектора необходимое количество
		//преобразуем в необходимый тип
		//мен€ем значение 
		//убирем редактируем data(убираем лишнее)






	}


};