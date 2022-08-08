//
// Created by sozonov on 21.05.2022.
//

#ifndef BOOST_LOG_LOGGER_H
#define BOOST_LOG_LOGGER_H

#include <memory>

#include "spdlog/logger.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"

#include "spdlog/fmt/bin_to_hex.h"

namespace util::log {

/**
 * Класс осущуствляющий логгирование.
 *
 * @note https://spdlog.docsforge.com/v1.x/getting-started/
 * @example
    Logger log;<br>
    log.error("Error!");<br>
    log.info("Info 1");<br>
    log.warn("Warn");<br>
    log.critical("Critical");<br>
 */
class Logger {
private:
    //5 MB
    uint32_t MAX_LOG_FILE_SIZE = 1048576 * 5;
    uint32_t MAX_LOG_FILE_NUMBER = 10;
    
public:
    Logger() :
    consoleSink( std::make_shared<spdlog::sinks::stdout_color_sink_mt>() ),
    rotateFileSink( std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/InfoLog.log",
                                                                           Logger::MAX_LOG_FILE_SIZE,
                                                                           Logger::MAX_LOG_FILE_NUMBER) ),
    exceptionFileSink( std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/ExceptionLog.log") ),
    sinks{consoleSink, rotateFileSink, exceptionFileSink}
    {
        spdlog::init_thread_pool(8192, 1);
        multiSinkLog = std::make_shared<spdlog::async_logger>(spdlog::async_logger("Logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block) );
        spdlog::register_logger(multiSinkLog);
        consoleSink->set_level(spdlog::level::info); //4
    
        rotateFileSink->set_level(spdlog::level::info); //4
    
        exceptionFileSink->set_level(spdlog::level::critical); //7
    
        multiSinkLog->set_level(spdlog::level::info); //5
        multiSinkLog->set_pattern(Logger::format);
    
    }
    
    Logger( const std::string& _fileLogPath, std::uint16_t _fileLogFileSize, std::uint16_t _fileLogFileNumber ):
    consoleSink( std::make_shared<spdlog::sinks::stdout_color_sink_mt>() ),
    rotateFileSink( std::make_shared<spdlog::sinks::rotating_file_sink_mt>( _fileLogPath, _fileLogFileSize, _fileLogFileNumber) ),
    exceptionFileSink( std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/ExceptionLog.log") ),
    sinks{consoleSink, rotateFileSink, exceptionFileSink}
    {
        spdlog::init_thread_pool(8192, 1);
        multiSinkLog = std::make_shared<spdlog::async_logger>(spdlog::async_logger("Logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block) );
        spdlog::register_logger(multiSinkLog);
    
        consoleSink->set_level(spdlog::level::info); //4
    
        rotateFileSink->set_level(spdlog::level::info); //4
    
        exceptionFileSink->set_level(spdlog::level::critical); //7
    
        multiSinkLog->set_level(spdlog::level::info); //5
        multiSinkLog->set_pattern(Logger::format);
    
    }
    
    void consoleSinkOff(){
        consoleSink->set_level(spdlog::level::off); //4
    }
    
    void rotateFileSinkOff(){
        rotateFileSink->set_level(spdlog::level::off); //4
    }
    
    void exceptionFileSinkOff(){
        exceptionFileSink->set_level(spdlog::level::off); //7
    }
    
    void consoleSinkOn(){
        consoleSink->set_level(spdlog::level::info); //4
    }
    
    void rotateFileSinkOn(){
        rotateFileSink->set_level(spdlog::level::info); //4
    }
    
    void exceptionFileSinkOn(){
        exceptionFileSink->set_level(spdlog::level::critical); //7
    }
    
    void setConsoleSinkLevel(spdlog::level::level_enum _lvl){
        consoleSink->set_level(_lvl); //4
    }
    
    void setRotateFileSinkLevel(spdlog::level::level_enum _lvl){
        rotateFileSink->set_level(_lvl); //4
    }
    
    void setExceptionFileSinkLevel(spdlog::level::level_enum _lvl){
        exceptionFileSink->set_level(_lvl); //7
    }
    
    template<typename T>
    static inline auto toHex( const T &&msg )
    {
        return spdlog::to_hex( std::forward<decltype(msg)>( msg ) );
    }

    template<typename T>
    static inline auto toHex( T &&msg )
    {
        return spdlog::to_hex( std::forward<decltype(msg)>( msg ) );
    }
    
    template<typename T>
    void info( const T &&msg )
    {
        multiSinkLog->info( std::forward<decltype( msg) >( msg ) );
    }

    template<typename T>
    void info( T &&msg )
    {
        multiSinkLog->info( std::forward<decltype( msg) >( msg ) );
    }
    
    template<typename T>
    void warn( const T &&msg )
    {
        multiSinkLog->warn( std::forward<decltype( msg) >( msg ) );
    }

    template<typename T>
    void warn( T &&msg )
    {
        multiSinkLog->warn( std::forward<decltype( msg) >( msg ) );
    }
    
    template<typename T>
    void error( const T &&msg )
    {
        multiSinkLog->error( std::forward<decltype( msg) >( msg ) );
    }

    template<typename T>
    void error(  T &&msg )
    {
        multiSinkLog->error( std::forward<decltype( msg) >( msg ) );
    }

    
    template<typename T>
    void critical( const T &&msg )
    {
        multiSinkLog->critical( std::forward<decltype( msg) >( msg ) );
    }

    template<typename T>
    void critical( T &&msg )
    {
        multiSinkLog->critical(  std::forward<decltype( msg) >( msg ) );
    }

private:
    std::shared_ptr<spdlog::sinks::sink> consoleSink;
    std::shared_ptr<spdlog::sinks::sink> rotateFileSink;
    std::shared_ptr<spdlog::sinks::sink> exceptionFileSink;
    
    std::vector<spdlog::sink_ptr> sinks;
    
    std::shared_ptr< spdlog::async_logger> multiSinkLog;
    
    /**
     * Формат записываемых данных.
     * @copydoc https://spdlog.docsforge.com/v1.x/3.custom-formatting/#pattern-flags
     */
    std::string format { "[%D %H:%M:%S:%f][%n][%^%L%$] : %v " };
public:
    const std::string &getFormat() const {
        return format;
    }
    
    /**
     * Установка фомрата вывода данных.
     *
     * @param format Форматирование выводимых данных.
     * @example setFormat("*** [%H:%M:%S %z] [thread %t] %v ***")
     * @copydoc https://spdlog.docsforge.com/v1.x/3.custom-formatting/#pattern-flags
     *
     */
    void setFormat( std::string && _format ) {
        Logger::format = std::forward<decltype(_format)>( _format );
        multiSinkLog->set_pattern(Logger::format);
    }

    /**
     * Установка фомрата вывода данных.
     *
     * @param format Форматирование выводимых данных.
     * @example setFormat("*** [%H:%M:%S %z] [thread %t] %v ***")
     * @copydoc https://spdlog.docsforge.com/v1.x/3.custom-formatting/#pattern-flags
     *
     */
    void setFormat( const std::string && _format ) {
        Logger::format = std::forward<decltype(_format)>( _format );
        multiSinkLog->set_pattern(Logger::format);
    }
    
     [[nodiscard]] const spdlog::logger &getMultiSinkLog() const {
        return *multiSinkLog;
    }
    
};

/**
 * Запись информационного лога в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::infoLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void infoLogToFile( T&& msg, const std::string& _filepath = "logs/InfoFileLog.log",
                           const std::string& _logName = "infoLogToConsole", const std::string& _pattern = "[%D %H:%M:%S:%f][%n][%^%L%$] : %v " ) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::basic_logger_mt( _logName, _filepath, true );
    }
    logger->set_pattern(_pattern);
    logger->info( std::forward<decltype( msg )>( msg ) );
}

/**
 * Запись информационного лога в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::infoLogToFile( util::log::Logger::toHex( buf ) );
 */
    template<typename T>
    inline void infoLogToFile( const T&& msg, const std::string& _filepath = "logs/InfoFileLog.log",
                               const std::string& _logName = "infoLogToConsole", const std::string& _pattern = "[%D %H:%M:%S:%f][%n][%^%L%$] : %v "  ) {
        std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
        if( !logger ) {
            logger = spdlog::basic_logger_mt( _logName, _filepath, true );
        }
        logger->set_pattern(_pattern);
        logger->info( std::forward<decltype( msg )>( msg ) );
    }

/**
 * Запись информационного лога в консоль (std::cout).
 * @param msg Информация для записи в лог.
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::infoLogToConsole( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void infoLogToConsole(  const T&& msg , const std::string& _logName = "infoLogToConsole") {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::stdout_color_mt( _logName );
    }
    logger->info( std::forward<decltype(msg)>( msg ) );
}

/**
 * Запись информационного лога в консоль (std::cout).
 * @param msg Информация для записи в лог.
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::infoLogToConsole( util::log::Logger::toHex( buf ) );
 */
    template<typename T>
    inline void infoLogToConsole( T&& msg , const std::string& _logName = "infoLogToConsole") {
        std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
        if( !logger ) {
            logger = spdlog::stdout_color_mt( _logName );
        }
        logger->info( std::forward<decltype(msg)>( msg ) );
    }

/**
 * Циклическая запись информационного лога в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @param max_size Максимальный размер файла
 * @param max_files Максимальное количество файлов
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::infoLogToRotatingFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void infoLogToRotatingFile( const T&& msg, const std::string& _filepath = "logs/InfoRotatingLog.log",
                                   const std::string& _logName = "infoLogToRotatingFile",
                                   const std::string& _pattern = "[%D %H:%M:%S:%f][%n][%^%L%$] : %v ",
                                   std::uint32_t max_size = 1048576 * 5,
                                   std::uint32_t max_files = 3) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::rotating_logger_mt( _logName, _filepath, max_size, max_files );
    }
    logger->set_pattern(_pattern);
    logger->info( std::forward<decltype(msg)>( msg ) );
}

/**
 * Циклическая запись информационного лога в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @param max_size Максимальный размер файла
 * @param max_files Максимальное количество файлов
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::infoLogToRotatingFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void infoLogToRotatingFile( T&& msg, const std::string& _filepath = "logs/InfoRotatingLog.log",
                                   const std::string& _logName = "infoLogToRotatingFile",
                                   std::uint32_t max_size = 1048576 * 5,
                                   std::uint32_t max_files = 3) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::rotating_logger_mt( _logName, _filepath, max_size, max_files );
    }
    logger->info( std::forward<decltype(msg)>( msg ) );
}

/**
 * Запись лога ошибки в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::errorLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void errorLogToFile( const T&& msg, const std::string& _filepath = "logs/ErrorFileLog.log",
                            const std::string& _logName = "errorLogToFile" ) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::basic_logger_mt( _logName, _filepath, true);
    }
    logger->error( std::forward<decltype(msg)>( msg ) );
}

/**
 * Запись лога ошибки в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::errorLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void errorLogToFile( T&& msg, const std::string& _filepath = "logs/ErrorFileLog.log",
                            const std::string& _logName = "errorLogToFile" ) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::basic_logger_mt( _logName, _filepath, true);
    }
    logger->error( std::forward<decltype(msg)>( msg ) );
}

/**
 * Запись лога ошибки в консоль (std::cout).
 * @param msg Информация для записи в лог.
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::errorLogToConsole( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void errorLogToConsole( const T&& msg , const std::string& _logName = "errorLogToConsole") {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::stdout_color_mt( _logName );
    }
    logger->error( std::forward<decltype(msg)>( msg ) );
}

/**
 * Запись лога ошибки в консоль (std::cout).
 * @param msg Информация для записи в лог.
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::errorLogToConsole( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void errorLogToConsole( T&& msg , const std::string& _logName = "errorLogToConsole") {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::stdout_color_mt( _logName );
    }
    logger->error( std::forward<decltype(msg)>( msg ) );
}

/**
 * Циклическая запись лога ошибки в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @param max_size Максимальный размер файла
 * @param max_files Максимальное количество файлов
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::infoLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void errorLogToRotatingFile( const T&& msg, const std::string& _filepath = "logs/errorRotatingLog.log",
                                   const std::string& _logName = "errorLogToRotatingFile",
                                   std::uint32_t max_size = 1048576 * 5,
                                   std::uint32_t max_files = 3) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::rotating_logger_mt( _logName, _filepath, max_size, max_files );
    }
    logger->error(  std::forward<decltype(msg)>( msg )  );
}

/**
 * Циклическая запись лога ошибки в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @param max_size Максимальный размер файла
 * @param max_files Максимальное количество файлов
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::infoLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void errorLogToRotatingFile( T&& msg, const std::string& _filepath = "logs/errorRotatingLog.log",
                                    const std::string& _logName = "errorLogToRotatingFile",
                                    std::uint32_t max_size = 1048576 * 5,
                                    std::uint32_t max_files = 3) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::rotating_logger_mt( _logName, _filepath, max_size, max_files );
    }
    logger->error(  std::forward<decltype(msg)>( msg )  );
}

/**
 * Запись лога предупреждения в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::warnLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void warnLogToFile( const T&& msg, const std::string& _filepath = "logs/WarnFileLog.log",
                           const std::string& _logName = "warnLogToFile" ) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::basic_logger_mt( _logName, _filepath, true);
    }
    logger->warn( std::forward<decltype(msg)>( msg ) );
}

/**
 * Запись лога предупреждения в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::warnLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void warnLogToFile( T&& msg, const std::string& _filepath = "logs/WarnFileLog.log",
                           const std::string& _logName = "warnLogToFile" ) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::basic_logger_mt( _logName, _filepath, true);
    }
    logger->warn( std::forward<decltype(msg)>( msg ) );
}

/**
 * Запись лога предупреждения в консоль (std::cout).
 * @param msg Информация для записи в лог.
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::warnLogToConsole( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void warnLogToConsole( const T&& msg , const std::string& _logName = "warnLogToConsole") {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::stdout_color_mt( _logName );
    }
    logger->warn( std::forward<decltype(msg)>( msg ) );
}

/**
 * Запись лога предупреждения в консоль (std::cout).
 * @param msg Информация для записи в лог.
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::warnLogToConsole( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void warnLogToConsole( T&& msg , const std::string& _logName = "warnLogToConsole") {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::stdout_color_mt( _logName );
    }
    logger->warn( std::forward<decltype(msg)>( msg ) );
}

/**
 * Циклическая запись лога предупреждения в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @param max_size Максимальный размер файла
 * @param max_files Максимальное количество файлов
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::infoLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void warnLogToRotatingFile( const T&& msg, const std::string& _filepath = "logs/warnRotatingLog.log",
                                    const std::string& _logName = "warnLogToRotatingFile",
                                    std::uint32_t max_size = 1048576 * 5,
                                    std::uint32_t max_files = 3) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::rotating_logger_mt( _logName, _filepath, max_size, max_files );
    }
    logger->warn( std::forward<decltype(msg)>( msg ) );
}

/**
 * Циклическая запись лога предупреждения в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @param max_size Максимальный размер файла
 * @param max_files Максимальное количество файлов
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::infoLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void warnLogToRotatingFile( T&& msg, const std::string& _filepath = "logs/warnRotatingLog.log",
                                   const std::string& _logName = "warnLogToRotatingFile",
                                   std::uint32_t max_size = 1048576 * 5,
                                   std::uint32_t max_files = 3) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::rotating_logger_mt( _logName, _filepath, max_size, max_files );
    }
    logger->warn( std::forward<decltype(msg)>( msg ) );
}

/**
 * Запись лога критической ошибки в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::criticalLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void criticalLogToFile( const T&& msg, const std::string& _filepath = "logs/CriticalFileLog.log",
                              const std::string& _logName = "criticalLogToFile" ) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::basic_logger_mt( _logName, _filepath, true);
    }
    logger->critical( std::forward<decltype(msg)>( msg ) );
}

/**
 * Запись лога критической ошибки в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::criticalLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void criticalLogToFile( T&& msg, const std::string& _filepath = "logs/CriticalFileLog.log",
                               const std::string& _logName = "criticalLogToFile" ) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::basic_logger_mt( _logName, _filepath, true);
    }
    logger->critical( std::forward<decltype(msg)>( msg ) );
}

/**
 * Запись лога критической ошибки в консоль (std::cout).
 * @param msg Информация для записи в лог.
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::criticalLogToConsole( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void criticalLogToConsole( const T&& msg , const std::string& _logName = "criticalLogToConsole") {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::stdout_color_mt( _logName );
    }
    logger->critical( std::forward<decltype(msg)>( msg ) );
}

/**
 * Запись лога критической ошибки в консоль (std::cout).
 * @param msg Информация для записи в лог.
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::criticalLogToConsole( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void criticalLogToConsole( T&& msg , const std::string& _logName = "criticalLogToConsole") {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::stdout_color_mt( _logName );
    }
    logger->critical( std::forward<decltype(msg)>( msg ) );
}

/**
 * Циклическая запись лога лога критической ошибки в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @param max_size Максимальный размер файла
 * @param max_files Максимальное количество файлов
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::infoLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void criticalLogToRotatingFile( const T&& msg, const std::string& _filepath = "logs/criticalRotatingLog.log",
                                   const std::string& _logName = "criticalLogToRotatingFile",
                                   std::uint32_t max_size = 1048576 * 5,
                                   std::uint32_t max_files = 3) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::rotating_logger_mt( _logName, _filepath, max_size, max_files );
    }
    logger->critical( std::forward<decltype(msg)>( msg ) );
}

/**
 * Циклическая запись лога лога критической ошибки в файл.
 * @param msg Информация для записи в лог.
 * @param _filepath Файл, в который будет произведена запись.
 * @param _logName Название логгера
 * @param max_size Максимальный размер файла
 * @param max_files Максимальное количество файлов
 * @example
    std::vector<char> buf(80);<br>
    for (int i = 0; i < 80; i++)<br>
    {<br>
        buf.push_back(static_cast<char>(i & 0xff));<br>
    }<br>
    util::log::infoLogToFile( util::log::Logger::toHex( buf ) );
 */
template<typename T>
inline void criticalLogToRotatingFile( T&& msg, const std::string& _filepath = "logs/criticalRotatingLog.log",
                                       const std::string& _logName = "criticalLogToRotatingFile",
                                       std::uint32_t max_size = 1048576 * 5,
                                       std::uint32_t max_files = 3) {
    std::shared_ptr< spdlog::logger > logger{ spdlog::get( _logName ) };
    if( !logger ) {
        logger = spdlog::rotating_logger_mt( _logName, _filepath, max_size, max_files );
    }
    logger->critical( std::forward<decltype(msg)>( msg ) );
}

} // namespace util::log

#endif //BOOST_LOG_LOGGER_H
