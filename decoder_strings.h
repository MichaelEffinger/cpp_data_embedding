#include <array>
#include <string>
#include <string_view>

/*
constexpr inline auto g_ordering_prefix = std::to_array<std::string_view>({
    "Base",     // 0
    "Get",      // 1
    "Set",      // 2
    "Do",       // 3
    "m_",       // 4
    "SGet",     // 6
    "m_S_",     // 7
});
*/

constexpr inline auto g_ordering_prefix = std::to_array<std::string_view>({
    "base",     // 0
    "get_s",    // 1
    "get",      // 2
    "set",      // 3
    "do",       // 4 & 5
    "m_S",      // 6
    "m_",       // 7
    "",         // 8
});

/*
constexpr inline auto g_direction_strings = std::to_array<std::string_view>({
    "To",           // 0
    "From",         // 8
});
*/

constexpr inline auto g_direction_strings = std::to_array<std::string_view>({
    "to",           // 0
    "from",         // 1
});

constexpr inline auto g_type_prefix = std::to_array<std::string_view>({
    "_unsignedint_",        // 0
    "_int_",                // 1
    "_unsigned_short",      // 2
    "_short_",              // 3
    "_long_",               // 4
    "_unsignedlong_",       // 5
    "_longlong_",           // 6
    "_unsignedlonglong_",   // 7
    "_float_",              // 8
    "_double_",             // 9
    "_longdouble_",         // 10
    "_uint8_",              // 11
    "_int8_",               // 12
    "_uint16_",             // 13
    "_int16_",              // 14
    "_char_",               // 15
});

constexpr inline auto g_coolness_prefix = std::to_array<std::string_view>({
    "cooler",               // 0
    "coolest",              // 1
    "rad",                  // 2
    "amazing",              // 3
    "ultimate",             // 4
    "intelligent",          // 5
    "naive",                // 6
    "special",              // 7
    "outrageous",           // 8
    "crazy",                // 9
    "awesome",              // 10
    "epic",                 // 11
    "legendary",            // 12
    "spectacular",          // 13
    "wicked",               // 14
    "fantastic",            // 15
    //"genius",               // 16
    //"amusing",              // 17
});

constexpr inline auto g_first_unit_strings = std::to_array<std::string_view>({
    "IMPERIAL",     // 0
    "imperial",     // 1
    "Imperial",     // 2
    "Imprl",        // 3
    "IMPRL",        // 4
    "imprl",        // 5
    "US",           // 6
    "us",           // 7
    "U_S",          // 8
    "u_s",          // 9
    "IL",           // 10
    "il",           // 11
    "Il",           // 12
    "IPL",          // 13
    "Ipl",          // 14
    "Ipl",          // 15
});

constexpr inline auto g_second_unit_strings = std::to_array<std::string_view>({
    "METRIC",       // 0
    "Metric",       // 1
    "metric",       // 2
    "MTC",          // 3
    "Mtc",          // 4
    "mtc",          // 5
    "Standard",     // 6
    "STANDARD",     // 7
    "standard",     // 8
    "SCIENTIFIC",   // 9
    "Scientific",   // 10
    "scientific",   // 11
    "NORMAL",       // 12
    "Normal",       // 13
    "normal",       // 14
    "MC",           // 15
    //"Mc", 
    //"mc",
});

constexpr inline auto g_verb_strings = std::to_array<std::string_view>({
    "Converter",    // 0
    "CONVERTER",    // 1
    "converter",    // 2
    "Transformer",  // 3
    "TRANSFORMER",  // 4
    "transformer",  // 5
    "TRANSLATOR",   // 6
    "Translator",   // 7
    "translator",   // 8
    "ADAPTER",      // 9
    "Adapter",      // 10
    "adapter",      // 11
    "TRANSITIONER", // 12
    "Transitioner", // 13
    "transitioner", // 14
    "FUNCTION",     // 15
    //"Function",     // 16
    //"function",     // 17
});

constexpr inline auto g_coolness_suffix = std::to_array<std::string_view>({
    "Deluxe",           // 0
    "V2",               // 1
    "Alpha",            // 2
    "UltimateEdition",  // 3
    "Turbo",            // 4
    "andKnuckles",      // 5
    "DOTexe",           // 6
    "X64",              // 7
    "X86",              // 8
    "withNewFunkyMode", // 9
    "JR",               // 10
    "Returns",          // 11
    "Melee",            // 12
    "forWiiU",          // 13
    "Advanced",         // 14
    "FinalEdition",     // 15
});
