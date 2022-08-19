#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

#include <imgui_internal.h>
#include <Windows.h>
#include <vector>
#include <string>
#include "electrical.h"


TommyElectrical TFOhmsLaw, TFSeriesVoltage, TFSeriesResistance, TFParallelCurrent, TFParallelResistance;
TommyElectrical TFCapacitanceQV, TFCapacitorEnergy, TFACInductionMotor;
TommyElectrical TFInductance, TFCapacitance, TFImpedanceReactance, TFPowerfactor;
TommyElectrical TFThreePhaseNeutral, TFPythagoras, TFPowerfactorTrueOverApparent, TFReactivePowerFromApparent, TFLinePhase;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnAttach() override {
		//m_Image = std::make_shared<Walnut::Image>("../media/TommyElectricalCalculatorBackground.png");
	}
	virtual void OnUIRender() override
	{
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_DockSpace;
		ImGui::SetNextWindowClass(&window_class);

		ImGui::Begin("Cheat Sheet :)");

		ImGuiStyle& style = ImGui::GetStyle();

		style.Colors[ImGuiCol_ChildBg] = ImColor(25, 10, 50);
		ImGui::BeginChild("tommytech.uk", ImVec2(ImGui::GetContentRegionAvail().x, 50), true);
		if (ImGui::Button("tommytech.uk")) {
			ShellExecuteA(NULL, "open", "https://tommytech.uk/welcome", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::EndChild();

		style.Colors[ImGuiCol_ChildBg] = ImColor(25, 10, 50);
		ImGui::BeginChild("1st section",ImVec2(ImGui::GetContentRegionAvail().x,350),true);
		style.WindowPadding[0] = 10;//horizontal padding
		style.WindowPadding[1] = 10;//vertical padding
		style.FramePadding[0] = 7;
		style.FramePadding[1] = 2;

		style.Colors[ImGuiCol_ChildBg] = ImColor(30, 10, 25);
		ImGui::BeginChild("Ohms Law", ImVec2(500, 300), true);
		ImGui::Text("Ohms Law");
		ImGui::Text("V = I * R");
		static double Input1 = 0;
		static double Input2 = 0;
		static double Input3 = 0;
		ImGui::InputDouble("Voltage", &Input1, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Current", &Input2, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Resistance", &Input3, 0, 2, "%.2f", 0);
		TFOhmsLaw.calculateOhmsLaw(Input1, Input2, Input3);
		ImGui::Text("Voltage: %f V (volt)", TFOhmsLaw.Voltage);
		ImGui::Text("Current: %f A (ampere)", TFOhmsLaw.Current);
		ImGui::Text("Resistance: %f Ohm", TFOhmsLaw.Resistance);

		if (ImGui::Button("clearInputs")) {
			Input1 = 0;
			Input2 = 0;
			Input3 = 0;
		}

		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Link to Almost All Equations we need");
		ImGui::Text("http://www.physicslessons.com/equations.htm");
		if (ImGui::Button("Open Website(Almost all equations we need)")) {
			ShellExecuteA(NULL, "open", "http://www.physicslessons.com/equations.htm", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::EndChild();
		ImGui::EndChild();


		//SERIES CIRCUIT CALCULATIONS  <><><><><><><><><><><><><><><><><><><><><>
		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 10, 25);
		ImGui::BeginChild("2nd section", ImVec2(ImGui::GetContentRegionAvail().x, 350), true);
		ImGui::BeginChild("Series Circuit", ImVec2(500, 300), true);
		ImGui::Text("Series Circuit Calculation");
		if (ImGui::Button("Voltage/Current/Resistance"))
		{
			SeriesCircuitSelect++;
		}
		if (ImGui::Button("+Components"))
		{
			SeriesCircuitNum++;
		}
		if (ImGui::Button("-Components"))
		{
			SeriesCircuitNum--;
		}
		//main
		static double SeriesMainValue = 0;
		if (SeriesCircuitSelect % 3 == 0) {
			ImGui::InputDouble("Total Voltage", &SeriesMainValue, 1, 10, "%.2f", 0);
		}
		else if (SeriesCircuitSelect % 3 == 1) {
			ImGui::InputDouble("Total Current", &SeriesMainValue, 1, 10, "%.2f", 0);
		}
		else {
			ImGui::InputDouble("Total Resistance", &SeriesMainValue, 1, 10, "%.2f", 0);
		}
		//components
		for (int i = 0; i < SeriesCircuitNum; i++) { //Generate Components and LABELs for the list of components
			if (SeriesComponentValue.size() < SeriesCircuitNum) {
				SeriesComponentValue.push_back(0);
			}else if (SeriesComponentValue.size() > SeriesCircuitNum) {
				SeriesComponentValue.resize(SeriesCircuitNum);
			}
			std::string AppendNum = std::to_string(i);
			if (SeriesCircuitSelect % 3 == 0) {
				AppendNum.append("Voltage");
				ImGui::InputDouble(AppendNum.c_str(), &SeriesComponentValue[i], 1, 10, "%.2f", 0);
			}
			else if (SeriesCircuitSelect % 3 == 1) {
				AppendNum.append("Current");
				ImGui::InputDouble(AppendNum.c_str(), &SeriesComponentValue[i], 1, 10, "%.2f", 0);
			}
			else {
				AppendNum.append("Resistance");
				ImGui::InputDouble(AppendNum.c_str(), &SeriesComponentValue[i], 1, 10, "%.2f", 0);
			}
		}
		
		//===============
		
		if (SeriesCircuitSelect % 3 == 0) {
			TFSeriesVoltage.calculateSeriesVoltage(SeriesMainValue, SeriesComponentValue);
		}
		else if (SeriesCircuitSelect % 3 == 1) {
			//Current is same
			//I(total) = I(1) + 1(2) + 1(3)... + 1(n)
		}
		else {
			TFSeriesResistance.calculateSeriesResistance(SeriesMainValue, SeriesComponentValue);
		}
		//OUTPUT HERE
		ImGui::EndChild();
		ImGui::SameLine();
		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 10, 25);
		ImGui::BeginChildEx("Series Circuit Output",2, ImVec2(500, 300), true,1);
		if (SeriesCircuitSelect % 3 == 0) {
			if (TFSeriesVoltage.Voltage != 0) {
				seriesTmp = TFSeriesVoltage.Voltage;
			}
			ImGui::Text("Voltage: %f V (Volt)", seriesTmp);
			ImGui::Text("V(total) = V(1) + V(2) + V(3)... + V(n)");
		}
		else if (SeriesCircuitSelect % 3 == 1) {
			ImGui::Text("Current is equal throughout the circuit in series.");
			ImGui::Text("I(total) = I(1) = I(2) = I(3)... = I(n)");
		}
		else {
			if (TFSeriesResistance.Resistance != 0) {
				seriesTmp = TFSeriesResistance.Resistance;
			}
			ImGui::Text("Resistance: %f Ohm", seriesTmp);
			ImGui::Text("R(total) = R(1) + R(2) + R(3)... + R(n)");
		}

		if (ImGui::Button("clearInputs")) {
			SeriesMainValue = 0;
			SeriesCircuitNum = 0;
			SeriesComponentValue = empty;
		}

		ImGui::EndChild();
		ImGui::EndChild();

		//PARALLEL CIRCUIT CALCULATIONS <><><><><><><><><><><><><><><><><><><><><><><><><>
		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 10, 25);
		ImGui::BeginChild("3rd section", ImVec2(ImGui::GetContentRegionAvail().x, 350), true);
		ImGui::BeginChild("Parallel Circuit", ImVec2(500, 300), true);
		ImGui::Text("Parallel Circuit Calculation");
		if (ImGui::Button("Voltage/Current/Resistance"))
		{
			ParallelCircuitSelect++;
		}
		if (ImGui::Button("+Components"))
		{
			ParallelCircuitNum++;
		}
		if (ImGui::Button("-Components"))
		{
			ParallelCircuitNum--;
		}
		//main
		static double ParallelMainValue = 0;
		if (ParallelCircuitSelect % 3 == 0) {
			ImGui::InputDouble("Total Voltage", &ParallelMainValue, 1, 10, "%.2f", 0);
		}
		else if (ParallelCircuitSelect % 3 == 1) {
			ImGui::InputDouble("Total Current", &ParallelMainValue, 1, 10, "%.2f", 0);
		}
		else {
			ImGui::InputDouble("Total Resistance", &ParallelMainValue, 1, 10, "%.2f", 0);
		}

		//components
		for (int i = 0; i < ParallelCircuitNum; i++) { //Generate Components and LABELs for the list of components
			if (ParallelComponentValue.size() < ParallelCircuitNum) {
				ParallelComponentValue.push_back(0);
			}
			else if (ParallelComponentValue.size() > ParallelCircuitNum) {
				ParallelComponentValue.resize(ParallelCircuitNum);
			}
			std::string AppendNum = std::to_string(i);
			if (ParallelCircuitSelect % 3 == 0) {
				AppendNum.append("Voltage");
				ImGui::InputDouble(AppendNum.c_str(), &ParallelComponentValue[i], 1, 10, "%.2f", 0);
			}
			else if (ParallelCircuitSelect % 3 == 1) {
				AppendNum.append("Current");
				ImGui::InputDouble(AppendNum.c_str(), &ParallelComponentValue[i], 1, 10, "%.2f", 0);
			}
			else {
				AppendNum.append("Resistance");
				ImGui::InputDouble(AppendNum.c_str(), &ParallelComponentValue[i], 1, 10, "%.2f", 0);
			}
		}

		//===============

		if (ParallelCircuitSelect % 3 == 0) {
			//Voltage is same
		}
		else if (ParallelCircuitSelect % 3 == 1) {
			TFParallelCurrent.calculateParallelCurrent(ParallelMainValue, ParallelComponentValue);
		}
		else {
			TFParallelResistance.calculateParallelResistance(ParallelMainValue, ParallelComponentValue);
		}


		ImGui::EndChild();
		ImGui::SameLine();
		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 10, 25);
		ImGui::BeginChildEx("Parallel Circuit Output", 2, ImVec2(500, 300), true, 1);

		if (ParallelCircuitSelect % 3 == 0) {
			ImGui::Text("Voltage is equal throughout the circuit in parallel.");
			ImGui::Text("V(total) = V(1) = V(2) = V(3)... = V(n)");
		}
		else if (ParallelCircuitSelect % 3 == 1) {
			if (TFParallelCurrent.Current != 0) {
				parallelTmp = TFParallelCurrent.Current;
			}
			ImGui::Text("Current: %f A (Ampere)", parallelTmp);
			ImGui::Text("I(total) = I(1) + I(2) + I(3)... + I(n)");
		}
		else {
			if (TFParallelResistance.Resistance != 0) {
				parallelTmp = TFParallelResistance.Resistance;
			}
			ImGui::Text("Resistance: %f Ohm", parallelTmp);
			ImGui::Text("1/R(total) = 1/R(1) + 1/R(2) + 1/R(3)... + 1/R(n)");
		}

		if (ImGui::Button("clearInputs")) {
			ParallelMainValue = 0;
			ParallelCircuitNum = 0;
			ParallelComponentValue = empty;
		}

		ImGui::EndChild();
		ImGui::EndChild();

		//===================================================================================
		//Capacitance = Charge / Voltage >> Calculation

		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 10, 25);
		ImGui::BeginChild("4th section", ImVec2(ImGui::GetContentRegionAvail().x, 350), true);
		style.Colors[ImGuiCol_ChildBg] = ImColor(45, 35, 20);
		ImGui::BeginChild("Charge = Voltage * Capacitance", ImVec2(500, 300));

		ImGui::Text("Capacitance = Charge / Voltage");
		
		static double Input4 = 0;
		static double Input5 = 0;
		static double Input6 = 0;
		ImGui::InputDouble("Capacitance", &Input4, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Charge", &Input5, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Voltage", &Input6, 0, 2, "%.2f", 0);
		TFCapacitanceQV.calculateCapacitanceQV(Input4, Input5, Input6);
		ImGui::Text("Capacitance: %f F (Farads)", TFCapacitanceQV.Capacitance);
		ImGui::Text("Charge: %f Q (Coulomb)", TFCapacitanceQV.Charge);
		ImGui::Text("Voltage: %f V (Volt)", TFCapacitanceQV.Voltage);

		if (ImGui::Button("clearInputs")) {
			Input4 = 0; Input5 = 0; Input6 = 0;
		}

		ImGui::EndChild();
		ImGui::SameLine();
		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 40, 23);
		ImGui::BeginChild("Link to Capacitance Website", ImVec2(500, 300));
		ImGui::Text("https://byjus.com/physics/energy-stored-in-a-capacitor/");
		if (ImGui::Button("Open Website(Capacitance)")) {
			ShellExecuteA(NULL, "open", "https://byjus.com/physics/energy-stored-in-a-capacitor/", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::EndChild();

		ImGui::EndChild();

		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 10, 25);
		ImGui::BeginChild("5th section", ImVec2(ImGui::GetContentRegionAvail().x, 350), true);
		style.Colors[ImGuiCol_ChildBg] = ImColor(45, 35, 20);
		ImGui::BeginChild("Energy stored in a capacitor", ImVec2(500, 300));

		ImGui::Text("Energy stored in a capacitor");
		ImGui::Text("U = 1/2(C * V * V)");

		static double Input7 = 0;
		static double Input8 = 0;
		static double Input9 = 0;
		ImGui::InputDouble("Energy", &Input7, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Capacitance", &Input8, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Voltage", &Input9, 0, 2, "%.2f", 0);
		TFCapacitorEnergy.calculateEnergyInCapacitor(Input7, Input8, Input9);
		ImGui::Text("Energy: %f J (Joule)", TFCapacitorEnergy.Energy);
		ImGui::Text("Capacitance: %f F (Farads)", TFCapacitorEnergy.Capacitance);
		ImGui::Text("Voltage: %f V (Volt)", TFCapacitorEnergy.Voltage);

		if (ImGui::Button("clearInputs")) {
			Input7 = 0; Input8 = 0; Input9 = 0;
		}

		ImGui::EndChild();
		ImGui::EndChild();

		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 10, 25);
		ImGui::BeginChild("6th section", ImVec2(ImGui::GetContentRegionAvail().x, 350), true);
		style.Colors[ImGuiCol_ChildBg] = ImColor(45, 35, 20);
		ImGui::BeginChild("AC Induction Motor", ImVec2(700, 300));
		
		ImGui::Text("AC Induction Motor (RPM, Frequency & Poles)");
		ImGui::Text("n(synchoronous speed in RPM) = 120 x Frequency / Number of Poles");
		
		static double Input10 = 0;
		static double Input11 = 0;
		static double Input12 = 0;
		ImGui::InputDouble("Synchoronous Speed(n)", &Input10, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Frequency", &Input11, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Number of Poles", &Input12, 0, 2, "%.2f", 0);
		TFACInductionMotor.calculateACInductionMotor(Input10, Input11, Input12);
		ImGui::Text("Synchoronous Speed(n): %f RPM (Revolutions per minute)", TFACInductionMotor.RPM);
		ImGui::Text("Frequency: %f Hz (Hertz)", TFACInductionMotor.Frequency);
		ImGui::Text("Number of Poles: %f ", TFACInductionMotor.NumOfPoles);

		if (ImGui::Button("clearInputs")) {
			Input10 = 0;Input11 = 0;Input12 = 0;
		}

		ImGui::EndChild();
		ImGui::SameLine();
		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 40, 23);
		ImGui::BeginChild("Link to relevant website", ImVec2(500, 300));
		ImGui::Text("https://getcalc.com/electrical-speed-frequency-poles-calculator.htm");
		if (ImGui::Button("Open Website(electrical-speed-frequency-poles-calculator)")) {
			ShellExecuteA(NULL, "open", "https://getcalc.com/electrical-speed-frequency-poles-calculator.htm", NULL, NULL, SW_SHOWNORMAL);
		}
		ImGui::EndChild();
		ImGui::EndChild();
		
		//ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });

		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 10, 25);
		ImGui::BeginChild("7th section", ImVec2(ImGui::GetContentRegionAvail().x, 350), true);
		style.Colors[ImGuiCol_ChildBg] = ImColor(45, 35, 20);
		ImGui::BeginChild("Inductance", ImVec2(600, 300));

		ImGui::Text("Calculate Inductance & Inductive Reactance");
		ImGui::Text("Xl = 2 * PI * F * L");
		static double Input13 = 0;
		static double Input14 = 0;
		static double Input15 = 0;
		ImGui::InputDouble("Frequency", &Input13, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Inductance", &Input14, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Reactance", &Input15, 0, 2, "%.2f", 0);
		TFInductance.calculateInductance(Input13, Input14, Input15);
		ImGui::Text("Frequency(F): %f Herz", TFInductance.Frequency);
		ImGui::Text("Inductance(L): %f Henry", TFInductance.Inductance);
		ImGui::Text("InductiveReactance(Xl): %f Ohm", TFInductance.InductiveReactance);

		if (ImGui::Button("clearInputs")) {
			Input13 = 0;
			Input14 = 0;
			Input15 = 0;
		}

		ImGui::EndChild();
		ImGui::SameLine();
		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 40, 23);
		ImGui::BeginChild("Capacitance", ImVec2(600, 300));

		ImGui::Text("Calculate Capacitance & Capacitive Reactance");
		ImGui::Text("Xc = 1 / (2 * PI * F * C)");
		static double Input16 = 0;
		static double Input17 = 0;
		static double Input18 = 0;
		ImGui::InputDouble("Frequency", &Input16, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Capacitance", &Input17, 0, 2, "%.6f", 0);
		ImGui::InputDouble("Reactance", &Input18, 0, 2, "%.3f", 0);
		TFCapacitance.calculateCapacitance(Input16, Input17, Input18);
		ImGui::Text("Frequency(F): %f Herz", TFCapacitance.Frequency);
		ImGui::Text("Capacitance(C): %f Farad", TFCapacitance.Capacitance);
		ImGui::Text("CapacitiveReactance(Xl): %f Ohm", TFCapacitance.CapacitiveReactance);

		if (ImGui::Button("clearInputs")) {
			Input16 = 0;
			Input17 = 0;
			Input18 = 0;
		}

		ImGui::EndChild();
		ImGui::EndChild();

		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 10, 25);
		ImGui::BeginChild("8th section", ImVec2(ImGui::GetContentRegionAvail().x, 700), true);
		style.Colors[ImGuiCol_ChildBg] = ImColor(45, 35, 20);
		ImGui::BeginChild("Impedance from Reactance", ImVec2(600, 300));

		ImGui::Text("Calculate Impedance, Reactance & Resistance in Circuit");
		ImGui::Text("Z^2 = R^2 + X^2  Impedance from Resistance & Reactance in a circuit (X is Reactance, the difference between Inductive and Capacitive Reactance)");
		static double Input19 = 0;
		static double Input20 = 0;
		static double Input21 = 0;
		static double Input22 = 0;
		ImGui::InputDouble("Impedance", &Input19, 0, 2, "%.3f", 0);
		ImGui::InputDouble("Resistance", &Input20, 0, 2, "%.3f", 0);
		ImGui::InputDouble("Inductive Reactance", &Input21, 0, 2, "%.6f", 0);
		ImGui::InputDouble("Capacitive Reactance", &Input22, 0, 2, "%.6f", 0);
		TFImpedanceReactance.calcImpedanceFromReactance(Input19, Input20, Input21, Input22);
		ImGui::Text("Impedance(Z): %f Ohm", TFImpedanceReactance.Impedance);
		ImGui::Text("Resistance(R): %f Ohm", TFImpedanceReactance.Resistance);
		ImGui::Text("Reactance(X): %f Ohm", TFImpedanceReactance.Reactance);

		if (ImGui::Button("clearInputs")) {
			Input19 = 0;
			Input20 = 0;
			Input21 = 0;
			Input22 = 0;
		}

		ImGui::EndChild();

		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 40, 23);
		ImGui::BeginChild("Power Factor", ImVec2(600, 370));

		ImGui::Text("Calculate Power Factor");
		ImGui::Text("Powerfactor = Resistance / Impedance");
		ImGui::Text("Inverse Cosine of Powerfactor to get the Phase Angle.");
		ImGui::Text("Low power factor leads to power loss, voltage drop, not economical,");
		ImGui::Text("also requires larger cables because of volt drop.");
		static double Input23 = 0;
		static double Input24 = 0;
		static double Input25 = 0;
		static double Input26 = 0;
		ImGui::InputDouble("Power Factor", &Input23, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Phase Angle", &Input24, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Resistance", &Input25, 0, 2, "%.4f", 0);
		ImGui::InputDouble("Impedance", &Input26, 0, 2, "%.4f", 0);
		TFPowerfactor.calcPowerfactorFromResistance(Input23, Input24, Input25, Input26);
		ImGui::Text("Power Factor: %f ", TFPowerfactor.Powerfactor);
		ImGui::Text("Phase Angle: %f Degrees", TFPowerfactor.PhaseAngle);
		ImGui::Text("Resistance(R): %f Ohm", TFPowerfactor.Resistance);
		ImGui::Text("Impedance(Z): %f Ohm", TFPowerfactor.Impedance);

		if (ImGui::Button("clearInputs")) {
			Input23 = 0;
			Input24 = 0;
			Input25 = 0;
			Input26 = 0;
		}

		ImGui::EndChild();
		ImGui::EndChild();

		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 10, 25);
		ImGui::BeginChild("9th section", ImVec2(ImGui::GetContentRegionAvail().x, 350), true);
		style.Colors[ImGuiCol_ChildBg] = ImColor(45, 35, 20);
		ImGui::BeginChild("Three Phase Neutral", ImVec2(600, 320));
		ImGui::Text("Calculate Three Phase Neutral Current");
		ImGui::Text("N = sqrt(L1^2 + L2^2 + L3^2 - (L1*L2) - (L1*L3) - (L2*L3))");
		ImGui::Text("Its quadratic nature when calculating L1,L2,L3 creates 2 different results.");
		static double Input27 = 0;
		static double Input28 = 0;
		static double Input29 = 0;
		static double Input30 = 0;
		ImGui::InputDouble("Neutral", &Input27, 0, 2, "%.2f", 0);
		ImGui::InputDouble("L1", &Input28, 0, 2, "%.2f", 0);
		ImGui::InputDouble("L2", &Input29, 0, 2, "%.2f", 0);
		ImGui::InputDouble("L3", &Input30, 0, 2, "%.2f", 0);
		TFThreePhaseNeutral.calcThreePhaseNeutral(Input27, Input28, Input29, Input30);
		ImGui::Text("Neutral: %f A", TFThreePhaseNeutral.Neutral);
		if (TFThreePhaseNeutral.L1q == 0) {
			ImGui::Text("L1: %f A", TFThreePhaseNeutral.L1);
		}else {
			ImGui::Text("L1: %f A / %f A", TFThreePhaseNeutral.L1, TFThreePhaseNeutral.L1q);
		}
		if (TFThreePhaseNeutral.L2q == 0) {
			ImGui::Text("L2: %f A", TFThreePhaseNeutral.L2);
		}else {
			ImGui::Text("L2: %f A / %f A", TFThreePhaseNeutral.L2, TFThreePhaseNeutral.L2q);
		}
		if (TFThreePhaseNeutral.L3q == 0) {
			ImGui::Text("L3: %f A", TFThreePhaseNeutral.L3);
		}else {
			ImGui::Text("L3: %f A / %f A", TFThreePhaseNeutral.L3, TFThreePhaseNeutral.L3q);
		}
		if (ImGui::Button("clearInputs")) {
			Input27 = 0;
			Input28 = 0;
			Input29 = 0;
			Input30 = 0;
		}
		ImGui::EndChild();
		ImGui::EndChild();

		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 10, 25);
		ImGui::BeginChild("10th section", ImVec2(ImGui::GetContentRegionAvail().x, 700), true);
		style.Colors[ImGuiCol_ChildBg] = ImColor(45, 35, 20);
		ImGui::BeginChild("Pythagoras Theorem", ImVec2(600, 320));
		ImGui::Text("Pythagoras Theorem");
		ImGui::Text("Power Triangle: ApparentPower^2 = TruePower^2 + ReactivePower^2");
		ImGui::Text("Impedance Triangle: Impedance^2 = Resistance^2 + Net Reactance^2");
		ImGui::Text("Voltage Triangle: Voltage(S)^2 = Voltage(X)^2 + Voltage(R)^2");
		static double Input31 = 0;
		static double Input32 = 0;
		static double Input33 = 0;
		ImGui::InputDouble("Hypotenuse", &Input31, 0, 2, "%.4f", 0);
		ImGui::InputDouble("Leg 1", &Input32, 0, 2, "%.4f", 0);
		ImGui::InputDouble("Leg 2", &Input33, 0, 2, "%.4f", 0);
		TFPythagoras.PythagorasTheorem(Input31, Input32, Input33);
		ImGui::Text("Hypotenuse: %f", TFPythagoras.Hypotenuse);
		ImGui::Text("Leg 1: %f", TFPythagoras.Angle1);
		ImGui::Text("Leg 2: %f", TFPythagoras.Angle2);
		if (ImGui::Button("clearInputs")) {
			Input31 = 0;
			Input32 = 0;
			Input33 = 0;
		}
		ImGui::EndChild();
		ImGui::SameLine();
		style.Colors[ImGuiCol_ChildBg] = ImColor(45, 35, 20);
		ImGui::BeginChild("LinePhaseVoltage", ImVec2(600, 320));
		ImGui::Text("Phase Voltage = Line Voltage / sqrt(3)");
		static double Input40 = 0;
		static double Input41 = 0;
		ImGui::InputDouble("Phase Voltage", &Input40, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Line Voltage", &Input41, 0, 2, "%.2f", 0);
		TFLinePhase.calcLinePhaseVoltage(Input40, Input41);
		ImGui::Text("Phase Voltage: %f V", TFLinePhase.PhaseVoltage);
		ImGui::Text("Line Voltage: %f V", TFLinePhase.LineVoltage);
		if (ImGui::Button("clearInputs")) {
			Input40 = 0;
			Input41 = 0;
		}
		ImGui::EndChild();
		style.Colors[ImGuiCol_ChildBg] = ImColor(45, 35, 20);
		ImGui::BeginChild("Powerfactor True Over Apparent", ImVec2(600, 320));
		ImGui::Text("Power factor = True power / Apparent power");
		static double Input34 = 0;
		static double Input35 = 0;
		static double Input36 = 0;
		ImGui::InputDouble("Power Factor", &Input34, 0, 2, "%.2f", 0);
		ImGui::InputDouble("True Power", &Input35, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Apparent Power", &Input36, 0, 2, "%.2f", 0);
		TFPowerfactorTrueOverApparent.calcPowerfactorTrueOverApparent(Input34,Input35,Input36);
		ImGui::Text("Power Factor: %f", TFPowerfactorTrueOverApparent.Powerfactor);
		ImGui::Text("True Power: %f kW", TFPowerfactorTrueOverApparent.Truepower);
		ImGui::Text("Apparent Power: %f kVA", TFPowerfactorTrueOverApparent.Apparentpower);
		if (ImGui::Button("clearInputs")) {
			Input34 = 0;
			Input35 = 0;
			Input36 = 0;
		}
		ImGui::EndChild();
		ImGui::SameLine();
		style.Colors[ImGuiCol_ChildBg] = ImColor(45, 35, 20);
		ImGui::BeginChild("ReactiveApparentTruePower", ImVec2(600, 320));
		ImGui::Text("ReactivePower = ApparentPower * sin(PhaseAngle)");
		static double Input37 = 0;
		static double Input38 = 0;
		static double Input39 = 0;
		ImGui::InputDouble("Reactive Power", &Input37, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Apparent Power", &Input38, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Phase Angle", &Input39, 0, 2, "%.2f", 0);
		TFReactivePowerFromApparent.calcReactivepowerFromApparent(Input37, Input38, Input39);
		ImGui::Text("Reactive Power: %f kVAr", TFReactivePowerFromApparent.Reactivepower);
		ImGui::Text("Apparent Power: %f kVA", TFReactivePowerFromApparent.Apparentpower);
		ImGui::Text("Phase Angle: %f degrees", TFReactivePowerFromApparent.PhaseAngle);
		if (ImGui::Button("clearInputs")) {
			Input37 = 0;
			Input38 = 0;
			Input39 = 0;
		}
		ImGui::EndChild();
		ImGui::EndChild();
		ImGui::End();
	}

private:
	std::shared_ptr<Walnut::Image> m_Image;

	std::vector<double> empty;

	// for series circuit
	int SeriesCircuitSelect = 0;
	int SeriesCircuitNum = 0;
	std::vector<double> SeriesComponentValue;
	double seriesTmp = 0;

	int ParallelCircuitSelect = 0;
	int ParallelCircuitNum = 0;
	std::vector<double> ParallelComponentValue;
	double parallelTmp = 0;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Tommy's Electrical App";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	/*
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	*/
	return app;
}