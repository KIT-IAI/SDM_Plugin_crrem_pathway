#pragma once

#include <ifcdb/IfcDbInclude.h>
#include <ifcdb/utils/PopulationObserver.hpp>
#include <unordered_map>
#include "CRREMData.h"

namespace sdm
{
	namespace plugin
	{
		class LiveLogInterface;
	}
}

class CRREMPathwayDlgImpl;

class CRREMPathway
{
  public:
    CRREMPathway(IfcDB::Populationi* pDB, IfcDB::utils::PopulationStates* pStates, sdm::plugin::LiveLogInterface* pLogger, CRREMPathwayDlgImpl* pDlg);
    ~CRREMPathway() = default;

    void setLogger(sdm::plugin::LiveLogInterface* pLogger) { m_pLogger = pLogger; }
    void setCRREMPathwayDlg(CRREMPathwayDlgImpl* pDlg) { m_pCRREMPathwayDlg = pDlg; }
    CRREMPathwayDlgImpl* getCRREMPathwayDlg() { return m_pCRREMPathwayDlg; }

    CRREMData calculate();

    std::vector<double> getPathway(const std::string& country, const std::string& buildingType, ScenarioType scenario, ValueType value, int startYear, int targetYear);

  private:
    std::vector<double> linearPath(int startYear, int targetYear, double startValue, double targetValue);
    std::vector<double> exponentialPath(int startYear, int targetYear, double startValue, double annualReductionRate);
    std::optional<int> calculateStrandingYear(const std::vector<double>& crremPath, const std::vector<double>& actualPath, int startYear);

  private:
    IfcDB::Populationi* m_pDB = nullptr;
		IfcDB::utils::PopulationStates* m_pStates = nullptr;
		sdm::plugin::LiveLogInterface* m_pLogger = nullptr;

    CRREMPathwayDlgImpl* m_pCRREMPathwayDlg = nullptr;
};
