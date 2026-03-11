# CRREM Pathway
The CRREM plugin is a small tool for displaying the CREEM path based on a 3D building model. Currently, buildings in CityGML, gbXML, and IFC formats are supported. 
If the georeference can be interpreted correctly, the available data is automatically retrieved from the [NaiS database](). 


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













