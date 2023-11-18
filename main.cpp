#include <iostream>
#include <vector>
#include <map>

int main() {
    // 定义一个以 vector<int> 为键的 map
    std::map<std::vector<int>, std::string> myMap;

    // 创建一些 vector 作为键
    std::vector<int> key1 = { 1, 2, 3 };
    std::vector<int> key2 = { 4, 5, 6 };

    // 在 map 中插入键值对
    myMap[key1] = "Value 1";
    myMap[key2] = "Value 2";

    // 使用迭代器遍历 map
    for (const auto& entry : myMap) {
        const std::vector<int>& key = entry.first;
        const std::string& value = entry.second;

        // 输出键值对
        std::cout << "Key: ";
        for (int num : key) {
            std::cout << num << " ";
        }
        std::cout << ", Value: " << value << std::endl;
    }

    return 0;
}
