#include "WS_Flow.h"
#include <string.h>

Adafruit_NeoMatrix Matrix = Adafruit_NeoMatrix(8, 8, RGB_Control_PIN, 
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +                     
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,                
  NEO_GRB            + NEO_KHZ800);                           

int MatrixWidth = 0;

const uint16_t colors[] = {
  Matrix.Color(255, 0, 0), 
  Matrix.Color(0, 255, 0), 
  Matrix.Color(0, 0, 255) 
};

int getCharWidth(char c) {
  if (c == 'i' || c == 'l' || c == '!' || c == '.') {
    return 3;
  } else {
    return 5;
  }
}

int getStringWidth(const char* str) {
  int width = 0;
  int length = strlen(str);
  for (int i = 0; i < length; i++) {
    width += getCharWidth(str[i]);
    width += 1;      
  }
  return width;
}

void Matrix_Init() {
  Matrix.begin();
  Matrix.setTextWrap(false);  
  Matrix.setBrightness(40);           
  Matrix.setTextColor(colors[0]); 
  MatrixWidth = Matrix.width(); 
}

void Text_Flow(char* Text, char* direction) {
  int textWidth = getStringWidth(Text);
  Matrix.fillScreen(0);
  Matrix.setCursor(MatrixWidth, 0);
  Matrix.print(F(Text));
  Matrix.show();

  if (strcmp(direction, "left") == 0) {
    MatrixWidth--;                  
    if (MatrixWidth < -textWidth) {
      MatrixWidth = Matrix.width();
    }
  } else if (strcmp(direction, "right") == 0) {
    MatrixWidth++;                  
    if (MatrixWidth > Matrix.width()) {
      MatrixWidth = -textWidth;
    }
  }
}
