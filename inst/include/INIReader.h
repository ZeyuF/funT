// Read an INI file into easy-to-access name/value pairs.

// inih and INIReader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// http://code.google.com/p/inih/

#ifndef __INIREADER_H__
#define __INIREADER_H__

#include <map>
#include <string>

namespace Hector {

// Read an INI file into easy-to-access name/value pairs. (Note that I've gone
// for simplicity here rather than speed, but it should be pretty decent.)
class INIReader {
public:
  // Construct INIReader and parse given filename. See ini.h for more info
  // about the parsing.
  INIReader(std::string filename);

  // Return the result of ini_parse(), i.e., 0 on success, line number of
  // first error on parse error, or -1 on file open error.
  int ParseError();

  // Get a string value from INI file, returning default_value if not found.
  std::string Get(std::string section, std::string name,
                  std::string default_value);

  // Get an integer (long) value from INI file, returning default_value if
  // not found.
  long GetInteger(std::string section, std::string name, long default_value);

private:
  int _error;
  std::map<std::string, std::string> _values;
  static std::string MakeKey(std::string section, std::string name);
  static int ValueHandler(void *user, const char *section, const char *name,
                          const char *value);
};

} // namespace Hector

#endif // __INIREADER_H__
