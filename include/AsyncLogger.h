//
// Created by andrey on 08.08.22.
//

#ifndef BOOST_LOG_ASYNCLOGGER_H
#define BOOST_LOG_ASYNCLOGGER_H
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
     * Класс осуществляющий асинхронное логгирование.
     *
     * @note https://spdlog.docsforge.com/v1.x/getting-started/
     * @example
        Logger log;<br>
        log.error("Error!");<br>
        log.info("Info 1");<br>
        log.warn("Warn");<br>
        log.critical("Critical");<br>
     */
    class AsyncLogger {
    private:
        //5 MB
        uint32_t MAX_LOG_FILE_SIZE = 1048576 * 5;
        uint32_t MAX_LOG_FILE_NUMBER = 10;

    public:
        AsyncLogger() :
                consoleSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>()),
                rotateFileSink(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/InfoLog.log",
                                                                                      AsyncLogger::MAX_LOG_FILE_SIZE,
                                                                                      AsyncLogger::MAX_LOG_FILE_NUMBER)),
                exceptionFileSink(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/ExceptionLog.log")),
                sinks{consoleSink, rotateFileSink, exceptionFileSink} {
            spdlog::init_thread_pool(8192, 1);
            multiSinkLog = std::make_shared<spdlog::async_logger>(
                    spdlog::async_logger("Logger", sinks.begin(), sinks.end(), spdlog::thread_pool(),
                                         spdlog::async_overflow_policy::block));
            spdlog::register_logger(multiSinkLog);
            consoleSink->set_level(spdlog::level::info); //4

            rotateFileSink->set_level(spdlog::level::info); //4

            exceptionFileSink->set_level(spdlog::level::critical); //7

            multiSinkLog->set_level(spdlog::level::info); //5
            multiSinkLog->set_pattern(AsyncLogger::format);

        }

        AsyncLogger(const std::string &_fileLogPath, std::uint16_t _fileLogFileSize, std::uint16_t _fileLogFileNumber) :
                consoleSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>()),
                rotateFileSink(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(_fileLogPath, _fileLogFileSize,
                                                                                      _fileLogFileNumber)),
                exceptionFileSink(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/ExceptionLog.log")),
                sinks{consoleSink, rotateFileSink, exceptionFileSink} {
            spdlog::init_thread_pool(8192, 1);
            multiSinkLog = std::make_shared<spdlog::async_logger>(
                    spdlog::async_logger("Logger", sinks.begin(), sinks.end(), spdlog::thread_pool(),
                                         spdlog::async_overflow_policy::block));
            spdlog::register_logger(multiSinkLog);

            consoleSink->set_level(spdlog::level::info); //4

            rotateFileSink->set_level(spdlog::level::info); //4

            exceptionFileSink->set_level(spdlog::level::critical); //7

            multiSinkLog->set_level(spdlog::level::info); //5
            multiSinkLog->set_pattern(AsyncLogger::format);

        }

        void consoleSinkOff() {
            consoleSink->set_level(spdlog::level::off); //4
        }

        void rotateFileSinkOff() {
            rotateFileSink->set_level(spdlog::level::off); //4
        }

        void exceptionFileSinkOff() {
            exceptionFileSink->set_level(spdlog::level::off); //7
        }

        void consoleSinkOn() {
            consoleSink->set_level(spdlog::level::info); //4
        }

        void rotateFileSinkOn() {
            rotateFileSink->set_level(spdlog::level::info); //4
        }

        void exceptionFileSinkOn() {
            exceptionFileSink->set_level(spdlog::level::critical); //7
        }

        void setConsoleSinkLevel(spdlog::level::level_enum _lvl) {
            consoleSink->set_level(_lvl); //4
        }

        void setRotateFileSinkLevel(spdlog::level::level_enum _lvl) {
            rotateFileSink->set_level(_lvl); //4
        }

        void setExceptionFileSinkLevel(spdlog::level::level_enum _lvl) {
            exceptionFileSink->set_level(_lvl); //7
        }

        template<typename T>
        static inline auto toHex(const T &&msg) {
            return spdlog::to_hex(std::forward<decltype(msg)>(msg));
        }

        template<typename T>
        static inline auto toHex(T &&msg) {
            return spdlog::to_hex(std::forward<decltype(msg)>(msg));
        }

        template<typename T>
        void info(const T &&msg) {
            multiSinkLog->info(std::forward<decltype(msg)>(msg));
        }

        template<typename T>
        void info(T &&msg) {
            multiSinkLog->info(std::forward<decltype(msg)>(msg));
        }

        template<typename T>
        void warn(const T &&msg) {
            multiSinkLog->warn(std::forward<decltype(msg)>(msg));
        }

        template<typename T>
        void warn(T &&msg) {
            multiSinkLog->warn(std::forward<decltype(msg)>(msg));
        }

        template<typename T>
        void error(const T &&msg) {
            multiSinkLog->error(std::forward<decltype(msg)>(msg));
        }

        template<typename T>
        void error(T &&msg) {
            multiSinkLog->error(std::forward<decltype(msg)>(msg));
        }


        template<typename T>
        void critical(const T &&msg) {
            multiSinkLog->critical(std::forward<decltype(msg)>(msg));
        }

        template<typename T>
        void critical(T &&msg) {
            multiSinkLog->critical(std::forward<decltype(msg)>(msg));
        }

    private:
        std::shared_ptr<spdlog::sinks::sink> consoleSink;
        std::shared_ptr<spdlog::sinks::sink> rotateFileSink;
        std::shared_ptr<spdlog::sinks::sink> exceptionFileSink;

        std::vector<spdlog::sink_ptr> sinks;

        std::shared_ptr<spdlog::async_logger> multiSinkLog;

        /**
         * Формат записываемых данных.
         * @copydoc https://spdlog.docsforge.com/v1.x/3.custom-formatting/#pattern-flags
         */
        std::string format{"[%D %H:%M:%S:%f][%n][%^%L%$] : %v "};
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
        void setFormat(std::string &&_format) {
            AsyncLogger::format = std::forward<decltype(_format)>(_format);
            multiSinkLog->set_pattern(AsyncLogger::format);
        }

        /**
         * Установка фомрата вывода данных.
         *
         * @param format Форматирование выводимых данных.
         * @example setFormat("*** [%H:%M:%S %z] [thread %t] %v ***")
         * @copydoc https://spdlog.docsforge.com/v1.x/3.custom-formatting/#pattern-flags
         *
         */
        void setFormat(const std::string &&_format) {
            AsyncLogger::format = std::forward<decltype(_format)>(_format);
            multiSinkLog->set_pattern(AsyncLogger::format);
        }

        [[nodiscard]] const spdlog::logger &getMultiSinkLog() const {
            return *multiSinkLog;
        }

    };

}




#endif //BOOST_LOG_ASYNCLOGGER_H
