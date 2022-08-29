#if !defined(__PANTALLA__)
#define __PANTALLA__

#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void inicializarPantalla(void) {
    lcd.begin(16, 4);
}

#endif // __PANTALLA__
