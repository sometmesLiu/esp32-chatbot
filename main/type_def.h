#pragma once

#include <list>
#include <functional>
#include <mutex>

using ListFunction = std::list<std::function<void()>>;
