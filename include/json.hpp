#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <any>

using namespace std;

class Json {
private:
    vector<any> keys;
    vector<any> values;
    vector<any> array;

public:
    // Конструктор из строки, содержащей Json-данные.
    Json(const std::string& s);
    
    // Метод возвращает true, если данный экземпляр содержит в себе JSON-массив. Иначе false.
    bool is_array() const;
    // Метод возвращает true, если данный экземпляр содержит в себе JSON-объект. Иначе false.
    bool is_object() const;

    // Метод возвращает значение по ключу key, если экземпляр является JSON-объектом.
    // Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
    // Если экземпляр является JSON-массивом, генерируется исключение.
    std::any& operator[](const std::string& key);
    
    // Метод возвращает значение по индексу index, если экземпляр является JSON-массивом.
    // Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
    // Если экземпляр является JSON-объектом, генерируется исключение.
    std::any& operator[](int index);
    
    // Метод возвращает объект класса Json из строки, содержащей Json-данные.
    static Json parse(const std::string& s) {
      int i=0;
      string str="\0";
      while(s[i] != '\0') {
         if(s[i] != '\n' && s[i] != '\t' && s[i] != ' ')
             str+=s[i];
         i++;
      }
      Json cur(str);
      return cur;
    };
    
    // Метод возвращает объекта класса Json из файла, содержащего Json-данные в текстовом формате.
    static Json parseFile(const std::string& path_to_file);
};

Json::Json(const std::string& s){
    int i=1;
    string str;
    if(s[0] == '[' && s[s.length()-1] == ']'){
        while(s[i] != ']'){
            if(s[i] == ',' || s[i] == ']'){
                i++;
                array.push_back(atoi(str.c_str()));
                str="";
            }
            str+=s[i];
            i++;
        }
        array.push_back(atoi(str.c_str()));
        str="";
    }
    if(s[0] == '{' && s[s.length()-1] == '}')
        while(i<s.length()-1){
            if(s[i] == ',')
                i++;
            if(s[i] == '"'){
                i++;
                while(s[i] != '"'){
                    str+=s[i];
                    i++;
                }
                i++;
                //добавление в ключ
                keys.push_back(str);
                //cout<<str<<endl;
                str="";
            }else if(s[i] == ':'){
                i++;
                if(s[i] == '"'){
                    i++;
                    while(s[i] != '"'){
                        str+=s[i];
                        i++;
                    }
                    i++;
                    values.push_back(str);
                    str="";
                    //добавление в значения
                }else if(s[i] == '['){
                    while(s[i] != ']'){
                        str+=s[i];
                        i++;
                    }
                    str+=s[i];
                    i++;
                    Json cur(str);
                    values.push_back(cur);
                    //cout<<str<<endl;
                    str="";
                }else if(s[i] == 't' || s[i] == 'f'){
                    bool test;
                    if(s[i] == 'f') {
                        test=false;
                        values.push_back(test);
                    }else{
                        test=true;
                        values.push_back(test);
                    }
                    while(s[i] != ','){
                        str+=s[i];
                        i++;
                    }
                    //cout<<str<<endl;
                    i++;
                    str="";
                }else if(s[i] == '{'){
                    while(s[i] != '}'){
                        str+=s[i];
                        i++;
                    }
                    str+=s[i];
                    i++;
                    Json cur1(str);
                    values.push_back(cur1);
                    //cout<<str<<endl;
                    str="";
                }else{
                    while(s[i] != ','){
                        str+=s[i];
                        i++;
                    }
                    i++;
                    values.push_back(atoi(str.c_str()));
                    //cout<<str<<endl;
                    str="";
                }
            }
        }
}
any& Json::operator[](const std::string& key) {
    for (int i = 0; i < keys.size(); i++) {
        if (any_cast<string>(keys[i]) == key) {
            return values[i];
        }
    }
}
std::any& Json::operator[](int index) {
    return array[index];
}
