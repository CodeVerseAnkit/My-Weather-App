#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>

#include "/sdcard/Coding.Cpp/PROJECT/Weather app/helper_function.h"
using namespace std;
/* 
  <-------- MY WEATHER APP -------->
  
PROJECT:- My Weather app
PROGRAMMER:- Ankit
DATE:- 27_1_2026
DESCRIPTION:- It is a weather app that shows weather very accurate weather information. 

*/

enum class ApiError
{ // for handling errors
    None,
    CurlInit,
    Network,
    InvalidKey,
    CityNotFound,
    RateLimit,
    Unknown
};
class WeatherApi
{
  private:
    string response;
    ApiError error;
    long http_code;

  public:
    // constrictor
    WeatherApi()
    {
        error = ApiError::None;

        http_code = 0;
    }
    // destructor
    ~WeatherApi() {}
    // Callback function: API ka data yahan aata hai
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, string *output)
    {
        size_t totalSize = size * nmemb;
        output->append((char *)contents, totalSize);
        return totalSize;
    }

    // send request and generate respone
    void connect_api(string city_name)
    {
        CURL *curl;
        CURLcode res;
        response.clear();
        error = ApiError::None;
        http_code = 0;

        curl = curl_easy_init();
        if (!curl)
        {
            error = ApiError::CurlInit;
            return;
        }
        // URL encoding
        char *encoded = curl_easy_escape(curl, city_name.c_str(), 0);
        string safe_city = encoded;
        curl_free(encoded);
        string URL = "https://api.openweathermap.org/data/2.5/weather?q=" + safe_city + "&appid="+paste_your_api_key;
        //--->
        curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            error = ApiError::Network;
            curl_easy_cleanup(curl);
            return;
        }

        // error types
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (http_code == 401)
            error = ApiError::InvalidKey;
        else if (http_code == 404)
            error = ApiError::CityNotFound;
        else if (http_code == 429)
            error = ApiError::RateLimit;
        else if (http_code != 200)
            error = ApiError::Unknown;
        curl_easy_cleanup(curl);
    }
    // return error
    ApiError getError() const
    {
        return error;
    }
    // return raw jsons data
    string get_raw_json()
    {
        return response;
    }
};
// error to string conversion and printing
string errorToString(ApiError e)
{
    switch (e)
    {
    case ApiError::None:
        return "";
    case ApiError::CurlInit:
        return "Internal error: curl init failed";
    case ApiError::Network:
        return "Network error. Check internet connection";
    case ApiError::InvalidKey:
        return "Invalid API key";
    case ApiError::CityNotFound:
        return "City not found";
    case ApiError::RateLimit:
        return "Too many requests";
    default:
        return "Unknown error";
    }
}
// ---> find data in a raw json
// for number value
string getNumberValue(const string &json, const string &key)
{
    size_t pos = json.find("\"" + key + "\":");
    if (pos == string::npos)
        return "Not Found";

    pos += key.length() + 3; // "key":
    size_t end = json.find(",", pos);
    return json.substr(pos, end - pos);
}
// for string value
string getStringValue(const string &json, const string &key)
{
    size_t pos = json.find("\"" + key + "\":");
    if (pos == string::npos)
        return "Not Found";

    pos = json.find("\"", pos + key.length() + 3);
    size_t end = json.find("\"", pos + 1);
    return json.substr(pos + 1, end - pos - 1);
}
// format and print data
void format_data(string city, string temp, string feels_like, string pressure, string humidity, string weather, string description)
{
    cout << "   " << string(40, '-') << endl;
    cout << "   |\033[1m  " << left << setw(15) << "City Name"
         << " \033[0m|  \033[1m" << setw(15) << city << "  \033[0m|" << endl;

    cout << "   " << string(40, '-') << endl;
    cout << "   |\033[1;31m  " << setw(15) << "Temperature"
         << " \033[0m|  ";
    if (stoi(temp) > 30)
        cout << "\033[1;31m";
    if (stoi(temp) < 20)
        cout << "\033[1;36m";
    cout << setw(15) << temp + " °C"
         << "   \033[0m|" << endl;
    cout << "   " << string(40, '-') << endl;
    cout << "   |\033[1m  " << setw(15) << "Feels Like"
         << " \033[0m|  \033[1m" << setw(15) << feels_like + "°"
         << "   \033[0m|" << endl;
    cout << "   " << string(40, '-') << endl;
    cout << "   |\033[1m  " << setw(15) << "Pressure"
         << " \033[0m|  \033[1;33m" << setw(15) << pressure << "  \033[0m|" << endl;
    cout << "   " << string(40, '-') << endl;
    cout << "   |\033[1m  " << setw(15) << "Humidity"
         << " \033[0m|  \033[1;36m" << setw(15) << humidity + "%"
         << "  \033[0m|" << endl;
    cout << "   " << string(40, '-') << endl;
    cout << "   |\033[1;32m  " << setw(15) << "Weather"
         << " \033[0m|  \033[1;32m" << setw(15) << weather << "  \033[0m|" << endl;
    cout << "   " << string(40, '-') << endl;
    cout << "   |\033[1m  " << setw(15) << "Description"
         << " \033[0m|  \033[1m" << setw(15) << description << "  \033[0m|" << endl;
    cout << "   " << string(40, '-') << endl;
}
// api result
void weather_result(string city_name, string raw_json, ApiError error)
{
    if (error != ApiError::None)
    {
        cout << "\n\n\n\n\033[1;31m";
        print_center(errorToString(error));
        print_center("❗❗❗");
        cout << "\033[0m";
        return;
    }

    string temperature, feels_like, pressure, humidity, weather, description;

    temperature = getNumberValue(raw_json, "temp");

    pressure = getNumberValue(raw_json, "pressure");

    humidity = getNumberValue(raw_json, "humidity");

    weather = getStringValue(raw_json, "main");

    description = getStringValue(raw_json, "description");

    feels_like = getNumberValue(raw_json, "feels_like");
    format_data(city_name, temperature, feels_like, pressure, humidity, weather, description);
}

// it is use to maintain user search history
void print_in_file(string city)
{
    ofstream write;
    write.open("/sdcard/Coding.Cpp/PROJECT/Weather app/user_history.txt", ios::app);
    write << " -->" << city << endl;
    write.close();
}
// Menu (UI)
void menu()
{
    cout << "\n"
         << string(46, '-');
    cout << "\033[1;33m";
    print_center("MY WEATHER APP");
    cout << "\033[0m";
    print_center("_-_-_-_-_-_-_-_-_");

    print_text_with_animation("\n\n  1.\033[1;32mCheck Weather\033[0m", 50);

    print_text_with_animation("\t  2.\033[1;36mCompare Weather\033[0m", 50);
    print_text_with_animation("\n  3.\033[1;31mSearch History\033[0m", 50);
    print_text_with_animation("\t  4.\033[1;35mApp Info\033[0m", 50);
    print_text_with_animation("\n  5.\033[1mExit\033[0m", 50);
    cout << "\n\n"
         << string(46, '-');
}

// 1. check weather
void check_weather()
{
    WeatherApi api;
    char ch = 'a';
    cout << "\033[1;33m";
    print_center("WELCOME TO MY WEATHER APP");
    cout << "\033[0m";
    print_center("_-_-_-_-_-_-_-_-_-_-_-_-_-_");
    do
    {
        cout << "\033[1;31m";
        print_text_with_animation("\n\n  Enter city name to see the weather☀️:-", 50);
        cout << "\033[0m\n\n";

        string city_name;

        while (true)
        {
            cout << "\033[1m";
            cout << "  Enter city name: ";
            cout << "\033[0m";

            getline(cin, city_name);

            api.connect_api(city_name);
            if (api.getError() == ApiError::CityNotFound)

                cout << "\tWrong City Name!!" << endl;

            else

                break;
        }
        cout << "\n";
        loading_animation1("  Searching");
        cout << "\n\n";
        print_in_file(city_name);
        weather_result(city_name, api.get_raw_json(), api.getError());
        cout << "\n\n";
        while (true)
        {
            cout << "  Do you want to check weather again(y/n) : ";
            cin >> ch;
            if (tolower(ch) == 'n' || tolower(ch) == 'y')
            {
                break;
            }
        }
        cin.ignore();
        if (tolower(ch) == 'y')
            clear_screen();
    } while (tolower(ch) != 'n');
    back_button();
}
// 2. compare weather
void compare_weather()
{
    WeatherApi api1, api2;
    char ch = 'a';
    cout << "\033[1;33m";
    print_center("WELCOME TO MY WEATHER APP");
    cout << "\033[0m";
    print_center("_-_-_-_-_-_-_-_-_-_-_-_-_-_");
    do
    {
        cout << "\033[1;31m";
        print_text_with_animation("\n\n  Enter city name to see the weather☀️:-", 50);
        cout << "\033[0m\n\n";

        string first_city, second_city;

        while (true)
        {
            cout << "\033[1m";
            cout << "  Enter first city name: ";
            cout << "\033[0m";

            getline(cin, first_city);

            api1.connect_api(first_city);
            if (api1.getError() == ApiError::CityNotFound)

                cout << "\tWrong City Name!!" << endl;

            else

                break;
        }
        while (true)
        {
            cout << "\033[1m";
            cout << "  Enter second city name: ";
            cout << "\033[0m";

            getline(cin, second_city);

            api2.connect_api(second_city);
            if (api2.getError() == ApiError::CityNotFound)

                cout << "\tWrong City Name!!" << endl;

            else

                break;
        }

        cout << "\n";
        loading_animation1("  Searching");
        cout << "\n\n";
        weather_result(first_city, api1.get_raw_json(), api1.getError());
        cout << "\n\n";

        weather_result(second_city, api2.get_raw_json(), api2.getError());
        cout << "\n\n";

        while (true)
        {
            cout << "  Do you want to check weather again(y/n) : ";
            cin >> ch;
            if (tolower(ch) == 'n' || tolower(ch) == 'y')
            {
                break;
            }
        }

        cin.ignore();
        if (tolower(ch) == 'y')
            clear_screen();
    } while (tolower(ch) != 'n');
    back_button();
}
// 3 Search history
void search_history()
{
    cout << "\033[1;33m";
    print_center("WELCOME TO MY WEATHER APP");
    cout << "\033[0m";
    print_center("_-_-_-_-_-_-_-_-_-_-_-_-_-_");
    cout << "\n\n  " << string(28, '-') << endl;
    cout << "  |\033[1;36m \tSearch History\033[0m \t 🔍  |" << endl;
    cout << "  " << string(28, '-') << "\n"
         << endl;
    ifstream read;
    read.open("/sdcard/Coding.Cpp/PROJECT/Weather app/user_history.txt");
    string city;
    while (read >> city)
    {
        cout << "  " << city << endl;
    }
    read.close();
    char ch;
    while (true)
    {
        cout << "\n  \033[1mDo you want to clean your\n  search history(y/n): "
                "\033[0m";
        cin >> ch;
        if (tolower(ch) == 'y' || tolower(ch) == 'n')
            break;
    }
    if (tolower(ch) == 'y')
    {
        remove("/sdcard/Coding.Cpp/PROJECT/Weather app/user_history.txt");
        loading_animation1("  Deleting");
        cout << "\n  Search history deleted successfully 👍\n"
             << endl;
    }
    back_button();
}
// Project information function

void info();
int main()
{
    char input;
    do
    {
        menu();
        cout << "\n\n";
        // input
        while (true)
        {
            cout << "  INPUT: ";
            cin >> input;
            if (input >= '1' && input <= '5')
                break;
        }
        // switch cases
        switch (input)
        {
        case '1':
            clear_screen();
            cin.ignore();
            loading_bar();
            check_weather();
            break;
        case '2':
            clear_screen();
            cin.ignore();
            loading_bar();
            compare_weather();
            break;
        case '3':
            clear_screen();
            search_history();
            break;
        case '4':
            clear_screen();
            info();
            break;

        case '5':
            cout << "\n\n\033[1;33m";
            print_center("😁THANKS FOR VISIT😁");
            cout << "\033[0m\n"
                 << endl;
            loading_animation1("  Exiting");
            break;
        default:
            cout << "  Invalid Input" << endl;
        }

        clear_screen();
    } while (input != '5');

    return 0;
}
// project information
void info()
{
    cout << "\033[1;33m";
    print_center("WELCOME TO MY WEATHER APP");
    cout << "\033[0m";
    print_center("_-_-_-_-_-_-_-_-_-_-_-_-_-_");

    cout << "\n\n  \033[1;4mPROJECT INFORMATION\033[0m :- \n"
         << endl;
    cout << "  \033[1m" << left << setw(11) << "PROJECT"
         << "\033[0m : My Weather app" << endl;
    cout << "  \033[1m" << setw(11) << "PROGRAMMER"
         << "\033[0m : Ankit" << endl;
    cout << "  \033[1m" << setw(11) << "VERSION"
         << "\033[0m : V1.0" << endl;
    cout << "  \033[1m" << setw(11) << "DATE"
         << "\033[0m : 1_2_2026" << endl;
    cout << "  \033[1m" << setw(11) << "DESCRIPTION"
         << "\033[0m : It is a weather app that shows \t        real time weather information." << endl;

    cout << "\n\n\033[1;33m";
    print_center("THANKS FOR VISIT");
    cout << "\033[0m";
    print_center("_-_-_-_-_-_-_-_-_-_");
    cout << "\n\n";
    back_button();
}
