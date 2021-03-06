//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "AboutProgram.h"
#include "IniFiles.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CPort"
#pragma resource "*.dfm"


TForm1 *Form1;
AnsiString dir = GetCurrentDir(); 
//**************** COMPORT variables*******************************************
unsigned char ReadInfo[] ={0x07, 0x34, 0x00, 0x00, 0x00, 0xBB, 0xF0 } ;
bool new_paket=true;  //  read the new package from the port
unsigned char read_byte; // the number of bits read from comport
unsigned char work_buffer[256];  //com buffers
unsigned int Packet_Send=0;
unsigned char flag_IDP=0; //  IDP=2 -day begin, IDP=4 - month begin
unsigned char IDP; //  request ID  1-byte
unsigned char IDR; //  additional  request 1-byte


bool Meter_connect=false;   // Flag of meter's connection
unsigned int KTU=1,KTI=1;   // coefficient of transformation
unsigned char Count_Day=0;  // Dept days archive,max 95;
unsigned char Count_Month=0;  // Dept months archive,max 23;
unsigned char Count_Year=0;  // Dept years archive,max 5;
bool Packet_received=false; // Flag permission to send the next packet
unsigned int Factory_Number=0;
AnsiString Short_info="";

AnsiString Intervals[]={"00:00-00:30","00:30-01:00","01:00-01:30","01:30-02:00","02:00-02:30","02:30-03:00",
                        "03:00-03:30","03:30-04:00","04:00-04:30","04:30-05:00","05:00-05:30","05:30-06:00",
                        "06:00-06:30","06:30-07:00","07:00-07:30","07:30-08:00","08:00-08:30","08:30-09:00",
                        "09:00-09:30","09:30-10:00","10:00-10:30","10:30-11:00","11:00-11:30","11:30-12:00",
                        "12:00-12:30","12:30-13:00","13:00-13:30","13:30-14:00","14:00-14:30","14:30-15:00",
                        "15:00-15:30","15:30-16:00","16:00-16:30","16:30-17:00","17:00-17:30","17:30-18:00",
                        "18:00-18:30","18:30-19:00","19:00-19:30","19:30-20:00","02:00-20:30","20:30-21:00",
                        "21:00-21:30","21:30-22:00","22:00-22:30","22:30-23:00","23:00-23:30","23:30-24:00"} ;


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------



void __fastcall TForm1::N4Click(TObject *Sender)
{
 Form1->Close();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::toExcelCell(int Row,int Column, AnsiString data)
{
  try {
    Variant  cur = Sh.OlePropertyGet("Cells", Row,Column);
    cur.OlePropertySet("Value", data.c_str());
  } catch(...) { ; }
}/* toExcelCell() */


void __fastcall TForm1::N2Click(TObject *Sender)
{
//Form2->Visible=true;
     dir = GetCurrentDir();
     ComPort1->LoadSettings(stIniFile, dir + "\\PortSettings.ini");
     ComPort1->ShowSetupDialog();
     ComPort1->StoreSettings(stIniFile, dir + "\\PortSettings.ini");
}
//--------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
{
  if (Button2->Caption=="����������")
  {
   Button2->Caption="���������" ;
   dir = GetCurrentDir();
   ComPort1->LoadSettings(stIniFile, dir + "\\PortSettings.ini");
   ComPort1->Open();
   SendData(0x00,0x00,0x01);
   Form1->Timer1->Enabled=true;
   StatusBar1->SimpleText="������� ���������";
  }
  else
  {
   ComPort1->Close();
   Button1->Enabled=false;
   Button2->Caption="����������";
   Label1->Caption="";
   StatusBar1->SimpleText="������� ��������";
  }

}
//---------------------------------------------------------------------------


void __fastcall TForm1::ComPort1RxChar(TObject *Sender, int Count)
{
     Timer1->Enabled=false;
    if (new_paket==true)
     {
     new_paket =false;
     read_byte=0;
     }

     if ((read_byte+Count)>250)
     {
     ComPort1->ClearBuffer(true, true);
     ShowMessage("������������ �������");
      memset(work_buffer,0,256);
     read_byte=0;
     new_paket = true;

     return;
     }

     ComPort1->Read(&work_buffer[read_byte], Count);
     read_byte+=Count;

      if  ( (read_byte == work_buffer[0])&&(read_byte>=7))
      {
      // ShowMessage("����� ������");
        if(DecodeInBuffer())
        {
        // ShowMessage("����� ��������");
        if(Meter_connect==true)
        Button1->Enabled=true;
        else
        Button1->Enabled=false;

          new_paket = true;
          read_byte=0;

         if (IDP==03 && Count_Day<=95 && Packet_received  )
         {
           Packet_received=false;
           Sleep(100);
           SendData(0x02,Count_Day,0x01);
         }
         else
        {
         if(Count_Day==96)
         StatusBar1->SimpleText="����� ��������!" ;
       //  Timer1->Enabled=true;
        }


          if (IDP==05 && Count_Month<23 && Packet_received  )
         {
           Packet_received=false;
           Sleep(100);
           SendData(0x04,Count_Month,0x01);
         }
         else
        {
       //  Timer1->Enabled=true;
        }
          if (IDP==07 && Count_Year<5 && Packet_received  )
         {
           Packet_received=false;
           Sleep(100);
           SendData(0x06,Count_Year,0x01);
         }
         else
        {
       //  Timer1->Enabled=true;
        }
           if (IDP==11 && Count_Day<95 && Packet_received  )
         {
           Packet_received=false;
           Sleep(100);
           SendData(0x0A,Count_Day,0x01);
         }
         else
        {
       //  Timer1->Enabled=true;
        }

        if (IDP==13 && Count_Month<23 && Packet_received  )
         {
           Packet_received=false;
           Sleep(100);
           SendData(0x0C,Count_Month,0x01);
         }
         else
        {
       //  Timer1->Enabled=true;
        }
       // Form1->Timer1Timer( Sender);
         //Button3->Enabled=true;
        ///Timer1Timer(0);
        }
        else
        {
      //  ShowMessage("����� �� ��������");
        read_byte=0;
    //    Timer1->Enabled=true;
        }

      }


}
//---------------------------------------------------------------------------


TForm1::SendData(unsigned char i, unsigned char j, unsigned char k)
{
      if (k)
 {
 ReadInfo[1]=0;
 ReadInfo[2]=1;
 ReadInfo[3]= i;
 ReadInfo[4]= j;
 //*************add CRC to packet*****************************************
 ReadInfo[5]=CRC16b(&ReadInfo[0],ReadInfo[0]-2);
 ReadInfo[6]=CRC16b(&ReadInfo[0],ReadInfo[0]-2)>>8;
 ComPort1->Write(ReadInfo,sizeof(ReadInfo)) ;
 //Sleep(1000);
 //TimerTimeout->Enabled=true;
 }
   else
{
 // Button3->Enabled=true;
  ShowMessage("��� ��������� ���������� �������� ������� !");
 }
 return true;
}

unsigned short TForm1::CRC16b(unsigned char *msg, int len)
{
    int i,j;
  unsigned char bute,check_flag;
  unsigned short CRC=0;

  for (j=0;j<len;j++)
  {
   bute=msg[j];
   for (i=0;i<8;i++)
   {
    check_flag=((bute^CRC)&0x01);
    CRC>>=1;
    bute>>=1;
    if (check_flag==1)CRC^=0xA001;
   }
  }
  return CRC;

}

bool TForm1::DecodeInBuffer()
{
//***********  Check CRC *******************************************************
 if (!CRC16b(&work_buffer[0],work_buffer[0]))  ;
 else
 {
  ShowMessage("������ ����������� �����!!!" + IntToStr(CRC16b(&work_buffer[0],work_buffer[0]-2)));
  return false;
 }
//***********  Check Error packet***********************************************
 if (work_buffer[0]==0x0A && work_buffer[5]==0xAA && work_buffer[7]==0xAA)
 {
  switch(work_buffer[6])
  {
   case 1:
   StatusBar1->SimpleText="������ ������� � ���������� (EEPROM) ������";
   break;
   case 2:
   StatusBar1->SimpleText="����������� �������";
   break;
   case 4:
   StatusBar1->SimpleText="��������� ������������� ������ ������������";
   break;
   case 8:
   StatusBar1->SimpleText="������ ������������ �� ������";
   break;
   case 16:
   StatusBar1->SimpleText="������ ������������ �� ���������";
   break;
   case 32:
   StatusBar1->SimpleText="������ ���������� � ������";
   Count_Day=0;
   Count_Month=0;
   Count_Year=0;
   KTU=KTI=1;
    Label2->Caption="KU: " + IntToStr(KTU);
    Label3->Caption="KI: " + IntToStr(KTI);
   break;
   case 64:
   StatusBar1->SimpleText="������������ ������ �� ������������ ����������";
   break;
   case 128:
   StatusBar1->SimpleText="������ ������� ������ ��������������� ������";
   break;
   default:
   StatusBar1->SimpleText="����������� ������!";
  }
// ShowMessage("������ �������� ������!!! ���: " + IntToStr(work_buffer[6]));

  return false;
 }

//************** Chek Net Adress ***********************************************

 /*if (GetCurrentNA()==(((work_buffer[2]&0x3f)<<8) + work_buffer[1] ))
 // {
 //    ValueListEditor1->Cells[1][11]=IntToStr(((work_buffer[2]&0x3f)<<8) + work_buffer[1] )  ;
 // }
 //    else
  {
   ValueListEditor1->Cells[1][11]=IntToStr(((work_buffer[2]&0x3f)<<8) + work_buffer[1] )  ;
   ShowMessage("�� ���������� ������� �����!!");
   return false;
  }
          */
//*********Check IDP & IDR******************************************************
 IDP = work_buffer[3];
 IDR = work_buffer[4];
 switch (IDP)
 {case 1:  //  Read system parameters
  {
    switch (IDR)
    {
      case 0:  //  Read system parameters  of meters
      {
       if(ReadSysPar())
       {
        Meter_connect=true;
        break;
       }
       else
       {
        Meter_connect=false;
        return false;
       }
      }

      default:  //
      {
      ShowMessage("����������� �������������� ������������� ������ IDR = " + IntToStr(IDR));
      return false;
      }

     }

     break;
  }

  case 3:  // energy reading at the beginning of the day
  {
   if (IDR <= 95)
   {
    KTU= (work_buffer[5]<<8)+work_buffer[6];
    KTI= (work_buffer[7]<<8)+work_buffer[8]; //     ShowMessage(IntToStr(KTU));
    Label2->Caption="KU: " + IntToStr(KTU);
    Label3->Caption="KI: " + IntToStr(KTI);
    if (CheckBox1->Checked==false)  KTU=KTI=1;
    StringGrid2->Cells[5][1+Count_Day]= FloatToStr(float((work_buffer[9]<<24)+ (work_buffer[10]<<16)+ (work_buffer[11]<<8)+ work_buffer[12])/1000*KTI*KTU);
    StringGrid2->Cells[1][1+Count_Day]= FloatToStr(float((work_buffer[13]<<24)+ (work_buffer[14]<<16)+ (work_buffer[15]<<8)+ work_buffer[16])/1000*KTI*KTU);
    StringGrid2->Cells[2][1+Count_Day]= FloatToStr(float((work_buffer[17]<<24)+ (work_buffer[18]<<16)+ (work_buffer[19]<<8)+ work_buffer[20])/1000*KTI*KTU);
    StringGrid2->Cells[3][1+Count_Day]= FloatToStr(float((work_buffer[21]<<24)+ (work_buffer[22]<<16)+ (work_buffer[23]<<8)+ work_buffer[24])/1000*KTI*KTU);
    StringGrid2->Cells[4][1+Count_Day]= FloatToStr(float((work_buffer[25]<<24)+ (work_buffer[26]<<16)+ (work_buffer[27]<<8)+ work_buffer[28])/1000*KTI*KTU);
    StringGrid2->Cells[0][1+Count_Day]= IntPlusZero(work_buffer[33])+ "-" + IntPlusZero(work_buffer[34])+ "-20" + IntPlusZero(work_buffer[35])+ " " + IntPlusZero(work_buffer[31]) + ":" +IntPlusZero(work_buffer[30])+ ":" + IntPlusZero(work_buffer[29]);
    Count_Day++;
    Packet_received=true;

   }
   else
   {
    ShowMessage("����������� �������������� ������������� ������!������ 95" );
    return false;
   }
     break;
  }

  case 5:  // energy reading at the beginning of the month
  {
     if (IDR < 23)
   {
    KTU= (work_buffer[5]<<8)+work_buffer[6];
    KTI= (work_buffer[7]<<8)+work_buffer[8]; //     ShowMessage(IntToStr(KTU));
    Label2->Caption="KU: " + IntToStr(KTU);
    Label3->Caption="KI: " + IntToStr(KTI);
    if (CheckBox1->Checked==false)  KTU=KTI=1;
    StringGrid2->Cells[5][1+Count_Month]= FloatToStr(float((work_buffer[9]<<24)+ (work_buffer[10]<<16)+ (work_buffer[11]<<8)+ work_buffer[12])/1000*KTI*KTU);
    StringGrid2->Cells[1][1+Count_Month]= FloatToStr(float((work_buffer[13]<<24)+ (work_buffer[14]<<16)+ (work_buffer[15]<<8)+ work_buffer[16])/1000*KTI*KTU);
    StringGrid2->Cells[2][1+Count_Month]= FloatToStr(float((work_buffer[17]<<24)+ (work_buffer[18]<<16)+ (work_buffer[19]<<8)+ work_buffer[20])/1000*KTI*KTU);
    StringGrid2->Cells[3][1+Count_Month]= FloatToStr(float((work_buffer[21]<<24)+ (work_buffer[22]<<16)+ (work_buffer[23]<<8)+ work_buffer[24])/1000*KTI*KTU);
    StringGrid2->Cells[4][1+Count_Month]= FloatToStr(float((work_buffer[25]<<24)+ (work_buffer[26]<<16)+ (work_buffer[27]<<8)+ work_buffer[28])/1000*KTI*KTU);
    StringGrid2->Cells[0][1+Count_Month]= IntPlusZero(work_buffer[33])+ "-" + IntPlusZero(work_buffer[34])+ "-20" + IntPlusZero(work_buffer[35])+ " " + IntPlusZero(work_buffer[31]) + ":" +IntPlusZero(work_buffer[30])+ ":" + IntPlusZero(work_buffer[29]);
    Count_Month++;
    Packet_received=true;

   }
   else
   {
    ShowMessage("����������� �������������� ������������� ������!������ 23" );
    return false;
   }
     break;
  }

    case 7:  // energy reading at the beginning of the yers
  {
     if (IDR < 5)
   {
    KTU= (work_buffer[5]<<8)+work_buffer[6];
    KTI= (work_buffer[7]<<8)+work_buffer[8]; //     ShowMessage(IntToStr(KTU));
    Label2->Caption="KU: " + IntToStr(KTU);
    Label3->Caption="KI: " + IntToStr(KTI);
    if (CheckBox1->Checked==false)  KTU=KTI=1;
    StringGrid2->Cells[5][1+Count_Year]= FloatToStr(float((work_buffer[9]<<24)+ (work_buffer[10]<<16)+ (work_buffer[11]<<8)+ work_buffer[12])/1000*KTI*KTU);
    StringGrid2->Cells[1][1+Count_Year]= FloatToStr(float((work_buffer[13]<<24)+ (work_buffer[14]<<16)+ (work_buffer[15]<<8)+ work_buffer[16])/1000*KTI*KTU);
    StringGrid2->Cells[2][1+Count_Year]= FloatToStr(float((work_buffer[17]<<24)+ (work_buffer[18]<<16)+ (work_buffer[19]<<8)+ work_buffer[20])/1000*KTI*KTU);
    StringGrid2->Cells[3][1+Count_Year]= FloatToStr(float((work_buffer[21]<<24)+ (work_buffer[22]<<16)+ (work_buffer[23]<<8)+ work_buffer[24])/1000*KTI*KTU);
    StringGrid2->Cells[4][1+Count_Year]= FloatToStr(float((work_buffer[25]<<24)+ (work_buffer[26]<<16)+ (work_buffer[27]<<8)+ work_buffer[28])/1000*KTI*KTU);
    StringGrid2->Cells[0][1+Count_Year]= IntPlusZero(work_buffer[33])+ "-" + IntPlusZero(work_buffer[34])+ "-20" + IntPlusZero(work_buffer[35])+ " " + IntPlusZero(work_buffer[31]) + ":" +IntPlusZero(work_buffer[30])+ ":" + IntPlusZero(work_buffer[29]);
    Count_Year++;
    Packet_received=true;

   }
   else
   {
    ShowMessage("����������� �������������� ������������� ������!������ 5" );
    return false;
   }
     break;
  }
  case 9:  //  Reading current energy savings
      {
       if(IDR==1)
       {
       // ShowMessage("������� �������");
        KTU= (work_buffer[5]<<8)+work_buffer[6];
        KTI= (work_buffer[7]<<8)+work_buffer[8]; //     ShowMessage(IntToStr(KTU));
        Label2->Caption="KU: " + IntToStr(KTU);
        Label3->Caption="KI: " + IntToStr(KTI);
        if (CheckBox1->Checked==false)  KTU=KTI=1;
        StringGrid2->Cells[5][1]= FloatToStr(float((work_buffer[9]<<24)+ (work_buffer[10]<<16)+ (work_buffer[11]<<8)+ work_buffer[12])/1000*KTI*KTU);
        StringGrid2->Cells[1][1]= FloatToStr(float((work_buffer[13]<<24)+ (work_buffer[14]<<16)+ (work_buffer[15]<<8)+ work_buffer[16])/1000*KTI*KTU);
        StringGrid2->Cells[2][1]= FloatToStr(float((work_buffer[17]<<24)+ (work_buffer[18]<<16)+ (work_buffer[19]<<8)+ work_buffer[20])/1000*KTI*KTU);
        StringGrid2->Cells[3][1]= FloatToStr(float((work_buffer[21]<<24)+ (work_buffer[22]<<16)+ (work_buffer[23]<<8)+ work_buffer[24])/1000*KTI*KTU);
        StringGrid2->Cells[4][1]= FloatToStr(float((work_buffer[25]<<24)+ (work_buffer[26]<<16)+ (work_buffer[27]<<8)+ work_buffer[28])/1000*KTI*KTU);
        StringGrid2->Cells[0][1+Count_Day]= IntPlusZero(work_buffer[33])+ "-" + IntPlusZero(work_buffer[34])+ "-20" + IntPlusZero(work_buffer[35])+ " " + IntPlusZero(work_buffer[31]) + ":" +IntPlusZero(work_buffer[30])+ ":" + IntPlusZero(work_buffer[29]);
       }
       else
       {
        if(IDR==5)
        {
         //ShowMessage("������� ��������");
         if (CheckBox1->Checked==false)  KTU=KTI=1;
         if(work_buffer[7]<=47 && work_buffer[10]<=47 && work_buffer[13]<=47 && work_buffer[16]<=47 && work_buffer[19]<=47)
         {
          StringGrid1->Cells[5][1]= Intervals[work_buffer[7]] + " - " + FloatToStr(float((work_buffer[5]<<8)+ (work_buffer[6]))/1000*KTI*KTU);
          StringGrid1->Cells[1][1]= Intervals[work_buffer[10]] + " - " + FloatToStr(float((work_buffer[8]<<8)+ (work_buffer[9]))/1000*KTI*KTU);
          StringGrid1->Cells[2][1]= Intervals[work_buffer[13]] + " - " + FloatToStr(float((work_buffer[11]<<8)+ (work_buffer[12]))/1000*KTI*KTU);
          StringGrid1->Cells[3][1]= Intervals[work_buffer[16]] + " - " + FloatToStr(float((work_buffer[14]<<8)+ (work_buffer[15]))/1000*KTI*KTU);
          StringGrid1->Cells[4][1]= Intervals[work_buffer[19]] + " - " + FloatToStr(float((work_buffer[17]<<8)+ (work_buffer[18]))/1000*KTI*KTU);
          StringGrid1->Cells[0][1]= Now();
         }
         else
         {
          ShowMessage("�������� �������� �������!");
          return false;
         }
        }
        else
        {
         ShowMessage("����������� �������������� ������������� ������ IDR = " + IntToStr(IDR));
         return false;
        }
       }
     break;
  }
   case 11:  //  Reading max power on begining days
      {
       if(IDR<90)
       {
        //ShowMessage("������� ��������");
        if (CheckBox1->Checked==false)  KTU=KTI=1;
        if(work_buffer[7]<=47 && work_buffer[10]<=47 && work_buffer[13]<=47 && work_buffer[16]<=47 && work_buffer[19]<=47)
        {
         StringGrid1->Cells[5][1+Count_Day]= Intervals[work_buffer[7]] + " - " + FloatToStr(float((work_buffer[5]<<8)+ (work_buffer[6]))/1000*KTI*KTU);
         StringGrid1->Cells[1][1+Count_Day]= Intervals[work_buffer[10]] + " - " + FloatToStr(float((work_buffer[8]<<8)+ (work_buffer[9]))/1000*KTI*KTU);
         StringGrid1->Cells[2][1+Count_Day]= Intervals[work_buffer[13]] + " - " + FloatToStr(float((work_buffer[11]<<8)+ (work_buffer[12]))/1000*KTI*KTU);
         StringGrid1->Cells[3][1+Count_Day]= Intervals[work_buffer[16]] + " - " + FloatToStr(float((work_buffer[14]<<8)+ (work_buffer[15]))/1000*KTI*KTU);
         StringGrid1->Cells[4][1+Count_Day]= Intervals[work_buffer[19]] + " - " + FloatToStr(float((work_buffer[17]<<8)+ (work_buffer[18]))/1000*KTI*KTU);
         StringGrid1->Cells[0][1+Count_Day]= IntPlusZero(work_buffer[24])+ "-" + IntPlusZero(work_buffer[25])+ "-20" + IntPlusZero(work_buffer[26])+ " " + IntPlusZero(work_buffer[22]) + ":" +IntPlusZero(work_buffer[21])+ ":" + IntPlusZero(work_buffer[20]);
         Count_Day++;
         Packet_received=true;
        }
        else
        {
        // ShowMessage("�������� �������� �������! - "+ IntToStr(work_buffer[7]));
         StatusBar1->SimpleText= "�������� �������� �������! - "+ IntToStr(work_buffer[7]);
         return false;
        }
       }
       else
       {
        ShowMessage("����������� �������������� ������������� ������ IDR = " + IntToStr(IDR));
        return false;
       }

     break;
  }
  case 13:  //  Reading max power on begining months
      {
       if(IDR<23)
       {
        //ShowMessage("������� ��������");
        if (CheckBox1->Checked==false)  KTU=KTI=1;
        if(work_buffer[7]<=47 && work_buffer[10]<=47 && work_buffer[13]<=47 && work_buffer[16]<=47 && work_buffer[19]<=47)
        {
         StringGrid1->Cells[5][1+Count_Month]= Intervals[work_buffer[7]] + " - " + FloatToStr(float((work_buffer[5]<<8)+ (work_buffer[6]))/1000*KTI*KTU);
         StringGrid1->Cells[1][1+Count_Month]= Intervals[work_buffer[10]] + " - " + FloatToStr(float((work_buffer[8]<<8)+ (work_buffer[9]))/1000*KTI*KTU);
         StringGrid1->Cells[2][1+Count_Month]= Intervals[work_buffer[13]] + " - " + FloatToStr(float((work_buffer[11]<<8)+ (work_buffer[12]))/1000*KTI*KTU);
         StringGrid1->Cells[3][1+Count_Month]= Intervals[work_buffer[16]] + " - " + FloatToStr(float((work_buffer[14]<<8)+ (work_buffer[15]))/1000*KTI*KTU);
         StringGrid1->Cells[4][1+Count_Month]= Intervals[work_buffer[19]] + " - " + FloatToStr(float((work_buffer[17]<<8)+ (work_buffer[18]))/1000*KTI*KTU);
         StringGrid1->Cells[0][1+Count_Month]= IntPlusZero(work_buffer[25])+ "-20" + IntPlusZero(work_buffer[26]);
         Count_Month++;
         Packet_received=true;
        }
        else
        {
        // ShowMessage("�������� �������� �������! - "+ IntToStr(work_buffer[7]));
         StatusBar1->SimpleText= "�������� �������� �������! - "+ IntToStr(work_buffer[7]);
         return false;
        }
       }
       else
       {
        ShowMessage("����������� �������������� ������������� ������ IDR = " + IntToStr(IDR));
        return false;
       }

     break;
  }

  default:
  {
   ShowMessage("����������� ������������� ������ IDP = "+IntToStr(IDP));
   return false;
  }
 }


   return true;
}
bool __fastcall TForm1::ReadSysPar()
{

 AnsiString InfoMeters="";
 AnsiString Ver_Protocol="";
 AnsiString Ver_Soft="";
 AnsiString Release_Date="";

// unsigned int Meter_Status=0;

 //************ Read Type of meters *********************************************
 if(work_buffer[5]=='C') // read character ID
 InfoMeters+="���-";
 else
 InfoMeters+="Unknow Type ";

 InfoMeters+=  IntToStr((work_buffer[6]<<8) + work_buffer[7]); //read number index
 Short_info= InfoMeters;
 InfoMeters+="      ����� ��������: " ;

 InfoMeters+=  FloatToStr(float(work_buffer[8])/100) ; //read accuracy class

 InfoMeters+= "     (";
 InfoMeters+= IntToStr(work_buffer[9]); //read nominal current

 Short_info+= "     (" + IntToStr(work_buffer[9])+  " - " +IntToStr(work_buffer[10] ) + ") A ";


 InfoMeters+= " - ";
 InfoMeters+= IntToStr(work_buffer[10]); //read max current
 InfoMeters+= ") A ";
 //read service function

/*  InfoMeters+= "  ��������� ������: ";
 if (work_buffer[11]&BIT7) InfoMeters+="A";
 else InfoMeters+=" ";
 if (work_buffer[11]&BIT6) InfoMeters+="R";
 else InfoMeters+=" ";
 InfoMeters+=" ";
 if (work_buffer[11]&BIT5) InfoMeters+="F";
 else InfoMeters+=" ";
 InfoMeters+=" ";
 if (work_buffer[11]&BIT4) InfoMeters+="R";
 else InfoMeters+=" ";
 if (work_buffer[11]&BIT3) InfoMeters+="J";
 else InfoMeters+=" ";
 InfoMeters+=" ";
 if (work_buffer[11]&BIT2) InfoMeters+="M";
 else InfoMeters+=" ";   */

 //ValueListEditor1->Cells[1][1]= InfoMeters ;

//****** read factory number ***************************************************

 Factory_Number=(work_buffer[13]<<24)+(work_buffer[14]<<16)+(work_buffer[15]<<8)+work_buffer[16];
// ValueListEditor1->Cells[1][2]= Factory_Number ;
  //ShowMessage("������� ��������� �: " +  IntToStr(Factory_Number) + "  "+ InfoMeters );
    Label1->Caption= "��������� �������  �: " +  IntToStr(Factory_Number) + "  "+ InfoMeters;
//****** read version protocol ***************************************************
/*
 Ver_Protocol=  IntToStr(work_buffer[17]) + "." + IntToStr(work_buffer[18]) ;
 ValueListEditor1->Cells[1][3]= Ver_Protocol;

 //****** read version soft ***************************************************

 Ver_Soft=  IntToStr(work_buffer[19]) + "." + IntToStr(work_buffer[20]) ;
 ValueListEditor1->Cells[1][4]= Ver_Soft;

//****** read release date  ***************************************************

  Release_Date = IntToStr(work_buffer[21]) + "." + IntToStr(work_buffer[22]) + ".20" + IntToStr(work_buffer[23]) ;
  ValueListEditor1->Cells[1][5]= Release_Date;

//****** Count resets  ****************************************************

   ValueListEditor1->Cells[1][6]= IntToStr(work_buffer[61]);

//****** Count WDT ****************************************************

   ValueListEditor1->Cells[1][7]= IntToStr(work_buffer[63]);

//****** Count Errors ****************************************************

   ValueListEditor1->Cells[1][8]= IntToStr(work_buffer[66]);

//******  Battery Voltage****************************************************

   ValueListEditor1->Cells[1][9]= FloatToStr(float(work_buffer[69])/10);

//******  Working time in hours****************************************************

   ValueListEditor1->Cells[1][10]= IntToStr(((work_buffer[74]<<24)+(work_buffer[73]<<16)+(work_buffer[72]<<8)+work_buffer[71]));

//****** The  current meter status ****************************************************
//(byte[0]<<24)+(byte[1]<<16)+byte[2]<<8)+byte[3];
    Meter_Status = (work_buffer[48]<<24) + (work_buffer[49]<<16) + (work_buffer[50]<<8) + work_buffer[51];///

       */
return true;
}

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
Timer1->Enabled=false;
Button2->Caption="����������";
ShowMessage("C������ �� ��������!  ��������� ��������� ����� � ����������� � ��������!");

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
 Button1->Enabled=false;
 Form1->StringGrid2->Cells[0][0]= "����/�����";
 Form1->StringGrid2->Cells[1][0]= "������� ���1, ���/�";
 Form1->StringGrid2->Cells[2][0]= "������� ���2, ���/�";
 Form1->StringGrid2->Cells[3][0]= "������� ���3, ���/�";
 Form1->StringGrid2->Cells[4][0]= "������� ���4, ���/�" ;
 Form1->StringGrid2->Cells[5][0]= "������� ���������, ���/� ";

 Form1->StringGrid1->Cells[0][0]= "����/�����";
 Form1->StringGrid1->Cells[1][0]= "�����/�������� �1, ���";
 Form1->StringGrid1->Cells[2][0]= "�����/�������� �2, ���";
 Form1->StringGrid1->Cells[3][0]= "�����/�������� �3, ���";
 Form1->StringGrid1->Cells[4][0]= "�����/�������� �4, ���" ;
 Form1->StringGrid1->Cells[5][0]= "�����/�������� �����, ��� ";

 ComboBox1->ItemIndex=0;
 ComboBox2->ItemIndex=0;


}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
Count_Day=Count_Month=Count_Year=0;
 // ������� Stringrid2
 for (int i = 0; i < StringGrid2->ColCount; i++)
 for (int j = 1; j < StringGrid2->RowCount; j++)
 StringGrid2->Cells[i][j] = "";
  // ������� Stringrid2
 for (int i = 0; i < StringGrid1->ColCount; i++)
 for (int j = 1; j < StringGrid1->RowCount; j++)
 StringGrid1->Cells[i][j] = "";

// StringGrid1->Row=1  ;

 if(PageControl1->ActivePage==TabSheet1)
 switch(ComboBox1->ItemIndex)
 {
  case 0:  //������ ���������� �������
  {
   StatusBar1->SimpleText="������ ������� ��������� �������";
   SendData(0x08,0x01,0x01);
   break;
  }
  case 1:  //������ �������  �� ������ �����;
  {
   SendData(0x02,Count_Day,0x01);
   StatusBar1->SimpleText="������ ��������� ������� �� ������ �����" ;
   break;
  }
  case 2:  //������ �������  �� ������ ������;
  {
   SendData(0x04,Count_Month,0x01);
   StatusBar1->SimpleText="������ ��������� ������� �� ������ ������" ;
   break;
  }
  case 3:  //������ �������  �� ������ ����;
  {
   SendData(0x06,Count_Year,0x01);
   StatusBar1->SimpleText="������ ��������� ������� �� ������ ����" ;
   break;
  }
  default:
  {
   ShowMessage("����������� ������ �������: " + IntToStr(ComboBox1->ItemIndex));
  }
 }

 if(PageControl1->ActivePage==TabSheet2)
 switch(ComboBox2->ItemIndex)
 {
  case 0:  //������ ��������� ��������  �� ������� �����
  {
   StatusBar1->SimpleText="������ �������� �������� �� ������� �����";
   SendData(0x08,0x05,0x01);
   break;
  }
 case 1:  //������ ��������� �������� �� ������ �����;
  {
   SendData(0x0A,Count_Day,0x01);
   StatusBar1->SimpleText="������ �������� �������� �� ������ �����" ;
   break;
  }
   case 2:  //������ �������  �� ������ ������;
  {
   SendData(0x0C,Count_Month,0x01);
   StatusBar1->SimpleText="������ �������� �������� �� ������ ������" ;
   break;
  }

  default:
  {
   ShowMessage("����������� ������ ��������: " + IntToStr(ComboBox2->ItemIndex));
  }
 }


}
//---------------------------------------------------------------------------




String TForm1::IntPlusZero(unsigned char buf)
{
 String s;
 s=IntToStr(buf);
 if (buf<10)
 s="0"+s;
 return s;
}
void __fastcall TForm1::Button3Click(TObject *Sender)
{

  // ������������� ���� � ����� �������
     AnsiString sFile = ExtractFilePath(Application->ExeName)+"\\ACT.xlt";
     AnsiString  datenumber="1";
     // �������������� Excel, ��������� ���� ������
     try {
       App=Variant::GetActiveObject("Excel.Application");
     } catch(...) {
     try { App=Variant::CreateObject("Excel.Application"); }
     catch (...) {
      Application->MessageBox("���������� ������� Microsoft Excel!"
      " �������� ����� ���������� �� ���������� �� �����������.","������",MB_OK+MB_ICONERROR);
        }}
     try {
      App.OlePropertyGet("WorkBooks").OleProcedure("Open",sFile.c_str(),0,false,1);
      Sh=App.OlePropertyGet("WorkSheets",1);
     } catch(...) {
      Application->MessageBox("������ �������� ������� Microsoft Excel!","������",MB_OK+MB_ICONERROR);
     }
    App.OlePropertySet("DisplayAlerts",true) ;

   if(PageControl1->ActivePage==TabSheet1)
   datenumber=Now().FormatString("dd.mm.yy - ") + IntToStr(Factory_Number)+ "_" + IntToStr(ComboBox1->ItemIndex);
   else
   {
   if(PageControl1->ActivePage==TabSheet2)
   datenumber=Now().FormatString("dd.mm.yy - ") + IntToStr(Factory_Number)+ "_" + IntToStr(ComboBox2->ItemIndex+4);
   else
   datenumber=Now().FormatString("dd.mm.yy - ") + IntToStr(Factory_Number);
   }
   if (PageControl1->ActivePage==TabSheet1)
   toExcelCell(2,1,ComboBox1->Text);

   if (PageControl1->ActivePage==TabSheet2)
   {
    toExcelCell(2,1,"�������� �������� " + ComboBox2->Text);
    toExcelCell(6,2,"������� - ��������� ���������� �������� ���");
   }

   toExcelCell(3,3,Short_info);
   toExcelCell(4,3,IntToStr(Factory_Number));

  for (int i=1;i<50;i++)
  for (int j=0;j<10;j++)
  {
  if (PageControl1->ActivePage==TabSheet1)
  toExcelCell(i+7,j+1,StringGrid2->Cells[j][i]);
  if (PageControl1->ActivePage==TabSheet2)
  toExcelCell(i+7,j+1,StringGrid1->Cells[j][i]);

  }// ����������� �������

App.OlePropertySet("Visible",true);
AnsiString  aaa= ExtractFilePath(Application->ExeName)+"������\\"+ datenumber +".xls";
  try {
App.OlePropertyGet("WorkSheets",1).OleProcedure("SaveAs",aaa.c_str());
   } catch(...) {
      //Application->MessageBox("�������� ���� Excel � ��������� �������","������2",MB_OK+MB_ICONERROR);
      App.OlePropertySet("DisplayAlerts",false);   //��������� ����� ��������� � ��������� ���� "�������� ����..."
      App.OleProcedure("Quit");    //close excel
     }
        Sh.Clear();
    App.Clear();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N5Click(TObject *Sender)
{
Form2->Visible=True;
}
//---------------------------------------------------------------------------

