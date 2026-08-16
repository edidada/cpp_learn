#include <iostream>
#include <memory>
#include <cstdio>

// 自定义删除器用于FILE指针
struct FileDeleter {
    void operator()(FILE* file) const {
        if (file) {
            std::cout << "Closing file...\n";
            fclose(file);
        }
    }
};

using FilePtr = std::unique_ptr<FILE, FileDeleter>;

FilePtr openFile(const char* filename, const char* mode) {
    FILE* file = fopen(filename, mode);
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }
    return FilePtr(file);
}

int main() {
    try {
        auto file = openFile("example.txt", "w");
        if (file) {
            fprintf(file.get(), "Hello, custom deleter!\n");
            // 文件会在file离开作用域时自动关闭
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
