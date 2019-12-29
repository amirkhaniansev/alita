#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#include "../../include/db.hpp"

void test_add_cache(alita::alita_db& db)
{
    std::wstring link, content, input;

    std::wcout << L"Link : ";
    std::wcin >> link;
    std::wcout << "Content : ";
    std::wcin >> content;

    std::wcout << std::endl;

    int id = db.add_cache(link, content);

    std::wcout << "ID : " << id << std::endl;
}

void test_add_index(alita::alita_db& db)
{
    std::wstring word, link;
    int frequency;

    std::wcout << L"Word : ";
    std::wcin >> word;
    std::wcout << L"Link : ";
    std::wcin >> link;
    std::wcout << L"Frequency : ";
    std::wcin >> frequency;

    db.add_index(word, link, frequency);
}

void test_get_cache(alita::alita_db& db)
{
    int id;
    std::wcout << L"Id : ";
    std::wcin >> id;

    alita::cache cache = db.get_cache_by_id(id);

    std::wcout << L"Id : " << cache._id << '\n'
               << L"Link : " << cache._link << '\n' 
               << L"Content : " << cache._content << '\n'
               << L"Created : " << std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(cache._created) << '\n'
               << L"Modified : " << std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(cache._modified) << '\n'
               << L"Process State : " << cache._process_state << '\n'; 
}

void test_set_state(alita::alita_db& db)
{
    int link_id;
    short state;

    std::wcout << L"Link Id : ";
    std::wcin >> link_id;

    std::wcout << L"State : ";
    std::wcin >> state;
    std::wcout << std::endl;

    db.set_cache_state(link_id, state);
}

void test_search(alita::alita_db& db)
{
    std::wstring word;
    std::wcout << L"Word : ";
    std::wcin >> word;

    auto result = db.search(word, 0);
    if(result.empty()) {
        std::wcout << "Result Empty" << std::endl;
        return;
    }

    for(auto it = result.begin(); it != result.end(); it++) {
        std::wcout << "Index ID : " << it->_index_id << '\n'
                   << "Frequency : " << it->_frequency << '\n'
                   << "Page : " << it->_page << '\n'
                   << "Word : " << it->_word << '\n'
                   << "Last Indexed : " << std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(it->_last_indexed) << '\n';
    }
}

int main(int argc, char** argv)
{
    alita::db_connection_info info;
    info._host = "tcp://127.0.0.1:3306";
    info._username = "sev";
    info._password = "password";
    info._scheme = "Alita";

    alita::alita_db db(info);

    while(true) {
        std::wstring input;
        std::wcout << L"Test Method : ";
        std::wcin >> input;
        
        std::wcout << std::endl;
        std::wcout << L"You entered : " << input;
        std::wcout << std::endl;

        if(input == L"search")
            test_search(db);
        else if(input == L"add-cache")
            test_add_cache(db);
        else if(input == L"add-index")
            test_add_index(db);
        else if(input == L"get-cache")
            test_get_cache(db);
        else if(input == L"set-state")
            test_set_state(db);
        else if(input == L"exit")
            break;
    }

    return 0;
}