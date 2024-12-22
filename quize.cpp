#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

class Question
{
public:
    string text;
    vector<string> options;
    int correctAnswer;

    Question(const string &qText, const vector<string> &qOptions, int ans)
    {
        text = qText;
        options = qOptions;
        correctAnswer = ans;
    }
};

vector<Question> loadQuestions(const string &filename)
{
    vector<Question> questions;
    ifstream file(filename);

    if (!file)
    {
        cerr << "Error: Could not open file " << filename << endl;
        return questions;
    }

    string line;
    while (getline(file, line))
    {
        vector<string> options;
        options.push_back(line);

        for (int i = 0; i < 4; ++i)
        {
            getline(file, line);
            options.push_back(line);
        }

        getline(file, line);
        int correctAnswer = stoi(line);

        questions.emplace_back(options[0], vector<string>(options.begin() + 1, options.end()), correctAnswer);

        getline(file, line);
    }

    return questions;
}

void clearScreen()
{
    system("cls");
}

void shuffleQuestions(vector<Question> &questions)
{
    srand(static_cast<unsigned>(time(nullptr)));
    for (size_t i = 0; i < questions.size(); ++i)
    {
        size_t randomIndex = rand() % questions.size();
        swap(questions[i], questions[randomIndex]);
    }
}

void playQuiz(vector<Question> &questions)
{
    shuffleQuestions(questions);

    int score = 0;

    clearScreen();
    cout << "=== Welcome to the Quiz Game ===" << endl;
    cout << "===============================" << endl;
    cout << "Press Enter to start...\n";
    cin.ignore();
    cin.get();

    for (size_t i = 0; i < questions.size(); ++i)
    {
        clearScreen();

        const auto &q = questions[i];
        cout << q.text << endl;

        for (size_t j = 0; j < q.options.size(); ++j)
        {
            cout << (j + 1) << ") " << q.options[j] << endl;
        }

        int answer;
        cout << "Your answer: ";
        cin >> answer;

        if (answer == q.correctAnswer)
        {
            cout << "Correct!\n"
                 << endl;
            ++score;
        }
        else
        {
            cout << "Wrong! The correct answer was " << q.correctAnswer << ".\n"
                 << endl;
        }

        char choice;
        cout << "Do you want to continue? (y/n): ";
        cin >> choice;

        if (choice == 'n' || choice == 'N')
        {
            clearScreen();
            cout << "Exiting the quiz. Your final score: " << score << "/" << questions.size() << endl;
            return;
        }
    }

    clearScreen();
    cout << "Quiz completed! Your final score: " << score << "/" << questions.size() << endl;
}

int main()
{
    string filename = "questions.txt";
    auto questions = loadQuestions(filename);

    if (questions.empty())
    {
        cerr << "No questions loaded. Exiting the game." << endl;
        return 1;
    }

    playQuiz(questions);

    return 0;
}