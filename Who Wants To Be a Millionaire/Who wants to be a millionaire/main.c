#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_QUESTIONS 15
#define MAX_NAME_LENGTH 50
#define LEADERBOARD_FILE "leaderboard.txt"
#define CLEAR_SCREEN "cls"

typedef struct {
    char question[256];
    char options[4][100];
    int correct_option;
} Question;

typedef struct {
    char name[MAX_NAME_LENGTH];
    float score;
} Player;

void load_questions(Question questions[]);
void shuffle_questions(Question questions[]);
void shuffle_answers(Question *question);
void play_game();
void save_score(const char *name, float score);
void view_leaderboard();
void view_user_history();

int main() {
    int choice;
    while (1) {
        printf("\nWho Wants to Be a Millionaire?\n");
        printf("1. Play Game\n2. View Leaderboard\n3. View User History\n4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                play_game();
                break;
            case 2:
                view_leaderboard();
                break;
            case 3:
                view_user_history();
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

void load_questions(Question questions[]) {
    const char *question_texts[MAX_QUESTIONS] = {
        "What is the capital of France?",
        "Which planet is known as the Red Planet?",
        "Who wrote 'To Kill a Mockingbird'?",
        "What is the square root of 64?",
        "Who painted the Mona Lisa?",
        "What is the largest ocean on Earth?",
        "Which element has the chemical symbol 'O'?",
        "Who developed the theory of relativity?",
        "What is the hardest natural substance on Earth?",
        "Which country is known for the Great Wall?",
        "What is the currency of Japan?",
        "Which organ pumps blood in the human body?",
        "What is the tallest mountain in the world?",
        "Who discovered penicillin?",
        "Which gas do plants use for photosynthesis?"
    };

    const char *options[MAX_QUESTIONS][4] = {
        {"Paris", "London", "Berlin", "Madrid"},
        {"Mars", "Venus", "Jupiter", "Saturn"},
        {"Harper Lee", "J.K. Rowling", "Ernest Hemingway", "Mark Twain"},
        {"8", "6", "10", "12"},
        {"Leonardo da Vinci", "Pablo Picasso", "Vincent van Gogh", "Claude Monet"},
        {"Pacific Ocean", "Atlantic Ocean", "Indian Ocean", "Arctic Ocean"},
        {"Oxygen", "Hydrogen", "Carbon", "Nitrogen"},
        {"Albert Einstein", "Isaac Newton", "Galileo Galilei", "Stephen Hawking"},
        {"Diamond", "Gold", "Iron", "Quartz"},
        {"China", "India", "Japan", "Russia"},
        {"Yen", "Dollar", "Euro", "Pound"},
        {"Heart", "Liver", "Brain", "Lungs"},
        {"Mount Everest", "K2", "Kilimanjaro", "Mont Blanc"},
        {"Alexander Fleming", "Marie Curie", "Louis Pasteur", "Jonas Salk"},
        {"Carbon Dioxide", "Oxygen", "Nitrogen", "Hydrogen"}
    };

    int correct_answers[MAX_QUESTIONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < MAX_QUESTIONS; i++) {
        strcpy(questions[i].question, question_texts[i]);
        for (int j = 0; j < 4; j++) {
            strcpy(questions[i].options[j], options[i][j]);
        }
        questions[i].correct_option = correct_answers[i];
        shuffle_answers(&questions[i]);
    }
}

void shuffle_questions(Question questions[]) {
    for (int i = MAX_QUESTIONS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Question temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }
}

void shuffle_answers(Question *question) {
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp[100];
        strcpy(temp, question->options[i]);
        strcpy(question->options[i], question->options[j]);
        strcpy(question->options[j], temp);
        if (question->correct_option == i) {
            question->correct_option = j;
        } else if (question->correct_option == j) {
            question->correct_option = i;
        }
    }
}

void play_game() {
    Question questions[MAX_QUESTIONS];
    load_questions(questions);
    shuffle_questions(questions);
    float score = 0;
    int used_5050 = 0;
    char name[MAX_NAME_LENGTH];

    for (int i = 0; i < MAX_QUESTIONS; i++) {
        system(CLEAR_SCREEN);
        printf("\nQ%d: %s\n", i + 1, questions[i].question);
        for (int j = 0; j < 4; j++) {
            printf("%c. %s\n", 'A' + j, questions[i].options[j]);
        }

         printf("(Enter A-D or '50' for 50/50)\nChoice: ");
        char choice[3];
        scanf("%s", choice);

        if (strcmp(choice, "50") == 0 && !used_5050) {
            used_5050 = 1;
            int correct = questions[i].correct_option;
            int removed = 0;
            printf("\n50/50 activated! Remaining options:\n");
            for (int j = 0; j < 4; j++) {
                if (j == correct || removed == 2) {
                    printf("%c. %s\n", 'A' + j, questions[i].options[j]);
                } else {
                    removed++;
                }
            }
            printf("Choice: ");
            scanf("%s", choice);
            score += (choice[0] - 'A' == correct) ? 1 : 0;
        } else if (choice[0] - 'A' == questions[i].correct_option) {
            score += 1;
        } else {
            break;
        }
    }

    printf("\nGame over! Enter your name: ");
    scanf("%s", name);
    save_score(name, score);
    system(CLEAR_SCREEN);
}

void save_score(const char *name, float score) {
    FILE *file = fopen(LEADERBOARD_FILE, "a");
    if (file) {
        fprintf(file, "%s %.1f\n", name, score);
        fclose(file);
    }
}

void view_leaderboard() {
    system(CLEAR_SCREEN);
    FILE *file = fopen(LEADERBOARD_FILE, "r");
    if (!file) {
        printf("No leaderboard data available.\n");
        return;
    }
    char name[MAX_NAME_LENGTH];
    float score;
    printf("\nLeaderboard:\n");
    while (fscanf(file, "%s %f", name, &score) == 2) {
        printf("%s - %.1f\n", name, score);
    }
    fclose(file);
}

void view_user_history() {
    system(CLEAR_SCREEN);
    char search_name[MAX_NAME_LENGTH];
    printf("Enter name to search: ");
    scanf("%s", search_name);

    FILE *file = fopen(LEADERBOARD_FILE, "r");
    if (!file) {
        printf("No history available.\n");
        return;
    }
    char name[MAX_NAME_LENGTH];
    float score;
    int found = 0;
    printf("\nHistory for %s:\n", search_name);
    while (fscanf(file, "%s %f", name, &score) == 2) {
        if (strcmp(name, search_name) == 0) {
            printf("%.1f\n", score);
            found = 1;
        }
    }
    if (!found){
        printf("No records found for %s.\n", search_name);
    }
    fclose(file);
}
