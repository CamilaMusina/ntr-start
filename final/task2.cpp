#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <queue>

constexpr int BUFFER_SIZE = 1024;
constexpr int PORT = 8080;

int main() {
    // Объявляем переменные
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    std::queue<std::string> messageQueue;
    
    // Создаем сокет
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }
    
    // Сетевой адрес сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
    
    // Привязка сокета к адресу
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Bind failed" << std::endl;
        return 1;
    }
    
    // Подключение клиента
    if (listen(server_fd, 5) == -1) {
        std::cerr << "Listen failed" << std::endl;
        return 1;
    }
    
    std::cout << "Server is listening on port: " << PORT << std::endl;
    
    while(true) {
        // Принятие подключения
        if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len)) == -1) {
            std::cerr << "Accept failed" << std::endl;
            return 1;
        }
        
        std::cout << "Client connected" << std::endl;
        
        while (true) {
            // Чтение данных от клиента
            int bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
            if (bytes_read == -1) {
                std::cerr << "Recv failed" << std::endl;
                break;
            } else if (bytes_read == 0) {
                std::cout << "Client disconnected" << std::endl;
                break;
            }
            
            // Отправка подтверждения клиенту
            if (send(client_fd, buffer, bytes_read, 0) == -1) {
                std::cerr << "Send failed" << std::endl;
                break;
            }
            
            // Добавление и обработка сообщения в циклическом буфере
            messageQueue.push(std::string(buffer, bytes_read));
            if (messageQueue.size() > 5) {
                std::cout << "Processing messages: " << std::endl;
                while(!messageQueue.empty()) {
                    std::cout << messageQueue.front() << std::endl;
                    messageQueue.pop();
                }
            }
            
            // Обнуление буфера
            memset(buffer, 0, BUFFER_SIZE);
        }
        
        close(client_fd);
    }
    
    close(server_fd);
    
    return 0;
}
