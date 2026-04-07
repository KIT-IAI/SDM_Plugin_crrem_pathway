#pragma once

#include <optional>
#include <string>
#include <vector>

enum ValueType { KWH, CO2 };
enum ScenarioType { CRREM_1_5, CREEM_2_0 };
enum EnergySource { Electricity, Gas, HeatingOil, DistrictHeating, Coal, RenewableElectricity, Biomass, Undefined };

struct EnergyData
{
  std::string type;
  EnergySource source = Electricity;
  double energyDemand = 0.0;
  double emissionFactor = 1.0;

  inline static double getDefaultEmissionFactor(EnergySource source)
  {
    switch (source)
    {
      case Electricity:          return 0.40;
      case Gas:                  return 0.20;
      case HeatingOil:           return 0.27;
      case DistrictHeating:      return 0.25;
      case Coal:                 return 0.40;
      case RenewableElectricity: return 0.05;
      case Biomass:              return 0.05;
      case Undefined:
      default:                   return 0.4;
    }
  }

  inline static std::string toString(EnergySource source)
  {
    switch (source)
    {
      case Electricity:          return "Electricity";
      case Gas:                  return "Gas";
      case HeatingOil:           return "Heating Oil";
      case DistrictHeating:      return "District Heating";
      case Coal:                 return "Coal";
      case RenewableElectricity: return "RenewableElectricity";
      case Biomass:              return "Biomass";
      case Undefined:
      default:                   return "Undefined";
    }

    return {};
  }
};

struct CRREMData
{
  int startYear  = 2020;
  int targetYear = 2050;
  std::string country;
  std::string buildingType;
  ScenarioType scenario = CRREM_1_5;
  ValueType valueType = KWH;
  std::vector<double> pathway;
  std::vector<double> resultPathway;
  std::optional<int> strandingYear;
  std::vector<EnergyData> energyDemands;

  double totalArea = 0.0;
  double annualValue = 0.0;

  double startValue = 0.0;
  double annualReductionRate = 0.0;
  int renewableEnergy = 0;

  void updateStartValue()
  {
    if (annualValue > 0.0 && totalArea > 0.0)
    {
      if (valueType == ValueType::KWH)
      {
        startValue = annualValue / totalArea;
      }
      else if (valueType == ValueType::CO2)
      {
        double totalCO2(0.0);

        for (const auto& energyData : energyDemands)
        {
          totalCO2 += energyData.energyDemand * energyData.emissionFactor;
        }

        startValue = totalCO2 / totalArea;
      }
    }
  }

  void updateAnnualValue()
  {
    if (valueType == ValueType::KWH)
    {
      double totalEnergy(0.0);

      for (const auto& energyData : energyDemands)
      {
        totalEnergy += energyData.energyDemand;
      }

      annualValue = totalEnergy;
    }
    else if (valueType == ValueType::CO2)
    {
      double totalCO2(0.0);

      for (const auto& energyData : energyDemands)
      {
        totalCO2 += energyData.energyDemand * energyData.emissionFactor;
      }

      annualValue = totalCO2;
    }
  }

  inline std::string getValueTypeString() const
  { 
    switch (valueType)
    {
      case KWH: return "kWh";
      case CO2: return "kgCO²";
    }

    return {};
  }

  inline std::string getScenarioTypeString() const
  {
    switch (scenario)
    {
      case CRREM_1_5: return "1,5°";
      case CREEM_2_0: return "2,0°";
    }

    return {};
  }
};
