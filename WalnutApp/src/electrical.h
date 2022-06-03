#pragma once
#include <iostream>
#include <vector>
#include <limits>

class TommyElectrical {
private:
	


public:

	int Position = 0;

	double Voltage = 0;
	double Current = 0;
	double Resistance = 0;

	std::vector<double> Voltages;
	std::vector<double> Currents;
	std::vector<double> Resistances;
	std::vector<double> Empty;

	void calculateOhmsLaw(double voltage_in, double current_in, double resistance_in) {
		Voltage = voltage_in;
		Current = current_in;
		Resistance = resistance_in;

		double result;

		if (Voltage == 0) {
			Voltage = Current * Resistance;
		}
		else if (Current == 0) {
			Current = Voltage / Resistance;
		}
		else {
			Resistance = Voltage / Current;
		}
	}

	void calculateSeriesVoltage(double TotalValue, std::vector<double> Value_in) {
		Voltage = 0;
		if (TotalValue == 0) {
			for (int i = 0; i < Value_in.size(); i++) {
				Voltage += Value_in[i]; //in series, voltage goes +++++++
			}
		}
		else {
			for (int i = 0; i < Value_in.size(); i++) {
				if (Value_in[i] == 0) {
					for (int j = 0; j < Value_in.size(); j++) {
						Voltage += Value_in[j];
					}
					Voltage = TotalValue - Voltage;
				}
			}
		}
	}

	void calculateSeriesResistance(double TotalValue, std::vector<double> Value_in) {
		Resistance = 0;
		if (TotalValue == 0) {
			for (int i = 0; i < Value_in.size(); i++) {
				Resistance += Value_in[i]; //in series, voltage goes +++++++
			}
		}
		else {
			for (int i = 0; i < Value_in.size(); i++) {
				if (Value_in[i] == 0) {
					for (int j = 0; j < Value_in.size(); j++) {
						Resistance += Value_in[j];
					}
					Resistance = TotalValue - Resistance;
				}
			}
		}
	}



};