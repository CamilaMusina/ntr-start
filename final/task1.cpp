#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include <thread>
#include <mutex>

struct Student {
    std::string name;
    int age;
};

class StudentDB{
public:
    void addStudent(int id, const std::string& name, int age);
    void deleteStudent(int id);
    std::shared_ptr<Student> getById(int id);
    
    void write();
    void read();

private:
    std::unordered_map<int, std::shared_ptr<Student>> students;
    std::mutex mtx;
};

void StudentDB::addStudent(int id, const std::string& name, int age) {
    auto student = std::make_shared<Student>(Student{name, age});
    this->students[id] = student;
}

void StudentDB::deleteStudent(int id) {
    this->students.erase(id);
}

std::shared_ptr<Student> StudentDB::getById(int id) {
    if (this->students.find(id) != this->students.end()) 
        return this->students[id];
    return nullptr;
}

void StudentDB::write() {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Enter student data (ID, name, age) or type 'exit' " << std::endl;
    
    int id;
    std::string name;
    int age;
    
    while(true){
        if (!(std::cin >> id))
            break;
        
        std::cin >> name >> age;
        addStudent(id, name, age);
        
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void StudentDB::read(){
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Students Database:" << std::endl;
    for (const auto& pair : students)
        std::cout << "ID: " << pair.first << ", Name: " << pair.second->name 
        << ", Age: " << pair.second->age << std::endl;
}

int main() {
    StudentDB db;
    
    std::thread writerThread(&StudentDB::write, &db);
    std::thread readerThread(&StudentDB::read, &db);
    
    writerThread.join();
    readerThread.join();
    
    return 0;
}
