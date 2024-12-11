#pragma once

class ModbusRegister {
public:
	unsigned int address; // Адрес регистра

	ModbusRegister(unsigned int addr) : address(addr) {}
	virtual ~ModbusRegister() = default; // Виртуальный деструктор

	virtual void printValue() const = 0; // Чисто виртуальная функция для вывода значения
};