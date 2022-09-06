//定义引脚连接
int CLK = 2;//CLK->D2
int DT = 3;//DT->D3
int SW = 4;//SW->D4
const int interrupt0 = 0;// Interrupt 0 在 pin 2 上
int count = 0;//计数值
int lastCLK = 0;//CLK历史值

void setup()
{
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  attachInterrupt(interrupt0, ClockChanged, CHANGE);//设置中断0的处理函数，电平变化触发
  Serial.begin(9600);
}

void loop()
{
  if (!digitalRead(SW) && count != 0) //读取到按钮按下并且计数值不为0时把计数器清零
  {
    count = 0;
    Serial.print("count:");
    Serial.println(count);
  }
}

//中断处理函数
void ClockChanged()
{
  int clkValue = digitalRead(CLK);//读取CLK引脚的电平
  int dtValue = digitalRead(DT);//读取DT引脚的电平
  if (lastCLK != clkValue)
  {
    lastCLK = clkValue;
    count += (clkValue != dtValue ? 1 : -1);//CLK和DT不一致时+1，否则-1
    Serial.print("count:");
    Serial.print(count);
    int du =abs(count%360);
     Serial.print("\t");
    Serial.println(du);
  }
}
