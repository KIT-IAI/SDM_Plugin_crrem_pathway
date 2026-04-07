# CRREM Pathway
[CRREM (Carbon Risk Real Estate Monitor)](https://crrem.org/) provides science-based decarbonization pathways for the real estate sector. It enables the assessment of CO2 emissions from buildings and their adjustment to the targets of the Paris Climate Agreement.

The CRREM plugin for [KITModelViewer](https://github.com/KIT-IAI/SDM_KITModelViewer) displays the CRREM pathways provided via the [NaiS database](https://github.com/KIT-IAI/SDM_NaiS-DB). If the required values and a valid georeference of a building are available, the corresponding CREMM path is automatically retrieved from the NaiS database and displayed in a simple chart. Missing values can be added manually and existing values overwritten if necessary.

Supported data models are:
* CityGML / CityGML Energy ADE - Geography Markup Language
* gbXML - Green Building XML
* IFC - Industry Foundation Classes

  <img width="1592" height="847" alt="grafik" src="https://github.com/user-attachments/assets/aeafcb4e-fcd6-4318-b141-68326438e113" />

> [!IMPORTANT]
> This plugin was developed as a functional test for the NaiS database and is not intended as a substitute for professional tools. It cannot perform a economic assessment, nor is it designed to generate a report.

## User Interface
The user interface is based on [wxWidgets](https://www.wxwidgets.org/) and was made with [wxFormBuilder](https://github.com/wxFormBuilder/wxFormBuilder).

## Dependencies

[KITModelViewer](https://www.iai.kit.edu/ifc) ia a Windows® freeware application. To build the project, you will need the [Plugin SDK](https://github.com/KIT-IAI/SDM_Plugin_SDK) and the libraries listed below.

### Use of vcpkg:

|Package Name         |Install Command                            |
|:---                 |:---                                       |
|wxwidgets            |vcpkg install wxwidgets triplet=x64-windows|
|curl                 |vcpkg install curl triplet=x64-windows     |
|sqlite3              |vcpkg install sqlite3 triplet=x64-windows  |
|geographiclib        |vcpkg install geographiclib triplet=x64-windows  |

## How to cite

```bibtex
@software{SDM_Plugin_crrem_pathway,
	title        = {SDM_Plugin_crrem_pathway},
	author       = {Andreas Geiger},
	url          = {https://github.com/KIT-IAI/SDM_Plugin_crrem_pathway},
	date         = {2026}
}
```























