#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <netinet/in.h>

using namespace std;

int randomScore() {
    return rand() % 5 + 1;
}

string buildJsonResponse() {
    int accuracy = randomScore();
    int tone = randomScore();
    int completeness = randomScore();
    int finalScore = accuracy + tone + completeness;

    stringstream ss;
    ss << "{"
       << "\"results\": ["
       << "{"
       << "\"prompt\": \"Sample Prompt\","
       << "\"response\": \"Mock response generated\","
       << "\"accuracy\": " << accuracy << ","
       << "\"tone\": " << tone << ","
       << "\"completeness\": " << completeness << ","
       << "\"final_score\": " << finalScore
       << "}"
       << "],"
       << "\"best_prompt_index\": 0"
       << "}";

    return ss.str();
}

int main() {
    srand(time(0));

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    cout << "C++ Backend running on http://localhost:8080\n";

    while (true) {
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr*)&address,
                                (socklen_t*)&addrlen);

        char buffer[30000] = {0};
        read(new_socket, buffer, 30000);

        string responseBody = buildJsonResponse();

        string httpResponse =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Length: " + to_string(responseBody.length()) + "\r\n\r\n" +
            responseBody;

        send(new_socket, httpResponse.c_str(), httpResponse.length(), 0);
        close(new_socket);
    }

    return 0;
}
