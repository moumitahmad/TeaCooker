#include "dataService.h"

Tea* getTea(int tea) {
  switch(tea) {
    case 1:
      return new Tea("Grüner Tee", 3.5, 80); 
    case 2:
      return new Tea("Pfefferminze", 6, 100); 
    case 3:
     return new Tea("Erdbeer-Minze", 6.5, 100); 
  }
}
