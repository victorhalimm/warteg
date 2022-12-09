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

void popHead() {
    if (dishHead == NULL) {
        return;
    }
    else if (dishHead == dishTail) {
        dishHead = dishTail = NULL;
    }
    else {
        dish* temp = dishHead;
        temp->next->prev = NULL;
        dishHead = temp->next;
        temp->next = NULL;
        free(temp);
    }
}

void popTail() {
    if (dishHead == NULL) {
        return;
    }
    else if (dishHead == dishTail) {
        dishHead = dishTail = NULL;
    }
    else {
        dish* temp = dishTail;
        temp->prev->next = NULL;
        dishTail = temp->prev;
        temp->prev = NULL;
        free(temp);
    }
}

void popMid(char dishName[]) {
    if (dishHead == NULL) {
        return;
    }
    else if (dishHead == dishTail) {
        dishHead = dishTail = NULL;
    }
    else {
        if (strcmp(dishHead->dishName, dishName) == 0) {
            popHead();
        }
        else if (strcmp(dishTail->dishName, dishName) == 0) {
            popTail();
        }
        else {
            dish* curr = dishHead;
            while (curr) {
                if (strcmp(curr->dishName, dishName) == 0) {
                    curr->prev->next = curr->next;
                    curr->next->prev = curr->prev;
                    curr->next = curr->prev = NULL;
                    free(curr);
                    return;
                }
                curr = curr->next;
            }
        }
    }
}

unsigned long 
    hash(char *str)
    {
        unsigned long hash = 5381;
        int c;

        while (c = *str++)
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash % 9999;
    }

struct customer {
    char customerName[100];
    char* food[100] = {0};
    customer* next;
    int totalPrice;
} *customerHead[9999] = {NULL}, *customerTail[9999] = {NULL};

customer* createCustomer(char customerName[]) {
    customer* newCustomer = (customer *) malloc(sizeof(customer));
    strcpy(newCustomer->customerName, customerName);
    newCustomer->next = NULL;
    newCustomer->totalPrice = 0;
    return newCustomer;
}

void insertTable(char *customerName) {
    customer* newCustomer = createCustomer(customerName);
    int index = hash(customerName);
    if (customerHead[index]) {
        customerTail[index]->next = newCustomer;
        customerTail[index] = newCustomer;
    }
    else {
        customerHead[index] = customerTail[index] = newCustomer;
    }
}

int lookupName(char *name) {
    int index = hash(name);
    if (customerHead[index]) {
        if (strcmp(customerHead[index]->customerName, name) == 0) {
            return 1;
        }
        else {
            for (customer* curr = customerHead[index]; curr; curr = curr->next) {
                if (strcmp(curr->customerName, name) == 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}


int selector;
void menu();
void addDish();
void removeDish();
int checkLower(char dishName[]);
void load(dish* load, char dishName[], int dishPrice, int quantity);
int checkDish(char dishName[]);
int checkName(char customerName[]);
void addCustomer();
void searchCustomer();

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
    else if (selector == 3) {
        addCustomer();
    }
    else if(selector == 4) {
        searchCustomer();
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
    pushTail(temp);
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
    puts("Bude's Menu");
    printf("\n");
    dish* curr = dishHead;
    int counter = 1;
    printf("%-3s%-20s%-10s%s\n", "No", "Name", "Quantity", "Price");
    while (curr) {
        printf("%-3d%-20s%-10dRp%d\n", counter, curr->dishName, curr->quantity, curr->dishPrice);
        counter++;
        curr = curr->next;
    }
    printf("\n");
}

void removeDish() {
    system("cls");
    printDishTable();
    char dishName[100];
    printf("Insert dish's name to be deleted: ");
    scanf("%[^\n]", dishName); getchar();
    if (checkDish(dishName) == 1) {
        popMid(dishName);
        printf("\nThe dish has been removed!\n");
        printf("\nPress enter to continue");
        getch();
        menu();
    }
    else {
        puts("There is no such dish with that name");
        printf("\nPress enter to continue");
        getch();
        menu();
    }

}

void addCustomer() {
    system("cls");
    char customerName[100];
    do
    {
        printf("Insert the customer's name [Without space]: ");
        scanf("%[^\n]", customerName); getchar();
    } while (checkName(customerName) == 0);
    insertTable(customerName);
    printf("\n\nCustomer has been added!\n");
    printf("\nPress enter to continue");
    getch();
    menu();
}

void searchCustomer() {
    char customerName[100];
    system("cls");
    do
    {
        printf("Insert the customer's name to be searched: ");
        scanf("%[^\n]", customerName); getchar();
    } while (checkName(customerName) == 0);
    if (lookupName(customerName) == 1) {
        printf("%s is present\n", customerName);
    }
    else {
        printf("%s is not present\n", customerName);
    }
    printf("\nPress enter to continue");
    getchar();
    menu(); 
}

int checkName(char customerName[]) {
    int length = strlen(customerName);
    for (int i = 0;i<length;i++) {
        if (customerName[i] == ' ' && (customerName[i] >= '0' && customerName[i] <= '9')) {
            return 0;
        }
    }
    return 1;
}

void load(dish* load, char dishName[], int dishPrice, int quantity) {
    strcpy(load->dishName, dishName);
    load->dishPrice = dishPrice;
    load->quantity = quantity;
}

int checkDish(char dishName[]) {
    dish* curr = dishHead;
    while (curr) {
        if (strcmp(curr->dishName, dishName) == 0) {
            puts("check");
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}