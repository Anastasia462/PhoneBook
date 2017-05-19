#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

typedef struct record
{
  char* Surname;
  char* Name;
  char* PhoneNumber;
  struct record* next;
}record;

typedef struct page
{
  record* records;
  struct page* next;
  char letter;
}page;

int number = 0;
/// Настя письк
void Hello();
record* EnterRecord();///Записываем Имя фамилию и телефон, в структуру record
record* AddRecord(record* HeadRecords,  record* NewRecord);///Добавить новый узел в список, возвращает начало списка
record* DeleteRecord(record* HeadRecord, int number, int* over);///удалить одну запись
void ShowAllRecords(record* HeadRecords);///показать все записи
int ChangeRecord(record* HeadRecord,char *StrSurname);///Изменить запись
void SearchOnSurname(page* RootBook, char* StrSurname);
int CheckStr(char* Str);///проверка имени и фамилии
int CheckPhoneNumber(char* number);///проверка номера
page* AddPage(page* RootBook, char letter, page** nPage);/// Добавления страницы
page* ClearBook(page* RootBook);/// Очистки книги(удалить все страницы и все записи)
page* SearchPage(page* RootPage, char letter);///Поиск страницы
page* DeletePage(page* RootBook);///удалить страницу без записей
void RecordFile(FILE* myfile, page* RootBook);///Вывод записей а файл


int main()
{
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);
  int choice;
  int Exit = 1;

  ///good job!
  if(1)
    printf("\nHello");

  page* RootBook = NULL;
  FILE* myfile;
  record* NewRec;
  page* NewPg;

  myfile = fopen("PhoneBook.txt", "r");
  if(myfile)
  {
     while(1)
    {
      NewRec = malloc(sizeof(record));

      NewRec->Surname = (char*)malloc(sizeof(char)*100);
      NewRec->Name = (char*)malloc(sizeof(char)*100);
      NewRec->PhoneNumber = (char*)malloc(sizeof(char)*100);
      NewRec->next = NULL;

      fscanf(myfile, "%s %s %s", NewRec->Surname, NewRec->Name, NewRec->PhoneNumber);

      if(feof(myfile) != 0)
      {
        free(NewRec->Surname);
        free(NewRec->Name);
        free(NewRec->PhoneNumber);
        free(NewRec);
        break;
      }

      NewRec->Surname = (char*)realloc(NewRec->Surname, sizeof(char)*(strlen(NewRec->Surname)+1));
      NewRec->Surname[strlen(NewRec->Surname)] = '\0';
      NewRec->Name = (char*)realloc(NewRec->Name, sizeof(char)*(strlen(NewRec->Name)+1));
      NewRec->Name[strlen(NewRec->Name)] = '\0';
      NewRec->PhoneNumber = (char*)realloc(NewRec->PhoneNumber, sizeof(char)*(strlen(NewRec->PhoneNumber)+1));
      NewRec->PhoneNumber[strlen(NewRec->PhoneNumber)] = '\0';

      NewPg = SearchPage(RootBook, NewRec->Surname[0]);

      if(NewPg == NULL)/// если не нашли
        RootBook = AddPage(RootBook, NewRec->Surname[0], &NewPg);///создаём страницу

      NewPg->records = AddRecord(NewPg->records, NewRec);

      }
  }
  fclose(myfile);

  while(Exit == 1)
    {
      Hello();
      printf("\n Введите номер операции: ");
      do{
          scanf("%d", &choice);
          if(getchar() == '\n')
            break;

          while(getchar() != '\n');
        }while(1);

  switch(choice)
  {
    case 1:
    {
      record* NewRecord = EnterRecord();

      page* NewPage = SearchPage(RootBook, NewRecord->Surname[0]);/// Поиск

      if(NewPage == NULL)/// если не нашли
      {
        RootBook = AddPage(RootBook, NewRecord->Surname[0], &NewPage);///создаём страницу
      }
      NewPage->records = AddRecord(NewPage->records, NewRecord);///если нашли страницу или создали, то вставляем запись

      break;
    }

    case 2:
    {
      record *NewRecord;
      page* ptr;
      char letter;

      printf("\nВведите первую букву фамилии, котрую хотите изменить: ");
      do{
          scanf("%c", &letter);
          if(getchar() == '\n')
            break;

          while(getchar() != '\n');
        }while(1);

      ptr = SearchPage(RootBook, letter);

      if(ptr == NULL)///если
        printf("\nТакой страницы нет!");

      else
      {
        int number, over = 0;
        ShowAllRecords(ptr->records);
        printf("\nВведите номер записи, которую хотите изменить: ");
        scanf("%d", &number);
        ptr->records = DeleteRecord(ptr->records, number, &over);

        if(ptr->records == NULL)
          RootBook = DeletePage(RootBook);

        if(over)
          break;

        NewRecord = EnterRecord();
        ptr = SearchPage(RootBook, NewRecord->Surname[0]);/// Поиск
          if(ptr == NULL)/// если не нашли
        {
          RootBook = AddPage(RootBook, NewRecord->Surname[0], &ptr);///создаём страницу
        }
        ptr->records = AddRecord(ptr->records, NewRecord);///если нашли страницу или создали, то вставляем запись
      }

      break;
    }

    case 3:
    {
      page* ptr;
      char letter;

      printf("\nВведите первую букву фамилии, котрую хотите удалить: ");
      do{
          scanf("%c", &letter);
          if(getchar() == '\n')
            break;

          while(getchar() != '\n');
        }while(1);

      ptr = SearchPage(RootBook, letter);

      if(ptr == NULL)///если
        printf("\nТакой страницы нет!");
      else
      {
        int number, state;
        ShowAllRecords(ptr->records);
        do{
          printf("\nВведите номер записи, которую хотите удалить: ");
          scanf("%d", &number);
        }while(number < 1);

        ptr->records = DeleteRecord(ptr->records, number, &state);

        if(ptr->records == NULL)
          RootBook = DeletePage(RootBook);


      }
      break;
    }

    case 4:
    {

        if(RootBook == NULL)
        {
          printf("\nНет записей в справочнике!");
        }
        else
        {
          RootBook = ClearBook(RootBook);
          printf("\nЗаписи удалены!");
        }

      break;
    }

    case 5:
    {
      page* ptr = RootBook;

      if(ptr == NULL)
        printf("\nНет записей в справочнике!");

      while(ptr)
      {
        printf("\n\n%c", ptr->letter);
        ShowAllRecords(ptr->records);
        ptr = ptr->next;
      }

      break;
    }

    case 6:
    {
      page* temp = RootBook;
      char* StrSurname = (char*)malloc(sizeof(char)*50);
      printf("\nВведите фамилию человека, которого хотите найти: ");
      scanf("%s", StrSurname);
      StrSurname = (char*)realloc(StrSurname, sizeof(char)*(strlen(StrSurname)+1));

      SearchOnSurname(temp, StrSurname);

      break;
    }

    case 7:
    {
      char letter;
      page* ptr;
      printf("\nВведите букву: ");
      scanf("%c", &letter);

      ptr = SearchPage(RootBook, letter);

      if(ptr == NULL)
        printf("\nТакой страницы нет!");

      else
        ShowAllRecords(ptr->records);

      break;
    }

    case 8:
      {
        page* temp = RootBook;

      myfile = fopen("PhoneBook.txt", "w");
      RecordFile(myfile, temp);
      fclose(myfile);

      RootBook = ClearBook(RootBook);
      Exit = 0;
      break;
      }

    default:
      printf("\nВведите управляющие цифры!");
      break;
  }
      getch();
      system("cls");

    }

    return 0;
}

record* EnterRecord()
{
  int check;

  record* NewRecord = malloc(sizeof(record));///выделям память для новой записи

  NewRecord->Surname = (char*)malloc(sizeof(char)*100);
  NewRecord->Name = (char*)malloc(sizeof(char)*100);
  NewRecord->PhoneNumber = (char*)malloc(sizeof(char)*20);
  NewRecord->next = NULL;///последний узел указывает на NULL

  do{
      printf("\nВведите Фамилию Имя Телефон через пробел: ");
      scanf("%s %s %s", NewRecord->Surname, NewRecord->Name, NewRecord->PhoneNumber);

      check =(CheckStr(NewRecord->Surname) &&
              (CheckStr(NewRecord->Name)) &&
              (CheckPhoneNumber(NewRecord->PhoneNumber))? 1: 0);

  }while(check == 0);

  NewRecord->Surname = (char*)realloc(NewRecord->Surname, sizeof(char)*(strlen(NewRecord->Surname)+1));
  NewRecord->Surname[strlen(NewRecord->Surname)] = '\0';
  NewRecord->Name = (char*)realloc(NewRecord->Name, sizeof(char)*(strlen(NewRecord->Name)+1));
  NewRecord->Name[strlen(NewRecord->Name)] = '\0';
  NewRecord->PhoneNumber = (char*)realloc(NewRecord->PhoneNumber, sizeof(char)*(strlen(NewRecord->PhoneNumber)+1));
  NewRecord->PhoneNumber[strlen(NewRecord->PhoneNumber)] = '\0';

 return NewRecord;
}

record* AddRecord(record* HeadRecords, record* NewRecord)
{
  if (!HeadRecords)
  {
    HeadRecords = NewRecord;
    return HeadRecords;
  }

  record* temp = HeadRecords;

  while(temp->next)
  {
    temp = temp->next;
  }

  temp->next = NewRecord;

  return HeadRecords;
}

int ChangeRecord(record* HeadRecord,char *StrSurname)
{

  while(HeadRecord)
  {
    if(strcmp(HeadRecord->Surname, StrSurname) == 0)
    {
      return 1;
    }
    HeadRecord = HeadRecord->next;
  }
    return 0;

}

record* DeleteRecord(record* HeadRecord, int number, int* over)
{
  record* temp = HeadRecord;
  record* buf;


  if(number == 1)
  {
    HeadRecord = temp->next;

    free(temp->Surname);
    free(temp->Name);
    free(temp->PhoneNumber);

    return HeadRecord;
  }

  while ((number > 1) && temp)///пока не дойдём до номера записи, которую нужно удалить
  {
    number--;
    buf = temp;
    temp = temp->next;
  }

	///	Если не дошли до записи, или дошли до конца списка, то такой записи нету.
  if(number > 1 || !temp)
  {
    printf("\nТакой записи нет!");
    *over = 1;
    return HeadRecord;
  }

  free(temp->Surname);
  free(temp->Name);
  free(temp->PhoneNumber);

  buf->next = temp->next;
  free(temp);

  return HeadRecord;
}

void SearchOnSurname(page* RootBook, char* StrSurname)
{
  page* tempPage = RootBook;
  record* tempRecord;

  while(tempPage)
  {
    if (tempPage->letter == *StrSurname)
    {
      tempRecord = tempPage->records;
      while(tempRecord)
      {
        if(strcmp(tempRecord->Surname, StrSurname) == 0)
          printf("\n%s %s %s", tempRecord->Surname, tempRecord->Name, tempRecord->PhoneNumber);

        tempRecord = tempRecord->next;
      }
      break;
    }
    tempPage = tempPage->next;
  }
}

page* ClearBook(page* RootBook)
{

  if(RootBook == NULL)
  {
    return NULL;
  }

  page* buf;

  while(RootBook)
  {
    buf = RootBook;

    while(RootBook->records)
    {
      if(RootBook->records->Name && RootBook->records->Surname && RootBook->records->PhoneNumber)
      {
        free(RootBook->records->Name);
        free(RootBook->records->Surname);
        free(RootBook->records->PhoneNumber);
      }
      RootBook->records = RootBook->records->next;
    }

    RootBook = RootBook->next;
    free(buf);
  }

  return RootBook;
}

void ShowAllRecords(record* HeadRecords)
{
  int i = 0;

  if(HeadRecords == NULL)
    return;

  while(HeadRecords)
  {
    printf("\n%d %s %s %s", ++i, HeadRecords->Surname,HeadRecords->Name, HeadRecords->PhoneNumber);
    HeadRecords = HeadRecords->next;
  }
}

int CheckStr(char* Str)
{
  if(Str[0] < 'А' || Str[0] > 'Я')
  {
    return 0;
  }

  Str++;

  while(*Str)
  {
    if(*Str < 'а' || *Str > 'я')
      return 0;

    Str++;
  }

  return 1;
}

int CheckPhoneNumber(char* number)
{
  while(*number)
  {
    if(*number < '0' || *number > '9')
      return 0;

    number++;
  }

  return 1;
}

page* AddPage(page* RootBook, char letter, page** nPage)
{
  page* NewNode = (page*)malloc(sizeof(page));
  *nPage = NewNode;
  NewNode->letter = letter;
  NewNode->next = NULL;
  NewNode->records = NULL;

  if(!RootBook)
  {
    return NewNode;
  }
  else
  {
    if(letter < RootBook->letter)
    {
      NewNode->next = RootBook;
      return NewNode;
    }

    page* temp = RootBook;
    while(temp->next && temp->next->letter < letter)
    {
      temp = temp->next;
    }

    NewNode->next = temp->next;
    temp->next = NewNode;
    return RootBook;
  }
}

page* SearchPage(page* RootPage, char letter)
{
  if(!RootPage)///если нет страниц
    return RootPage;

  while(RootPage)
  {
    if(RootPage->letter == letter)///если нашли правильную страницу
      break;

    RootPage = RootPage->next;
  }

  return RootPage;
}

page* DeletePage(page* RootBook)
{
  page* temp = RootBook;
  page* buf;

  if(RootBook->records == NULL)
  {
    RootBook = RootBook->next;
    free(temp);
    return RootBook;
  }

  while(temp->next && temp->next->records)
  {
    temp = temp->next;
  }

  buf = temp->next;

  temp->next = buf->next;
  free(buf);

  return RootBook;
}

void RecordFile(FILE* myfile, page* RootBook)
{
  page* ptrPage = RootBook;
  record* ptrRecord;

  while(ptrPage)
  {
    ptrRecord = ptrPage->records;
    while(ptrRecord)
    {
      fprintf(myfile, "%s %s %s\n", ptrRecord->Surname, ptrRecord->Name, ptrRecord->PhoneNumber);
      ptrRecord= ptrRecord->next;
    }
    ptrPage = ptrPage->next;
  }
}
