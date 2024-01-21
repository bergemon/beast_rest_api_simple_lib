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
    protected:
        BodyType m_type = BodyType::TEXT;
        unsigned char* m_body;
        size_t m_size = 0;
        std::list<Field> m_setFields;
        std::list<Field> m_insertFields;

    private:
        size_t check(const char* body) {
            uint64_t counter = 0;
            while(body[counter] != '\0') { ++counter; }
            return ++counter;
        }
        size_t check(char* body) {
            uint64_t counter = 0;
            while(body[counter] != '\0') { ++counter; }
            return ++counter;
        }

        void initialize(size_t size, const char* body) {
            m_size = size;
            m_body = new unsigned char[size];
            for (unsigned long long i = 0; i < size; ++i)
            { m_body[i] = body[i]; }
        }

        void initialize(size_t size, unsigned char* body) {
            m_size = size;
            m_body = new unsigned char[size];
            for (unsigned long long i = 0; i < size; ++i)
            { m_body[i] = body[i]; }
        }

    public:
        // default constructor
        Response() { }
        // copying constructor for child class
        Response(const Response& res) : m_type(res.m_type),
            m_setFields(res.m_setFields), m_insertFields(res.m_insertFields)
        { initialize(res.m_size, res.m_body); }
        // array of chars body
        Response(const char* body) : m_type(BodyType::TEXT)
        { initialize(check(body), body); }
        // const char body
        Response(BodyType type, char* body) : m_type(type)
        { initialize(check(body), body); }
        // string body
        Response(std::string body) : m_type(BodyType::TEXT)
        { initialize(check(body.c_str()), body.c_str()); }
        // ~Response() { delete[] m_body; }

        // set body type
        void bodyType(BodyType type) { m_type = type; }

        // array of chars body
        void body(const char* body) { initialize(check(body), body); }
        // const char body
        void body(char* body) { initialize(check(body), body); }
        // if we need to set size before
        void body(char* body, size_t size) { initialize(size, body); }
        void body(const char* body, size_t size) { initialize(size, body); }
        // string body
        void body(std::string body) { initialize(check(body.c_str()), body.c_str()); }

        void set(std::string name, std::string value) {
            m_setFields.push_back(Field(name, value));
        }
        void insert(std::string name, std::string value) {
            m_insertFields.push_back(Field(name, value));
        }
    };

    class utility_class : private Response {
    public:
        utility_class(const Response& res)
            : Response(res)
        { }

        // Getters for handler
        BodyType getType() { return m_type; }
        unsigned char* getBody() { return m_body; }
        // Body length
        size_t getSize() { return m_size; }
        // List of fields to set
        std::list<Field>& getSetFields()
        { return m_setFields; }
        // List of field to insert
        std::list<Field>& getInsertFields()
        { return m_insertFields; }
    };
}

using b_net::BodyType::BINARY;
using b_net::BodyType::TEXT;