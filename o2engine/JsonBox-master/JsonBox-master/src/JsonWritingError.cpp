#include <JsonBox/JsonWritingError.h>

namespace JsonBox {
	JsonWritingError::JsonWritingError(const std::string &what) : std::runtime_error(what) {
	}
	
	JsonWritingError::JsonWritingError(const char *what) : std::runtime_error(what) {
	}
}