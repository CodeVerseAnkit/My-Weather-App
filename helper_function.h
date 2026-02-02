// ------> HELPER FUNCTIONS <-------
//         °°°°°°°°°°°°°°°°

// header
#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
// this are the important function
// 1. navigation button
void back_button()
{
    char ch;
    do
    {
        std::cout << "  Do you want to back(y): ";
        std::cin >> ch;
    } while (tolower(ch) != 'y');
}

// 2. clear screen
void clear_screen()
{
   std::cout << "\033[2J\033[H"; // Clear screen + Move cursor to (0,0)
  
    std::cout.flush();
}

// 3. print text in the center
void print_center(std::string text, int width = 45)
{
    int pad = (width - text.size()) / 2;
    if (pad > 0)
        std::cout << std::string(pad, ' ');
    std::cout << text << std::endl;
}

// 4. loading animation
void loading_animation1(std::string text)
{
    int dotCount = 0;

    for (int i = 0; i < 20; i++)
    {                                  // loop count adjust karo
        dotCount = (dotCount + 1) % 4; // 0,1,2,3 → cycle

        std::cout << "\r" << text;

        // dots print
        for (int j = 0; j < dotCount; j++)
        {
            std::cout << ".";
        }

        // purane dots erase karne ke liye extra spaces print karo
        std::cout << "   ";

        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // speed control
    }

    std::cout << std::endl;
}

// 5. loading bar

void loading_bar()
{
    const int barWidth = 10; // no. of character in bracket
    std::cout << "\t\t "
              << "\033[31;1m"
              << "LOADING... "
              << "\033[0m" << std::endl;
    std::cout << "\033[36;1m";
    for (int percent = 0; percent <= 100; ++percent)
    {
        int filled = (percent * barWidth) / 100;

        std::cout << "\r\t\t[";
        for (int i = 0; i < filled; ++i)
            std::cout << ':';
        for (int i = filled; i < barWidth; ++i)
            std::cout << ' ';
        std::cout << "] " << std::setw(3) << percent << "%"; // percent aligned

        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // speed
    }
    std::cout << "\033[0m";
    std::cout << "\n"
              << std::endl; // newline
}

// 6. print text with animation
void print_text_with_animation(std::string text, int speed)
{
    for (char c : text)
    {
        std::cout << c;
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
    }
  //  std::cout << std::endl;
}
