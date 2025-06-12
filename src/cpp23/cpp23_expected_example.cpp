#include <expected>
#include <iostream>

enum class Error { Failed };

std::expected<int, Error> getValue(bool shouldFail) {
    if (shouldFail) {
        return std::unexpected(Error::Failed);
    }
    return 42;
}

int main() {
    auto result = getValue(false);
    if (result) {
        std::cout << "Value: " << *result << "\n";
    } else {
        std::cout << "Error: " << static_cast<int>(result.error()) << "\n";
    }
    return 0;
}