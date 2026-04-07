#include "NaisConnector.h"
#include <sqlite3.h>
#include <exception>
#include <filesystem>
#include <set>
#include <LiveLogFeature.hpp>
#include <stdutils/executable.hpp>

std::string quoteWord(const std::string& str)
{
  return "'" + str + "'";
}

std::string transform(sqlite3_stmt* stmt, int index)
{
  auto data = sqlite3_column_text(stmt, index);

  if (data)
  {
    return std::string(reinterpret_cast<const char*>(data));
  }

  return {};
}

NaisConector::NaisConector(sdm::plugin::LiveLogInterface* pLogger)
  : m_pLogger(pLogger)
{
}

NaisConector::~NaisConector()
{
  sqlite3_close(m_sqlite);
}

bool NaisConector::connect()
{
  try
  {
    const auto executableDir = stdutils::findExecutableFileName();
    const auto dbPath = executableDir.parent_path() / "plugins" / "crrem_pathway" / "NaiS.db";

    if (!std::filesystem::exists(dbPath))
    {
      m_pLogger->log(sdm::plugin::MessageLevel::Error, "CRREMPathway", "Database missing: " + dbPath.string());

      return false;
    }

    int returnCode = sqlite3_open_v2(dbPath.string().c_str(), &m_sqlite, SQLITE_OPEN_READONLY, nullptr);

    if (returnCode != SQLITE_OK)
    {
      m_pLogger->log(sdm::plugin::MessageLevel::Error, "CRREMPathway", sqlite3_errmsg(m_sqlite));

      return false;
    }

    return true;
  }
  catch (const std::exception exception)
  {
    m_pLogger->log(sdm::plugin::MessageLevel::Error, "CRREMPathway", exception.what());
  }

  return false;
}

std::vector<std::string> NaisConector::getCountries(ValueType valueType)
{
  std::vector<std::string> vCountries;
  std::set<std::string> countries;

  std::string table = getTableNameFromVauleType(valueType);

  std::string sentence = "SELECT country FROM " + table + " WHERE id NOTNULL";
  const char* sqlSentence = sentence.c_str();
  sqlite3_stmt* stmt = nullptr;

  int result = sqlite3_prepare_v2(m_sqlite, sqlSentence, -1, &stmt, NULL);
  if (result == SQLITE_OK)
  {
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      auto asset = transform(stmt, 0);
      countries.insert(asset);
    }
  }
  else
  {
    m_pLogger->log(sdm::plugin::MessageLevel::Error, "CRREMPathway", sqlite3_errmsg(m_sqlite));
  }

  sqlite3_finalize(stmt);

  std::copy(countries.begin(), countries.end(), std::back_inserter(vCountries));

  return vCountries;
}

std::vector<std::string> NaisConector::getAssetLabels(ValueType valueType, const std::string& country)
{
  std::vector<std::string> vAssets;
  std::set<std::string> assets;

  std::string table = getTableNameFromVauleType(valueType);

  std::string sentence = "SELECT * FROM " + table + " WHERE id NOTNULL";
  sentence += " AND country IS " + quoteWord(country);
  const char* sqlSentence = sentence.c_str();
  sqlite3_stmt* stmt = nullptr;

  int result = sqlite3_prepare_v2(m_sqlite, sqlSentence, -1, &stmt, NULL);
  if (result == SQLITE_OK)
  {
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      auto asset = transform(stmt, 3);
      assets.insert(asset);
    }
  }
  else
  {
    m_pLogger->log(sdm::plugin::MessageLevel::Error, "CRREMPathway", sqlite3_errmsg(m_sqlite));
  }

  sqlite3_finalize(stmt);

  std::copy(assets.begin(), assets.end(), std::back_inserter(vAssets));

  return vAssets;
}

std::vector<double> NaisConector::getPathway(ValueType valueType, const std::string& country, const std::string& assetLabel, int startYear, int targetYear)
{
  std::vector<double> pathway;

  std::string table = getTableNameFromVauleType(valueType);

  std::string sentence = "SELECT value FROM " + table + " WHERE id NOTNULL";
  sentence += " AND country IS " + quoteWord(country);
  sentence += " AND asset_label IS " + quoteWord(assetLabel);
  sentence += " AND year >= " + std::to_string(startYear) + " AND year <= " + std::to_string(targetYear);

  sqlite3_stmt* stmt = nullptr;
  const char* sqlSentence = sentence.c_str();

  int result = sqlite3_prepare_v2(m_sqlite, sqlSentence, -1, &stmt, NULL);
  if (result == SQLITE_OK)
  {
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      double value = std::stod(transform(stmt, 0));
      pathway.emplace_back(value);
    }
  }
  else
  {
    m_pLogger->log(sdm::plugin::MessageLevel::Error, "CRREMPathway", sqlite3_errmsg(m_sqlite));
  }

  sqlite3_finalize(stmt);

  return pathway;
}

std::string NaisConector::getTableNameFromVauleType(ValueType valueType)
{
  if (valueType == KWH)
  {
    return "crrem_1_5C_kwh";
  }
  else if (valueType == CO2)
  {
    return "crrem_1_5C_co2";
  }

  assert(0);

  return {};
}
