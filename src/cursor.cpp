#include "cursor.h"

cursor::~cursor(){
    delete curs_vec[0].line;
    delete curs_vec[1].line;
}
