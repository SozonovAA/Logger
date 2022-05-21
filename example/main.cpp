#include <iostream>
#include <vector>

#include "Logger.h"

int main() {

    util::log::Logger log;
    
    log.error("Error!");
    log.info("Info 1");
    log.warn("Warn");
    log.critical("Critical");

    log.consoleSinkOff();
    
    log.error("Error!");
    log.info("Info 1");
    log.warn("Warn");
    log.critical("Critical");
    
    log.consoleSinkOn();
    
    log.error("Error!");
    log.info("Info 1");
    log.warn("Warn");
    log.critical("Critical");
    
    
    std::vector<char> buf(80);
    for (int i = 0; i < 80; i++)
    {
        buf.push_back(static_cast<char>(i & 0xff));
    }
    util::log::infoLogToConsole("asd");
    util::log::infoLogToConsole( util::log::Logger::toHex( buf ) );
    
    util::log::infoLogToConsole("asd", std::string{"test"} );
    
    util::log::criticalLogToConsole( util::log::Logger::toHex( buf ) );
    
    
    return 0;
}
