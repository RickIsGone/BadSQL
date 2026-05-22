#include <expected>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>
#include "lexer.h"
#include "token.h"
namespace fs = std::filesystem;

namespace BadSQL {
    std::expected<std::vector<Token>, std::string> Lexer::tokenize(const fs::path& path) {
        std::ifstream inFile(path);
        if (!inFile)
            return std::unexpected("failed to open file: " + path.string());

        std::stringstream ss;
        ss << inFile.rdbuf();
        _raw = ss.str();

        inFile.close();
        std::vector<Token> tokens;

        return std::move(tokens);
    }
} // namespace BadSQL