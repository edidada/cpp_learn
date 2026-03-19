#include <iostream>
#include <regex>
#include <string>

void test_basic_regex() {
    std::cout << "=== Basic Regex ===" << std::endl;
    
    std::string text = "Hello, World! 123";
    std::regex pattern("[A-Za-z]+");
    
    std::smatch matches;
    if (std::regex_search(text, matches, pattern)) {
        std::cout << "Found: " << matches.str() << std::endl;
    }
    
    std::string text2 = "The answer is 42";
    std::regex numPattern("\\d+");
    if (std::regex_search(text2, matches, numPattern)) {
        std::cout << "Number found: " << matches.str() << std::endl;
    }
}

void test_regex_match() {
    std::cout << "\n=== Regex Match ===" << std::endl;
    
    std::regex emailPattern("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
    
    std::string email1 = "user@example.com";
    std::string email2 = "invalid-email";
    
    std::cout << email1 << " is " 
              << (std::regex_match(email1, emailPattern) ? "valid" : "invalid") 
              << std::endl;
    std::cout << email2 << " is " 
              << (std::regex_match(email2, emailPattern) ? "valid" : "invalid") 
              << std::endl;
    
    std::regex phonePattern("\\d{3}-\\d{3}-\\d{4}");
    std::string phone = "123-456-7890";
    std::cout << phone << " is " 
              << (std::regex_match(phone, phonePattern) ? "valid" : "invalid") 
              << std::endl;
}

void test_regex_search() {
    std::cout << "\n=== Regex Search ===" << std::endl;
    
    std::string text = "The quick brown fox jumps over the lazy dog";
    std::regex wordPattern("\\b\\w{4}\\b");
    
    std::smatch matches;
    std::string::const_iterator searchStart(text.cbegin());
    
    std::cout << "4-letter words: ";
    while (std::regex_search(searchStart, text.cend(), matches, wordPattern)) {
        std::cout << matches.str() << " ";
        searchStart = matches.suffix().first;
    }
    std::cout << std::endl;
}

void test_regex_replace() {
    std::cout << "\n=== Regex Replace ===" << std::endl;
    
    std::string text = "Hello, World! Hello, C++!";
    std::regex pattern("Hello");
    
    std::string result = std::regex_replace(text, pattern, "Hi");
    std::cout << "Original: " << text << std::endl;
    std::cout << "Replaced: " << result << std::endl;
    
    std::string text2 = "123-456-7890";
    std::regex dashPattern("-");
    std::string result2 = std::regex_replace(text2, dashPattern, ".");
    std::cout << "Phone format: " << result2 << std::endl;
    
    std::string text3 = "  Multiple   spaces   here  ";
    std::regex spacePattern("\\s+");
    std::string result3 = std::regex_replace(text3, spacePattern, " ");
    std::cout << "Spaces normalized: " << result3 << std::endl;
}

void test_capture_groups() {
    std::cout << "\n=== Capture Groups ===" << std::endl;
    
    std::string text = "John: 25, Jane: 30, Bob: 35";
    std::regex pattern("(\\w+): (\\d+)");
    
    std::smatch matches;
    std::string::const_iterator searchStart(text.cbegin());
    
    while (std::regex_search(searchStart, text.cend(), matches, pattern)) {
        std::cout << "Name: " << matches[1].str() 
                  << ", Age: " << matches[2].str() << std::endl;
        searchStart = matches.suffix().first;
    }
}

void test_regex_iterator() {
    std::cout << "\n=== Regex Iterator ===" << std::endl;
    
    std::string text = "abc 123 def 456 ghi 789";
    std::regex pattern("\\d+");
    
    std::sregex_iterator begin(text.begin(), text.end(), pattern);
    std::sregex_iterator end;
    
    std::cout << "Numbers found: ";
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::cout << match.str() << " ";
    }
    std::cout << std::endl;
}

void test_regex_token_iterator() {
    std::cout << "\n=== Regex Token Iterator ===" << std::endl;
    
    std::string text = "apple,banana;cherry:date";
    std::regex delimiter("[,;:]");
    
    std::sregex_token_iterator begin(text.begin(), text.end(), delimiter, -1);
    std::sregex_token_iterator end;
    
    std::cout << "Tokens: ";
    for (std::sregex_token_iterator i = begin; i != end; ++i) {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
}

void test_regex_flags() {
    std::cout << "\n=== Regex Flags ===" << std::endl;
    
    std::string text = "Hello\nWorld";
    
    std::regex pattern1("Hello.World");
    std::cout << "Default (no match with newline): " 
              << (std::regex_search(text, pattern1) ? "match" : "no match") << std::endl;
    
    std::string text2 = "HELLO world";
    std::regex pattern3("hello", std::regex::icase);
    std::cout << "Case insensitive: " 
              << (std::regex_search(text2, pattern3) ? "match" : "no match") << std::endl;
}

int main() {
    test_basic_regex();
    test_regex_match();
    test_regex_search();
    test_regex_replace();
    test_capture_groups();
    test_regex_iterator();
    test_regex_token_iterator();
    test_regex_flags();
    return 0;
}
