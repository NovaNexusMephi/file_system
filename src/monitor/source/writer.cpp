#include "monitor/writer.h"
#include <string>
using monitor::Writer;

void Writer::write(std::ostream& to, const std::string& message) {
    to << message;
}