#include "dataService.h"

Tea* getTea(int tea) {
  switch(tea) {
    case 1:
      return new Tea("Grüner Tee", 1, 80); 
    case 2:
      return new Tea("Pfefferminze", 0.3, 100); 
    case 3:
     return new Tea("Erdbeer-Minze", 0.5, 21);//6.5; 100 
  }
}
