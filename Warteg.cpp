#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
struct dish {
    char dishName[100];
    int dishPrice;
    int quantity;
    dish* next;
    dish* prev;
};

dish* dishHead = NULL;
dish* dishTail = NULL;

dish* createDish(dish* toBeAdded) {
    dish* newDish = (dish *) malloc(sizeof(dish));
    strcpy(newDish->dishName, toBeAdded->dishName);
    newDish->dishPrice = toBeAdded->dishPrice;
    newDish->quantity = toBeAdded->quantity;
    newDish->next = NULL;
    newDish->prev = NULL;
    return newDish;
}

void pushHead(dish* toBeAdded) {
    dish* newDish = createDish(toBeAdded);
    printf("%s\n", newDish->dishName);
    if (dishHead == NULL) {
        dishHead = dishTail = newDish;
    }
    else {
        newDish->next = dishHead;
        dishHead->prev = newDish;
        dishHead = newDish;
    }
}

void pushTail(dish* toBeAdded) {
    dish* newDish = createDish(toBeAdded);
    if (dishHead == NULL) {
        dishHead = dishTail = newDish;
    }
    else {
        dishTail->next = newDish;
        newDish->prev = dishTail;
        dishTail = newDish;
    }
}


int selector;
void menu();
void addDish();
void removeDish();
int checkLower(char dishName[]);
void load(dish* load, char dishName[], int dishPrice, int quantity);

int main() {
    menu();
    return 0;
}

void menu() {
    system("cls");
    struct tm* ptr;
    time_t t;
    t = time(NULL);
    ptr = localtime(&t);
    printf("System: \n");
    printf("%s\n", asctime(ptr));
    printf("1. Add Dish\n");
    puts("2. Remove Dish");
    puts("3. Add Customer");
    puts("4. Search Customer");
    puts("5. View Warteg");
    puts("6. Order");
    puts("7. Payment");
    puts("8. Exit Warteg");
    printf(">> ");
    scanf("%d", &selector); getchar();
    if (selector > 8 || selector < 1) {
        menu();
    }
    if (selector == 1) {
        addDish();
    }
    else if (selector == 2) {
        removeDish();
    }
}

void addDish() {
    system("cls");
    char dishName[100];
    do
    {
        printf("Insert the name of the dish [Lowercase letters]: ");
        scanf("%[^\n]", dishName); getchar();
    } while (checkLower(dishName) == 0);
    int dishPrice;
    do
    {
        printf("Insert the price of the dish [1000..50000]: ");
        scanf("%d", &dishPrice);
    } while (dishPrice < 1000 || dishPrice > 50000);
    int quantity;
    do
    {
        printf("Insert the quantity of the dish [1..999]: ");
        scanf("%d", &quantity);
    } while (quantity < 1 || quantity > 999);
    dish* temp = (dish *) malloc(sizeof(dish));
    load(temp, dishName, dishPrice, quantity);
    pushHead(temp);
    free(temp);
    puts("The dish has been added!");
    printf("\nPress enter to continue...");
    getch();
    menu();
}

int checkLower(char dishName[]) {
    for (int i = 0;i<strlen(dishName);i++) {
        if (dishName[i] >= 'A' && dishName[i] <= 'Z') {
            return 0;
        }
    }
    return 1;
}

void printDishTable() {
    dish* curr = dishHead;
    int counter = 1;
    while (curr) {
        printf("%-5d.%-25s%-10dRp%d\n", counter, curr->dishName, curr->quantity, curr->dishPrice);
        counter++;
        curr = curr->next;
    }
}

void removeDish() {
    printDishTable();
}

void load(dish* load, char dishName[], int dishPrice, int quantity) {
    strcpy(load->dishName, dishName);
    load->dishPrice = dishPrice;
    load->quantity = quantity;
}