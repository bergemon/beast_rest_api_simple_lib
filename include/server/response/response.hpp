#pragma once
#include "../utility/utilityFunctions.hpp"

namespace bergemon {

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
        char* m_body;
        size_t m_size;
        std::list<Field> m_setFields;
        std::list<Field> m_insertFields;

    private:
        size_t check(const char* body) {
            uint32_t counter = 0;
            while(body[counter] != '\0') { ++counter; }
            return counter;
        }
        size_t check(char* body) {
            uint32_t counter = 0;
            while(body[counter] != '\0') { ++counter; }
            return counter;
        }

        void initialize(size_t size, const char* body) {
            m_size = size;
            for (int i = 0; i < (int(size) - 1); ++i)
            { m_body[i] = body[i]; }
        }

        void initialize(size_t size, char* body) {
            m_size = size;
            for (int i = 0; i < (int(size) - 1); ++i)
            { m_body[i] = body[i]; }
        }

    public:
        // default constructor
        Response() { }
        // copying constructor for child class
        Response(const Response& res) : m_type(res.m_type)
        { initialize(check(res.m_body), res.m_body); }
        // array of chars body
        Response(BodyType type, const char* body) : m_type(type)
        { initialize(check(body), body); }
        // const char body
        Response(BodyType type, char* body) : m_type(type)
        { initialize(check(body), body); }
        // string body
        Response(std::string body) : m_type(BodyType::TEXT)
        { initialize(check(body.c_str()), body.c_str()); }

        // set body type
        void bodyType(BodyType type) { m_type = type; }

        // array of chars body
        void body(const char* body) { initialize(check(body), body); }
        // const char body
        void body(char* body) { initialize(check(body), body); }
        // string body
        void body(std::string body) { initialize(check(body.c_str()), body.c_str()); }

        void set(std::string name, std::string value) {
            m_setFields.push_back(Field(name, value));
        }
        void insert(std::string name, std::string value) {
            m_insertFields.push_back(Field(name, value));
        }
    };

    class ChildResponse : private Response {
    public:
        ChildResponse(const Response& res)
            : Response(res)
        { }

        // Getters for handler
        BodyType getType() { return m_type; }
        char* getBody() { return m_body; }
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

using bergemon::BodyType::BINARY;
using bergemon::BodyType::TEXT;