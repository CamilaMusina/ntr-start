#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <variant>
#include <unordered_map>
#include <cctype>

// (1 + 2) * 3 / 4 + 5 * (6 - 7)

// 1. Токенизация
// 2. Парсер (построение дерева разбора выражения)

// +, -, *, /, %

struct OpeningBracket {};

struct ClosingBracket {};

struct Number {
    int value;
};

struct UnknownToken {
    std::string value;
};

struct MinToken {};

struct AbsToken {};

struct Plus {};

struct Minus {};

struct Multiply {};

struct Modulo {};

struct Divide {};

struct Max {};

struct Sqr {};

using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken, MinToken, AbsToken, Plus, Minus, Max, Sqr>;

// 1234

const std::unordered_map<char, Token> kSymbol2Token{
    {'+', Plus{}}, {'-', Minus{}}, {'*', Multiply{}}, {'/', Divide{}}, {'%', Modulo{}} };

int ToDigit(unsigned char symbol) {
    return symbol - '0';
}

Number ParseNumber(const std::string& input, size_t& pos) {
    int value = 0;
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isdigit(symbol)) {
        value = value * 10 + ToDigit(symbol);
        if (pos == input.size() - 1) {
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    return Number{ value };
}

Token ParseName(const std::string& input, size_t& pos) {
    std::string name;
    while (pos < input.size() && std::isalpha(input[pos])) {
        name += input[pos++];
    }

    if (name == "min") {
        return MinToken{};
    }
    else if (name == "abs") {
        return AbsToken{};
    }
    else if (name == "max") {
        return Max{};
    }
    else if (name == "sqr") {
        return Sqr{};
    }
    else {
        return UnknownToken{ name };
    }
}

std::vector<Token> Tokenize(const std::string& input) {
    std::vector<Token> tokens;
    const size_t size = input.size();
    size_t pos = 0;

    while (pos < size) {
        const auto symbol = static_cast<unsigned char>(input[pos]);

        if (std::isspace(symbol)) {
            ++pos;
        }
        else if (std::isdigit(symbol)) {
            tokens.emplace_back(ParseNumber(input, pos));
        }
        else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
            tokens.emplace_back(it->second);
        }
        else if (symbol == '(') {
            tokens.emplace_back(OpeningBracket{});
            pos++;
        }
        else if (symbol == ')') {
            tokens.emplace_back(ClosingBracket{});
            pos++;
        }
        else if (std::isalpha(symbol)) {
            tokens.emplace_back(ParseName(input, pos));
        }
        else {
            std::string unknownToken;
            while (pos < size && !std::isspace(input[pos]) && !std::isdigit(input[pos])) {
                unknownToken += input[pos++];
            }
            tokens.emplace_back(UnknownToken{ unknownToken });
        }
    }
    return tokens;
}
