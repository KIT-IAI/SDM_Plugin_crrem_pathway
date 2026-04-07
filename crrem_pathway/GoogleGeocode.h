#pragma once

namespace tinyxml2
{
  class XMLElement;
}

class GoogleGeocode
{
  public:
    struct AddressComponent
    {
      std::wstring m_LongName;
      std::wstring m_ShortName;
      std::vector<std::wstring> m_Types;

      bool hasType(const std::wstring& searchType);
    };

    struct Result
    {
      std::wstring m_FormatedAddress;
      IfcDB::Point m_Location;
      IfcDB::Point m_SouthWest;
      IfcDB::Point m_NorthEast;
      std::vector<std::wstring> m_Types;
      std::vector<AddressComponent> m_AddressComponents;
    };

  public:
    GoogleGeocode() {}
    GoogleGeocode(const std::wstring& request);
    GoogleGeocode(const IfcDB::Point& position);

    virtual ~GoogleGeocode() {}

  public:
    bool Request(const std::wstring& request);
    bool Request(const IfcDB::Point& position);

    std::vector<Result>& getResults() { return m_Results; }
    std::wstring getErrorMessage() { return m_errorMessage; }

    bool getResultByType(Result& result, const std::wstring& type);

    std::wstring getCountryShortName();
    void setLanguage(const std::wstring& language);

  private:
    std::vector<Result> m_Results;
    std::wstring m_errorMessage;
    std::wstring m_language = L"en";
};

class GoogleGeocodeParser
{
  public:
    GoogleGeocodeParser() {}
    virtual ~GoogleGeocodeParser() {}

  public:
    bool parse(std::stringstream& stream);
    std::vector<GoogleGeocode::Result> getResults() { return std::move(m_Results); }
    std::wstring getErrorMessage() { return m_errorMessage; }

  private:
    void parse_result(tinyxml2::XMLElement* pParentElement);
    void parse_address_component(tinyxml2::XMLElement* pParentElement, GoogleGeocode::Result& result);
    void parse_geometry(tinyxml2::XMLElement* pParentElement, GoogleGeocode::Result& result);
    void parse_bounds(tinyxml2::XMLElement* pParentElement, GoogleGeocode::Result& result);
    void parse_lat_lng(tinyxml2::XMLElement* pParentElement, IfcDB::Point& pos);

  private:
    std::vector<GoogleGeocode::Result> m_Results;
    std::wstring m_errorMessage;
};
