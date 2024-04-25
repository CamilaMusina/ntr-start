#include <iostream>

int main() {
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    auto ptr1 = arr; // auto для варьирования типа
    std::cout << *(ptr1 + 3) << std::endl;
    
    void* ptr2 = arr; // указатель на void, приведение к типу через разыменовывание static_cast
    std::cout << *(static_cast<int*>(ptr2) + 3) << std::endl;
    
    return 0;
}
