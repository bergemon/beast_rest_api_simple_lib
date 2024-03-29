#pragma once
#include "../request/request.hpp"

namespace b_net {
    class Response final {
        struct HeaderField {
        private:
            std::string m_name;
            std::string m_value;

        public:
            HeaderField(std::string name, std::string value)
                : m_name(name), m_value(value)
            { }

            const char* name() const { return m_name.c_str(); }
            const char* value() const { return m_value.c_str(); }
        };

    protected:
        unsigned char* m_body = nullptr;
        size_t m_size = 0;
        std::list<HeaderField> m_set_fields;
        std::list<HeaderField> m_ins_fields;
        
        ////////////////////////////////////////////
        // Getters and friendly funtion for them //
        ///////////////////////////////////////////
        // Get pointer to body
        [[nodiscard]] unsigned char* get_body() { return m_body; }
        // Body length
        [[nodiscard]] size_t get_size() { return m_size; }
        // List of fields to set
        [[nodiscard]]
            std::list<HeaderField>& set_fields() { return m_set_fields; }
        // List of field to insert
        [[nodiscard]]
            std::list<HeaderField>& insert_fields() { return m_ins_fields; }
        // Boost Beast response creator friendly function
        friend http::message_generator create_response(Response&, const uint32_t, const bool, const method);

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
        void body(const char* body, body_type type = JSON)
        {
            using namespace utility_;
            initialize(check(body), body);
            content_type(mime_type(bodyType_to_mimeType(type)));
        }
        // Array of chars response body
        void body(char* body, body_type type = JSON)
        {
            using namespace utility_;
            initialize(check(body), body);
            content_type(mime_type(bodyType_to_mimeType(type)));
        }
        // String response body
        void body(std::string body, body_type type = JSON)
        {
            using namespace utility_;
            initialize(body.length(), body.c_str());
            content_type(mime_type(bodyType_to_mimeType(type)));
        }
        // Binary body. You need to point number of body octets.
        void body(char* body, size_t size, body_type type = BINARY)
        {
            using namespace utility_;
            initialize(size, body);
            content_type(mime_type(bodyType_to_mimeType(type)));
        }
        // Binary body. You need to point number of body octets.
        void body(const char* body, size_t size, body_type type = BINARY)
        {
            using namespace utility_;
            initialize(size, body);
            content_type(mime_type(bodyType_to_mimeType(type)));
        }
        // file body, write a path to the file
        [[nodiscard]] b_net::error_code file_body(const std::string path)
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

        // Set http header field. The value can be an empty string.
        //It will replace last field with the same name.
        // For purpose of setting another field with the same name use insert method.
        void set(std::string name, std::string value)
        {
            m_set_fields.emplace_back(name, value);
        }
        // Insert a field. If one or more fields with the same name already exist,
        // the new field will be inserted after the last field with the matching name
        // in serialization order. The value can be an empty string.
        void insert(std::string name, std::string value)
        {
            m_ins_fields.emplace_back(name, value);
        }

        // Set content type for binary file
        void content_type(std::string text)
        {
            m_set_fields.emplace_back("Content-Type", text);
        }
    };
}