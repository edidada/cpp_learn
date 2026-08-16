#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Observer;

class Subject {
public:
    void registerObserver(std::shared_ptr<Observer> obs);
    void notifyObservers();
private:
    std::vector<std::weak_ptr<Observer>> observers_;
};

class Observer {
public:
    Observer(const std::string& name) : name_(name) {}
    void update() {
        std::cout << "Observer " << name_ << " notified!\n";
    }
private:
    std::string name_;
};

void Subject::registerObserver(std::shared_ptr<Observer> obs) {
    observers_.emplace_back(obs);
}

void Subject::notifyObservers() {
    for (auto& weakObs : observers_) {
        if (auto obs = weakObs.lock()) {
            obs->update();
        } else {
            std::cout << "Observer expired, removing...\n";
            // 在实际应用中应该从列表中移除失效的观察者
        }
    }
}

int main() {
    Subject subject;
    
    {
        auto observer1 = std::make_shared<Observer>("Observer1");
        subject.registerObserver(observer1);
        
        auto observer2 = std::make_shared<Observer>("Observer2");
        subject.registerObserver(observer2);
        
        subject.notifyObservers();
    } // observer1和observer2离开作用域
    
    subject.notifyObservers(); // 检测到观察者已失效
    
    return 0;
}