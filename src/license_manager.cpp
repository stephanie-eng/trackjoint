/*********************************************************************
 * Copyright (c) 2019, PickNik Consulting
 * All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *********************************************************************/

#include "trackjoint/cxxopts.hpp"
#include "trackjoint/license_manager.h"

#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>

#if _WIN32
#if _WIN64
#pragma comment(lib, "x64/LexActivator")
#else
#pragma comment(lib, "x86/LexActivator")
#endif
#endif

namespace trackjoint
{
const std::string kVersion = "1.0.0";

LicenseManager::LicenseManager()
{
  SetProductInfo();
}

bool LicenseManager::CheckLicenseStatus()
{
  int status = IsLicenseGenuine();
  if (LA_OK == status)
  {
    unsigned int licenseExpiryDate = 0;
    GetLicenseExpiryDate(&licenseExpiryDate);
    int hoursLeft = (licenseExpiryDate - time(NULL)) / 86400;
    std::cout << "License hours left: " << hoursLeft << std::endl;
    int daysLeft = (licenseExpiryDate - time(NULL)) / 3600;
    std::cout << "License hours left: " << daysLeft << std::endl;
    std::cout << "License is genuinely activated" << std::endl;
    return true;
  }
  else if (LA_EXPIRED == status)
  {
    std::cout << "License is activated, but has expired" << std::endl;
  }
  else if (LA_SUSPENDED == status)
  {
    std::cout << "License is activated, but has been suspended" << std::endl;
  }
  else if (LA_GRACE_PERIOD_OVER == status)
  {
    std::cout << "License is activated, but requires server sync" << std::endl;
  }
  else if (LA_E_PRODUCT_ID == status)
  {
    std::cout << "The product id is incorrect" << std::endl;
  }
  else
  {
    int trialStatus;
    trialStatus = IsTrialGenuine();
    if (LA_OK == trialStatus)
    {
      unsigned int trialExpiryDate = 0;
      GetTrialExpiryDate(&trialExpiryDate);
      if (trialExpiryDate - time(NULL) < 86400)
      {
        int hoursLeft = (trialExpiryDate - time(NULL)) / 3600;
        std::cout << "Trial hours left: " << hoursLeft << std::endl;
      }
      else
      {
        int daysLeft = (trialExpiryDate - time(NULL)) / 86400;
        std::cout << "Trial days left: " << daysLeft << std::endl;
      }
      return true;
    }
    else if (LA_TRIAL_EXPIRED == trialStatus)
    {
      std::cout << "Trial has expired" << std::endl;
    }
    else
    {
      std::cout << "No license found, status " << status << std::endl;
    }
  }
  return false;
}

void LicenseManager::Parse(int argc, char* argv[])
{
  try
  {
    cxxopts::Options options("./license_manager", "License management for PickNik TrackSuite");
    options.show_positional_help();
    // clang-format off
    options.allow_unrecognised_options().add_options()
      ("h,help", "Print help")
      ("t,trial", "Activate evaluation key", cxxopts::value<std::string>(), "TRIAL-KEY xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx")
      ("l,license", "Activate license key", cxxopts::value<std::string>(), "LICENSE-KEY xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx")
      ("s,status", "Check on license status")
      ("d,deactivate", "Deactivate license")
      ("v,version", "Print TrackSuite version");
    // clang-format on

    auto result = options.parse(argc, argv);

    if (result.count("help") || result.arguments().empty())
    {
      // std::cout << options.help() << std::endl;
      std::cout << options.help({ "", "Group" }) << std::endl;
      exit(0);
    }

    // TODO: Only allow 1 arg

    if (result.count("t") > 0)
    {
      ActivateTrackSuiteTrial(result["trial"].as<std::string>());
    }
    if (result.count("l") > 0)
    {
      ActivateTrackSuiteLicense(result["license"].as<std::string>());
    }
    if (result.count("s") > 0)
    {
      CheckLicenseStatus();
    }
    if (result.count("d") > 0)
    {
      DeactivateTrackSuiteLicense();
    }
    if (result.count("v") > 0)
    {
      PrintVersion();
    }

    // std::cout << "Arguments remain = " << argc << std::endl;

    auto arguments = result.arguments();
    // std::cout << "Saw " << arguments.size() << " arguments" << std::endl;
  }
  catch (const cxxopts::OptionException& e)
  {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }
}

void LicenseManager::SetProductInfo()
{
  int status;

#if _WIN32
  status = SetProductData(
      L"MkNGQjNEREQ1QjZGRTc4OEM3RDk2RUNCNkIzQjgxMDE=.KohGT9MxA7p8ufFoSOYXFQNeA+Ytf5p+P1eo42vmVZ1XDMkvEE/"
      L"LoT0FddPX6c4kz+ondUjuCkApvuNEpYBL1gfgoqXU+ScNhDiUeAc4mZBmREDGsJRbko6EjBPIXMinGcmq3M6mfd/"
      L"M5j5En0PGRSZQaYdAxnHW4rNpDeYxYqbtwJgvFaPRd4sAn5720mo2E6qI6kE5wE1g0LAlFiNn6A0wrxnAEuHgRKy6pQvR4A17isaDvkKsayUCGh"
      L"HlInHITzRwnn1DE+1mjD1IGMULciI5oCcOpS+vExyhVnDRHZ76dU4lHGPGiJFt0Z5E9DkoX9mfHhzRl2pdJwBGp7NzkMzxiHgsRgZGN/"
      L"hqAN6qD+bYajaGuM97BhlZ+/PX2aDVQE++3VkHodi3nYvu6ebNpXqfqnzjvwDWf1sYthDhNdUSzcRkgGr1MU6wqb31OqVLdDxK792M87/"
      L"msL7MZp2UeqN/xFN634z6Y/kGsL26SNl0yq0MjO6ocK4NSKeJC+gqrszCy7MZvtlzNaJOksbxMWl/"
      L"acnY0C38PBvCeirdBH+WHVEFh739SZdtGYQJo3KmnjtD6zjMRCCcxjvZqpXQuW/xU3RMxMYAbPdULKvLdmW+z8CazBml/"
      L"5bUCmvOwnNCsJGU2m1TjlJlGNTneVnRcyn15AH/9zCydySxW3t3qBgYspcFQ1/8c5/"
      L"ryakHQeAMURB221I48UfGcxs1ADOL60Xk9Gum+j1cZoOFQkoLgjE=");
#else
  status = SetProductData(
      "MkNGQjNEREQ1QjZGRTc4OEM3RDk2RUNCNkIzQjgxMDE=.KohGT9MxA7p8ufFoSOYXFQNeA+Ytf5p+P1eo42vmVZ1XDMkvEE/"
      "LoT0FddPX6c4kz+ondUjuCkApvuNEpYBL1gfgoqXU+ScNhDiUeAc4mZBmREDGsJRbko6EjBPIXMinGcmq3M6mfd/"
      "M5j5En0PGRSZQaYdAxnHW4rNpDeYxYqbtwJgvFaPRd4sAn5720mo2E6qI6kE5wE1g0LAlFiNn6A0wrxnAEuHgRKy6pQvR4A17isaDvkKsayUCGhH"
      "lInHITzRwnn1DE+1mjD1IGMULciI5oCcOpS+vExyhVnDRHZ76dU4lHGPGiJFt0Z5E9DkoX9mfHhzRl2pdJwBGp7NzkMzxiHgsRgZGN/"
      "hqAN6qD+bYajaGuM97BhlZ+/PX2aDVQE++3VkHodi3nYvu6ebNpXqfqnzjvwDWf1sYthDhNdUSzcRkgGr1MU6wqb31OqVLdDxK792M87/"
      "msL7MZp2UeqN/xFN634z6Y/kGsL26SNl0yq0MjO6ocK4NSKeJC+gqrszCy7MZvtlzNaJOksbxMWl/"
      "acnY0C38PBvCeirdBH+WHVEFh739SZdtGYQJo3KmnjtD6zjMRCCcxjvZqpXQuW/xU3RMxMYAbPdULKvLdmW+z8CazBml/"
      "5bUCmvOwnNCsJGU2m1TjlJlGNTneVnRcyn15AH/9zCydySxW3t3qBgYspcFQ1/8c5/"
      "ryakHQeAMURB221I48UfGcxs1ADOL60Xk9Gum+j1cZoOFQkoLgjE=");
#endif
  if (LA_OK != status)
  {
    std::cout << "SetProductData failed: " << status << std::endl;
    exit(status);
  }

#if _WIN32
  status = SetProductId(L"74354709-c988-4941-88fc-4e648c6e8402", LA_USER);
#else
  status = SetProductId("74354709-c988-4941-88fc-4e648c6e8402", LA_USER);
#endif
  if (LA_OK != status)
  {
    std::cout << "SetProductId failed: " << status << std::endl;
    exit(status);
  }

#if _WIN32
  wstring wide_key = wstring(kVersion.begin(), kVersion.end());
  status = SetAppVersion(wide_string.c_str());
#else
  status = SetAppVersion(kVersion.c_str());
#endif
  if (LA_OK != status)
  {
    std::cout << "SetAppVersion failed: " << status << std::endl;
    exit(status);
  }
}

void LicenseManager::DeactivateTrackSuiteLicense()
{
  int status = DeactivateLicense();
  if (LA_OK != status)
  {
    std::cout << "DeactivateLicense failed: " << status << std::endl;
    exit(status);
  }
  std::cout << "License deactivated successfully" << std::endl;
}

void LicenseManager::ActivateTrackSuiteLicense(std::string key)
{
  std::cout << "License key is '" << key << "'" << std::endl;
  int status;
#if _WIN32
  wstring wide_string = wstring(key.begin(), key.end());
  status = SetLicenseKey(wide_string.c_str());
#else
  status = SetLicenseKey(key.c_str());
#endif
  if (LA_OK != status)
  {
    std::cout << "SetLicenseKey failed: " << status << std::endl;
    exit(status);
  }

  status = ActivateLicense();
  if (LA_OK == status)
  {
    std::cout << "License activated successfully" << std::endl;
  }
  else if (LA_EXPIRED == status)
  {
    std::cout << "License has expired!" << std::endl;
  }
  else
  {
    std::cout << "License activation failed: " << status << std::endl;
  }
}

void LicenseManager::ActivateTrackSuiteTrial(std::string key)
{
  std::cout << "Trial key is '" << key << "'" << std::endl;
  int status;

#if _WIN32
  wstring wide_string = wstring(key.begin(), key.end());
  status = SetLicenseKey(wide_string.c_str());
#else
  status = SetLicenseKey(key.c_str());
#endif
  if (LA_OK != status)
  {
    std::cout << "SetLicenseKey failed: " << status << std::endl;
    exit(status);
  }

  status = SetTrialActivationMetadata("key1", "value1");
  if (LA_OK != status)
  {
    std::cout << "Activation metadata failed: " << status << std::endl;
    exit(status);
  }

  status = ActivateTrial();
  if (LA_OK == status)
  {
    std::cout << "Trial activated successfully!" << std::endl;
  }
  else if (LA_TRIAL_EXPIRED == status)
  {
    std::cout << "Trial has expired!" << std::endl;
  }
  else
  {
    std::cout << "Trial activation failed: " << status << std::endl;
  }
}

void LicenseManager::PrintVersion()
{
  std::cout << "Version: " << kVersion << std::endl;
}
}
