#pragma once
#include "lib_core_dependencies.hpp"

namespace b_net {
    enum status {
        OK,
        FILE_NOT_FOUND,
        CANT_OPEN_FILE
    };

    // Custom error code class to write status of some operations 
    // Without throwing an exception
    class error_code {
    protected:
        status m_status = status::OK;
        char* m_message = nullptr;
    public:
        error_code(status st_, const char* str) : m_status(st_)
        {
            unsigned long long length = 0;
            char tempSymb = str[length];
            while (tempSymb != '\0') {
                tempSymb = str[++length];
            }
            m_message = new char[length];
            for (int i = 0; i < length; ++i) {
                m_message[i] = str[i];
            }
        }

        // Getters
        status get_status() const { return m_status; }
        const char* message() const { return m_message; }
    };
}