#pragma once
#include <iostream>
#include <vector>
#include <limits>
#include <math.h>

constexpr double PI = 3.1415926535897932385;

class TommyElectrical {
private:
	


public:

	double Voltage = 0;
	double Current = 0;
	double Resistance = 0;
	double Impedance = 0;
	double Capacitance = 0;
	double Inductance = 0;
	double CapacitiveReactance = 0;
	double InductiveReactance = 0;
	double Reactance = 0;
	double Powerfactor = 0;
	double Charge = 0;
	double Energy = 0;
	double RPM = 0;
	double Frequency = 0;
	double NumOfPoles = 0;

	double Angle = 0;

	double Neutral = 0;
	double L1 = 0;
	double L2 = 0;
	double L3 = 0;
	double L1q = 0;
	double L2q = 0;
	double L3q = 0;
	double Ans1 = 0;
	double Ans2 = 0;

	std::vector<double> Voltages;
	std::vector<double> Currents;
	std::vector<double> Resistances;
	std::vector<double> Empty;

	void clearObj() {
		this->Voltage = 0;
		this->Current = 0;
		this->Resistance = 0;
		this->Impedance = 0;
		this->Capacitance = 0;
		this->Inductance = 0;
		this->CapacitiveReactance = 0;
		this->InductiveReactance = 0;
		this->Reactance = 0;
		this->Powerfactor = 0;
		this->Charge = 0;
		this->Energy = 0;
		this->RPM = 0;
		this->Frequency = 0;
		this->NumOfPoles = 0;

		this->Angle = 0;

		this->Neutral = 0;
		this->L1 = 0;
		this->L2 = 0;
		this->L3 = 0;
		this->L1q = 0;
		this->L2q = 0;
		this->L3q = 0;
		this->Ans1 = 0;
		this->Ans2 = 0;

		this->Voltages = Empty;
		this->Currents = Empty;
		this->Resistances = Empty;
	}

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
	
	void calculateACInductionMotor(double RPM_in, double Frequency_in, double NumOfPoles_in) {
		RPM = RPM_in;
		Frequency = Frequency_in;
		NumOfPoles = NumOfPoles_in;

		if (RPM == 0) {
			RPM = 120 * Frequency / NumOfPoles;
		}
		else if (Frequency == 0) {
			Frequency = NumOfPoles * RPM / 120;
		}
		else {
			NumOfPoles = 120 * Frequency / RPM;
		}
	}

	void calculateInductance(double Frequency_in, double Inductance_in, double Reactance_in) {
		Frequency = Frequency_in;
		Inductance = Inductance_in;
		InductiveReactance = Reactance_in;

		if (InductiveReactance == 0) {
			calcInductiveReactance(Frequency,Inductance);
		}else if(Inductance == 0) {
			calcInductanceFromReactance(Frequency, InductiveReactance);
		}else {
			calcFrequencyFromInductance(Inductance, InductiveReactance);
		}
	}
	void calculateCapacitance(double Frequency_in, double Capacitance_in, double Reactance_in) {
		Frequency = Frequency_in;
		Capacitance = Capacitance_in;
		CapacitiveReactance = Reactance_in;

		if (CapacitiveReactance == 0) {
			calcCapacitiveReactance(Frequency, Capacitance);
		}else if (Capacitance == 0) {
			calcCapacitanceFromReactance(Frequency, CapacitiveReactance);
		}else {
			calcFrequencyFromCapacitance(Capacitance, CapacitiveReactance);
		}
	}

	void calcInductiveReactance(double Frequency_in, double Inductance_in) {
		InductiveReactance = 2 * PI * Frequency_in * Inductance_in; //Xl = 2 * PI * F * L
	}
	void calcInductanceFromReactance(double Frequency_in, double Reactance_in) {
		Inductance = Reactance_in / (2 * PI * Frequency_in); // L = Xl / (2 * PI * F)
	}
	void calcFrequencyFromInductance(double Inductance_in, double Reactance_in) {
		Frequency = Reactance_in / (2 * PI * Inductance_in); //F = Xl / (2 * PI * L) Frequency from Inductance & Inductive Reactance
	}
	
	void calcCapacitiveReactance(double Frequency_in, double Capacitance_in) {
		CapacitiveReactance = 1 / (2 * 3.1415926535897932385 * Frequency_in * Capacitance_in); //Xc = 1 / (2 * PI * F * C)
	}
	void calcCapacitanceFromReactance(double Frequency_in, double Reactance_in) {
		Capacitance = 1 / (2 * PI * Frequency_in * Reactance_in); // C = 1 / (2 * PI * F * Xc)
	}
	void calcFrequencyFromCapacitance(double Capacitance_in, double Reactance_in) {
		Frequency = 1 / (2 * PI * Capacitance_in * Reactance_in); //F = 1 / (2 * PI * C * Xc) Frequency from Capacitance & Capacitive Reactance
	}

	void calcResultingCurrentFlow(double Voltage_in, double Reactance_in) {
		Current = Voltage_in / Reactance_in; // V = I * R  ~  Xc is resistance (well or Xl) ~ V = I * Xc
	}

	void calcImpedanceFromReactance(double Impedance_in, double Resistance_in, double InductiveReactance_in, double CapacitiveReactance_in) {
		double TmpReactance = 0;
		if (InductiveReactance_in > CapacitiveReactance_in) {
			TmpReactance = InductiveReactance_in - CapacitiveReactance_in;
		}else if (CapacitiveReactance_in > InductiveReactance_in) {
			TmpReactance = CapacitiveReactance_in - InductiveReactance_in;
		}

		Impedance = Impedance_in; // z = sqrt(R^2 + (diff(Xl,Xc))^2)
		Resistance = Resistance_in;

		if (Impedance == 0) {
			Impedance = sqrt((Resistance * Resistance) + (TmpReactance * TmpReactance));
		}else if (Resistance == 0) {
			Resistance = sqrt((TmpReactance * TmpReactance) - (Impedance * Impedance));
		}else {
			Reactance = sqrt((Impedance * Impedance) - (Resistance * Resistance));
		}
	}

	void calcPowerfactor(double Powerfactor_in, double Angle_in, double Resistance_in, double Impedance_in) {
		Powerfactor = Powerfactor_in;
		Angle = Angle_in;
		Resistance = Resistance_in;
		Impedance = Impedance_in;
		if (Angle_in != 0) {
			Powerfactor = cos(Angle*PI/180);
		}

		if (Powerfactor == 0) {
			Powerfactor = Resistance / Impedance;
		}else if(Resistance == 0) {
			Resistance = Powerfactor * Impedance;
		}else {
			Impedance = Resistance / Powerfactor;
		}
		Angle = acos(Powerfactor)*(180.0 / PI);
	}

	void getL1fromL2L3Neutral(double N, double L2_in, double L3_in) {
		Ans1 = (L2_in + L3_in + sqrt((-3) * L2_in * L2_in - 3 * L3_in * L3_in + 6 * L2_in * L3_in + 4 * N * N)) / 2;
		Ans2 = (L2_in + L3_in - sqrt((-3) * L2_in * L2_in - 3 * L3_in * L3_in + 6 * L2_in * L3_in + 4 * N * N)) / 2;
	}

	void calcThreePhaseNeutral(double Neutral_in, double L1_in, double L2_in, double L3_in) {
		Neutral = Neutral_in;
		if (L1_in == 0) {
			getL1fromL2L3Neutral(Neutral, L2_in, L3_in);
			L2 = L2_in;
			L3 = L3_in;
			L1 = Ans1;
			L1q = Ans2;
			L2q = L3q = 0;
		}
		else if (L2_in == 0) {
			getL1fromL2L3Neutral(Neutral, L1_in, L3_in);
			L1 = L1_in;
			L3 = L3_in;
			L2 = Ans1;
			L2q = Ans2;
			L1q = L3q = 0;
		}
		else if (L3_in == 0) {
			getL1fromL2L3Neutral(Neutral, L1_in, L2_in);
			L1 = L1_in;
			L2 = L2_in;
			L3 = Ans1;
			L3q = Ans2;
			L1q = L2q = 0;
		}
		else {
			L1 = L1_in;
			L2 = L2_in;
			L3 = L3_in;
			L1q = L2q = L3q = 0;
			Neutral = sqrt(L1 * L1 + L2 * L2 + L3 * L3 - (L1 * L2) - (L1 * L3) - (L2 * L3));
		}
		//N = sqrt(L1^2 + L2^2 + L3^2 - (L1*L2) - (L1*L3) - (L2*L3))
		//sqrt everything that the lines multiply themselves and deduct those multiplying the other lines.
		//L1 = (L2 + L3 (+ or -) sqrt((-3)*L2*L2-3*L3*L3+6*L2*L3+4*Neutral*Neutral))/2
	}
};