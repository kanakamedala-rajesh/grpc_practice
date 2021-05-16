#include <iostream>
#include "HelloWorld.pb.h"
#include "Users.grpc.pb.h"

int main() {
    entities::HelloRequest helloRequest;
    helloRequest.set_requestmessage("Hello World");

    std::cout << "Request message: " << helloRequest.requestmessage() << std::endl;

    entities::Users users;
    users.set_id(1);
    users.set_username("Rajesh542");


    std::cout << "UserName: " << users.has_firstname() << std::endl;

    return 0;
}
