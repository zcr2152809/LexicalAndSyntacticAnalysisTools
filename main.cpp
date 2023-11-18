#include <iostream>
#include <vector>
#include <map>

int main() {
    // ����һ���� vector<int> Ϊ���� map
    std::map<std::vector<int>, std::string> myMap;

    // ����һЩ vector ��Ϊ��
    std::vector<int> key1 = { 1, 2, 3 };
    std::vector<int> key2 = { 4, 5, 6 };

    // �� map �в����ֵ��
    myMap[key1] = "Value 1";
    myMap[key2] = "Value 2";

    // ʹ�õ��������� map
    for (const auto& entry : myMap) {
        const std::vector<int>& key = entry.first;
        const std::string& value = entry.second;

        // �����ֵ��
        std::cout << "Key: ";
        for (int num : key) {
            std::cout << num << " ";
        }
        std::cout << ", Value: " << value << std::endl;
    }

    return 0;
}
