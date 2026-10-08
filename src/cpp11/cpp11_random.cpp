#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <iomanip>

void test_random_device() {
    std::cout << "=== Random Device ===" << std::endl;
    
    std::random_device rd;
    std::cout << "Random device entropy: " << rd.entropy() << std::endl;
    std::cout << "Random values: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << rd() << " ";
    }
    std::cout << std::endl;
}

void test_mersenne_twister() {
    std::cout << "\n=== Mersenne Twister ===" << std::endl;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::cout << "mt19937 values: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << gen() << " ";
    }
    std::cout << std::endl;
    
    std::mt19937_64 gen64(rd());
    std::cout << "mt19937_64 values: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << gen64() << " ";
    }
    std::cout << std::endl;
}

void test_uniform_int_distribution() {
    std::cout << "\n=== Uniform Int Distribution ===" << std::endl;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    
    std::cout << "Random integers [1, 100]: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << dis(gen) << " ";
    }
    std::cout << std::endl;
    
    std::uniform_int_distribution<> dice(1, 6);
    std::cout << "Dice rolls: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << dice(gen) << " ";
    }
    std::cout << std::endl;
}

void test_uniform_real_distribution() {
    std::cout << "\n=== Uniform Real Distribution ===" << std::endl;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Random doubles [0.0, 1.0): ";
    for (int i = 0; i < 5; ++i) {
        std::cout << dis(gen) << " ";
    }
    std::cout << std::endl;
    
    std::uniform_real_distribution<> dis2(-10.0, 10.0);
    std::cout << "Random doubles [-10.0, 10.0): ";
    for (int i = 0; i < 5; ++i) {
        std::cout << dis2(gen) << " ";
    }
    std::cout << std::endl;
}

void test_normal_distribution() {
    std::cout << "\n=== Normal Distribution ===" << std::endl;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dis(5.0, 2.0);
    
    std::cout << "Normal distribution (mean=5, stddev=2): ";
    for (int i = 0; i < 10; ++i) {
        std::cout << dis(gen) << " ";
    }
    std::cout << std::endl;
    
    std::vector<int> histogram(10, 0);
    for (int i = 0; i < 1000; ++i) {
        double val = dis(gen);
        int bin = static_cast<int>(val);
        if (bin >= 0 && bin < 10) {
            histogram[bin]++;
        }
    }
    
    std::cout << "Histogram (1000 samples):" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << i << ": " << std::string(histogram[i] / 10, '*') << std::endl;
    }
}

void test_other_distributions() {
    std::cout << "\n=== Other Distributions ===" << std::endl;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::bernoulli_distribution bernoulli(0.7);
    std::cout << "Bernoulli (p=0.7): ";
    for (int i = 0; i < 10; ++i) {
        std::cout << bernoulli(gen) << " ";
    }
    std::cout << std::endl;
    
    std::binomial_distribution<> binomial(10, 0.5);
    std::cout << "Binomial (n=10, p=0.5): ";
    for (int i = 0; i < 10; ++i) {
        std::cout << binomial(gen) << " ";
    }
    std::cout << std::endl;
    
    std::poisson_distribution<> poisson(4.0);
    std::cout << "Poisson (mean=4): ";
    for (int i = 0; i < 10; ++i) {
        std::cout << poisson(gen) << " ";
    }
    std::cout << std::endl;
    
    std::exponential_distribution<> exponential(1.0);
    std::cout << "Exponential (lambda=1): ";
    for (int i = 0; i < 5; ++i) {
        std::cout << exponential(gen) << " ";
    }
    std::cout << std::endl;
}

void test_shuffle() {
    std::cout << "\n=== Shuffle ===" << std::endl;
    
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::cout << "Original: ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;
    
    std::shuffle(v.begin(), v.end(), gen);
    
    std::cout << "Shuffled: ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;
}

void test_seed_sequence() {
    std::cout << "\n=== Seed Sequence ===" << std::endl;
    
    std::seed_seq seq{1, 2, 3, 4, 5};
    std::mt19937 gen(seq);
    
    std::cout << "Seeded values: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << gen() << " ";
    }
    std::cout << std::endl;
    
    std::vector<std::uint32_t> seeds(10);
    seq.generate(seeds.begin(), seeds.end());
    
    std::cout << "Generated seeds: ";
    for (auto s : seeds) {
        std::cout << s << " ";
    }
    std::cout << std::endl;
}

int main() {
    test_random_device();
    test_mersenne_twister();
    test_uniform_int_distribution();
    test_uniform_real_distribution();
    test_normal_distribution();
    test_other_distributions();
    test_shuffle();
    test_seed_sequence();
    return 0;
}
