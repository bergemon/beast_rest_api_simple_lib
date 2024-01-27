#pragma once
#include "../utility/utilityFunctions.hpp"

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
        BodyType m_type = BodyType::TEXT;
        unsigned char* m_body = nullptr;
        size_t m_size = 0;
        std::list<Field> m_setFields;
        std::list<Field> m_insertFields;

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

            for (unsigned long long i = 0; i < size; ++i)
            { m_body[i] = body[i]; }
        }

        void initialize(size_t size, unsigned char* body) {
            m_size = size;
            if(m_size > 0) m_body = new unsigned char[size];

            for (unsigned long long i = 0; i < size; ++i)
            { m_body[i] = body[i]; }
        }

    public:
        // deconstructor
        ~Response() { delete[] m_body; }

        // const char body
        void body(const char* body)
        {
            initialize(check(body), body);
            m_type = TEXT;
        }
        // array of chars body
        void body(char* body)
        {
            initialize(check(body), body);
            m_type = TEXT;
        }
        // For binary body. You need to point number of body octets.
        void body(char* body, size_t size,
            BodyType type = TEXT)
        {
            initialize(size, body);
            m_type = type;
        }
        void body(const char* body, size_t size,
            BodyType type = TEXT)
        {
            initialize(size, body);
            m_type = type;
        }
        // string body
        void body(std::string body)
        {
            initialize(body.length() + 1, body.c_str());
            m_type = TEXT;
        }

        void set(std::string name, std::string value) {
            m_setFields.push_back(Field(name, value));
        }
        void insert(std::string name, std::string value) {
            m_insertFields.push_back(Field(name, value));
        }

        friend class utility_class;
    };

    class utility_class {
    public:
        // Getters for handler
        BodyType getType(const Response& res) { return res.m_type; }
        unsigned char* getBody(const Response& res) { return res.m_body; }
        // Body length
        size_t getSize(const Response& res) { return res.m_size; }
        // List of fields to set
        std::list<Field>& getSetFields(Response& res)
        { return res.m_setFields; }
        // List of field to insert
        std::list<Field>& getInsertFields(Response& res)
        { return res.m_insertFields; }
    };
}

using b_net::BodyType::BINARY;
using b_net::BodyType::TEXT;