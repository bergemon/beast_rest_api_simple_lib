#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif

#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <ctime>
#include <unordered_set>