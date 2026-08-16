#include <iostream>
#include <memory>
#include <string>

class Resource {
public:
    Resource(const std::string& name) : name_(name) {
        std::cout << "Resource " << name_ << " created\n";
    }
    ~Resource() {
        std::cout << "Resource " << name_ << " destroyed\n";
    }
    void use() {
        std::cout << "Using resource " << name_ << std::endl;
    }
private:
    std::string name_;
};

// 接收独占所有权
void takeOwnership(std::unique_ptr<Resource> res) {
    res->use();
    // 函数结束时res会被自动销毁
}

int main() {
    auto resource = std::make_unique<Resource>("UniqueResource");
    
    // 转移所有权给函数
    takeOwnership(std::move(resource));
    
    // 此时resource已经是nullptr
    if (!resource) {
        std::cout << "Resource ownership was transferred\n";
    }
    
    return 0;
}
