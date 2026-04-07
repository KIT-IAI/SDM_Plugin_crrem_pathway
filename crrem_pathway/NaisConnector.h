#pragma once

#include "CRREMData.h"

#include <string>
#include <vector>

struct sqlite3;

namespace sdm
{
  namespace plugin
  {
    class LiveLogInterface;
  }
}

class NaisConector
{
  public:
    NaisConector(sdm::plugin::LiveLogInterface* pLogger);
    ~NaisConector();

    bool connect();
    std::vector<std::string> getCountries(ValueType valueType);
    std::vector<std::string> getAssetLabels(ValueType valueType, const std::string& country);
    std::vector<double> getPathway(ValueType valueType, const std::string& country, const std::string& assetLabel, int startYear, int targetYear);

  private:
    std::string getTableNameFromVauleType(ValueType valueType);

  private:
    sqlite3* m_sqlite = nullptr;
    sdm::plugin::LiveLogInterface* m_pLogger = nullptr;
};