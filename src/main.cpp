#include <cstdlib>
#include "logger.h"
#include "lexer.h"

int main() {
    BadSQL::Logger::setLogLevel(BadSQL::Logger::LogLevel::Info);
    BadSQL::Logger::info("Welcome to BadSQL!");
    BadSQL::Lexer lexer;
    auto result = lexer.tokenize("test.badsql");
    if (!result) {
        BadSQL::Logger::error(result.error());
        return EXIT_FAILURE;
    }

    auto tokens = std::move(result.value());

    return EXIT_SUCCESS;
}