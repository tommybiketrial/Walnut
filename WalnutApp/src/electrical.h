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
	double Capacitance = 0;
	double Charge = 0;
	double Energy = 0;

	std::vector<double> Voltages;
	std::vector<double> Currents;
	std::vector<double> Resistances;
	std::vector<double> Empty;

	void calculateOhmsLaw(double voltage_in, double current_in, double resistance_in) {
		Voltage = voltage_in;
		Current = current_in;
		Resistance = resistance_in;

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
			ImGui::Text("Calculating Total");
			for (int i = 0; i < Value_in.size(); i++) {
				Voltage += Value_in[i]; //in series, voltage goes +++++++
			}
		}
		else {
			ImGui::Text("Calculating Component");
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
			ImGui::Text("Calculating Total");
			for (int i = 0; i < Value_in.size(); i++) {
				Resistance += Value_in[i]; //in series, resistance goes +++++++
			}
		}
		else {
			ImGui::Text("Calculating Component");
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

	void calculateParallelCurrent(double TotalValue, std::vector<double> Value_in) {
		Current = 0;
		if (TotalValue == 0) {
			ImGui::Text("Calculating Total");
			for (int i = 0; i < Value_in.size(); i++) {
				Current += Value_in[i]; //in parallel, current goes +++++++
			}
		}
		else {
			ImGui::Text("Calculating Component");
			for (int i = 0; i < Value_in.size(); i++) {
				if (Value_in[i] == 0) {
					for (int j = 0; j < Value_in.size(); j++) {
						Current += Value_in[j];
					}
					Current = TotalValue - Current;
				}
			}
		}
	}

	void calculateParallelResistance(double TotalValue, std::vector<double> Value_in) {
		Resistance = 0;
		double sum = 0;
		if (TotalValue == 0) {
			ImGui::Text("Calculating Total");
			for (int i = 0; i < Value_in.size(); i++) {
				if (Value_in[i] != 0) {
					Resistance += 1 / Value_in[i];
				}
				//in parallel, resistance goes inverse ++... 1/R(total) = 1/R(1) + 1/R(2) + 1/R(3)... + 1/R(n)
			}
			Resistance = 1 / Resistance;
		}
		else {
			ImGui::Text("Calculating Component");
			for (int i = 0; i < Value_in.size(); i++) {
				if (Value_in[i] == 0) {
					for (int j = 0; j < Value_in.size(); j++) {
						if (Value_in[j] != 0) {
							sum += 1 / Value_in[j];
						}
					}
				}
			}
			Resistance = 1 / ((1 / TotalValue) - sum);
		}
	}

	void calculateCapacitanceQV(double Capacitance_in, double Charge_in, double Voltage_in) {
		Capacitance = Capacitance_in;
		Charge = Charge_in;
		Voltage = Voltage_in;
		//C = Q/V  ~~  Q = VC ~~  V = Q/C
		if (Capacitance == 0) {
			Capacitance = Charge / Voltage;
		}
		else if (Charge == 0) {
			Charge = Voltage * Capacitance;
		}
		else {
			Voltage = Charge / Capacitance;
		}
	}

	void calculateEnergyInCapacitor(double Energy_in, double Capacitance_in, double Voltage_in) {
		Energy = Energy_in;
		Capacitance = Capacitance_in;
		Voltage = Voltage_in;
		//energy stored in a capacitor formula U = 1/2C(V^2)
		//https://calctool.org/CALC/eng/electronics/capacitor_energy
		if (Energy == 0) {
			Energy = (Capacitance * Voltage * Voltage)/2;
		}
		else if (Capacitance == 0) {
			Capacitance = Energy / (Voltage * Voltage) * 2; //C = U / (V * V) * 2
		}
		else {
			Voltage = sqrt(Energy * 2 / Capacitance); // V = sqrt((U * 2) / C)
		}
	}
	


};