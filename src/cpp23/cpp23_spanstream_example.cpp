#include <spanstream>
#include <iostream>
#include <vector>

int main() {
    std::vector<char> buffer(10, ' ');
    std::ospanstream os(buffer);
    os << "Hello, world!";
    std::cout.write(buffer.data(), os.span().size());
    return 0;
}