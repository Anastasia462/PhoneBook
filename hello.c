#include <stdio.h>
#include <windows.h>

void Hello()
{
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);

  printf("\nДобро пожаловать в телефонный справочник!");
  printf("\nВы можете: ");

  printf("\n\n1. Ввести запись");
  printf("\n2. Изменить запись");
  printf("\n3. Удалить запись");
  printf("\n4. Удалить все записи");
  printf("\n5. Показать все записи");
  printf("\n6. Поиск по фамилии");
  printf("\n7. Посмотреть страницу по букве");
  printf("\n8. Выход");

}
