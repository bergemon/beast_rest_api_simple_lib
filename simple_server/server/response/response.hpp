#pragma once
#include "../request/request.hpp"

namespace b_net {

    enum BodyType { TEXT, BINARY };

    struct Field {
    private:
        std::string m_name;
        std::string m_value;

    public:
        Field(std::string name, std::string value)
            : m_name(name), m_value(value)
        { }

        const char* getName() const { return m_name.c_str(); }
        const char* getValue() const { return m_value.c_str(); }
    };

    class Response {
    protected:
        BodyType m_type = BodyType::TEXT;
        unsigned char* m_body = nullptr;
        size_t m_size = 0;
        std::list<Field> m_setFields;
        std::list<Field> m_insertFields;
        std::string m_contentType;

        size_t check(const char* body = nullptr) {
            uint64_t counter = 0;
            if (body == nullptr) return counter;

            while(body[counter] != '\0') { ++counter; }
            return ++counter;
        }
        size_t check(char* body = nullptr) {
            uint64_t counter = 0;
            if (body == nullptr) return counter;

            while(body[counter] != '\0') { ++counter; }
            return ++counter;
        }

        void initialize(size_t size, const char* body) {
            m_size = size;
            if(m_size > 0) m_body = new unsigned char[size];
            else m_body = nullptr;

            for (unsigned long long i = 0; i < size; ++i)
            { m_body[i] = body[i]; }
        }

        void initialize(size_t size, unsigned char* body) {
            m_size = size;
            if(m_size > 0) m_body = new unsigned char[size];
            else m_body = nullptr;

            for (unsigned long long i = 0; i < size; ++i)
            { m_body[i] = body[i]; }
        }

    public:
        // Method to free memory from session deconstructor
        // instead of doing it in these class
        // to avoid of double freeing memory
        // Or if you want to erase class from everywhere else
        Response& clear() {
            delete[] m_body;
            m_body = nullptr;
            m_size = 0;
            m_setFields.clear();
            m_insertFields.clear();

            return *this;
        }

        // const char body
        void body(const char* body) {
            initialize(check(body), body);
            m_type = TEXT;
        }
        // array of chars body
        void body(char* body) {
            initialize(check(body), body);
            m_type = TEXT;
        }
        // string body
        void body(std::string body) {
            initialize(body.length() + 1, body.c_str());
            m_type = TEXT;
        }
        // For binary body. You need to point number of body octets.
        void body(
            char* body,
            size_t size,
            BodyType type = TEXT
        )
        {
            initialize(size, body);
            m_type = type;
        }
        void body(
            const char* body,
            size_t size,
            BodyType type = TEXT
        )
        {
            initialize(size, body);
            m_type = type;
        }
        // file body, write a path to the file
        [[nodiscard]] b_net::error_code file_body(std::string path)
        {
            std::filesystem::path p(path);

            if (!std::filesystem::exists(p)) {
                return b_net::error_code
                    (b_net::status::FILE_NOT_FOUND, "File not found");
            }

            std::ifstream file(p.c_str());
            unsigned char* data_ptr = nullptr;
            size_t file_size = 0;

            if (file.is_open()) {
                file.seekg(0, std::ios::end);
                file_size = file.tellg();
                file.seekg(0, std::ios::beg);
                data_ptr = new unsigned char[file_size];
                file.read(reinterpret_cast<char*>(data_ptr), file_size);
                file.close();
            }
            else {
                return b_net::error_code
                    (b_net::status::CANT_OPEN_FILE, "Can't open the file");
            }

            initialize(file_size, data_ptr);
            m_type = BINARY;
            content_type(utility_::mime_type(path));

            return b_net::error_code
                (b_net::status::OK, "Body was filled with the file octets");
        }

        void set(std::string name, std::string value) {
            m_setFields.push_back(Field(name, value));
        }
        void insert(std::string name, std::string value) {
            m_insertFields.push_back(Field(name, value));
        }

        // Set content type for binary file
        void content_type(const char* text)
        { m_setFields.push_back(Field("Content-Type", text)); }
        void content_type(std::string text)
        { m_setFields.push_back(Field("Content-Type", text)); }

        friend class utility_class;
    };
}

using b_net::BodyType::BINARY;
using b_net::BodyType::TEXT;