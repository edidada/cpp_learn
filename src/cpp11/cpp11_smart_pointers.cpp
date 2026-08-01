#include <iostream>
#include <memory>
#include <vector>

class Resource {
public:
    Resource(int id) : id_(id) { 
        std::cout << "Resource " << id_ << " created" << std::endl; 
    }
    ~Resource() { 
        std::cout << "Resource " << id_ << " destroyed" << std::endl; 
    }
    void use() { 
        std::cout << "Using Resource " << id_ << std::endl; 
    }
    int getId() const { return id_; }
private:
    int id_;
};

void test_unique_ptr() {
    std::cout << "=== unique_ptr ===" << std::endl;
    
    std::unique_ptr<Resource> ptr1(new Resource(1));
    ptr1->use();
    
    std::unique_ptr<Resource> ptr2(new Resource(2));
    ptr2->use();
    
    std::unique_ptr<Resource> ptr3 = std::move(ptr1);
    if (!ptr1) {
        std::cout << "ptr1 is null after move" << std::endl;
    }
    ptr3->use();
    
    std::cout << "ptr3.get() = " << ptr3.get() << std::endl;
    
    Resource* raw = ptr3.release();
    std::cout << "After release, raw = " << raw << std::endl;
    delete raw;
}

void test_shared_ptr() {
    std::cout << "\n=== shared_ptr ===" << std::endl;
    
    std::shared_ptr<Resource> ptr1 = std::make_shared<Resource>(3);
    std::cout << "ptr1 use_count: " << ptr1.use_count() << std::endl;
    
    {
        std::shared_ptr<Resource> ptr2 = ptr1;
        std::cout << "ptr1 use_count (with ptr2): " << ptr1.use_count() << std::endl;
        
        std::shared_ptr<Resource> ptr3 = ptr1;
        std::cout << "ptr1 use_count (with ptr2, ptr3): " << ptr1.use_count() << std::endl;
    }
    
    std::cout << "ptr1 use_count (after scope): " << ptr1.use_count() << std::endl;
    
    auto ptr4 = std::make_shared<Resource>(4);
    ptr4 = ptr1;
    std::cout << "After assignment, ptr4 use_count: " << ptr4.use_count() << std::endl;
}

void test_weak_ptr() {
    std::cout << "\n=== weak_ptr ===" << std::endl;
    
    std::weak_ptr<Resource> weak;
    
    {
        auto shared = std::make_shared<Resource>(5);
        weak = shared;
        
        std::cout << "shared use_count: " << shared.use_count() << std::endl;
        std::cout << "weak expired: " << weak.expired() << std::endl;
        
        if (auto locked = weak.lock()) {
            std::cout << "Successfully locked, use_count: " << locked.use_count() << std::endl;
            locked->use();
        }
    }
    
    std::cout << "After shared destroyed, weak expired: " << weak.expired() << std::endl;
    
    if (auto locked = weak.lock()) {
        locked->use();
    } else {
        std::cout << "Failed to lock, resource expired" << std::endl;
    }
}

void test_shared_ptr_in_container() {
    std::cout << "\n=== shared_ptr in Container ===" << std::endl;
    
    std::vector<std::shared_ptr<Resource> > resources;
    
    resources.push_back(std::make_shared<Resource>(10));
    resources.push_back(std::make_shared<Resource>(11));
    resources.push_back(std::make_shared<Resource>(12));
    
    for (const auto& r : resources) {
        r->use();
    }
    
    std::cout << "Resources in vector: " << resources.size() << std::endl;
}

void test_custom_deleter() {
    std::cout << "\n=== Custom Deleter ===" << std::endl;
    
    auto deleter = [](Resource* r) {
        std::cout << "Custom deleter for Resource " << r->getId() << std::endl;
        delete r;
    };
    
    std::unique_ptr<Resource, decltype(deleter) > ptr(new Resource(20), deleter);
    ptr->use();
    
    auto sharedDeleter = [](Resource* r) {
        std::cout << "Shared custom deleter for Resource " << r->getId() << std::endl;
        delete r;
    };
    
    std::shared_ptr<Resource> sharedPtr(new Resource(21), sharedDeleter);
    sharedPtr->use();
}

int main() {
    test_unique_ptr();
    test_shared_ptr();
    test_weak_ptr();
    test_shared_ptr_in_container();
    test_custom_deleter();
    return 0;
}
