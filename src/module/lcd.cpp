#include "_header.h"



void lcd_clearLine(std::int16_t line){
  llemu::editLabel(lines[line - 1], "");
}

void lcd_clear(){
  for(int i = 0;i<8;i++){
    llemu::editLabel(lines[i], "");
  }
}
