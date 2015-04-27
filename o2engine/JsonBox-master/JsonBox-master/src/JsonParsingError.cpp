#include <JsonBox/JsonParsingError.h>

namespace JsonBox {
	JsonParsingError::JsonParsingError(const std::string &what) : std::runtime_error(what) {
	}
	
	JsonParsingError::JsonParsingError(const char *what) : std::runtime_error(what) {
	}
}