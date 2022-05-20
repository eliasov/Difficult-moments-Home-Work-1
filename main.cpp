#include <string>
#include <iostream>
#include <optional>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Person
{
	string name;
	string surname;
	optional<string> patrontmic;
};

//Вывод отчества(если оно присуствует)
optional<string> getOutputString(string& patr) {
	if (patr == "")
	{
		return nullopt;
	}
	return patr;
}

//перегрузка операторов(выводы(<<), меньше(<), равно(==))
std::ostream& operator << (std::ostream& out, const Person& person)
{
	out << setw(15) << person.name << setw(12)  << person.surname;

    if (person.patrontmic.has_value())
    {
        out << setw(17) << person.patrontmic.value();
    }
    else
    {
        out << setw(18);
    }

    return out;
}

bool operator < (const Person& p1, const Person& p2)
{
	return tie(p1.name, p1.surname, p1.patrontmic) < tie(p2.name, p2.surname, p2.patrontmic);
}
bool operator == (const Person& p1, const Person& p2)
{
	return tie(p1.name, p1.surname, p1.patrontmic) == tie(p2.name, p2.surname, p2.patrontmic);
}



//Структура PhoneNumber

struct PhoneNumber
{
	int countryCode;
	int cityCode;
	string number;
	optional<int> addationalNumber;
};
//Вывод добавочного номера(если он присуствует)
optional<int> getInputInt(string& in)
{
	if (in == "")
	{
		return nullopt;
	} return stoi(in);  //stoi преобразует строки в целые числа
}

//перегрузка операторов(меньше(<), равно(==))
bool operator < (const PhoneNumber& phonNun1, const PhoneNumber& phonNun2)
{
	return tie(phonNun1.countryCode, phonNun1.cityCode, phonNun1.number, phonNun1.addationalNumber) < 
		tie(phonNun2.countryCode, phonNun2.cityCode, phonNun2.number, phonNun2.addationalNumber);
}
bool operator == (const PhoneNumber& phonNun1, const PhoneNumber& phonNun2)
{
	return tie(phonNun1.countryCode, phonNun1.cityCode, phonNun1.number, phonNun1.addationalNumber) ==
		tie(phonNun2.countryCode, phonNun2.cityCode, phonNun2.number, phonNun2.addationalNumber);
}

ostream& operator<<(ostream& out, const PhoneNumber& p)
{
    out << '+' << p.countryCode << '(' << p.cityCode << ')' << p.number;

    if (p.addationalNumber.has_value())
    {
        out << ' ' << p.addationalNumber.value();
    }

    return out;
}


class PhoneBook
{
private:
    vector <pair<Person, PhoneNumber>> container;
public:
    PhoneBook(ifstream& file)
    {
        while (!file.eof())
        {
            for (string line; getline(file, line);)
            {
                stringstream str(line);
                vector<string> rowData;

                for (string s; getline(str, s, ';');)
                {
                    rowData.push_back(s);
                }

                pair<Person, PhoneNumber> inp;

                for (size_t i = 0; i < rowData.size(); ++i)
                {
                    switch (i)
                    {
                    case 0:
                        inp.first.name = rowData[i];
                        break;
                    case 1:
                        inp.first.surname = rowData[i];
                        break;
                    case 2:
                        inp.first.patrontmic = getOutputString(rowData[i]);
                        break;
                    case 3:
                        inp.second.countryCode = stoi(rowData[i]);
                        break;
                    case 4:
                        inp.second.cityCode = stoi(rowData[i]);
                        break;
                    case 5:
                        inp.second.number = rowData[i];
                        break;
                    case 6:
                        inp.second.addationalNumber = getInputInt(rowData[i]);
                        break;
                    }
                }
                container.push_back(inp);
            }
        }

    }
    friend ostream& operator<<(ostream& out, const PhoneBook& phb)
    {
        for (const auto& [first, second] : phb.container)
        {
            out << first << ' ' << second << endl;
        }

        return out;
    }

    void SortByName()
    {
        sort(container.begin(), container.end(), [](const pair<Person, PhoneNumber> &a, const pair<Person, PhoneNumber>&b)
            {
                return a.first < b.first;
            });
    }

    void SortByPhone()
    {
        sort(container.begin(), container.end(), [](const pair<Person, PhoneNumber>& a, const pair<Person, PhoneNumber>& b)
            {
                return a.second < b.second;
            });
    }

    pair<string, vector<PhoneNumber>> GetPhoneNumber(const string& name)
    {
        vector<PhoneNumber> phoneNumbers;
        int count = 0;

        for_each(container.begin(), container.end(), [&](const auto& entry)
            {
                if (entry.first.name == name)
                {
                    phoneNumbers.push_back(entry.second);
                    ++count;
                }
            });

        switch (count)
        {
        case 0:
            return { "Not phone", phoneNumbers };
        case 1:
            return { "", phoneNumbers };
        default:
            return { "there are several", phoneNumbers };
        }
    }

    void ChangePhoneNumber(const Person& p, const PhoneNumber& phn)
    {
        auto entry = find_if(container.begin(), container.end(), [&](const auto& entry)
            {
                return entry.first == p;
            });

        if (entry != container.end())
        {
            entry->second = phn;
        }
    }
};





int main()
{
    ifstream file("PhoneBook.txt");
    PhoneBook book(file);
    cout << book;

    cout << "------SortByPhone-------" << endl;
    book.SortByPhone();
    cout << book;

    cout << "------SortByName--------" << endl;
    book.SortByName();
    cout << book;

    cout << "-----GetPhoneNumber-----" << endl;
    // лямбда функция, которая принимает фамилию и выводит номер телефона этого     человека, либо строку с ошибкой
    auto print_phone_number = [&book](const string& surname)
    {
        cout << surname << "\t";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty())
        {
            for (size_t i = 0; i < get<1>(answer).size(); ++i)
            {
                cout << get<1>(answer)[i];
            }
        }
        else
        {
            cout << get<0>(answer);
        }
        cout << endl;
    };

    // вызовы лямбды
    print_phone_number("Ivanov");
    print_phone_number("Petrov");

    cout << "----ChangePhoneNumber----" << endl;
    book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", nullopt });
    book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
    cout << book;

    return 0;
}