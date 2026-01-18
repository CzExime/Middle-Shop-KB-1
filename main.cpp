#include <iostream>
#include <Windows.h>
#include <ctime>
#include <iomanip>
#include <string>
#include <unordered_set>
#include <algorithm>

// ---------------------------------------------------------------------------------------------- User's --------------------------------------------------------------------------------------------

size_t userSize = 2;
size_t staffCount = 1;
std::string userStatus[3]{ "Супер администратор", "Администратор", "Сотрудник" };
std::string* loginArr = new std::string[userSize]{ "admin","user" };
std::string* passArr = new std::string[userSize]{ "admin","user" };
std::string* statusArr = new std::string[userSize]{ userStatus[0], userStatus[2] };
double* awardArr = new double[userSize] {0.0, 0.0};
std::string currentStatus;
unsigned int* userIdArr = new unsigned[userSize] {1, 2};
int currentId = -1;

//  -------------------------------------------------------------------------------------------- STORAGE  --------------------------------------------------------------------------------------------

std::size_t storageSize = 0;
bool staticStorageCreated = false;

int* idArr = nullptr;
std::string* nameArr = nullptr;
unsigned int* countArr = nullptr;
double* priceArr = nullptr;

// Функции склада
void CreateStorage();
void ShowStorage(int mode = 0);
template<typename T>
void SwapArr(T*& Arr, T* Arr2, std::size_t SizeArr);
void AddStorageItem();
void RemoveStorageItem();
void ChangePrice();
void ChangeStorage();
void AddNewItem();
void ChangeName();
void DeleteItem();
void AddNewstorage();
void ShowAccount(int mode = 0);
void ChangeUsers();
void ChangePass();
void AddNewUser();
void DeleteUser();

// ---------------------------------------------------------------------------------------------- SELL ----------------------------------------------------------------------------------------------

size_t checkSize = 0;
int* idArrCheck = nullptr;
std::string* nameArrCheck = nullptr;
unsigned int* countArrCheck = nullptr;
double* priceArrCheck = nullptr;
double* totalPriceArrCheck = nullptr;

double cash = 4000 + rand() % 7000;
double cashIncome = 0.0;
double bankIncome = 0.0;

bool discountUsed = false; //использование скидки в текущей покупке

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void PrintCheck(double totalSum);
void StorageReturner();
void Selling();
void CheckArrAppend();
void FreeCheckMemory();
// функции - скидки
void SellActiv(double& totalSum);
void ShowIncome();

// Служебные функции
void Start();
bool Login();
bool Logout();
void ShowSuperAdminMenu();
void ShowAdminMenu();
void ShowUserMenu();
inline void Getline(std::string& str);
inline void Err(int time = 1500);
bool CheckLogin(const std::string& str);
bool CheckPass(const std::string& str);
void Error();
bool IsNumber(const std::string& str);

//  -------------------------------------------------------------------------------------------- Main  ---------------------------------------------------------------------------------------------

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::srand(std::time(NULL));

    Start();

    // Освобождение памяти
    delete[] loginArr;
    delete[] passArr;
    delete[] statusArr;
    delete[] awardArr;
    delete[] userIdArr;

    if (staticStorageCreated)
    {
        delete[] idArr;
        delete[] nameArr;
        delete[] countArr;
        delete[] priceArr;
    }

    FreeCheckMemory(); //Очистка памяти чеков

    return 0;
}

void Selling()
{
    std::string chooseId, chooseCount, choose, chooseCash;
    unsigned int id = 0, count = 0;
    double totalSum = 0.0, money = 0.0;
    bool isFirst = false;

    //сброс скидки для новой покупки
    discountUsed = false;

    //Очистка старых чеков
    FreeCheckMemory();
    checkSize = 0;

    while (true)
    {
        ShowStorage();

        std::cout << "\nВведите ID товара для покупки или \"exit\" для завершения покупок - ";
        Getline(chooseId);

        if (chooseId == "exit")
        {
            if (!isFirst)
            {
                std::cout << "Выход без покупок\n";
                Sleep(1500);
                break;
            }

            //Вывод чека и подтверждение покупки
            system("cls");
            PrintCheck(totalSum);


            if (!discountUsed && checkSize > 0)
            {
                std::cout << "\nПодтвердите покупку?\n1 - Да\n2 - Добавить ещё товар\n3 - Отмена\n4 - Принять участие в акции\nВвод - ";
            }
            else
            {
                std::cout << "\nПодтвердите покупку?\n1 - Да\n2 - Добавить ещё товар\n3 - Отмена\nВвод - ";
            }


            Getline(choose);

            if (choose == "1")
            {
                while (true)
                {
                    system("cls");
                    std::cout << "Выберите способ оплаты\n1 - Наличными\n2 - Безнал\nВвод - ";
                    Getline(choose);

                    if (choose == "1")
                    {
                        std::cout << "Введите кол-во наличных - ";
                        Getline(chooseCash);

                        if (IsNumber(chooseCash))
                        {
                            money = std::stod(chooseCash);

                            if (money < totalSum)
                            {
                                std::cout << "Недостаточно средств!\n";
                                Sleep(1500);
                                continue;
                            }
                            else
                            {
                                double change = money - totalSum;

                                std::cout << "Ваши " << money << " рублей\n";
                                Sleep(400);

                                if (change > 0)
                                {
                                    if (change <= cash)
                                    {
                                        cash += totalSum;
                                        cashIncome += totalSum;
                                        std::cout << "Оплата прошла успешно. Ваша сдача " << change << " рублей\n";
                                    }
                                    else
                                    {
                                        std::cout << "Нет возможности дать сдачи. Повторите попытку\n";
                                        Sleep(1500);
                                        continue;
                                    }
                                }
                                else
                                {
                                    cash += money;
                                    cashIncome += totalSum;
                                    std::cout << "Оплата прошла успешно. Сдачи нет\n";
                                }

                                if (currentId > 0)
                                {
                                    awardArr[currentId] += totalSum * 0.01;
                                }

                                Sleep(1800);
                                system("cls");
                                break;
                            }
                        }
                        else
                        {
                            std::cout << "Некорректная сумма!\n";
                            Sleep(1500);
                            continue;
                        }
                    }
                    else if (choose == "2")
                    {
                        system("cls");
                        std::cout << "Приложите карту\n\n";
                        Sleep(1000);

                        if (rand() % 10 <= 2)
                        {
                            for (size_t i = 0; i < 5; i++)
                            {
                                std::cout << i + 1 << "\t";
                                Sleep(800);
                            }
                            std::cout << "\nСоединение не установлено. Повторите попытку\n";
                            Sleep(1500);
                            continue;
                        }
                        else
                        {
                            for (size_t i = 0; i < 5; i++)
                            {
                                std::cout << i + 1 << "\t";
                                Sleep(800);
                            }
                            std::cout << "\nОплата прошла успешно\n\nСпасибо за покупку!\n\n";

                            bankIncome += totalSum;

                            if (currentId > 0)
                            {
                                awardArr[currentId] += totalSum * 0.01;
                            }

                            Sleep(2500);
                            system("cls");
                            break;
                        }
                    }
                    else if (choose == "Den" || choose == "den")
                    {
                        std::cout << "За вас оплатили уже\n";
                        Sleep(3000);
                        break;
                    }
                    else
                    {
                        Err();
                        continue;
                    }
                }

                //Освобождение памяти после успешной покупки
                FreeCheckMemory();
                checkSize = 0;
                totalSum = 0.0;
                isFirst = false;
                discountUsed = false; //Сброс скидок

                std::cout << "Покупка завершена успешно!\n";
                system("pause");
                system("cls");
                break;
            }
            else if (choose == "2")
            {
                system("cls");
                continue;
            }
            else if (choose == "3")
            {
                std::cout << "Отмена покупки!\n";
                StorageReturner();
                FreeCheckMemory();
                checkSize = 0;
                totalSum = 0.0;
                isFirst = false;
                discountUsed = false; //Сброс скидок

                Sleep(1500);
                system("cls");
                return;
            }
            else if (choose == "4" && !discountUsed && checkSize > 0)
            {
                SellActiv(totalSum);
                system("cls");
                continue;
            }
            else
            {
                Err();
                system("cls");
                continue;
            }
        }

        // Проверка ID товара
        if (IsNumber(chooseId))
        {
            id = std::stoi(chooseId) - 1;

            if (id < 0 || id >= storageSize)
            {
                std::cout << "Ошибка ID! Нет такого товара\n";
                Sleep(1500);
                system("cls");
                continue;
            }
        }
        else
        {
            std::cout << "Некорректный ID!\n";
            Sleep(1500);
            system("cls");
            continue;
        }

        // Ввод кол-ва товара
        std::cout << "\nВведите кол-во товара или \"exit\" для выбора другого товара - ";
        Getline(chooseCount);

        if (chooseCount == "exit")
        {
            std::cout << "Отмена покупки товара - " << nameArr[id] << "\n";
            Sleep(1500);
            system("cls");
            continue;
        }

        if (IsNumber(chooseCount))
        {
            count = std::stoi(chooseCount);

            if (count <= 0)
            {
                std::cout << "Количество должно быть больше 0!\n";
                Sleep(1500);
                system("cls");
                continue;
            }

            if (count > countArr[id])
            {
                std::cout << "Ошибка количества! Максимум - " << countArr[id] << "\n";
                Sleep(1500);
                system("cls");
                continue;
            }
        }
        else
        {
            std::cout << "Некорректное количество!\n";
            Sleep(1500);
            system("cls");
            continue;
        }

        // Добавление товара в чек
        CheckArrAppend();


        int lastIndex = checkSize - 1;


        idArrCheck[lastIndex] = id + 1;
        nameArrCheck[lastIndex] = nameArr[id];
        priceArrCheck[lastIndex] = priceArr[id];
        countArrCheck[lastIndex] = count;
        totalPriceArrCheck[lastIndex] = count * priceArr[id];


        countArr[id] -= count;
        totalSum += count * priceArr[id];

        std::cout << "\nТовар добавлен в чек\n\n";
        isFirst = true;
        Sleep(1000);
        system("cls");
    }
}

void CheckArrAppend()
{

    size_t oldSize = checkSize;


    checkSize++;


    int* tempIdArr = new int[checkSize];
    std::string* tempNameArr = new std::string[checkSize];
    unsigned int* tempCountArr = new unsigned int[checkSize];
    double* tempPriceArr = new double[checkSize];
    double* tempTotalArr = new double[checkSize];


    if (oldSize > 0)
    {
        for (size_t i = 0; i < checkSize; i++)
        {
            tempIdArr[i] = idArrCheck[i];
            tempNameArr[i] = nameArrCheck[i];
            tempCountArr[i] = countArrCheck[i];
            tempPriceArr[i] = priceArrCheck[i];
            tempTotalArr[i] = totalPriceArrCheck[i];
        }
    }


    delete[] idArrCheck;
    delete[] nameArrCheck;
    delete[] countArrCheck;
    delete[] priceArrCheck;
    delete[] totalPriceArrCheck;


    idArrCheck = tempIdArr;
    nameArrCheck = tempNameArr;
    countArrCheck = tempCountArr;
    priceArrCheck = tempPriceArr;
    totalPriceArrCheck = tempTotalArr;
}

// РЕАЛИЗАЦИЯ АКЦИЙ
void SellActiv(double& totalSum)
{
    std::string choose;
    double result = 0.0;
    system("cls");
    std::cout << "Акции в нашем магазине\n";
    std::cout << "Акция 1. При покупке на сумму свыше 20000 рублей - скидка 10% на весь чек\n";
    std::cout << "Акция 2. При покупке двух рулей - третий в подарок\n";
    std::cout << "\n\nВыберите акцию (1-2) или \"exit\" для отмены: ";
    Getline(choose);

    if (choose == "exit")
    {
        std::cout << "Отмена участия в акции\n";
        Sleep(1000);
        return;
    }

    if (discountUsed)
    {
        std::cout << "Вы уже использовали скидку в этой покупке!\n";
        Sleep(1500);
        return;
    }

    if (choose == "1")
    {
        if (totalSum > 20000.0)
        {
            double discount = totalSum * 0.10;
            double newTotal = totalSum - discount;

            std::cout << "\nВаш чек составляет сумму: " << totalSum << " рублей\n";
            std::cout << "Скидка 10%: " << discount << " рублей\n";
            std::cout << "Новая сумма: " << newTotal << " рублей\n";
            std::cout << "Подтвердить применение скидки? 1 - Да, 2 - Нет\n";
            std::cout << "Ввод: ";
            Getline(choose);

            if (choose == "1")
            {
                totalSum = newTotal;
                std::cout << "Скидка применена!\n";
                discountUsed = true;
                Sleep(1500);
                return;
            }
            else
            {
                std::cout << "Скидка не применена\n";
                Sleep(1000);
                return;
            }
        }
        else
        {
            result = 20000.0 - totalSum;
            std::cout << "\nВаша сумма: " << totalSum << " рублей. Меньше 20000 рублей.\n";
            std::cout << "Вам не хватает еще: " << result << " рублей\n";
            std::cout << "Добавить товаров на " << result << " рублей? 1 - Да, 2 - Нет\n";
            Getline(choose);

            if (choose == "1")
            {
                std::cout << "Продолжайте добавлять товары...\n";
                Sleep(1000);
                return;
            }
            else
            {
                std::cout << "Возврат к покупкам...\n";
                Sleep(1000);
                return;
            }
        }
    }
    else if (choose == "2")
    {
        // Проверяем, есть ли рули в чеке
        int steeringWheelCount = 0;
        int steeringWheelIndex = -1;

        // Находим товар "Спорт-Руль" (ID = 10)
        for (size_t i = 0; i < checkSize; i++)
        {
            if (idArrCheck[i] == 10) // ID руля = 10
            {
                steeringWheelCount = countArrCheck[i];
                steeringWheelIndex = i;
                break;
            }
        }

        if (steeringWheelCount >= 2)
        {
            std::cout << "\nВ вашем чеке " << steeringWheelCount << " руля(ей)\n";
            std::cout << "Акция: купи 2 руля - получи третий в подарок!\n";
            std::cout << "Вы можете получить " << (steeringWheelCount / 2) << " руль(ей) в подарок\n";
            std::cout << "\nПрименить акцию? 1 - Да, 2 - Нет\n";
            std::cout << "Ввод: ";
            Getline(choose);

            if (choose == "1")
            {
                int freeWheels = steeringWheelCount / 2;
                double discount = freeWheels * priceArr[9] - 2500; // priceArr[9] - цена руля
                double newTotal = totalSum - discount;

                // Добавляем бесплатные рули в чек
                if (steeringWheelIndex != -1)
                {
                    countArrCheck[steeringWheelIndex] += freeWheels;
                    totalPriceArrCheck[steeringWheelIndex] = countArrCheck[steeringWheelIndex] * priceArrCheck[steeringWheelIndex];

                    // Обновляем общую сумму
                    totalSum = newTotal;

                    // Вычитаем бесплатные рули из склада
                    countArr[9] -= freeWheels; // index 9 - Спорт-Руль

                    std::cout << "\nВы получили " << freeWheels << " руль(ей) в подарок!\n";
                    std::cout << "Скидка: " << discount << " рублей\n";
                    std::cout << "Новая сумма: " << newTotal << " рублей\n";
                    discountUsed = true;
                    Sleep(2000);
                    return;
                }
            }
            else if (choose == "2")
            {
                std::cout << "Акция не применена\n";
                Sleep(1000);
                return;
            }
            else
            {
                Err();
                return;
            }
        }
        else
        {
            if (steeringWheelCount == 1)
            {
                std::cout << "\nУ вас в чеке только 1 руль. Добавьте еще один руль для участия в акции!\n";
            }
            else
            {
                std::cout << "\nВ вашем чеке нет рулей. Добавьте рули для участия в акции!\n";
            }

            std::cout << "Продолжить покупки? 1 - Да, 2 - Нет (вернуться к выбору акций)\n";
            std::cout << "Ввод: ";
            Getline(choose);

            if (choose == "1")
            {
                std::cout << "Продолжайте добавлять товары...\n";
                Sleep(1000);
                return;
            }
            else
            {
                std::cout << "Возврат к выбору акций...\n";
                Sleep(1000);
                return;
            }
        }
    }
    else
    {
        Err();
    }
}

void PrintCheck(double totalSum)
{
    if (checkSize == 0)
    {
        std::cout << "Чек пуст!\n";
        return;
    }

    std::cout << "ID\t" << std::left << std::setw(25) << "Название товара"
        << "Цена 1 еденицу товара\t\t" << "Количество\t" << "Итого:\n";
    std::cout << std::string(90, '-') << std::endl;

    for (std::size_t i = 0; i < checkSize; i++)
    {
        std::cout << idArrCheck[i] << "\t" << std::left << std::setw(20) << nameArrCheck[i]
            << "\t\t" << priceArrCheck[i] << " Рублей\t\t"
            << countArrCheck[i] << "\t\t" << totalPriceArrCheck[i] << "\n";
    }

    std::cout << std::string(90, '-') << std::endl;
    std::cout << "\nОбщая сумма: " << totalSum << " рублей\n";

    if (discountUsed)
    {
        std::cout << "Скидка уже была применена к этой покупке\n";
    }
}

void StorageReturner()
{
    if (checkSize == 0 || idArrCheck == nullptr)
    {
        return;
    }

    for (size_t i = 0; i < checkSize; i++)
    {
        int itemId = idArrCheck[i] - 1;
        if (itemId >= 0 && itemId < storageSize)
        {
            countArr[itemId] += countArrCheck[i];
        }
        else
        {
            std::cerr << "Ошибка: неверный ID товара в чеке: " << idArrCheck[i] << std::endl;
        }
    }
}

void FreeCheckMemory()
{
    delete[] idArrCheck;
    delete[] nameArrCheck;
    delete[] countArrCheck;
    delete[] priceArrCheck;
    delete[] totalPriceArrCheck;

    idArrCheck = nullptr;
    nameArrCheck = nullptr;
    countArrCheck = nullptr;
    priceArrCheck = nullptr;
    totalPriceArrCheck = nullptr;

    checkSize = 0;
}

void ShowIncome()
{
    system("cls");
    std::cout << "Текущая прибыль за смену\n\n";
    std::cout << "Наличный расчёт: " << cashIncome << "\n";
    std::cout << "Безналичный расчёт: " << bankIncome << "\n";
    std::cout << "Итого: " << cashIncome + bankIncome << "\n\n";
    if (currentId >= 0 && currentId < userSize)
    {
        std::cout << "Сумма ваших продаж: " << awardArr[currentId] << "\n";
    }
    system("pause");
    system("cls");
}

//  ----------------------------------------------------------------------------------- СЛУЖЕБНЫЕ ФУНКЦИИ  -------------------------------------------------------------------------------------------------------------

void Start()
{
    std::string choose;
    while (true)
    {
        if (Login())
        {
            if (currentStatus == userStatus[0])
            {
                while (true)
                {
                    system("cls");               
                    std::cout << "Выберите склад:\n1 - Готовый\n2 - Создать новый\nВыбор: ";
                    Getline(choose);

                    if (choose == "1")
                    {
                        if (staticStorageCreated == false)
                        {
                            CreateStorage();
                        }
                        system("cls");
                        ShowSuperAdminMenu();
                        break;
                    }
                    else if (choose == "2")
                    {
                        if (staticStorageCreated == false)
                        {
                            AddNewstorage();
                        }
                        break;
                    }
                    else
                    {
                        Err();
                    }
                }
            }
            else if (currentStatus == userStatus[1])
            {
                if (staticStorageCreated == false)
                {
                    CreateStorage();
                }
                system("cls");
                ShowAdminMenu();
            }
            else if (currentStatus == userStatus[2])
            {
                if (staticStorageCreated == false)
                {
                    CreateStorage();
                }
                system("cls");
                ShowUserMenu();
            }
        }
        else
        {
            system("cls");
            std::cout << "Введите пароль супер администратора для закрытия смены: ";
            Getline(choose);
            if (choose == passArr[0])
            {
                system("cls");
                std::cout << "Итоговая прибыль за смену - " << cashIncome + bankIncome << " рублей";
                Sleep(1500);
                std::cout << "\n\nЗавершение работы магазина\n\n";
                Sleep(2000);
                break;
            }
        }
    }
}

bool Login()
{
    std::string login, pass;

    while (true)
    {
        system("cls");
        std::cout << "\n\n\n\t\t\tAutoTune\n\n\n";
        std::cout << "Введите логин: ";
        Getline(login);
        std::cout << "Введите пароль: ";
        Getline(pass);

        if (login == "exit" && pass == "exit")
        {
            currentStatus = "";
            currentId = -1;
            return false;
        }

        if (login == loginArr[0] && pass == passArr[0])
        {
            std::cout << "Пользователь - " << loginArr[0] << "\n\nДобро пожаловать!\n";
            std::cout << "Ваш статус - " << statusArr[0] << "\n\n";
            currentStatus = statusArr[0];
            currentId = 0;
            system("pause");
            return true;
        }

        for (std::size_t i = 0; i < userSize; i++)
        {
            if (login == loginArr[i] && pass == passArr[i])
            {
                std::cout << "Пользователь - " << loginArr[i] << "\n\nДобро пожаловать!\n";
                std::cout << "Ваш статус - " << statusArr[i] << "\n\n";
                currentStatus = statusArr[i];
                currentId = i;
                system("pause");
                return true;
            }
        }
        std::cout << "Неверный логин или пароль!\n";
        Sleep(1500);
    }
}

bool Logout()
{
    std::string choose;
    while (true)
    {
        system("cls");
        std::cout << "Для подтверждения выхода введите свой пароль или \"exit\" для возврата в меню: ";
        Getline(choose);
        if (choose == "exit")
        {
            system("cls");
            return false;
        }
        else if (choose == passArr[currentId] || (currentId == 0 && choose == passArr[0]))
        {
            system("cls");
            currentId = -1;
            return true;
        }
        else
        {
            Err();
        }
    }
}

void ShowSuperAdminMenu()
{
    std::string choose;

    while (true)
    {
        system("cls");
        std::cout << "1 - Начать продажу\n";
        std::cout << "2 - Показать склад\n";
        std::cout << "3 - Пополнить склад\n";
        std::cout << "4 - Списать товар\n";
        std::cout << "5 - Изменить цену\n";
        std::cout << "6 - Редактировать склад\n";
        std::cout << "7 - Редактировать персонал\n";
        std::cout << "8 - Отчет о прибыли\n";
        std::cout << "0 - Закрыть магазин\n";
        std::cout << "Выбор: ";
        Getline(choose);

        if (choose == "1" && storageSize > 0)
        {
            Selling();
        }
        else if (choose == "2" && storageSize > 0)
        {
            ShowStorage();
        }
        else if (choose == "3" && storageSize > 0)
        {
            AddStorageItem();
        }
        else if (choose == "4" && storageSize > 0)
        {
            RemoveStorageItem();
        }
        else if (choose == "5" && storageSize > 0)
        {
            ChangePrice();
        }
        else if (choose == "6")
        {
            ChangeStorage();
        }
        else if (choose == "7")
        {
            ChangeUsers();
        }
        else if (choose == "8")
        {
            ShowIncome();
        }
        else if (choose == "0")
        {
            if (Logout())
            {
                break;
            }
        }
        else
        {
            if (storageSize <= 0)
            {
                std::cout << "Склад пустой!\n";
            }
            Err();
        }
    }
}

void ShowAdminMenu()
{
    std::string choose;

    while (true)
    {
        system("cls");
        std::cout << "1 - Начать продажу\n";
        std::cout << "2 - Показать склад\n";
        std::cout << "3 - Пополнить склад\n";
        std::cout << "4 - Списать товар\n";
        std::cout << "5 - Редактировать склад\n";
        std::cout << "6 - Редактировать персонал\n";
        std::cout << "7 - Отчет о прибыли\n";
        std::cout << "0 - Закрыть магазин\n";
        std::cout << "Выбор: ";
        Getline(choose);

        if (choose == "1" && storageSize > 0)
        {
            Selling();
        }
        else if (choose == "2" && storageSize > 0)
        {
            ShowStorage();
        }
        else if (choose == "3" && storageSize > 0)
        {
            AddStorageItem();
        }
        else if (choose == "4" && storageSize > 0)
        {
            RemoveStorageItem();
        }
        else if (choose == "5")
        {
            ChangeStorage();
        }
        else if (choose == "6")
        {
            ChangeUsers();
        }
        else if (choose == "7")
        {
            ShowIncome();
        }
        else if (choose == "0")
        {
            if (Logout())
            {
                break;
            }
        }
        else
        {
            if (storageSize <= 0)
            {
                std::cout << "Склад пустой!\n";
            }
            Err();
        }
    }
}

void ShowUserMenu()
{
    std::string choose;

    while (true)
    {
        system("cls");
        std::cout << "1 - Начать продажу\n";
        std::cout << "2 - Показать склад\n";
        std::cout << "3 - Отчет о прибыли\n";
        std::cout << "0 - Закрыть смену\n";
        std::cout << "Выбор: ";
        Getline(choose);

        if (choose == "1" && storageSize > 0)
        {
            Selling();
        }
        else if (choose == "2" && storageSize > 0)
        {
            ShowStorage();
        }
        else if (choose == "3")
        {
            ShowIncome();
        }
        else if (choose == "0")
        {
            if (Logout())
            {
                break;
            }
        }
        else
        {
            if (storageSize <= 0)
            {
                std::cout << "Склад пустой!\n";
            }
            Err();
        }
    }
}

//  -------------------------------------------------------------------------------------  STORAGE  -------------------------------------------------------------------------------------------------------------------

bool IsNumber(const std::string& str)
{
    if (str.empty() || str.size() >= 10)
    {
        return false;
    }

    for (std::size_t i = 0; i < str.size(); i++)
    {
        if (!std::isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

void CreateStorage()
{
    const int staticSize = 10;
    int id[staticSize]{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::string name[staticSize]{
        "Турбо-кит","Билет-Блок (2JZ)",
        "Комплект дисков (Work Emotion T7R)","Комплект шин (Toyo Proxes Sport)",
        "Подвеска ( койлы )","Спорт-Топливо 10 литров (109 актан)",
        "Система Метанола","Метанол 5 литров",
        "Датчики (надув, давление масла)", "Спорт-Руль"
    };
    unsigned int count[staticSize]{ 32, 54, 213, 65, 23, 88, 433, 42, 66, 100 };
    double price[staticSize]{ 150000, 999999, 450000, 150000, 200000, 50000, 200000, 50000, 5000, 2500 };

    if (staticStorageCreated)
    {
        delete[] idArr;
        delete[] nameArr;
        delete[] countArr;
        delete[] priceArr;
    }

    storageSize = staticSize;
    idArr = new int[storageSize];
    nameArr = new std::string[storageSize];
    countArr = new unsigned int[storageSize];
    priceArr = new double[storageSize];

    staticStorageCreated = true;

    SwapArr(idArr, id, storageSize);
    SwapArr(priceArr, price, storageSize);
    SwapArr(countArr, count, storageSize);
    SwapArr(nameArr, name, storageSize);
}

void ShowStorage(int mode)
{
    system("cls");

    if (storageSize == 0)
    {
        std::cout << "Склад пуст!\n";
        system("pause");
        return;
    }

    if (mode == 0)
    {
        std::cout << "ID\t" << std::left << std::setw(37) << "Название товара\t\t" << "Цена\t\t" << "Количество\n";
        std::cout << std::string(80, '-') << std::endl;

        for (std::size_t i = 0; i < storageSize; i++)
        {
            std::cout << idArr[i] << "\t" << std::left << std::setw(45) << nameArr[i]
                << priceArr[i] << " Рублей\t" << countArr[i] << "\n";
        }
        system("pause");
    }
    else if (mode == 1)
    {
        std::cout << "ID\t" << std::left << std::setw(37) << "Название товара" << "Количество\n";
        std::cout << std::string(80, '-') << std::endl;

        for (std::size_t i = 0; i < storageSize; i++)
        {
            std::cout << idArr[i] << "\t" << std::left << std::setw(45) << nameArr[i] << countArr[i] << "\n";
        }
    }
    else if (mode == 2)
    {
        std::cout << "ID\t" << std::left << std::setw(37) << "Название товара" << "Цена\n";
        std::cout << std::string(80, '-') << std::endl;

        for (std::size_t i = 0; i < storageSize; i++)
        {
            std::cout << idArr[i] << "\t" << std::left << std::setw(45) << nameArr[i] << priceArr[i] << " Рублей\n";
        }
    }
    else if (mode == 3)
    {
        std::cout << "ID\t" << std::left << std::setw(37) << "Название товара" << "\n";
        std::cout << std::string(80, '-') << std::endl;

        for (std::size_t i = 0; i < storageSize; i++)
        {
            std::cout << idArr[i] << "\t" << std::left << std::setw(45) << nameArr[i] << "\n";
        }
    }
    else
    {
        std::cout << "Ошибка режима отображения\n\n";
    }
}

void AddStorageItem()
{
    std::string chooseId, chooseCount, choose;
    int id = 0;
    unsigned int count = 0;

    while (true)
    {
        system("cls");
        ShowStorage(1);
        std::cout << "\nВведите ID товара или \"exit\" для выхода: ";
        Getline(chooseId);
        if (chooseId == "exit")
        {
            std::cout << "Отмена операции пополнения\n";
            Sleep(1500);
            break;
        }

        std::cout << "Введите количество товара для пополнения: ";
        Getline(chooseCount);

        if (IsNumber(chooseId) && IsNumber(chooseCount))
        {
            id = std::stoi(chooseId) - 1;
            count = std::stoi(chooseCount);

            if (id < 0 || id >= storageSize || count <= 0 || count > 199)
            {
                std::cout << "Некорректный ID или количество! Максимальное количество 199\n";
                Sleep(1500);
            }
            else
            {
                std::cout << std::left << std::setw(25) << nameArr[id] << "\t"
                    << countArr[id] << " --> " << countArr[id] + count << "\n\n";
                std::cout << "Подтвердить?\n1 - Да\n2 - Нет\nВыбор: ";
                Getline(choose);

                if (choose == "1")
                {
                    countArr[id] += count;
                    std::cout << "Товар успешно пополнен!\n\n";
                    Sleep(1500);
                    break;
                }
                else if (choose == "2")
                {
                    std::cout << "Отмена пополнения товара\n";
                    Sleep(1500);
                }
                else
                {
                    Err();
                }
            }
        }
        else
        {
            Error();
        }
    }
}

void RemoveStorageItem()
{
    std::string chooseId, chooseCount, choose;
    int id = 0;
    unsigned int count = 0;

    while (true)
    {
        system("cls");
        ShowStorage(1);
        std::cout << "\nВведите ID товара или \"exit\" для выхода: ";
        Getline(chooseId);
        if (chooseId == "exit")
        {
            std::cout << "Отмена операции списания товара\n";
            Sleep(1500);
            break;
        }

        std::cout << "Введите количество товара для списания: ";
        Getline(chooseCount);

        if (IsNumber(chooseId) && IsNumber(chooseCount))
        {
            id = std::stoi(chooseId) - 1;
            count = std::stoi(chooseCount);

            if (id < 0 || id >= storageSize || count <= 0 || count > countArr[id])
            {
                std::cout << "Некорректный ID или количество!\n";
                Sleep(1500);
            }
            else
            {
                std::cout << std::left << std::setw(25) << nameArr[id] << "\t"
                    << countArr[id] << " --> " << countArr[id] - count << "\n\n";
                std::cout << "Подтвердить?\n1 - Да\n2 - Нет\nВыбор: ";
                Getline(choose);

                if (choose == "1")
                {
                    countArr[id] -= count;
                    std::cout << "Товар успешно списан!\n\n";
                    Sleep(1500);
                    break;
                }
                else if (choose == "2")
                {
                    std::cout << "Отмена списания товара\n";
                    Sleep(1500);
                }
                else
                {
                    Err();
                }
            }
        }
        else
        {
            Error();
        }
    }
}

void ChangePrice()
{
    std::string chooseId, newPrice, choose;
    int id = 0;
    double price = 0.0;

    while (true)
    {
        system("cls");
        ShowStorage(2);
        std::cout << "\nВведите ID товара или \"exit\" для выхода: ";
        Getline(chooseId);
        if (chooseId == "exit")
        {
            std::cout << "Отмена операции изменения цены\n";
            Sleep(1500);
            break;
        }

        std::cout << "Введите новую цену товара: ";
        Getline(newPrice);

        if (IsNumber(chooseId) && IsNumber(newPrice))
        {
            id = std::stoi(chooseId) - 1;
            price = std::stod(newPrice);

            if (id < 0 || id >= storageSize || price < 0.0 || price > 999999)
            {
                std::cout << "Некорректный ID или цена! Максимальная цена 999999\n";
                Sleep(1500);
            }
            else
            {
                std::cout << std::left << std::setw(25) << nameArr[id] << "\t"
                    << priceArr[id] << " --> " << price << "\n\n";
                std::cout << "Подтвердить?\n1 - Да\n2 - Нет\nВыбор: ";
                Getline(choose);

                if (choose == "1")
                {
                    priceArr[id] = price;
                    std::cout << "Цена успешно изменена!\n\n";
                    Sleep(1500);
                    break;
                }
                else if (choose == "2")
                {
                    std::cout << "Отмена изменения цены\n";
                    Sleep(1500);
                }
                else
                {
                    Err();
                }
            }
        }
        else
        {
            Error();
        }
    }
}

void AddNewItem()
{
    std::string newName, newPrice, newCount, choose;
    double price = 0.0;
    unsigned int count = 0;
    bool exit = true;

    while (true)
    {
        while (true)
        {
            system("cls");
            std::cout << "\tДобавление нового товара!\n\nВведите \"exit\" для прекращения операции\n\n";
            std::cout << "Введите название нового товара: ";
            Getline(newName);
            if (newName == "exit")
            {
                std::cout << "Операция добавления товара прервана!\n\n";
                Sleep(1500);
                exit = false;
                break;
            }

            if (newName.empty() || newName.size() >= 60)
            {
                std::cout << "Ошибка названия. Максимальная длина 60 символов\n";
                Sleep(1500);
            }
            else
            {
                break;
            }
        }
        if (!exit) break;

        while (true)
        {
            system("cls");
            std::cout << "\tДобавление нового товара!\n\nВведите \"exit\" для прекращения операции\n\n";
            std::cout << "Введите количество нового товара: ";
            Getline(newCount);
            if (newCount == "exit")
            {
                std::cout << "Операция добавления товара прервана!\n\n";
                Sleep(1500);
                exit = false;
                break;
            }
            if (IsNumber(newCount))
            {
                count = std::stoi(newCount);
                if (count > 199)
                {
                    std::cout << "Ошибка максимального количества товара. Не более 199 ед.\n";
                    Sleep(1500);
                }
                else
                {
                    break;
                }
            }
            else
            {
                Error();
            }
        }
        if (!exit) break;

        while (true)
        {
            system("cls");
            std::cout << "\tДобавление нового товара!\n\nВведите \"exit\" для прекращения операции\n\n";
            std::cout << "Введите цену нового товара: ";
            Getline(newPrice);
            if (newPrice == "exit")
            {
                std::cout << "Операция добавления товара прервана!\n\n";
                Sleep(1500);
                exit = false;
                break;
            }
            if (IsNumber(newPrice))
            {
                price = std::stod(newPrice);
                if (price > 3000)
                {
                    std::cout << "Ошибка: слишком высокая цена.\n";
                    Sleep(1500);
                }
                else
                {
                    break;
                }
            }
            else
            {
                Error();
            }
        }
        if (!exit) break;

        while (true)
        {
            system("cls");
            std::cout << "Новый товар: " << newName << "\n";
            std::cout << "Количество: " << count << "\n";
            std::cout << "Цена: " << price << "\n\n";
            std::cout << "Подтвердить?\n1 - Да\n2 - Нет\nВыбор: ";
            Getline(choose);
            if (choose == "1")
            {
                storageSize++;
                int* idArrTemp = new int[storageSize];
                std::string* nameArrTemp = new std::string[storageSize];
                unsigned int* countArrTemp = new unsigned int[storageSize];
                double* priceArrTemp = new double[storageSize];

                SwapArr(idArrTemp, idArr, storageSize - 1);
                SwapArr(nameArrTemp, nameArr, storageSize - 1);
                SwapArr(countArrTemp, countArr, storageSize - 1);
                SwapArr(priceArrTemp, priceArr, storageSize - 1);

                idArrTemp[storageSize - 1] = storageSize;
                nameArrTemp[storageSize - 1] = newName;
                countArrTemp[storageSize - 1] = count;
                priceArrTemp[storageSize - 1] = price;

                delete[] idArr;
                delete[] nameArr;
                delete[] countArr;
                delete[] priceArr;

                idArr = idArrTemp;
                nameArr = nameArrTemp;
                countArr = countArrTemp;
                priceArr = priceArrTemp;
                std::cout << "Товар успешно добавлен!\n\n";
                Sleep(1500);
                break;
            }
            else if (choose == "2")
            {
                std::cout << "Отмена\n\n";
                Sleep(1500);
                break;
            }
            else
            {
                Err();
            }
        }
        break;
    }
}

void ChangeStorage()
{
    std::string choose;
    while (true)
    {
        system("cls");
        std::cout << "1 - Добавить новый товар\n";
        std::cout << "2 - Изменить название товара\n";
        std::cout << "3 - Удалить товар\n";
        std::cout << "0 - Выйти из редактора\n";
        std::cout << "Выбор: ";
        Getline(choose);

        if (choose == "1")
        {
            AddNewItem();
        }
        else if (choose == "2" && storageSize > 0)
        {
            ChangeName();
        }
        else if (choose == "3" && storageSize > 0)
        {
            DeleteItem();
        }
        else if (choose == "0")
        {
            system("cls");
            break;
        }
        else
        {
            if (storageSize <= 0)
            {
                std::cout << "Склад пустой!\n";
            }
            Err();
        }
    }
}

void ChangeName()
{
    std::string newName, choose, chooseId;
    unsigned int id = 0;

    while (true)
    {
        system("cls");
        ShowStorage(3);
        std::cout << "\nВведите ID товара или \"exit\" для выхода: ";
        Getline(chooseId);
        if (chooseId == "exit")
        {
            std::cout << "Отмена операции изменения названия\n";
            Sleep(1500);
            break;
        }

        std::cout << "Введите новое название товара: ";
        Getline(newName);

        if (newName.empty() || newName.size() >= 60)
        {
            std::cout << "Ошибка названия. Максимальная длина 60 символов\n";
            Sleep(1500);
        }
        else if (IsNumber(chooseId))
        {
            id = std::stoi(chooseId) - 1;
            if (id < 0 || id >= storageSize)
            {
                std::cout << "Ошибка ID!\n";
                Sleep(1500);
            }
            else
            {
                std::cout << std::left << std::setw(25) << nameArr[id] << " --> " << newName << "\n\n";
                std::cout << "Подтвердить?\n1 - Да\n2 - Нет\nВыбор: ";
                Getline(choose);
                if (choose == "1")
                {
                    nameArr[id] = newName;
                    std::cout << "Название успешно изменено!\n";
                    Sleep(1500);
                    break;
                }
                else if (choose == "2")
                {
                    std::cout << "Отмена операции\n";
                    Sleep(1500);
                }
                else
                {
                    Err();
                }
            }
        }
        else
        {
            Error();
        }
    }
}

void DeleteItem()
{
    std::string chooseId, choose;
    unsigned int id = 0;

    while (true)
    {
        system("cls");
        ShowStorage(3);
        std::cout << "\nВведите ID товара для удаления или \"exit\" для выхода: ";
        Getline(chooseId);
        if (chooseId == "exit")
        {
            std::cout << "Отмена операции удаления\n";
            Sleep(1500);
            break;
        }

        if (IsNumber(chooseId))
        {
            id = std::stoi(chooseId) - 1;
            if (id < 0 || id >= storageSize)
            {
                std::cout << "Ошибка ID\n";
                Sleep(1500);
            }
            else
            {
                std::cout << "Удаляемый товар - " << std::left << std::setw(25) << nameArr[id] << "\n\n";
                std::cout << "Подтвердить?\n1 - Да\n2 - Нет\nВыбор: ";
                Getline(choose);
                if (choose == "1")
                {
                    storageSize--;
                    int* idArrTemp = new int[storageSize];
                    std::string* nameArrTemp = new std::string[storageSize];
                    unsigned int* countArrTemp = new unsigned int[storageSize];
                    double* priceArrTemp = new double[storageSize];

                    for (std::size_t i = 0, c = 0; i < storageSize; i++, c++)
                    {
                        if (id == c)
                        {
                            c++;
                        }
                        idArrTemp[i] = i + 1;
                        nameArrTemp[i] = nameArr[c];
                        countArrTemp[i] = countArr[c];
                        priceArrTemp[i] = priceArr[c];
                    }

                    delete[] idArr;
                    delete[] nameArr;
                    delete[] countArr;
                    delete[] priceArr;

                    idArr = idArrTemp;
                    nameArr = nameArrTemp;
                    countArr = countArrTemp;
                    priceArr = priceArrTemp;

                    std::cout << "Товар успешно удален!\n\n";
                    Sleep(1500);
                    break;
                }
                else if (choose == "2")
                {
                    std::cout << "Отмена операции удаления!\n";
                    Sleep(1500);
                }
                else
                {
                    Err();
                }
            }
        }
        else
        {
            Error();
        }
    }
}

bool CheckLogin(const std::string& str)
{
    for (size_t i = 0; i < userSize; i++)
    {
        if (str == loginArr[i])
        {
            std::cout << "Такой пользователь существует!\n";
            Sleep(1500);
            return false;
        }
    }

    if (str.size() < 5 || str.size() >= 20)
    {
        std::cout << "Недопустимая длина логина. От 5 до 20 символов\n";
        Sleep(1500);
        return false;
    }

    std::unordered_set<char> specialSymbols;
    for (char i = 'A'; i <= 'Z'; i++)
    {
        specialSymbols.insert(i);
    }
    for (char i = 'a'; i <= 'z'; i++)
    {
        specialSymbols.insert(i);
    }

    for (char symb : str)
    {
        if (!specialSymbols.count(symb))
        {
            std::cout << "Некорректные символы в логине\n\n";
            Sleep(1500);
            return false;
        }
    }
    return true;
}

bool CheckPass(const std::string& str)
{
    if (str.size() < 5 || str.size() >= 20)
    {
        std::cout << "Недопустимая длина пароля! От 5 до 20 символов\n";
        Sleep(1500);
        return false;
    }

    int symbolsCount = 0;
    std::unordered_set<char> specialSymbols;
    std::unordered_set<char> passSymbols{ '!', '@', '#', '%', '^', '&', '*', '(',')', '-', '_', '=', '+', '/', '?', '|', '\\', '\"', '\'', ',', '.', '>', '<', '~', '`', ':', ';', '{','}', '[', ']' };

    for (char i = '!'; i <= '~'; i++)
    {
        specialSymbols.insert(i);
    }

    for (char symb : str)
    {
        if (!specialSymbols.count(symb))
        {
            std::cout << "Некорректные символы в пароле\n\n";
            Sleep(1500);
            return false;
        }
    }

    for (char symb : str)
    {
        if (passSymbols.count(symb))
        {
            symbolsCount++;
            if (symbolsCount == 3)
            {
                return true;
            }
        }
    }
    std::cout << "Требуется минимум 3 специальных символа\n\n";
    Sleep(1500);
    return false;
}

void AddNewUser()
{
    std::string newLogin, newPass, newRole, choose;
    bool exit = true;

    while (true)
    {
        exit = true;

        // Ввод логина
        while (true)
        {
            system("cls");
            std::cout << "\tСоздание нового пользователя\n";
            std::cout << "Введите логин нового пользователя или \"exit\": ";
            Getline(newLogin);
            if (newLogin == "exit")
            {
                std::cout << "Операция добавления пользователя прервана!\n\n";
                Sleep(1500);
                exit = false;
                break;
            }

            if (CheckLogin(newLogin))
            {
                break;
            }
        }
        if (!exit) break;

        // Ввод пароля
        while (true)
        {
            system("cls");
            std::cout << "\tСоздание нового пользователя\n";
            std::cout << "Введите пароль нового пользователя или \"exit\": ";
            Getline(newPass);
            if (newPass == "exit")
            {
                std::cout << "Операция добавления пользователя прервана!\n\n";
                Sleep(1500);
                exit = false;
                break;
            }
            if (CheckPass(newPass))
            {
                break;
            }
        }
        if (!exit) break;

        // Выбор роли
        while (true)
        {
            system("cls");
            std::cout << "\tСоздание нового пользователя\n";
            std::cout << "Выберите роль нового пользователя или \"exit\":\n";
            std::cout << "1 - Администратор\n2 - Сотрудник\nВыбор: ";
            Getline(choose);
            if (choose == "exit")
            {
                std::cout << "Операция добавления пользователя прервана!\n\n";
                Sleep(1500);
                exit = false;
                break;
            }
            if (choose == "1")
            {
                newRole = userStatus[1];
                break;
            }
            else if (choose == "2")
            {
                newRole = userStatus[2];
                break;
            }
            else
            {
                Err();
            }
        }
        if (!exit) break;

        // Подтверждение
        while (true)
        {
            system("cls");
            std::cout << "Пользователь: " << newLogin << "\n";
            std::cout << "Пароль: " << newPass << "\n";
            std::cout << "Роль: " << newRole << "\n\n";
            std::cout << "Подтвердить?\n1 - Да\n2 - Нет\nВыбор: ";
            Getline(choose);
            if (choose == "1")
            {
                userSize++;
                std::string* loginArrTemp = new std::string[userSize];
                std::string* passArrTemp = new std::string[userSize];
                std::string* statusArrTemp = new std::string[userSize];
                double* awardArrTemp = new double[userSize];
                unsigned int* userIdArrTemp = new unsigned int[userSize];

                SwapArr(loginArrTemp, loginArr, userSize - 1);
                SwapArr(passArrTemp, passArr, userSize - 1);
                SwapArr(statusArrTemp, statusArr, userSize - 1);
                SwapArr(awardArrTemp, awardArr, userSize - 1);
                SwapArr(userIdArrTemp, userIdArr, userSize - 1);

                loginArrTemp[userSize - 1] = newLogin;
                passArrTemp[userSize - 1] = newPass;
                statusArrTemp[userSize - 1] = newRole;
                awardArrTemp[userSize - 1] = 0.0;
                userIdArrTemp[userSize - 1] = userSize;

                delete[] loginArr;
                delete[] passArr;
                delete[] statusArr;
                delete[] awardArr;
                delete[] userIdArr;

                loginArr = loginArrTemp;
                passArr = passArrTemp;
                statusArr = statusArrTemp;
                awardArr = awardArrTemp;
                userIdArr = userIdArrTemp;

                std::cout << "Пользователь успешно добавлен!\n\n";
                Sleep(1500);
                break;
            }
            else if (choose == "2")
            {
                std::cout << "Отмена\n";
                Sleep(1500);
                break;
            }
            else
            {
                Err();
            }
        }
        break;
    }
}

void ChangePass()
{
    std::string newPass1, newPass2, choose;
    int userNumber = 0;
    int isAdmin = 0;

    while (true)
    {
        if (currentStatus == userStatus[0])
        {
            ShowAccount(1);
            isAdmin = 0;
        }
        else
        {
            ShowAccount(0);
            isAdmin = 1;
        }

        std::cout << "\nВыберите номер пользователя для смены пароля или \"exit\"для выхода - ";
        Getline(choose);
        if (choose == "exit")
        {
            std::cout << "Отмена изменения пароля!\n";
            Sleep(1500);
            break;
        }
        else if (IsNumber(choose))
        {
            userNumber = std::stoi(choose);
            if (userNumber < isAdmin || userNumber >= (int)userSize)
            {
                std::cout << "Пользователь с таким номером не существует!\n";
                Sleep(1500);
            }
            else
            {
                while (true)
                {
                    system("cls");

                    if (currentStatus == userStatus[1] && statusArr[userNumber] == userStatus[1])
                    {
                        std::cout << "Нельзя менять пароли Администраторам!\n";
                        Sleep(1500);
                        break;
                    }

                    std::cout << "Введите новый пароль для пользователя " << loginArr[userNumber] << " - ";
                    Getline(newPass1);
                    std::cout << "Подтвердите пароль для пользователя " << loginArr[userNumber] << " - ";
                    Getline(newPass2);
                    if (CheckPass(newPass1) && CheckPass(newPass2) && newPass1 == newPass2)
                    {
                        passArr[userNumber] = newPass1;
                        std::cout << "Успешно\n";
                        Sleep(1500);
                        break;
                    }
                    else
                    {
                        std::cout << "Повторите попытку\n";
                        Sleep(1500);
                    }
                }
            }
        }
        else
        {
            Err();
        }
    }
}

void ShowAccount(int mode)
{
    if (mode == 0)
    {
        system("cls");
        std::cout << "ID\t" << std::left << std::setw(15) << "Логин" << std::setw(15) << "Пароль" << std::setw(25) << "Роль" << std::endl;
        std::cout << std::string(60, '-') << std::endl;

        for (std::size_t i = 1; i < userSize; i++)
        {
            std::cout << i << "\t" << std::left << std::setw(15) << loginArr[i] << std::setw(15) << passArr[i] << std::setw(25) << statusArr[i] << std::endl;
        }
        system("pause");
    }
    else if (mode == 1)
    {
        system("cls");
        std::cout << "ID\t" << std::left << std::setw(15) << "Логин" << std::setw(15) << "Пароль" << std::setw(25) << "Роль" << std::endl;
        std::cout << std::string(60, '-') << std::endl;

        for (std::size_t i = 0; i < userSize; i++)
        {
            std::cout << i << "\t" << std::left << std::setw(15) << loginArr[i] << std::setw(15) << passArr[i] << std::setw(25) << statusArr[i] << std::endl;
        }
        system("pause");
    }
}

void ChangeUsers()
{
    std::string choose;
    while (true)
    {
        system("cls");
        std::cout << "1 - Добавить нового пользователя\n";
        std::cout << "2 - Показать пользователей\n";
        std::cout << "3 - Изменить пароль пользователя\n";
        std::cout << "4 - Удалить пользователя\n";
        std::cout << "0 - Выйти из редактора\n";
        std::cout << "Выбор: ";
        Getline(choose);

        if (choose == "1")
        {
            AddNewUser();
        }
        else if (choose == "2" && userSize > 1)
        {
            ShowAccount(0);
        }
        else if (choose == "3" && userSize > 1)
        {
            ChangePass();
        }
        else if (choose == "4" && userSize > 1)
        {
            DeleteUser();
        }
        else if (choose == "0")
        {
            system("cls");
            break;
        }
        else
        {
            if (userSize <= 1)
            {
                std::cout << "Пользователей нет\n";
            }
            Err();
        }
    }
}

void DeleteUser()
{
    std::string chooseNumber, checkPass, choose;
    int userNumber = 0, isAdmin = 0;

    while (true)
    {
        if (currentStatus == userStatus[0])
        {
            if (userSize < 2)
            {
                std::cout << "Нет доступных пользователей для удаления!\n";
                Sleep(1500);
                return;
            }
        }
        else if (currentStatus == userStatus[1])
        {
            if (staffCount < 1)
            {
                std::cout << "Нет доступных пользователей для удаления!\n";
                Sleep(1500);
                return;
            }
        }

        ShowAccount(0);
        isAdmin = 1;

        std::cout << "\nВыберите номер пользователя для удаления или \"exit\"для выхода - ";
        Getline(choose);
        if (choose == "exit")
        {
            std::cout << "Отмена удаления пользователя!\n";
            Sleep(1500);
            break;
        }
        else if (IsNumber(choose))
        {
            userNumber = std::stoi(choose);

            if (userNumber < isAdmin || userNumber >= (int)userSize)
            {
                std::cout << "Пользователя с таким номером не существует!\n";
                Sleep(1500);
                break;
            }

            while (true)
            {
                system("cls");

                if (currentStatus == userStatus[1] && statusArr[userNumber] != userStatus[2])
                {
                    std::cout << "Нельзя удалять администратора\n";
                    Sleep(1500);
                    break;
                }

                std::cout << "Удаление пользователя - " << loginArr[userNumber] << "\n";
                std::cout << "Для подтверждения введите пароль супер администратора или \"exit\" для выхода - ";
                Getline(checkPass);

                if (checkPass == "exit")
                {
                    std::cout << "Отмена удаления пользователя - " << loginArr[userNumber] << "\n\n";
                    Sleep(1500);
                    break;
                }
                else if (checkPass == passArr[0])
                {
                    if (statusArr[userNumber] == userStatus[2])
                    {
                        staffCount--;
                    }

                    userSize--;
                    std::string* loginArrTemp = new std::string[userSize];
                    std::string* passArrTemp = new std::string[userSize];
                    std::string* statusArrTemp = new std::string[userSize];
                    double* awardArrTemp = new double[userSize];
                    unsigned int* userIdArrTemp = new unsigned int[userSize];

                    for (size_t i = 0, c = 0; i < userSize; i++, c++)
                    {
                        if (userNumber == c)
                        {
                            c++;
                        }
                        loginArrTemp[i] = loginArr[c];
                        passArrTemp[i] = passArr[c];
                        statusArrTemp[i] = statusArr[c];
                        awardArrTemp[i] = awardArr[c];
                        userIdArrTemp[i] = userIdArr[c];
                    }

                    delete[] loginArr;
                    delete[] passArr;
                    delete[] statusArr;
                    delete[] awardArr;
                    delete[] userIdArr;

                    loginArr = loginArrTemp;
                    passArr = passArrTemp;
                    statusArr = statusArrTemp;
                    awardArr = awardArrTemp;
                    userIdArr = userIdArrTemp;

                    std::cout << "Идет подготовка... ";
                    Sleep(2000);
                    std::cout << "Пользователь успешно удалён!\n\n";
                    Sleep(1500);
                    break;
                }
                else
                {
                    std::cout << "Некорректный пароль\n";
                    Sleep(1500);
                }
            }
        }
        else
        {
            Err();
        }
    }
}

void AddNewstorage()
{
    std::string choose;
    std::cout << "Создание склада с нуля\n";
    Sleep(2000);
    while (true)
    {

        std::cout << "Вы хотите продолжить заполнение склада? 1 - Да, 2 - Нет\nВвод: ";
        Getline(choose);
        if (choose == "1")
        {
            AddNewItem();
        }
        else if (choose == "2")
        {
            ShowSuperAdminMenu();
            break;
        }
        else
        {
            Err();
        }
    }
}

// -------------------------------------------------------------------------------------- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ  ---------------------------------------------------------------------------------------------------------------

template<typename T>
void SwapArr(T*& Arr, T* Arr2, std::size_t SizeArr)
{
    for (std::size_t i = 0; i < SizeArr; i++)
    {
        Arr[i] = Arr2[i];
    }
}

inline void Getline(std::string& str)
{
    std::getline(std::cin, str, '\n');

    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(365000, '\n');
    }
}

inline void Err(int time)
{
    std::cout << "Некорректный ввод!\n";
    Sleep(time);
}

void Error()
{
    std::cout << "Некорректный ввод!\n";
    std::cout << "Введенные данные не являются числом\n\n";
    Sleep(1500);
}