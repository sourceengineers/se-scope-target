

#ifndef SE_SCOPE_FRAMEDIOTESTFUNCTIONS_H
#define SE_SCOPE_FRAMEDIOTESTFUNCTIONS_H

#include <vector>
#include <string>

extern "C" {
#include <Scope/Core/ScopeTypes.h>
}

static void writeLength(uint32_t length, std::vector<uint8_t> &container){
    for(size_t i = 0; i < 4; ++i){
        uint8_t byte = (length >> (3 - i) * 8) & 0xFF;
        container.push_back(byte);
    }
}

static void writeChecksum(std::vector<uint8_t> data, std::vector<uint8_t> &container){

    uint16_t checksum = 0;
    for(size_t i = 0; i < data.size(); ++i){
        uint8_t byte = data[i];
        checksum += byte;
    }

    container.push_back((checksum & 0xFF00) >> 8);
    container.push_back(checksum & 0xFF);
}

static void writeChecksum(uint8_t* data, size_t length, std::vector<uint8_t> &container){

    uint16_t checksum = 0;
    for(size_t i = 0; i < length; ++i){
        uint8_t byte = data[i];
        checksum += byte;
    }

    container.push_back((checksum & 0xFF00) >> 8);
    container.push_back(checksum & 0xFF);
}

static MessageType extractMessageType(std::vector<uint8_t> &container){
    return (MessageType) container[1];
}

static std::vector<uint8_t> prepareMessage(MessageType type, std::vector<uint8_t> message){

    std::vector<uint8_t> v_i;
    v_i.push_back('[');
    v_i.push_back(type);
    writeLength(message.size(), v_i);

    for(int i = 0; i < message.size(); ++i){
        v_i.push_back(message[i]);
    }

    writeChecksum(message, v_i);
    v_i.push_back(']');

    return v_i;
}

static std::vector<uint8_t> prepareMessage(MessageType type, std::string message){

    std::vector<uint8_t> v_i;
    v_i.push_back('[');
    v_i.push_back(type);
    writeLength(message.size(), v_i);

    for(int i = 0; i < message.size(); ++i){
        v_i.push_back(message.c_str()[i]);
    }

    writeChecksum((uint8_t*) message.c_str(), message.size(), v_i);
    v_i.push_back(']');

    return v_i;
}


#endif //SE_SCOPE_FRAMEDIOTESTFUNCTIONS_H
