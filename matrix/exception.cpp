#include "exception.h"
Exception::Exception(int x)
{
	err = x;
}

int Exception::get_err_no()
{
	return err;
}

void Exception::print_err()
{
	switch (err)
	{
		case ERR_MEM:
			cout << "Ошибка памяти" << endl;
			break;
		case ERR_FORMAT:
			cout << "Неверный формат числа" << endl;
			break;
		case ERR_NUL:
			cout << "Деление на ноль" << endl;
			break;
		case ERR_SIZE:
			cout << "Слишком большой размер" << endl;
			break;
		case ERR_OUT_OF_RANGE:
			cout << "Индекс вне доступа" << endl;
			break;
		case ERR_FILE_OPEN:
			cout << "Не удалось открыть файл" << endl;
			break;
		case ERR_FILE_FORMAT:
			cout << "Неправильный формат файла" << endl;
			break;
		case ERR_NOLIST:
			cout << "Пустой список" << endl;
			break;
		case ERR_WRONG_SIZE:
			cout << "Неподходящий размер" << endl;
			break;
		case ERR_NO_SIGN:
			cout << "Нет знака" << endl;
			break;
	}
}

