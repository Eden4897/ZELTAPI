#ifndef _ZELTAPI_LCD_H_
#define _ZELTAPI_LCD_H_

#include "llemu.h"
#include <iostream>


static const char * convert(std::string fmt)
{
	const char* returnStr = fmt.c_str();
  return returnStr;
}

template<typename Arg>
static const char * convert(std::string fmt, Arg arg)
{
	int location = fmt.find("%d");

	fmt.replace(location, 2, std::to_string(arg));

	return convert(fmt);
}

template<typename Arg,typename... Args>
static const char * convert(std::string fmt,Arg arg,Args ... args)
{

	int location = fmt.find("%d");

	fmt.replace(location, 2, std::to_string(arg));

	return convert(fmt,args...);
}



static void lcd_setText(std::int16_t line,std::string fmt){
  const char* string = fmt.c_str();
  llemu::editLabel(lines[line], string);
}

template<typename ... Args>
static void lcd_print(std::int16_t line,std::string fmt,Args...args){
	lcd_setText(line,convert(fmt,args...));
}

static void lcd_clearLine(std::int16_t line);

static void lcd_clear();


#endif
