/**
 * \file logger.h
 * \brief Logging of messages, warnings and errors.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_LOGGER_H
#define SMTLIB_PARSER_LOGGER_H

namespace smtlib {
    class Logger {
    public:
        /** Error codes to be used with exit() calls */
        enum ErrorCode {
            ERR_PARSE = 1
        };

        static void message(const char* msg);

        static void warning(const char* fun, const char* msg);

        static void error(const char* fun, const char* msg);

        static void parsingError(unsigned int rowLeft, unsigned int colLeft,
                                 unsigned int rowRight, unsigned int colRight,
                                 const char* filename, const char* msg);

        static void syntaxError(const char* fun, const char* msg);

        static void syntaxError(const char* fun, const char* file, const char* msg);

        static void sortednessError(const char* fun, const char* file, const char* msg);

        static void sortednessError(const char* fun, const char* msg);
    };
}

#endif //SMTLIB_PARSER_LOGGER_H
