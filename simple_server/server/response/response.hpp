#pragma once
#include "../request/request.hpp"

namespace b_net {
    struct Field {
    private:
        std::string m_name;
        std::string m_value;

    public:
        Field(std::string name, std::string value)
            : m_name(name), m_value(value)
        { }

        const char* name() const { return m_name.c_str(); }
        const char* value() const { return m_value.c_str(); }
    };

    class Response {
    protected:
        unsigned char* m_body = nullptr;
        size_t m_size = 0;
        std::list<Field> m_set_fields;
        std::list<Field> m_ins_fields;

        size_t check(const char* body = nullptr)
        {
            uint64_t counter = 0;
            if (body == nullptr) return counter;

            while(body[counter] != '\0') { ++counter; }
            return counter;
        }
        size_t check(char* body = nullptr)
        {
            uint64_t counter = 0;
            if (body == nullptr) return counter;

            while(body[counter] != '\0') { ++counter; }
            return counter;
        }

        void initialize(size_t size, const char* body)
        {
            m_size = size;
            if(m_size > 0) m_body = new unsigned char[size];
            else m_body = nullptr;

            for (unsigned long long i = 0; i < size; ++i)
            { m_body[i] = body[i]; }
        }

        void initialize(size_t size, unsigned char* body)
        {
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
        Response& clear()
        {
            delete[] m_body;
            m_body = nullptr;
            m_size = 0;
            m_set_fields.clear();
            m_ins_fields.clear();

            return *this;
        }

        // Const char response body
        void body(const char* body, BodyType type = JSON)
        {
            using namespace utility_;
            initialize(check(body), body);
            content_type(mime_type(bodyType_to_mimeType(type)));
        }
        // Array of chars response body
        void body(char* body, BodyType type = JSON)
        {
            using namespace utility_;
            initialize(check(body), body);
            content_type(mime_type(bodyType_to_mimeType(type)));
        }
        // String response body
        void body(std::string body, BodyType type = JSON)
        {
            using namespace utility_;
            initialize(body.length(), body.c_str());
            content_type(mime_type(bodyType_to_mimeType(type)));
        }
        // Binary body. You need to point number of body octets.
        void body(char* body, size_t size, BodyType type = BINARY)
        {
            using namespace utility_;
            initialize(size, body);
            content_type(mime_type(bodyType_to_mimeType(type)));
        }
        // Binary body. You need to point number of body octets.
        void body(const char* body, size_t size, BodyType type = BINARY)
        {
            using namespace utility_;
            initialize(size, body);
            content_type(mime_type(bodyType_to_mimeType(type)));
        }
        // file body, write a path to the file
        [[nodiscard]] b_net::error_code file_body(std::string path)
        {
            std::filesystem::path p(path);

            if (!std::filesystem::exists(p))
            {
                return b_net::error_code
                    (b_net::status::FILE_NOT_FOUND, "File not found");
            }

            std::ifstream file(p.c_str(), std::ios::binary);
            unsigned char* data_ptr = nullptr;
            size_t file_size = 0;

            if (file.is_open())
            {
                file.seekg(0, std::ios::end);
                file_size = file.tellg();
                file.seekg(0, std::ios::beg);
                data_ptr = new unsigned char[file_size];
                file.read(reinterpret_cast<char*>(data_ptr), file_size);
                file.close();
            }
            else
            {
                return b_net::error_code
                    (b_net::status::CANT_OPEN_FILE, "Can't open the file");
            }

            initialize(file_size, data_ptr);
            content_type(utility_::mime_type(path));

            return b_net::error_code
                (b_net::status::OK, "Body was filled with the file octets");
        }

        // Set http header fields
        void set(std::string name, std::string value)
        {
            m_set_fields.push_back(Field(name, value));
        }
        void insert(std::string name, std::string value)
        {
            m_ins_fields.push_back(Field(name, value));
        }

        // Set content type for binary file
        void content_type(std::string text)
        {
            m_set_fields.push_back(Field("Content-Type", text));
        }

        friend class utility_class;
    };
}