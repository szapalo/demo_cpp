

#include "./List.h"
#include <iostream>

class String: List<char>{
    private:

    String(char* begin_it, char* end_it){
        for(char* it = begin_it; begin_it != end_it; ++it){
            append(*it);
        }
    }

    public:

    String(){}
    String(std::string str){
        std::string::iterator it = str.begin();
        std::string::iterator it_end = str.end();
        for(; it!=it_end; ++it){
            append(*it);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const String& str){
        for(auto&& elem : str){
            os<<elem;
        }
        return os;
    }

    static String join(const List<String>& list_str, const String& delimiter=""){
        String result;
        int n = list_str.length()-1;
        for(int i=0; i < n; ++i){
            result.append(list_str[i]);
            result.append(delimiter);
        }
        result.append(list_str[n]);
        return result;
    }

    static String capitalize(const String& str){
        String result;
        for(char&& c : str){
            (c >= 97 && c <= 122 ? ) result.append(c-32) : result.append(c);
        }
        return result;
    }

    static String casefold(const String& str){
        String result;
        for(char&& c : str){
            (c >= 65 && c <= 90 ? ) result.append(c+32) : result.append(c);
        }
        return result;
    }

    static String count(const String& str, const String& substr){
        String result;
        for(char&& c : str){
            
        for(int i=0; i<list_str.length()-1; ++i){
            result+=list_str[i];
            result+=delimiter;
        }
        result+=list_str[length()-1];
        return result;
    }

    List<String> split(const String& delimiter){
        // [ ... ... [start_idx] ... [end_idx, del_idx] ... [del_idx+del_size-1], ... ]
        List<String> result;
        if((this->length)! || delimiter.length()>this->length){
            return {*this};
        }

        char* del_it = delimiter.begin();
        char* del_end = delimiter.end();
        int del_size = delimiter.length();

        int del_idx = 0;
        int start_idx = 0;
        int end_idx = 0;
        while(end_idx != length()){

        }

        return result;
    }

};


int main(){
    String s("straaaa");
    std::cout<<s;
    return 0;
}

