
#include <stdarg.h>
#include <src/macros.h>
#include <src/terminal.h>
#include <limits.h>
#include <stdbool.h>

#define EOF (-1)

int putchar(int ic)
{
	char c = (char) ic;
	terminal_write(&c, sizeof(c));
	return ic;
}

//========================================================

bool print(const char* data, size_t length)
{
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

//========================================================

char* itoa(int value, char* buffer)
{
	char* ptr = buffer;
	if (value < 0)
	{
		*ptr++ = '-';
		value = -value;
	}
	char* start = ptr;
	do
	{
		*ptr++ = '0' + (value % 10);
		value /= 10;
	} while (value);
	*ptr = '\0';

	for (char* end = ptr - 1; start < end; start++, end--)
	{
		char tmp = *start;
		*start = *end;
		*end = tmp;
	}
	return buffer;
}

//========================================================

char* utoa(unsigned int value, char* buffer)
{
	char* ptr = buffer;
	char* start = ptr;
	do
	{
		*ptr++ = '0' + (value % 10);
		value /= 10;
	} while (value);
	*ptr = '\0';

	for (char* end = ptr - 1; start < end; start++, end--)
	{
		char tmp = *start;
		*start = *end;
		*end = tmp;
	}
	return buffer;
}

//========================================================

int printf(const char* restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0')
	{
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%')
		{
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount)
			{
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c')
		{
			format++;
			char c = (char) va_arg(parameters, int);
			if (!maxrem) {
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		}
		else if (*format == 's')
		{
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len)
			{
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		}
		else if (*format == 'd')
		{
			format++;
			int value = va_arg(parameters, int);
			char buffer[32];
			itoa(value, buffer);
			size_t len = strlen(buffer);
			if (maxrem < len)
				return -1;
			if (!print(buffer, len))
				return -1;
			written += len;
		}
		else if (*format == 'u')
		{
			format++;
			unsigned int value = va_arg(parameters, unsigned int);
			char buffer[32];
			utoa(value, buffer);
			size_t len = strlen(buffer);
			if (maxrem < len)
				return -1;
			if (!print(buffer, len))
				return -1;
			written += len;
		}
		else
		{
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len)
			{
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}

