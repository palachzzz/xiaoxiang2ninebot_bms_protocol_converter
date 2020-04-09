/// Written by palachzzz 01.03.2020
#include <Snooze.h>

SnoozeDigital digital;
SnoozeTimer timer;
SnoozeUSBSerial usbSerial;
SnoozeBlock config(timer, usbSerial);

#define IN_PACKET_SIZE 64
#define OUT_PACKET_SIZE 64

elapsedMillis timerXiao;
elapsedMillis timerNine;
int stateXiao = 0;
int sleeping = 0;


//##########################################
byte buf_num_nine = 0;
byte buf_num_r_nine = 1;
byte buf_num_xiao = 0;
byte buf_num_r_xiao = 1;
byte buf_num_o1_nine = 2;
byte buf_num_o2_nine = 3;
byte c_state = 0;
byte x_state = 0;
char oldc_nine = 0;
char oldc_xiao = 0;
byte len_nine;
byte len_xiao;
byte i_n = 0;
byte i_x = 0;
byte iter_n = 0;
byte iter_x = 0;
byte packet_nine[4][IN_PACKET_SIZE];
byte packet_xiao[2][IN_PACKET_SIZE];
unsigned int pcrc_nine = 0;
unsigned int pcrc_xiao = 0;
char req_1[] = {0x5A, 0xA5, 0x01, 0x14, 0x12, 0x01, 0x10, 0x10, 0xB7, 0xFF};
char req_2[] = {0x5A, 0xA5, 0x01, 0x14, 0x12, 0x01, 0x30, 0x0C, 0x9B, 0xFF};
char req_3[] = {0x5A, 0xA5, 0x01, 0x14, 0x12, 0x55, 0x30, 0x0C, 0x47, 0xFF};

char req_xiao[][7] = {{0xDD, 0xA5, 0x03, 0x00, 0xFF, 0xFD, 0x77},
                      {0xDD, 0xA5, 0x04, 0x00, 0xFF, 0xFC, 0x77},
                      {0xDD, 0xA5, 0x05, 0x00, 0xFF, 0xFB, 0x77} };

uint8_t cx; 

char mem_bms[2][256] = {
              {0xA5,0xA5,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 00 // 00
              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 10 // 08
              0x34,0x39,0x59,0x45,0x51,0x31,0x38,0x48,0x37,0x52,0x30,0x30,0x33,0x33,0x12,0x01, // 20 // 10 // 10-SN,17-fw ver
              0x80,0x25,0x80,0x25,0xec,0x13,0x11,0x00,0x27,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 30 // 18 // 18-fac cap, 19-act cap, 1b - charge full cycles, 1c - charge count
              0x07,0x25,0x00,0x00,0x00,0x00,0x00,0x3a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 40 // 20 // 20 - mfg date
              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 50 // 28 
              0x01,0x00,0xbd,0x22,0x5c,0x00,0x04,0x00,0x1a,0x16,0x2d,0x2d,0x00,0x00,0x00,0x00, // 60 // 30 // lifedata
              0x00,0x00,0xbd,0x22,0xbd,0x22,0x62,0x00,0xc6,0x0f,0x00,0x00,0x00,0x00,0x00,0x00, // 70 // 38 // 3b- health
              0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f, // 80 // 40 // cells 1-8
              0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f, // 90 // 48 // cells 9-16
              0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // A0 // 50 // 51 -config strps
              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // B0 // 58
              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // C0 // 60 
              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // D0 // 68
              0x70,0x00,0x42,0x00,0x12,0x47,0x38,0x31,0x30,0x32,0x36,0x38,0x00,0x00,0x00,0x00, // E0 // 70 // MCU UID copy
              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // F0 // 78 
},{
              0xA5,0xA5,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 00 // 00
              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 10 // 08
              0x34,0x39,0x59,0x45,0x51,0x31,0x38,0x48,0x37,0x52,0x30,0x30,0x33,0x33,0x12,0x01, // 20 // 10 // 10-SN,17-fw ver
              0x80,0x25,0x80,0x25,0xec,0x13,0x11,0x00,0x27,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 30 // 18 // 18-fac cap, 19-act cap, 1b - charge full cycles, 1c - charge count
              0x07,0x25,0x00,0x00,0x00,0x00,0x00,0x3a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 40 // 20 // 20 - mfg date
              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 50 // 28 
              0x01,0x00,0xbd,0x22,0x5c,0x00,0x04,0x00,0x1a,0x16,0x2d,0x2d,0x00,0x00,0x00,0x00, // 60 // 30 // lifedata
              0x00,0x00,0xbd,0x22,0xbd,0x22,0x62,0x00,0xc6,0x0f,0x00,0x00,0x00,0x00,0x00,0x00, // 70 // 38 // 3b- health
              0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f, // 80 // 40 // cells 1-8
              0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f,0xc8,0x0f, // 90 // 48 // cells 9-16
              0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // A0 // 50 // 51 -config strps
              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // B0 // 58
              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // C0 // 60 
              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // D0 // 68
              0x70,0x00,0x42,0x00,0x12,0x47,0x38,0x31,0x30,0x32,0x36,0x38,0x00,0x00,0x00,0x00, // E0 // 70 // MCU UID copy
              0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // F0 // 78 
}
};
void setup() {

  Serial.begin(115200);
 
  Serial1.begin(115200);
  Serial2.begin(9600);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  setVoltage(3010, 0);
  setCurrent(0, 0);
  setTemp1(20, 0);
  setTemp2(20, 0);
  setMfgCap(9600, 0);
  setActCap(9600, 0);
  setChrgFullCycles(56, 0);
  setChrgCount(134, 0);
  setRemCap(32500, 0);
  setRemPerc(21, 0);
  setHealth(97, 0);

  setVoltage(3010, 1);
  setCurrent(0, 1);
  setTemp1(20, 1);
  setTemp2(20, 1);
  setMfgCap(9600, 1);
  setActCap(9600, 1);
  setChrgFullCycles(56, 1);
  setChrgCount(134, 1);
  setRemCap(32500, 1);
  setRemPerc(21, 1);
  setHealth(97, 1);
  timer.setTimer(1000);
  
  delay(1000);

  digitalWrite(13, LOW);
}




unsigned int computeCheckNine(byte buf, byte p_len) {

  unsigned int check = 0;
  for (int i=2; i <= p_len+6; i++) {
    //Serial.print(packet[buf][i], HEX);
    //Serial.print(' ');
    check = check + (packet_nine[buf][i] & 0xff);
    
  }
  check ^= 0xFFFF;
  check &= 0xFFFF;
  //Serial.println(' ');
  return check;
}

unsigned int computeCheckXiao(byte buf, byte p_len) {

  unsigned int check = 0xFFFF;
  for (int i=2; i <= p_len+3; i++) {
    check = check + (packet_xiao[buf][i] & 0xff);
  }
  check ^= 0xFFFF;
  check &= 0xFFFF;
  //Serial.print("CalcCRC=");
  //Serial.println(check, HEX);
  return check;
}


boolean verifyNine() {
  boolean check = false;
  if (pcrc_nine == computeCheckNine(buf_num_r_nine, len_nine)) {
    check = true;
//  Serial.println(cx, HEX);
    //Serial.println("crc ok");
  } else {
    //Serial.println("crc fail");
  }
  return check;
}


boolean verifyXiao() {
  boolean check = false;
  if (pcrc_xiao == computeCheckXiao(buf_num_r_xiao, len_xiao)) {
    check = true;
//  Serial.println(cx, HEX);
    //Serial.println("crc ok");
  } else {
    //Serial.println("crc fail");
  }
  return check;
}

boolean parseinNine(){
  char saddr = packet_nine[buf_num_r_nine][3];
  char daddr = packet_nine[buf_num_r_nine][4];
  char cmd = packet_nine[buf_num_r_nine][5];
  char maddr = packet_nine[buf_num_r_nine][6];
  char mlen = packet_nine[buf_num_r_nine][7];
/*  Serial.print("Received command: len ");
  Serial.print(len, HEX);
  
  Serial.print(" saddr: ");
  Serial.print(saddr, HEX);
  Serial.print(" daddr: ");
  Serial.print(daddr, HEX);
  Serial.print(" cmd: ");
  Serial.print(cmd, HEX);
  Serial.print(" maddr: ");
  Serial.print(maddr, HEX);
  Serial.print(" with len: ");
  Serial.println(mlen, HEX); */
  if ((daddr == 0x11) || (daddr == 0x12)) {
    if ((cmd == 0x55) || (cmd == 0x01)) {
      if (cmd == 0x01) {
       cmd = 0x04;
     }
     if (cmd == 0x55) {
       mlen = 0x1a;
     }
     if ((saddr == 0x3E) && (!((maddr == 0x10) || (maddr == 0x30)))) {
      stateXiao = 1;      
      Serial.print("Received command: len ");
      Serial.print(len_nine, HEX);
      Serial.print(" saddr: ");
      Serial.print(saddr, HEX);
      Serial.print(" daddr: ");
      Serial.print(daddr, HEX);
      Serial.print(" cmd: ");
      Serial.print(cmd, HEX);
      Serial.print(" maddr: ");
      Serial.print(maddr, HEX);
      Serial.print(" with len: ");
      Serial.println(mlen, HEX);
     }
     //Serial.print("Transmitt command: ");
     //Serial.println(cmd, HEX);
     genpacket(buf_num_o1_nine,mlen,daddr,saddr,cmd,maddr, daddr-0x11); 
    } else {
      Serial.print("Received unknown command: ");
      for (int rr = 0; rr < len_nine+9; rr++){
        Serial.print(packet_nine[buf_num_r_nine][rr], HEX);
        Serial.print(' ');
      }
      Serial.println(' ');  
      if (cmd == 0x54) {
        Serial.println("Snooze");
        delay(20);  
        //Snooze.deepSleep(config);
      }
    }
  } else if (daddr == 0x16) {
      if (!((cmd == 0x02 && maddr == 0x05 && mlen == 0x02) ||(cmd == 0x01 && maddr == 0x01 && mlen == 0x02) || (cmd == 0x02 && maddr == 0x01) || (cmd == 0x05 && maddr == 0x01 && mlen == 0x02))) {
        
      
      Serial.print("Received msg to 16: ");
      for (int rr = 0; rr < len_nine+9; rr++){
        Serial.print(packet_nine[buf_num_r_nine][rr], HEX);
        Serial.print(' ');
      }
      Serial.println(' ');
  }
  } else {
    if (daddr != 0x3E) {
    Serial.print("Received msg to XX: ");
      for (int rr = 0; rr < len_nine+9; rr++){
        Serial.print(packet_nine[buf_num_r_nine][rr], HEX);
        Serial.print(' ');
      }
      Serial.println(' ');
  }
  }
}

boolean parseinXiao(){


  if (packet_xiao[buf_num_r_xiao][1] == 0x03) {    
    int volt_xiao = (packet_xiao[buf_num_r_xiao][4] << 8) | (packet_xiao[buf_num_r_xiao][5]);
    setVoltage(volt_xiao, 0);
    setVoltage(volt_xiao-5, 1);
    int curr_xiao = (packet_xiao[buf_num_r_xiao][6] << 8) | (packet_xiao[buf_num_r_xiao][7]);    
    if (curr_xiao > 0) {
      setCurrent(-1 * curr_xiao, 0);
      setCurrent(0, 1);      
    }
    else {
      setCurrent(-1 * curr_xiao/2, 0);
      setCurrent(-1 * curr_xiao/2, 1);      
    }
    int remain_cap_xiao = ((packet_xiao[buf_num_r_xiao][8] << 8) | (packet_xiao[buf_num_r_xiao][9]))*10;
    setRemCap(remain_cap_xiao/2, 0);
    setRemCap(remain_cap_xiao/2, 1);
    int full_cap_xiao = ((packet_xiao[buf_num_r_xiao][10] << 8) | (packet_xiao[buf_num_r_xiao][11]))*10; 
    setMfgCap(full_cap_xiao/2, 0);
    setActCap(full_cap_xiao/2, 0);
    setMfgCap(full_cap_xiao/2, 1);
    setActCap(full_cap_xiao/2, 1);
    int cycles_xiao = ((packet_xiao[buf_num_r_xiao][12] << 8) | (packet_xiao[buf_num_r_xiao][13])); 
    setChrgFullCycles(cycles_xiao,0);
    setChrgFullCycles(cycles_xiao,1);
    int date_xiao = (packet_xiao[buf_num_r_xiao][14] << 8) | (packet_xiao[buf_num_r_xiao][15]);
    int year_xiao = date_xiao >> 9;
    int mounth_xiao = (date_xiao >> 5) & 0x0F;
    int day_xiao = (date_xiao) & 0x1F;
    char bal1_xiao = (packet_xiao[buf_num_r_xiao][16]);
    setBalancing1(bal1_xiao, 0);
    setBalancing1(bal1_xiao, 1);
    char bal2_xiao = (packet_xiao[buf_num_r_xiao][17]);
    setBalancing2(bal2_xiao, 0);
    setBalancing2(bal2_xiao, 1);
    char bal3_xiao = (packet_xiao[buf_num_r_xiao][18]);
    char bal4_xiao = (packet_xiao[buf_num_r_xiao][19]);
    unsigned int flags_xiao = ((packet_xiao[buf_num_r_xiao][20] << 8) | (packet_xiao[buf_num_r_xiao][21]));
    char fw_ver_xiao = (packet_xiao[buf_num_r_xiao][22]);
    int percent_xiao = (packet_xiao[buf_num_r_xiao][23]);
    char chrg_fet_xiao = (packet_xiao[buf_num_r_xiao][24]) & 0x01;
    char dschrg_fet_xiao = (packet_xiao[buf_num_r_xiao][24] >> 1) & 0x01;
    int cell_num_xiao = (packet_xiao[buf_num_r_xiao][25]);
    int temp_num_xiao = (packet_xiao[buf_num_r_xiao][26]);

    Serial.print("Received life data: V=");
    Serial.print(volt_xiao);
    Serial.print(" I="); 
    Serial.print(curr_xiao);
    Serial.print(" RemCap=");   
    Serial.print(remain_cap_xiao);
    Serial.print(" FullCap=");
    Serial.print(full_cap_xiao);    
    Serial.print(" cycles=");
    Serial.print(cycles_xiao);
    Serial.print(" YY=");    
    Serial.print(year_xiao);
    Serial.print(" MM=");
    Serial.print(mounth_xiao);
    Serial.print(" DD=");
    Serial.print(day_xiao);
    Serial.print(" BAL1=");    
    Serial.print(bal1_xiao, HEX);
    Serial.print(" BAL2=");
    Serial.print(bal2_xiao, HEX);
    Serial.print(" BAL3=");
    Serial.print(bal3_xiao, HEX);
    Serial.print(" BAL4=");
    Serial.print(bal4_xiao, HEX);    
    Serial.print(" flags=");
    Serial.print(flags_xiao, HEX);    
    Serial.print(" fw=");
    Serial.print(fw_ver_xiao, HEX);    
    Serial.print(" %=");
    Serial.print(percent_xiao);
    Serial.print(" CHRG=");
    Serial.print(chrg_fet_xiao, HEX);
    Serial.print(" DSCHRG=");
    Serial.print(dschrg_fet_xiao, HEX);
    Serial.print(" Cells=");    
    Serial.print(cell_num_xiao);
    Serial.print(" Temp_n=");    
    Serial.print(temp_num_xiao);

    if (temp_num_xiao > 0){
      int temp_1_xiao = ((packet_xiao[buf_num_r_xiao][27] << 8) | (packet_xiao[buf_num_r_xiao][28])) - 2732;
      setTemp1(temp_1_xiao/10 ,0);
      setTemp1(temp_1_xiao/10 ,1);
      Serial.print(" Temp_1=");
      Serial.print(temp_1_xiao);
    }    
    if (temp_num_xiao > 1){
      int temp_2_xiao = ((packet_xiao[buf_num_r_xiao][29] << 8) | (packet_xiao[buf_num_r_xiao][30])) - 2732;
      setTemp2(temp_2_xiao/10, 0);
      setTemp2(temp_2_xiao/10, 1);
      Serial.print(" Temp_2=");
      Serial.print(temp_2_xiao);
    }
    if (temp_num_xiao > 2){
      int temp_3_xiao = ((packet_xiao[buf_num_r_xiao][31] << 8) | (packet_xiao[buf_num_r_xiao][32])) - 2732;
      Serial.print(" Temp_3=");
      Serial.print(temp_3_xiao);
    }
    if (temp_num_xiao > 3){
      int temp_4_xiao = ((packet_xiao[buf_num_r_xiao][33] << 8) | (packet_xiao[buf_num_r_xiao][34])) - 2732;
      Serial.print(" Temp_4=");
      Serial.print(temp_4_xiao);
    }
    if (temp_num_xiao > 4){
      int temp_5_xiao = ((packet_xiao[buf_num_r_xiao][35] << 8) | (packet_xiao[buf_num_r_xiao][36])) - 2732;
      Serial.print(" Temp_5=");
      Serial.print(temp_5_xiao);
    }
    if (temp_num_xiao > 5){
      int temp_6_xiao = ((packet_xiao[buf_num_r_xiao][37] << 8) | (packet_xiao[buf_num_r_xiao][38])) - 2732;
      Serial.print(" Temp_6=");
      Serial.print(temp_6_xiao);
    }
    if (temp_num_xiao > 6){
      int temp_7_xiao = ((packet_xiao[buf_num_r_xiao][39] << 8) | (packet_xiao[buf_num_r_xiao][40])) - 2732;
      Serial.print(" Temp_7=");
      Serial.print(temp_7_xiao);
    }
    if (temp_num_xiao > 7){
      int temp_8_xiao = ((packet_xiao[buf_num_r_xiao][41] << 8) | (packet_xiao[buf_num_r_xiao][42])) - 2732;
      Serial.print(" Temp_8=");
      Serial.print(temp_8_xiao);
    }
    Serial.println(' ');     
    if (percent_xiao == 0 && cycles_xiao == 0 && remain_cap_xiao == 0) {
      Serial.print("Init mode, recalc percents... ");
      int fake_percents = (100*(volt_xiao - (cell_num_xiao*300)))/(cell_num_xiao*420-cell_num_xiao*300);
      Serial.println(fake_percents);
      setRemPerc(fake_percents,0);
      setRemPerc(fake_percents,1);
    } else {
      setRemPerc(percent_xiao, 0);
      setRemPerc(percent_xiao, 1);
    }   

  } else if (packet_xiao[buf_num_r_xiao][1] == 0x04) {
    //Serial.println("Cells: ");
    int cells_xiao[len_xiao/2];
    for (int cells_i = 0; cells_i < (len_xiao/2); cells_i++) {
      cells_xiao[cells_i] = (packet_xiao[buf_num_r_xiao][(cells_i*2)+4]<<8) | packet_xiao[buf_num_r_xiao][(cells_i*2)+5];
      setCellVoltage(cells_xiao[cells_i],cells_i, 0);
      setCellVoltage(cells_xiao[cells_i],cells_i, 1);
      Serial.print(cells_i);
      Serial.print(" : ");
      Serial.print(cells_xiao[cells_i]);
      if ((mem_bms[0][0x36*2 + (cells_i/8)] >> (7 - cells_i%8)) & 0x01 == 1) Serial.print(" B");
      Serial.println();
      
    }
  } else if (packet_xiao[buf_num_r_xiao][1] == 0x05) {
    String ser_num = "";      
    for (int sn = 0; sn<len_xiao;sn++) {
      ser_num = ser_num + (char)packet_xiao[buf_num_r_xiao][sn+4];
    }
    Serial.print("SN: ");
    Serial.println(ser_num);
  } else {
    // 
  }
  return true;
}

void writeIntToMem(int num, int offsett, int bms) {

  mem_bms[bms][offsett] = num & 0xFF;
  mem_bms[bms][offsett+1] = (num>>8) & 0xFF;
}


void setMfgCap(int num, int bms){
  writeIntToMem(num, 0x18*2, bms);
}
void setActCap(int num, int bms){
  writeIntToMem(num, 0x19*2, bms);
}
void setChrgFullCycles(int num, int bms){
  writeIntToMem(num, 0x1B*2, bms);
}
void setChrgCount(int num, int bms){
  writeIntToMem(num, 0x1C*2, bms);
}
void setMfgDate(int num, int bms){
  writeIntToMem(num, 0x20*2, bms);
}
void setRemCap(int num, int bms){
  writeIntToMem(num, 0x31*2, bms);
}
void setRemPerc(int num, int bms){
  writeIntToMem(num, 0x32*2, bms);
}
void setCurrent(int num, int bms){
  writeIntToMem(num, 0x33*2, bms);
}
void setVoltage(int num, int bms){
  writeIntToMem(num, 0x34*2, bms);
}
void setTemp1(int num, int bms){
  mem_bms[bms][0x35*2] = (num+20) & 0xFF;
}
void setTemp2(int num, int bms){
  mem_bms[bms][0x35*2+1] = (num+20) & 0xFF;
}
void setBalancing1(int num, int bms){
  mem_bms[bms][0x36*2] = num & 0xFF;
}
void setBalancing2(int num, int bms){
  mem_bms[bms][0x36*2+1] = num & 0xFF;
}
void setHealth(int num, int bms){
  writeIntToMem(num, 0x3B*2, bms);
}
void setCellVoltage(int num, int cell, int bms){
  if (cell<16) {
    writeIntToMem(num, (0x40+cell)*2, bms);
  }
}

boolean genpacket(byte buf, byte mlen, byte saddr, byte daddr, byte cmd, byte maddr, int bms) {
  packet_nine[buf][0] = 0x5A;
  packet_nine[buf][1] = 0xA5;
  packet_nine[buf][2] = mlen;
  packet_nine[buf][3] = saddr;
  packet_nine[buf][4] = daddr;
  packet_nine[buf][5] = cmd;
  packet_nine[buf][6] = maddr;
  for (int m = 0; m<mlen; m++) {
    packet_nine[buf][m+7] = mem_bms[bms][(maddr*2)+m];
  }
  unsigned int crc_o = computeCheckNine(buf,mlen);
  packet_nine[buf][mlen+7] = crc_o & 0xFF;
  packet_nine[buf][mlen+8] = (crc_o >> 8) & 0xFF;

  


  Serial1.write(packet_nine[buf], mlen + 9);

  for (int rr = 0; rr < mlen+9; rr++){
    //Serial.print(packet[buf][rr], HEX);
    //Serial.print(' ');
  }

}


boolean collectorNine(char c) {
  
  if (c_state == 2) {
    //Serial.print(c, HEX);
    //Serial.print(' ');
    packet_nine[buf_num_nine][i_n] = c;
    if (i_n == len_nine +8) {
      c_state = 0;
      pcrc_nine = (packet_nine[buf_num_nine][i_n] << 8) | (packet_nine[buf_num_nine][i_n-1]);
      
      i_n = 0;
//      Serial.println("]]]]]");
      return true;
    }
    i_n += 1;
  } else if (c_state == 1) {
//    Serial.print(c, HEX);
//    Serial.print(' ');
    len_nine = c;
    packet_nine[buf_num_nine][i_n] = c;
    i_n += 1;
    c_state = 2;
  } else if (c_state == 0) {
    if (oldc_nine==0x5a && c==0xa5) {
      c_state = 1;
      packet_nine[buf_num_nine][0] = 0x5a;
      packet_nine[buf_num_nine][1] = 0xa5;
      i_n = 2;
    }
  }
  oldc_nine = c;
  return false;
}

boolean collectorXiao(char c) {
  Serial.print(c, HEX);
  if (x_state == 3) {
    //Serial.print(c, HEX);
    //Serial.print(' ');
    packet_xiao[buf_num_xiao][i_x] = c;
    if (i_x >= len_xiao +6) {
      x_state = 0;
      pcrc_xiao = (packet_xiao[buf_num_xiao][i_x-2] << 8) | (packet_xiao[buf_num_xiao][i_x-1]); 
      
      i_x = 0;
      //Serial.println("]]]]]");
      //Serial.print("CRC=");
      //Serial.println(pcrc_xiao, HEX);
      return true;
    }
    i_x += 1;
  } else if (x_state == 2) {
    //Serial.print(c, HEX);
    //Serial.print(' ');
    len_xiao = len_xiao | c;
    packet_xiao[buf_num_xiao][i_x] = c;
    i_x += 1;
    x_state = 3;
  }  else if (x_state == 1) {
    //Serial.print(c, HEX);
    //Serial.print(' ');
    len_xiao = c<<8;
    packet_xiao[buf_num_xiao][i_x] = c;
    i_x += 1;
    x_state = 2;
  } else if (x_state == 0) {
    if (oldc_xiao==0xDD && (c==0x03 || c==0x04 || c==0x05)) {
      //Serial.print(c, HEX);
      //Serial.print(' ');
      x_state = 1;
      packet_xiao[buf_num_xiao][0] = 0xDD;
      packet_xiao[buf_num_xiao][1] = c;
      i_x = 2;
    }
  }
  oldc_xiao = c;
  return false;
}

char getdata1() {
  char c;
  if (iter_n < 10) {
    c = req_1[iter_n%10]; 
  }
  else if (iter_n < 20) {
    c = req_2[iter_n%10];
  }
  else {
    c = req_3[iter_n%10];
  }
  //Serial.print(iter);
  //Serial.print(" ");
  //Serial.println(c, HEX);
  iter_n += 1;
  iter_n %= 30;

  
  //Serial.println(iter, c);
  return c;
}

void loop() {

  
  // Ask XiaoXiang SmartBMS every XXX ms
  if ((timerXiao >= 125) && (sleeping == 0)) {
    
    Serial2.write(req_xiao[stateXiao], 7);
    
    Serial.println("Sent xiaoreq");
    //Serial.println(stateXiao);  
    timerXiao = 0;
    stateXiao += 1;
    stateXiao %=1;
  }
  // Check XiaoXiang Response
  while (Serial2.available()) {
    if (collectorXiao(Serial2.read())) {
      buf_num_r_xiao = buf_num_xiao;
      buf_num_xiao += 1;
      buf_num_xiao %= 2;
      if (verifyXiao()){
        parseinXiao();
      }
    }      
  }
  // Check controller's request
  while (Serial1.available()) {
    sleeping = 0;
    timerNine =0;
    if (collectorNine(Serial1.read())) {
      buf_num_r_nine = buf_num_nine;
      buf_num_nine += 1;
      buf_num_nine %= 2;
      if (verifyNine()) {
        parseinNine();
      }     
    }
  }  
  // Sleep if ninebot stoped communication for 0.5s
  if (timerNine >= 500) {
    sleeping = 1;
    timerNine = 0;
    timerXiao = 0;
    Serial.println("Snooze");
    delay(20);
    Snooze.deepSleep(config); // sleep if no requests from ninebot
    
  }
}
