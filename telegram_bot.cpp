#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <tgbot/tgbot.h>
#include <vector>
#include <string>
#include "json.hpp"
#include <curl/curl.h>
#include <stdlib.h>
#include <exception>

using namespace TgBot;

std::vector<std::string> bot_commands {"convert", "help", "start"};

static size_t Writer(char *buffer, size_t size, size_t nmemb, std::string *html){
    int result = 0;

    if(buffer != NULL){
        html->append(buffer, size*nmemb);
        result = size*nmemb;
    }
    return result;
}

std::string get_request(std::string link){
    CURL *curl;
    std::string data;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);   
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return data;
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

float get_currency(std::string what){
    auto js_obj = nlohmann::json::parse(get_request("https://www.cbr-xml-daily.ru/daily_json.js"));
    
if(what == "USD"){
    return js_obj["Valute"]["USD"]["Value"].get<float>() / js_obj["Valute"]["USD"]["Nominal"].get<float>();
}
else if(what == "EUR"){
    return js_obj["Valute"]["EUR"]["Value"].get<float>() / js_obj["Valute"]["EUR"]["Nominal"].get<float>();
}
else if(what == "PLN"){
    return js_obj["Valute"]["PLN"]["Value"].get<float>() / js_obj["Valute"]["PLN"]["Nominal"].get<float>();
}
else if(what == "UAH"){
    return js_obj["Valute"]["UAH"]["Value"].get<float>() / js_obj["Valute"]["UAH"]["Nominal"].get<float>();
}
else if(what == "GBP"){
    return js_obj["Valute"]["GBP"]["Value"].get<float>() / js_obj["Valute"]["GBR"]["Nominal"].get<float>();
}
else if(what == "JPY"){
    return js_obj["Valute"]["JPY"]["Value"].get<float>() / js_obj["Valute"]["JPY"]["Nominal"].get<float>();
}
else if(what == "CNY"){
    return js_obj["Valute"]["CNY"]["Value"].get<float>() / js_obj["Valute"]["CNY"]["Nominal"].get<float>();
}
else if(what == "AUD"){
    return js_obj["Valute"]["AUD"]["Value"].get<float>() / js_obj["Valute"]["AUD"]["Nominal"].get<float>();
}
else if(what == "BYN"){
    return js_obj["Valute"]["BYN"]["Value"].get<float>() / js_obj["Valute"]["BYN"]["Nominal"].get<float>();
}
else if(what == "CAD"){
    return js_obj["Valute"]["CAD"]["Value"].get<float>() / js_obj["Valute"]["CAD"]["Nominal"].get<float>();
}
else if(what == "CZK"){
    return js_obj["Valute"]["CZK"]["Value"].get<float>() / js_obj["Valute"]["CZK"]["Nominal"].get<float>();
}
else if(what == "BRL"){
    return js_obj["Valute"]["BRL"]["Value"].get<float>() / js_obj["Valute"]["BRL"]["Nominal"].get<float>();
}
    return -1;
}

std::string s_from = "";
std::string s_to = "";

float from = 0;
float to = 0; 

void createKeyboard(const std::vector<std::vector<std::string>>& buttonLayout, ReplyKeyboardMarkup::Ptr& kb)
{
  for (size_t i = 0; i < buttonLayout.size(); ++i) {
    std::vector<KeyboardButton::Ptr> row;
    for (size_t j = 0; j < buttonLayout[i].size(); ++j) {
      KeyboardButton::Ptr button(new KeyboardButton);
      button->text = buttonLayout[i][j];
      row.push_back(button);
    }
    kb->keyboard.push_back(row);
  }
}


std::string get_value(std::string str, float &value){
    str.erase(str.begin(), str.end() - 12);
    str.erase(str.begin() + 3, str.end());
    value = get_currency(str);
    return str;
}

int main() {
    Bot bot("PLACE YOUR TOKEN HERE");

    ReplyKeyboardMarkup::Ptr keyboardConvert(new ReplyKeyboardMarkup);
    createKeyboard({
      {"Convert"},
    }, keyboardConvert);

    ReplyKeyboardMarkup::Ptr keyboardFrom(new ReplyKeyboardMarkup);
    createKeyboard({
      {"From USD 🇺🇸", "From EUR 🇪🇺", "From AUD 🇦🇺", "From GBR 🇬🇧"},
      {"From JPY 🇯🇵", "From CNY 🇨🇳", "From PLN 🇵🇱", "From UAH 🇺🇦"},
      {"From BYN 🇧🇾", "From CAD 🇨🇦", "From CZK 🇨🇿", "From BRL 🇧🇷"}
    }, keyboardFrom);

    ReplyKeyboardMarkup::Ptr keyboardTo(new ReplyKeyboardMarkup);
    createKeyboard({
      {"To USD 🇺🇸", "To EUR 🇪🇺", "To AUD 🇦🇺", "To GBR 🇬🇧"},
      {"To JPY 🇯🇵", "To CNY 🇨🇳", "To PLN 🇵🇱", "To UAH 🇺🇦"},
      {"To BYN 🇧🇾", "To CAD 🇨🇦", "To CZK 🇨🇿", "To BRL 🇧🇷"}
    }, keyboardTo);

        bot.getEvents().onCommand("start", [&bot, &keyboardConvert](Message::Ptr message) {  
        bot.getApi().sendMessage(message->chat->id, "Hi, " + message->chat->firstName + 
        "\nSee /help for a list of currency codes." +
        "\nClick on the button to start converting", false, 0, keyboardConvert);
    });

        bot.getEvents().onCommand("convert", [&bot, &keyboardConvert](Message::Ptr message) {  
        bot.getApi().sendMessage(message->chat->id, "Click on the button to start converting", false, 0, keyboardConvert);
    });

        bot.getEvents().onCommand("help", [&bot](Message::Ptr message) {  
        bot.getApi().sendMessage(message->chat->id, "You can always go back using the command /convert\nThe list of currency codes:\nUSD 🇺🇸 - US Dollar\nEUR 🇪🇺 - Euro\nAUD 🇦🇺 - Australian Dollar\nGBP 🇬🇧 - Pound Sterling\nJPY 🇯🇵 - Japanese yen\nCNY 🇨🇳 - Chinese Yuan Renminbi\nPLN 🇵🇱 - Polish Zloty\nUAH 🇺🇦 - Ukranian Hryvnia\nBYN 🇧🇾 - Belarussian Ruble\nCAD 🇨🇦 - Canadian Dollar\nCZK 🇨🇿 - Czech Koruna\nBRL 🇧🇷 - Brazilian Real\nIf you have any questions, you can write to the developer @miles_42" );
    });

    bot.getEvents().onAnyMessage([&bot, &keyboardFrom, &keyboardTo, &keyboardConvert](Message::Ptr message) {

        if(StringTools::startsWith(message->text, "Convert")){
            bot.getApi().sendMessage(message->chat->id, "Select the first currency", false, 0, keyboardFrom);
            return;
        }
    if(StringTools::startsWith(message->text, "From")){
    std::string str = message->text;
    s_from = get_value(str, from);
    bot.getApi().sendMessage(message->chat->id, "Select the second currency", true, 0, keyboardTo);
    return;
    }
    if(StringTools::startsWith(message->text, "To")){
    std::string str = message->text;
    s_to = get_value(str, to);
    bot.getApi().sendMessage(message->chat->id, "\nAdd value:");
    return;
    }

        if(is_number(message->text) && from > 0 && to > 0){
            float num = ::atof(message->text.c_str());
            float result = num * from / to;
            std::string str_result = std::to_string(result);
            str_result.resize(str_result.size() - 4);
            bot.getApi().sendMessage(message->chat->id, message->text + " " + s_from + " to " + s_to + " is: " + str_result, false, 0, keyboardConvert);
            return;
        }

        for(const auto& command : bot_commands){
            if("/" + command == message->text){
                return;
            }
        }

        bot.getApi().sendMessage(message->chat->id, "Sorry, I don't understand you :(");
    });
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (std::exception& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}
