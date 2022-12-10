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
    char *food[100] = {0};
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
int totalPrice(char* dishName, int quantity);

void updateTable(char* customerName, char* dish, int quant) {
    int index = hash(customerName);
    if (customerHead[index]) {
        if (strcmp(customerHead[index]->customerName, customerName) == 0) {
            int i = 0;
            while (customerHead[index]->food[i] != 0) {
                i++;
            }
            char quantStr[4] = {0};
            sprintf(quantStr, " x%d", quant);
            customerHead[index]->food[i] = (char *) malloc(100);
            strcpy(customerHead[index]->food[i], dish);
            strcat(customerHead[index]->food[i], quantStr);
            customerHead[index]->totalPrice += totalPrice(dish, quant); //to be removed
        }
        else {
            customer* curr = customerHead[index];
            while (curr && strcmp(curr->customerName, customerName)) {
                int i = 0;
                while (curr->food[i] != 0) {
                    i++;
                }
                char quantStr[4] = {0};
                sprintf(quantStr, " x%d", quant);
                curr->food[i] = (char *) malloc(100);
                strcpy(curr->food[i], dish);
                strcat(curr->food[i], quantStr);
                curr->totalPrice += totalPrice(dish, quant);
            }
        }

    }
}

void removeTable(int index) {
    customer* temp = customerHead[index];
    if (temp->next) {
        customerHead[index] = temp->next;
        temp->next = NULL;
        free(temp);
    }
    else {
        free(temp);
        customerHead[index] = customerTail[index] = NULL;
    }
}

int totalPrice(char* dishName, int quantity) {
    dish* curr = dishHead;
    while (curr && strcmp(curr->dishName, dishName) != 0) {
        curr = curr->next;
    }
    puts("after this will return value");
    int price = curr->dishPrice * quantity;
    printf("%d\n", price);
    return price;
}

void printHashTable() {
    puts("Customer's List");
    for (int i = 0;i<9999;i++) {
        if (customerHead[i]) {
            for (customer* curr = customerHead[i];curr;curr=curr->next) {
                printf("%d. %s\n", i, curr->customerName);
            }
        }
    }
}

void printPayment(int index) {
    printf("\n");
    customer* temp = customerHead[index];
    printf("%s\n", temp->customerName);
    for (int i = 0;temp->food[i]!= 0;i++) {
        printf("[%d] %s\n", i+1, temp->food[i]);
    }
    printf("Total Price: %d\n", temp->totalPrice);
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
void viewWarteg();
void order();
int checkQuant(char *str);
void payment();
int checkIndex(char *str);
void systemSetter();

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
    printf("System: "); systemSetter();
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
    else if (selector == 5) {
        viewWarteg();
    }
    else if (selector == 6) {
        order();
    }
    else if (selector == 7) {
        payment();
    }
    else {
        exit(0);
    }
}

void systemSetter() {
    char os[30];
    #ifdef __APPLE__
	    // apple specific code
        strcpy(os, "Mac OSX");
	#elif _WIN32
	    // windows specific code
        strcpy(os, "Windows");
	#elif __LINUX__
	    // linux specific code
        strcpy(os, "Linux");
	#else
	    // general code or warning
        strcpy(os, "Others");
	#endif
    printf("%s\n", os);
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
    if (counter == 1) {
        puts("\nThere isn't any food left");
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

void viewWarteg() {
    printHashTable();
    printf("\nPress enter to continue");
    getch();
    menu();
}

void order() {
    system("cls");
    char customerName[100];
    printf("Insert the customer's name: ");
    scanf("%[^\n]", customerName); getchar();
    if (lookupName(customerName) == 1) {
        int menuSize;
        printf("Insert the amount of dish: ");
        scanf("%d", &menuSize);
        for (int i = 0;i<menuSize;i++) {
            char dishName[100];
            char quant[5];
            //continue here
            do
            {
                printf("[%d] Insert the dish's name and quantity: ", i+1);
                scanf("%s %s", dishName, quant); getchar();
            } while (checkDish(dishName) == 0 || checkQuant(quant) == 0);
            int quantity;
            char garbage;
            sscanf(quant, "%c%d", &garbage, &quantity);
            updateTable(customerName, dishName, quantity);
        }
        puts("Order Success!");
    }
    else {
        printf("%s is not present\n", customerName);
    }
    printf("Press enter to continue");
    getchar();
    menu();
}

void payment() {
    system("cls");
    char indexStr[5];
    do
    {
        /* code */
        printf("Insert the customer's index: ");
        scanf("%s", indexStr);
    } while (checkIndex(indexStr) == 0);
    int index;
    sscanf(indexStr, "%d", &index); getchar();
    if (customerHead[index]) {
        printPayment(index);
        removeTable(index);
    }
    else {
        puts("There is no such index");
    }
    printf("Press enter to continue");
    getchar();
    menu();
}

int checkIndex(char *str) {
    int length = strlen(str);
    for (int i = 0;i<length;i++) {
        if (*(str+i) >= '0' && *(str+i) <= '9') {
            continue;
        }
        else {
            return 0;
        }
    }
    return 1;
}

int checkQuant(char *str) {
    if (*str == 'x' && *(str+1) >= '1' && *(str+1) <='9') {
        puts("format accurate"); //to be removed
        return 1;
    }
    return 0;
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
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}