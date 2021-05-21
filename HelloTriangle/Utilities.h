#pragma once

namespace Utilities
{
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

    template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;
}