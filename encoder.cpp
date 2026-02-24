#include <cstdlib>
#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <iomanip>
#include <tuple>
#include <cstdint>
#include <fstream>
#include <vector>
#include <cstring>
#include <unordered_map>

constexpr std::array<char const * const,16> prefix{
    "_unsignedint_",
    "_int_",
    "_unsignedshort_",
    "_short_",
    "_long_",
    "_unsignedlong_",
    "_longlong_",
    "_unsignedlonglong_",
    "_float_",
    "_double_",
    "_longdouble_",
    "_uint8_",
    "_int8_",
    "_uint16_",
    "_int16_",
    "_char_",
};

constexpr std::array<char const * const, 16> cool{
    "cooler",
    "coolest",
    "rad",
    "amazing",
    "ultimate",
    "intelligent",
    "naive",
    "special",
    "outrageous",
    "crazy",
    "awesome",
    "epic",
    "legendary",
    "spectacular",
    "wicked",
    "fantastic"
};
constexpr std::array<char const * const, 16> imp{
    "IMPERIAL",
    "imperial",
    "Imperial",
    "Imprl",
    "IMPRL",
    "imprl",
    "US",
    "us",
    "U_S",
    "u_s",
    "IL",
    "il",
    "Il",
    "IPL",
    "Ipl",
    "Ipl",
};


constexpr std::array<char const * const, 16> met{
    "METRIC",
    "Metric",
    "metric",
    "MTC",
    "Mtc",
    "mtc",
    "STANDARD",
    "Standard",
    "standard",
    "SCIENTIFIC",
    "Scientific",
    "scientific",
    "NORMAL",
    "Normal",
    "normal",
    "MC",
};


constexpr std::array<char const * const, 16> conv{
    "Converter",
    "CONVERTER",
    "converter",
    "Transformer",
    "TRANSFORMER",
    "transformer",
    "TRANSLATOR",
    "Translator",
    "translator",
    "ADAPTER",
    "Adapter",
    "adapter",
    "TRANSITIONER",
    "Transitioner",
    "transitioner",
    "FUNCTION",
};


constexpr std::array<char const * const, 16> suffix{
    "Deluxe",
    "V2",
    "Alpha",
    "UltimateEdition",
    "Turbo",
    "andKnuckles",
    "DOTexe",
    "X64",
    "X86",
    "withNewFunkyMode",
    "JR",
    "Returns",
    "Melee",
    "forWiiU",
    "Advanced",
    "FinalEdition",
};


constexpr std::tuple<uint8_t, uint8_t> split_hex(char c) {
    uint8_t byte = static_cast<uint8_t>(c);

    return {
        static_cast<uint8_t>(byte >> 4),
        static_cast<uint8_t>(byte & 0x0F)
    };
}


// std::array< const char * const,8> returnType{"void","float","int", "bool", "char", "double", "unsigned", "unsigned long long" };
std::array<const char * const, 9> theGreatReplacement{
    "_base$NAME_",
    "_get_S_$NAME_",
    "_get$NAME_",
    "_set$NAME_",
    "_do$NAMEto_",
    "_do$NAMEfrom_",
    "_m_S_$NAME_",
    "_m_$NAME_",
    "_$NAME_",
};



std::array< const char * const, 9> theGreatprefix{
    "base",
    "get_s",
    "get",
    "set",
    "do",
    "do",
    "m_S",
    "m_",
    "",
};

std::vector<std::string> replacee{};

std::unordered_map<std::string, std::string> mymap{};



int main(){
    std::string input;
    std::cout << "Enter your secret message, note it needs to be longer than 27 THINK PRIVATE \n";
    std::getline(std::cin,input,'\n');

    auto iter(theGreatReplacement.cbegin());

    std::stringstream stream(input);
    std::string current_word;
    
    std::size_t countDracula = 0;
    std::stringstream output;
    while(std::getline(stream,current_word,' ')){
        std::size_t position = 0;
        for(char i:current_word){
            if (iter == theGreatReplacement.cend()) {
                break;
            }
            const auto&& [low,high] = split_hex(i);
            switch(countDracula){
                case 0:
                    output << prefix[low];
                    output << cool[high];
                    countDracula++;
                break;
                case 1:
                    output << imp[low];
                    output << met[high];
                    countDracula++;
                break;
                case 2:
                    output << conv[low];
                    output << suffix[high]; 
                    countDracula = 0;
                    replacee.push_back(output.str());
                    mymap.emplace(*iter,output.str());
                    iter++;
                    output = std::stringstream{};
                break;
            }
        }
        
    }


    if(input.length() < theGreatReplacement.size()*3){
        std::cout << "your string is too short, And so is the amount of time I have to finish this \n";
        return EXIT_FAILURE;
    }
    
    std::string theLeftovers = input.substr(theGreatReplacement.size()*3);


    std::ifstream in("encoder_template.hpp");
    std::string line;
    std::ofstream myoutput("output.cpp");
    
    while (std::getline(in, line)) {

        for (std::size_t i = 0; i < theGreatReplacement.size(); i++) {

            size_t pos = 0;

            while ((pos = line.find(theGreatReplacement[i], pos)) != std::string::npos) {
                std::string replacement = std::string(theGreatprefix[i]) + mymap[theGreatReplacement[i]];
                    if (i == 4) {
                        replacement += "to";
                    }
                    if (i == 5) {
                        replacement += "from";
                    }
                line.replace(pos, std::strlen(theGreatReplacement[i]), replacement);
                pos += replacement.length();
            }
        }

        myoutput << line << "\n";
    }
    
    std::cout  << theLeftovers;
}








/*


class base$NAME {

    private:

    float m_$NAME;
    
    void set$NAME(float input){

    }

    float get$NAME(){
        return m_$NAME;
    }

    void do$NAMEto(float input){
        set$NAME(input /3.28);
    }

    void do$NAMEfrom(float input){
        set$NAME(input * 3.28);
    }

    friend class $NAME;


};


class $NAME{
    base$NAME m_S_$NAME;

    base$NAME get_S_$NAME(){
        return m_S_$NAME;
    }

};






*/
