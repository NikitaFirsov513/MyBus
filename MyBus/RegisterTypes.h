#include <iostream>
#include <string>

#include "ModbusRegister.h"

#pragma once



class IntegerRegister : public ModbusRegister {
public:
	int value;

	IntegerRegister(unsigned int addr, int val) : ModbusRegister(addr), value(val) {}

	void printValue() const override {
		std::cout << value;
	}
};

class UInt8Register : public ModbusRegister {
public:
	uint8_t value;

	UInt8Register(unsigned int addr, uint8_t val) : ModbusRegister(addr), value(val) {}

	void printValue() const override {
		std::cout << std::to_string(value);
	}
};

class UInt16Register : public ModbusRegister {
public:
	uint16_t value;

	UInt16Register(unsigned int addr, uint16_t val) : ModbusRegister(addr), value(val) {}

	void printValue() const override {
		std::cout << std::to_string(value);
	}
};

class UInt32Register : public ModbusRegister {
public:
	uint32_t value;

	UInt32Register(unsigned int addr, uint32_t val) : ModbusRegister(addr), value(val) {}

	void printValue() const override {
		std::cout << std::to_string(value);
	}
};

// Класс для вещественных чисел
class FloatRegister : public ModbusRegister {
public:
	float value;

	FloatRegister(unsigned int addr, float val) : ModbusRegister(addr), value(val) {}

	void printValue() const override {
		std::cout << value;
	}
};
