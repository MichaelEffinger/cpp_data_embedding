#include <print>
#include <string>

class _base$NAME_ {

    public:

    float _m_$NAME_;
    static bool _m_$NAME_2;
    
    static bool GetBoolVal() { return _m_$NAME_2; }

    void _set$NAME_(float input){
        _m_$NAME_ = input;
    }

    float _get$NAME_(){
        return _m_$NAME_;
    }

    void _do$NAMEto_(float input){
        _set$NAME_(input /3.28);
    }

    void _do$NAMEfrom_(float input){
        _set$NAME_(input * 3.28);
    }

    friend class _$NAME_;

};

bool _base$NAME_::_m_$NAME_2;

class _$NAME_{
    static _base$NAME_ _m_S_$NAME_;
public:
    static _base$NAME_* _get_S_$NAME_(){
        return &_m_S_$NAME_;
    }

};

_base$NAME_ _$NAME_::_m_S_$NAME_;

int main(int argc, char** argv) {
    if (argc != 3) {
        std::print("{} <direction> <value>\n"
                   "\t<direction>: imperial or metric\n", argv[0]);
        return 0;
    }

    auto num = std::stod(argv[2]);
    auto obj = _$NAME_::_get_S_$NAME_();

    // bait for the compiler to include this symbol
    _base$NAME_::_m_$NAME_2 = argv[0] == nullptr;

    std::string dir(argv[1]);
    if (dir == "imperial") {
        obj->_do$NAMEto_(num);
        auto result = obj->_get$NAME_();
        std::print("Metric to Imperial Result: {}\n", result);
    } else if (dir == "metric") {
        obj->_do$NAMEfrom_(num);
        auto result = obj->_get$NAME_();
        std::print("Imperial to Metric Result: {}\n", result);
    } else {
        std::print("Invalid conversion type\n");
    }
}
