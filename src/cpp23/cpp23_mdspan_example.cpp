#include <mdspan>
#include <vector>

int main() {
    std::vector<double> data(6);
    for(int i = 0; i < 6; ++i) data[i] = i;

    std::mdspan<double, std::dextents<size_t, 2>> m(data.data(), 2, 3);

    for(size_t i = 0; i < m.extent(0); ++i)
        for(size_t j = 0; j < m.extent(1); ++j)
            std::cout << "m[" << i << "," << j << "] = " << m(i,j) << "\n";

    return 0;
}