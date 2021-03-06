#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <string>

using namespace std;

// local prototypes
int cust_pow(int number, int power);
void generate_rand_digits();
void print_intro();
void print_number();
bool is_game_over(int bulls);
int digits_cnt(int number);
char get_digit(int number, int index);
void mark_position(int& mask, int index);
bool is_pos_taken(int mask, int index);
void reset_masks();
void calculate_bulls(int number, int& bulls);
void calculate_cows(int number, int& cows);
bool is_char_numeric(char ch);
bool is_string_numeric(string input);
bool is_valid_input(string input);
bool process_game();
void play_bulls_and_cows();

#define DIGITS_CNT (4)

vector<char> digits(DIGITS_CNT);

// Using binary arithmetic
int target_mask = 0;
int guess_mask = 0;

int cust_pow(int number, int power)
{
    if (power == 0)
    {
        return 1;
    }

    if (power % 2 == 0)
    {
        number = cust_pow(number, power / 2);
        return number * number;
    }

    return number * cust_pow(number, power - 1);
}

void generate_rand_digits()
{
    int seed = time(NULL);
    srand(seed);
    // cout << "DEBUG Seed: " << seed << endl;

    for (int cnt = DIGITS_CNT - 1; cnt >= 0; --cnt)
    {
        digits[cnt] = rand() % 10;
    }
}

void print_intro()
{
    cout << "Welcome traveler. Wanna play some bulls and cows? Do your best!" << endl;
}

void print_number()
{
    cout << "Number to guess: ";
    for (int cnt = DIGITS_CNT - 1; cnt >= 0; --cnt)
    {
        cout << (char)(digits[cnt] + '0');
    }

    cout << endl;
}

bool is_game_over(int bulls)
{
    return (bulls == DIGITS_CNT);
}

int digits_cnt(int number)
{
    return (int)(log(number) + 1);
}

char get_digit(int number, int index)
{
    int divider = cust_pow(10, index);
    return ((number / divider) % 10);
}

void mark_position(int& mask, int index)
{
    mask |= (1 << index);
}

bool is_pos_taken(int mask, int index)
{
    return (((mask >> index) & 1) == 1);
}

void reset_masks()
{
    target_mask = 0;
    guess_mask = 0;
}

void calculate_bulls(int number, int& bulls)
{
    int digit;

    for (int cnt = 0; cnt < DIGITS_CNT; ++cnt)
    {
        digit = get_digit(number, cnt);
        if (digits[cnt] == digit)
        {
            mark_position(target_mask, cnt);
            mark_position(guess_mask, cnt);
            ++bulls;
        }
    }
}

void calculate_cows(int number, int& cows)
{
    int digit;

    for (int i = 0; i < DIGITS_CNT; ++i)
    {
        digit = get_digit(number, i);

        if (is_pos_taken(guess_mask, i))
        {
            continue;
        }

        for (int j = 0; j < DIGITS_CNT; ++j)
        {
            if (digits[j] == digit && !is_pos_taken(target_mask, j))
            {
                ++cows;
                mark_position(target_mask, j);
                mark_position(guess_mask, i);
            }
        }
    }
}

bool is_char_numeric(char ch)
{
    return (ch >= '0' && ch <= '9');
}

bool is_string_numeric(string input)
{
    for (int cnt = 0; cnt < input.size(); ++cnt)
    {
        if (!is_char_numeric(input[cnt]))
        {
            return false;
        }
    }

    return true;
}

bool is_valid_input(string input)
{
    if (is_string_numeric(input) && input.size() == DIGITS_CNT)
    {
        return true;
    }

    return false;
}

bool process_game()
{
    int turns = 0;
    int bulls = 0;
    int cows = 0;
    string input;
    int guess;

    while (!is_game_over(bulls))
    {
        ++turns;
        bulls = 0;
        cows = 0;
        reset_masks();
        cin >> input;

        if (!is_valid_input(input))
        {
            cout << "Invalid input. Try again" << endl;
            continue;
        }

        guess = atoi(input.c_str());

        calculate_bulls(guess, bulls);
        calculate_cows(guess, cows);

        cout << "Bulls: " << bulls << endl;
        cout << "Cows: " << cows << endl;
    }

    cout << "You won! It took you " << turns << " turns." << endl;
    cout << "Wanna try again? (y/n)" << endl;

    while (input != "y" && input != "n")
    {
        cin >> input;

        if (input == "y")
        {
            return true;
        }
        else if (input == "n")
        {
            return false;
        }
        else
        {
            cout << "I didn't understand you." << endl;
        }
    }

    return false;
}

void play_bulls_and_cows()
{
    bool continue_game = true;

    while (continue_game)
    {
        continue_game = process_game();
    }
}

int main()
{
    generate_rand_digits();
    print_intro();
    // print_number();
    play_bulls_and_cows();

    return 0;
}
