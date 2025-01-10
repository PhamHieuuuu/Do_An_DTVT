//     Sơ đồ kết nối 
//  LCD SDA-A4  |  SCL-A5
//  10 Servo
//  11 Led đỏ
//  12 led xanh
//  13 còi
#include "Keypad.h"
#include "LiquidCrystal_I2C.h"
#include "Servo.h"
Servo dc;

#define button1 A0
#define button2 A1
#define button3 A2
#define button4 A3
#define button5 A6
#define buttonfan A7

int buttons[] = {button1, button2, button3, button4, button5, buttonfan};  // Mảng chứa các chân của nút
bool daNhan[] = {false, false, false, false, false, false};  // Mảng trạng thái cho từng nút  


LiquidCrystal_I2C lcd(0x27, 16, 2); 
byte colpin[4] = {2, 3, 4, 5}; // Chân kết nối cột của keypad
byte rowpin[4] = {6, 7, 8, 9}; // Chân kết nối hàng của keypad
char keys[4][4] = {         // Mảng ký tự của keypad
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
char matkhau[] = {'4', '4', '4', '4'}; // Mật khẩu gốc
char nhap[4]; // Mảng lưu trữ các ký tự nhập vào
int viTri = 0; // Biến đếm vị trí nhập ký tự
Keypad kytu = Keypad(makeKeymap(keys), rowpin, colpin, 4, 4); // Khởi tạo keypad
bool nhapBatDau = false; // Biến kiểm soát việc bắt đầu nhập mật khẩu

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  pinMode(11, OUTPUT); 
  pinMode(12, OUTPUT); 
  pinMode(13,OUTPUT);

  dc.attach(10);
  dc.write(0);
  
  for (int i = 0; i < 4; i++) {
    pinMode(buttons[i], INPUT);
  }
}

void coi(int h){
 for(int i=0;i<h;i++){
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);}
}


void coifail(){
  digitalWrite(13,HIGH);
  delay(500);
  digitalWrite(13,LOW);
}

void key4x4() {
  char giatri = kytu.getKey(); // Đọc giá trị từ keypad
  lcd.setCursor(2, 0);
  lcd.print("Nhap Mat Khau");

    if (!nhapBatDau) { // đảo !nhapbatdau nếu là true sẽ thự hiện lệnh trong if
    lcd.setCursor(0, 1);
    lcd.print("Doi Mat Khau (D)");}

  if (giatri) { // Nếu nhấn phím
   coi(1);
   nhapBatDau = true; // để khi đảo sẽ không thực hiện lệnh  if (!nhapBatDau), để chỉ hiển thị dấu * dòng 2
 lcd.setCursor(0, 1);
  lcd.print("                "); 

  lcd.setCursor(0+7, 1); // Đặt lại con trỏ về đầu dòng thứ 2
  for (int i = 0; i < viTri; i++) { // In lại các ký tự '*' đã nhập
    lcd.print('*');
  }                    
  lcd.setCursor(viTri+7, 1); // In dấu * cho ký tự mới nhập
  lcd.print('*');
  
  nhap[viTri] = giatri; // Lưu giá trị của phím vừa nhập
  viTri++; // Tăng vị trí

    if (viTri == 4) { // Nếu nhập đủ 4 ký tự
     delay(300);
      if (nhap[0] == matkhau[0] && nhap[1] == matkhau[1] && nhap[2] == matkhau[2] && nhap[3] == matkhau[3]) { // Kiểm tra mật khẩu
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Mat Khau Dung");
        dc.write(90);
       coi(5);
        digitalWrite(12, HIGH); // Bật đèn xanh
        delay(3000);
        digitalWrite(12, LOW); // Tắt đèn xanh
        dc.write(0);
        lcd.clear();
        viTri = 0;
         nhapBatDau = false; // khi là false sẽ đảo ở dòng if (!nhapBatDau) thành true và thực hiện lệnh if
      } else {
        delay(300);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Sai Mat Khau");
         coifail();
        digitalWrite(11, HIGH); // Bật đèn đỏ
        delay(2000);
        digitalWrite(11, LOW); // Tắt đèn đỏ
        viTri = 0;
        nhapBatDau = false;
      }
    }

    if (giatri == 'D' && viTri == 1) { // Nếu nhấn phím 'D' để đổi mật khẩu
    nhapBatDau = false;// tạo dòng hiển thị dòng 2
      viTri = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nhap Mat Khau Cu");
      while (viTri < 4) {
        char mk = kytu.getKey();
        if (mk) {
           coi(1);
          lcd.setCursor(viTri + 7, 1);
          lcd.print('*'); // Hiển thị ký tự *
          nhap[viTri] = mk;
          viTri++;
        }
      }
      if (nhap[0] == matkhau[0] && nhap[1] == matkhau[1] && nhap[2] == matkhau[2] && nhap[3] == matkhau[3]) {
        delay(300);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Mat Khau Dung");
         coi(5);
        digitalWrite(12, HIGH); // Bật đèn xanh
        delay(2000);
        digitalWrite(12, LOW); // Tắt đèn xanh
        lcd.clear();
        viTri = 0;
        lcd.setCursor(2, 0);
        lcd.print("Nhap Key Moi");
        for (int i = 0; i < 4; i++) {
          while (true) {
            char chon = kytu.getKey();
            if (chon) {
               coi(1);
              lcd.setCursor(i + 7, 1);
              lcd.print('*'); 
              matkhau[i] = chon; // Cập nhật mật khẩu mới
              break;
            }
          }
        }
        delay(300);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Doi Mat Khau");
        lcd.setCursor(3,1);
        lcd.print("Thanh Cong");
        digitalWrite(12,HIGH);
         coi(5);
        delay(2000);
        lcd.clear();
        digitalWrite(12,LOW);
        viTri = 0;
      } else {
        delay(300);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Sai Mat Khau");
        lcd.setCursor(2, 1);
        lcd.print("Huy Doi Key");
        digitalWrite(11,HIGH);
        coifail();
        delay(3000);
        digitalWrite(11,LOW);
        lcd.clear();
        viTri = 0;
      }
    }
  }
}


void checkButton(int index) {
  if (index < 4) {
    // Đọc tín hiệu số từ A0 đến A3
    if (digitalRead(buttons[index]) == HIGH && !daNhan[index]) {  
      Serial.println(index + 1);  // In ra số nút (1-4)
      daNhan[index] = true;  
    } else if (digitalRead(buttons[index]) == LOW) {
      daNhan[index] = false;  
    }
  } else {
    // Đọc tín hiệu analog từ A6 và A7
    int analogValue = analogRead(buttons[index]);
    if (analogValue > 512 && !daNhan[index]) {  // Giá trị trung gian có thể điều chỉnh nếu cần
      Serial.println(index + 1);  // In ra số nút (5-6)
      daNhan[index] = true;
    } else if (analogValue < 512) {
      daNhan[index] = false;
    }
  }
}

void opendoor(){
   if (Serial.available()) {
    String data = Serial.readStringUntil('\n');  // Đọc chuỗi cho đến khi gặp '\n'
    int value = data.toInt();  // Chuyển đổi chuỗi thành số nguyên

    if (value == 2004) {
      Serial.println("Đã nhận tín hiệu mở cửa = 2004");  // Xác nhận tín hiệu nhận được
      dc.write(90);
      delay(3000);
      dc.write(0);
    }
  }
}


void loop() {
  opendoor();
  key4x4();
  for (int i = 0; i < 6; i++) {
    checkButton(i);
  }
}

