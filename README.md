# CRREM Pathway
[CRREM (Carbon Risk Real Estate Monitor)](https://crrem.org/) provides science-based decarbonization pathways for the real estate sector. It enables the assessment of CO2 emissions from buildings and their adjustment to the targets of the Paris Climate Agreement.

The CRREM plugin for [KITModelViewer]() displays the CRREM pathways provided via the [NaiS database](). Where possible, the necessary parameters are derived from 3D building models.

Supported data models are:
* CityGML / CityGML Energy ADE - Geography Markup Language
* gbXML - Green Building XML
* IFC - Industry Foundation Classes

If the georeference can be interpreted correctly, the available data is automatically retrieved from the NaiS database. 


## Dependencies

### Use of vcpkg:

|Package Name         |Install Command                            |
|:---                 |:---                                       |
|wxwidgets            |vcpkg install wxwidgets triplet=x64-windows|
|curl                 |vcpkg install curl triplet=x64-windows     |
|sqlite3              |vcpkg install sqlite3 triplet=x64-windows  |

## How to cite

```bibtex
@software{SDM_Plugin_crrem_pathway,
	title        = {SDM_Plugin_crrem_pathway},
	author       = {Andreas Geiger},
	url          = {https://github.com/KIT-IAI/SDM_Plugin_crrem_pathway},
	date         = {2025}
}
```























