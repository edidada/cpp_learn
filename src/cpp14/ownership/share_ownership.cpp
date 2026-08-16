#include <iostream>
#include <memory>
#include <string>

class SharedResource {
public:
    SharedResource(const std::string& name) : name_(name) {
        std::cout << "SharedResource " << name_ << " created\n";
    }
    ~SharedResource() {
        std::cout << "SharedResource " << name_ << " destroyed\n";
    }
    void use() {
        std::cout << "Using shared resource " << name_ << std::endl;
    }
private:
    std::string name_;
};

void shareOwnership(std::shared_ptr<SharedResource> res) {
    res->use();
    std::cout << "Use count inside function: " << res.use_count() << std::endl;
}

int main() {
    auto sharedRes = std::make_shared<SharedResource>("SharedResource");
    
    std::cout << "Use count before sharing: " << sharedRes.use_count() << std::endl;
    
    // 共享所有权
    shareOwnership(sharedRes);
    
    std::cout << "Use count after sharing: " << sharedRes.use_count() << std::endl;
    
    return 0;
}