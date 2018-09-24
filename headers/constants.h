#ifndef BASICINTERPRETATOR_CONSTANTS_H
#define BASICINTERPRETATOR_CONSTANTS_H

//Типы лексем
#define DELIMITER  1 //Разделитель
#define VARIABLE   2 //Переменная
#define NUMBER     3 //Число
#define COMMAND    4 //Команда
#define QUOTE      5 //Кавычки

//Внутренние представления лексем
#define PRINT 10
#define INPUT 11
#define IF 12
#define THEN 13
#define ELSE 14
#define GOTO 15
#define GOSUB 16
#define RETURN 17
#define EOL 18 //Конец строки файла
#define END 19 //Конец файла
#define FINISHED 20 //Конец программы
#define ENDFI 21

#endif
