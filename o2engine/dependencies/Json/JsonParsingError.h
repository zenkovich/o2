#ifndef JB_JSON_PARSING_ERROR_H
#define JB_JSON_PARSING_ERROR_H

#include <string>
#include <stdexcept>

namespace JsonBox {
	/**
	 * Defines a type of object to be thrown as exception. It reports errors
	 * that occur when parsing JSON.
	 */
	class JsonParsingError : public std::runtime_error {
	public:
		/**
		 * Constructs the exception with what as an explanatory string that can
		 * be accessed through what().
		 * @param what String describing the error.
		 */
		JsonParsingError(const std::string &what);
		
		/**
		 * Constructs the exception with what as an explanatory string that can
		 * be accessed through what().
		 * @param what String describing the error.
		 */
		JsonParsingError(const char *what);
	};
}

#endif
