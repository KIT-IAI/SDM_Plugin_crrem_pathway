#include "StdAfx.h"

#include <stdutils/utf8.hpp>
#include <curl/curl.h>

#include "GoogleGeocode.h"
#include <tinyxml2.h>

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

static const std::string GoogleApiKey("");

static size_t onWrite(char* ptr, size_t size, size_t nmemb, void* userdata)
{
  std::ostringstream& temp = *static_cast<std::ostringstream*>(userdata);

  temp.write(ptr, size * nmemb);

  return size * nmemb;
}

std::string url_encode(const std::string& decoded)
{
  const auto encoded_value = curl_easy_escape(nullptr, decoded.c_str(), static_cast<int>(decoded.length()));
  std::string result(encoded_value);
  curl_free(encoded_value);
  return result;
}

GoogleGeocode::GoogleGeocode(const std::wstring& request)
{
  Request(request);
}

GoogleGeocode::GoogleGeocode(const IfcDB::Point& position)
{
  Request(position);
}

bool GoogleGeocode::Request(const std::wstring& request)
{
  bool state(false);

  std::string requestEncoded = url_encode(toUtf8(request));

  std::string sServer("maps.googleapis.com");
  std::string sParameter;

  std::stringstream params;

  params << "?address=" << requestEncoded;
  params << "&sensor=false";
  params << "&key=" << GoogleApiKey;
  params << "&language=" << toUtf8(m_language);

  CURL* pCurl = curl_easy_init();

  std::string url = "https://" + sServer + "/maps/api/geocode/xml" + params.str();
  curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(pCurl, CURLOPT_HTTPGET, 1L);

  std::stringstream resultStream;
  curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, &onWrite);
  curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &resultStream);

  CURLcode res = curl_easy_perform(pCurl);

  long http_code = 0;
  curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &http_code);

  if (res == CURLE_OK && http_code == 200)
  {
    try
    {
      GoogleGeocodeParser geocodeParser;

      if (geocodeParser.parse(resultStream))
      {
        m_Results = geocodeParser.getResults();
        state = true;
      }
    }
    catch (const std::exception&)
    {
      throw;
    }
  }
  else
  {
    //createErrorMessage(resultStream);
  }

  curl_easy_cleanup(pCurl);

  return state;
}

bool GoogleGeocode::Request(const IfcDB::Point& position)
{
  bool state(false);

  std::string sServer("maps.googleapis.com");
  std::string sParameter;

  std::stringstream params;

  params << "?latlng=" << position.x << "," << position.y;
  params << "&sensor=false";
  params << "&key=" << GoogleApiKey;
  params << "&language=" << toUtf8(m_language);

  CURL* pCurl = curl_easy_init();

  std::string url = "https://" + sServer + "/maps/api/geocode/xml" + params.str();
  curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(pCurl, CURLOPT_HTTPGET, 1L);

  std::stringstream resultStream;
  curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, &onWrite);
  curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &resultStream);

  CURLcode res = curl_easy_perform(pCurl);

  long http_code = 0;
  curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &http_code);

  if (res == CURLE_OK && http_code == 200)
  {
    try
    {
      GoogleGeocodeParser geocodeParser;

      if (geocodeParser.parse(resultStream))
      {
        m_Results = geocodeParser.getResults();
        state = true;
      }
      else
      {
        m_errorMessage = geocodeParser.getErrorMessage();
      }
    }
    catch (const std::exception&)
    {
      throw;
    }
  }
  else
  {
    //createErrorMessage(resultStream);
  }

  curl_easy_cleanup(pCurl);

  return state;
}

bool GoogleGeocodeParser::parse(std::stringstream& stream)
{
  bool bSuccess(false);

  tinyxml2::XMLDocument xmlDoc;

  stream.seekp(0, std::ios::end);
  std::stringstream::pos_type offset = stream.tellp();
  size_t size = offset / sizeof(char);

  tinyxml2::XMLError loadOkay = xmlDoc.Parse(stream.str().c_str(), size);

  tinyxml2::XMLElement* pParentElement = xmlDoc.FirstChildElement();

  if (loadOkay == tinyxml2::XML_NO_ERROR)
  {
    if (_stricmp(pParentElement->Name(), "GeocodeResponse") == 0)
    {
      tinyxml2::XMLElement* pElement = pParentElement->FirstChildElement();

      while (pElement != nullptr)
      {
        if (_stricmp(pElement->Name(), "status") == 0)
        {
          const char* pText = pElement->GetText();
          if (pText && _stricmp(pText, "OK") == 0)
          {
            bSuccess = true;
          }
        }
        else if (_stricmp(pElement->Name(), "result") == 0)
        {
          parse_result(pElement);
        }
        else if (_stricmp(pElement->Name(), "error_message") == 0)
        {
          const char* pText = pElement->GetText();
          if (pText)
          {
            m_errorMessage = fromUtf8(pText);
          }
        }

        pElement = pElement->NextSiblingElement();
      }
    }
  }

  return bSuccess;
}

void GoogleGeocodeParser::parse_result(tinyxml2::XMLElement* pParentElement)
{
  GoogleGeocode::Result result;

  tinyxml2::XMLElement* pElement = pParentElement->FirstChildElement();

  while (pElement != nullptr)
  {
    if (_stricmp(pElement->Name(), "type") == 0)
    {
      const char* pText = pElement->GetText();
      if (pText)
      {
        result.m_Types.push_back(fromUtf8(pText));
      }
    }
    else if (_stricmp(pElement->Name(), "formatted_address") == 0)
    {
      const char* pText = pElement->GetText();
      if (pText)
      {
        result.m_FormatedAddress = fromUtf8(pText);
      }
    }
    else if (_stricmp(pElement->Name(), "address_component") == 0)
    {
      parse_address_component(pElement, result);
    }
    else if (_stricmp(pElement->Name(), "geometry") == 0)
    {
      parse_geometry(pElement, result);
    }

    pElement = pElement->NextSiblingElement();
  }

  m_Results.emplace_back(result);
}

void GoogleGeocodeParser::parse_address_component(tinyxml2::XMLElement* pParentElement, GoogleGeocode::Result& result)
{
  GoogleGeocode::AddressComponent addressComponent;

  tinyxml2::XMLElement* pElement = pParentElement->FirstChildElement();

  while (pElement != nullptr)
  {
    if (_stricmp(pElement->Name(), "long_name") == 0)
    {
      const char* pText = pElement->GetText();
      if (pText)
      {
        addressComponent.m_LongName = fromUtf8(pText);
      }
    }
    else if (_stricmp(pElement->Name(), "short_name") == 0)
    {
      const char* pText = pElement->GetText();
      if (pText)
      {
        addressComponent.m_ShortName = fromUtf8(pText);
      }
    }
    else if (_stricmp(pElement->Name(), "type") == 0)
    {
      const char* pText = pElement->GetText();
      if (pText)
      {
        addressComponent.m_Types.push_back(fromUtf8(pText));
      }
    }

    pElement = pElement->NextSiblingElement();
  }

  result.m_AddressComponents.emplace_back(addressComponent);
}

void GoogleGeocodeParser::parse_geometry(tinyxml2::XMLElement* pParentElement, GoogleGeocode::Result& result)
{
  tinyxml2::XMLElement* pElement = pParentElement->FirstChildElement();

  while (pElement != nullptr)
  {
    if (_stricmp(pElement->Name(), "location") == 0)
    {
      parse_lat_lng(pElement, result.m_Location);
    }
    else if (_stricmp(pElement->Name(), "bounds") == 0)
    {
      parse_bounds(pElement, result);
    }

    pElement = pElement->NextSiblingElement();
  }
}

void GoogleGeocodeParser::parse_bounds(tinyxml2::XMLElement* pParentElement, GoogleGeocode::Result& result)
{
  tinyxml2::XMLElement* pElement = pParentElement->FirstChildElement();

  while (pElement != nullptr)
  {
    if (_stricmp(pElement->Name(), "southwest") == 0)
    {
      parse_lat_lng(pElement, result.m_SouthWest);
    }
    else if (_stricmp(pElement->Name(), "northeast") == 0)
    {
      parse_lat_lng(pElement, result.m_NorthEast);
    }

    pElement = pElement->NextSiblingElement();
  }
}

void GoogleGeocodeParser::parse_lat_lng(tinyxml2::XMLElement* pParentElement, IfcDB::Point& pos)
{
  tinyxml2::XMLElement* pElement = pParentElement->FirstChildElement();

  while (pElement != nullptr)
  {
    if (_stricmp(pElement->Name(), "lat") == 0)
    {
      const char* pText = pElement->GetText();
      if (pText)
      {
        pos.x = atof(pText);
      }
    }
    else if (_stricmp(pElement->Name(), "lng") == 0)
    {
      const char* pText = pElement->GetText();
      if (pText)
      {
        pos.y = atof(pText);
      }
    }

    pElement = pElement->NextSiblingElement();
  }
}

bool GoogleGeocode::getResultByType(Result& result, const std::wstring& type)
{
  for(const auto& itResult : m_Results)
  {
    for(const auto& itType : itResult.m_Types)
    {
      if(itType == type)
      {
        result = itResult;
        return true;
      }
    }
  }

  return false;
}

std::wstring GoogleGeocode::getCountryShortName()
{
  for (const auto& result : m_Results)
  {
    for (const auto& addressComponent : result.m_AddressComponents)
    {
      for (const auto& type : addressComponent.m_Types)
      {
        if (type == L"country")
        {
          return addressComponent.m_ShortName;
        }
      }
    }
  }

  return {};
}

void GoogleGeocode::setLanguage(const std::wstring& language)
{
  std::wstring lang(language);

  std::transform(lang.begin(), lang.end(), lang.begin(), ::tolower);

  if (lang == L"ger")
  {
    m_language = L"de";
  }
  else if (lang == L"eng")
  {
    m_language = L"de";
  }
  else if (lang == L"fra")
  {
    m_language = L"fr";
  }
}

bool GoogleGeocode::AddressComponent::hasType(const std::wstring& searchType)
{
  for(const auto& type : m_Types)
  {
    if(type == searchType)
    {
      return true;
    }
  }

  return false;
}
