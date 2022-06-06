#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

#include <Windows.h>
#include <vector>
#include <string>
#include "electrical.h"
#include <imgui_internal.h>

TommyElectrical TFOhmsLaw, TFSeriesVoltage, TFSeriesResistance, TFParallelCurrent, TFParallelResistance;
TommyElectrical TFCapacitanceQV, TFCapacitorEnergy;

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

		ImGui::EndChild();
		ImGui::EndChild();

		
		//ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });

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