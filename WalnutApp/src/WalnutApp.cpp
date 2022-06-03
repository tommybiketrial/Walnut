#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

#include <vector>
#include <string>
#include "electrical.h"
#include <imgui_internal.h>

TommyElectrical TFOhmsLaw, TFSeriesVoltage, TFSeriesResistance, TFParallelCurrent, TFParallelResistance;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnAttach() override {
		m_Image = std::make_shared<Walnut::Image>("../media/TommyElectricalCalculatorBackground.png");
	}
	virtual void OnUIRender() override
	{
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
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
		static double Input1 = 0;
		static double Input2 = 0;
		static double Input3 = 0;
		ImGui::InputDouble("Voltage", &Input1, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Current", &Input2, 0, 2, "%.2f", 0);
		ImGui::InputDouble("Resistance", &Input3, 0, 2, "%.2f", 0);
		TFOhmsLaw.calculateOhmsLaw(Input1, Input2, Input3);
		ImGui::Text("Voltage: %f", TFOhmsLaw.Voltage);
		ImGui::Text("Current: %f", TFOhmsLaw.Current);
		ImGui::Text("Resistance: %f", TFOhmsLaw.Resistance);
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
			SeriesComponentValue.push_back(0);
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
			ImGui::Text("Voltage: %f", seriesTmp);
			ImGui::Text("V(total) = V(1) + V(2) + V(3)... + V(n)");
		}
		else if (SeriesCircuitSelect % 3 == 1) {
			ImGui::Text("Current is equal throughout the circuit in series.");
			ImGui::Text("I(total) = I(1) = 1(2) = 1(3)... = 1(n)");
		}
		else {
			if (TFSeriesResistance.Resistance != 0) {
				seriesTmp = TFSeriesResistance.Resistance;
			}
			ImGui::Text("Resistance: %f", seriesTmp);
			ImGui::Text("R(total) = R(1) + R(2) + R(3)... + R(n)");
		}
		ImGui::EndChild();
		ImGui::EndChild();

		//PARALLEL CIRCUIT CALCULATIONS <><><><><><><><><><><><><><><><><><><><><><><><><>
		style.Colors[ImGuiCol_ChildBg] = ImColor(40, 10, 25);
		ImGui::BeginChild("3rd section", ImVec2(ImGui::GetContentRegionAvail().x, 350), true);
		
		ImGui::EndChild();


		
		ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });

		ImGui::End();
	}

private:
	std::shared_ptr<Walnut::Image> m_Image;

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