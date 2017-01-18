//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//#include "Port_Settings.h"
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


bool Meter_connect=false;


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


void __fastcall TForm1::N2Click(TObject *Sender)
{
//Form2->Visible=true;
     dir = GetCurrentDir();
     ComPort1->LoadSettings(stIniFile, dir + "\\PortSettings.ini");
     ComPort1->ShowSetupDialog();
     ComPort1->StoreSettings(stIniFile, dir + "\\PortSettings.ini");



}
//---------------------------------------------------------------------------





void __fastcall TForm1::Button2Click(TObject *Sender)
{
  if (Button2->Caption=="Подключить")
  {
   Button2->Caption="Отключить" ;
   dir = GetCurrentDir();
   ComPort1->LoadSettings(stIniFile, dir + "\\PortSettings.ini");
   ComPort1->Open();
   SendData(0x00,0x00,0x01);
   Form1->Timer1->Enabled=true;
  }
  else
  {
   ComPort1->Close();
   Button1->Enabled=false;
   Button2->Caption="Подключить";
   Label1->Caption="";
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
     ShowMessage("Переполнение буффера");
     new_paket = true;

     return;
     }

     ComPort1->Read(&work_buffer[read_byte], Count);
     read_byte+=Count;

      if  ( (read_byte == work_buffer[0])&&(read_byte>=7))
      {
      // ShowMessage("Пакет принят");
        if(DecodeInBuffer())
        {
        // ShowMessage("Пакет разобран");
        if(Meter_connect==true)
        Button1->Enabled=true;
        else
        Button1->Enabled=false;

          new_paket = true;
          read_byte=0;

         if (Packet_Send  )
         {
          if(flag_IDP==2)
          {
          Sleep(50);
      //    SendData(0x02,Packet_Send+int(days_between),GetCurrentNA());
          Packet_Send--;
        //  if (!Packet_Send)
       //   DataToGrid=true;
    //      ProgressBar1->Position= Day-Packet_Send;
          }

          if(flag_IDP==4)
           {
          Sleep(50);
       //   SendData(0x04,Packet_Send+Month_Offset,GetCurrentNA());
          Packet_Send--;
        // if (!Packet_Send)
       //   DataToGrid2=true;
        //  ProgressBar1->Position= months_between-Packet_Send;
          }

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
      //  ShowMessage("Пакет не разобран");
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
  ShowMessage("Для получения инорфмации выберите счетчик !");
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
   ShowMessage("Ошибка контрольной суммы!!!" + IntToStr(CRC16b(&work_buffer[0],work_buffer[0]-2)));
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
   ShowMessage("Не правильный сетевой адрес!!");
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
      ShowMessage("Неизвестный дополнительный идентефикатор пакета IDR = " + IntToStr(IDR));
      return false;
      }

     }

     break;
  }
 /* case 3:  // energy reading at the beginning of the day
  {
    if (IDR < 93)
    {
      if(!Energy_begining_day(IDR-int(days_between)))
      return false;
    }
    else
    {
     ShowMessage("Неизвестный дополнительный идентефикатор пакета!Больше 93" );
     return false;
    }
     break;
  }

  case 5:  // energy reading at the beginning of the day
  {
    if (IDR < 24 )
    {
      if(!Energy_begining_month(IDR-Month_Offset))
      return false;
    }
    else
    {
     ShowMessage("Неизвестный дополнительный идентефикатор пакета!Больше 24");
     return false;
    }
     break;
  }  */
  case 9:  //  Reading current energy savings
      {
      /* if(ReadSysPar())
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
      ShowMessage("Неизвестный дополнительный идентефикатор пакета IDR = " + IntToStr(IDR));
      return false;
      }

     }
         */
     break;
  }


  default:
  {
   ShowMessage("Неизвестный идентефикатор пакета IDP = "+IntToStr(IDP));
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
 unsigned int Factory_Number=0;
// unsigned int Meter_Status=0;

 //************ Read Type of meters *********************************************
 if(work_buffer[5]=='C') // read character ID
 InfoMeters+="СЭТ-";
 else
 InfoMeters+="Unknow Type ";

 InfoMeters+=  IntToStr((work_buffer[6]<<8) + work_buffer[7]); //read number index
 InfoMeters+="      Класс точности: " ;

 InfoMeters+=  FloatToStr(float(work_buffer[8])/100) ; //read accuracy class

 InfoMeters+= "     (";
 InfoMeters+= IntToStr(work_buffer[9]); //read nominal current

 InfoMeters+= " - ";
 InfoMeters+= IntToStr(work_buffer[10]); //read max current
 InfoMeters+= ") A ";
 //read service function

/*  InfoMeters+= "  Сервисные фунции: ";
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
  //ShowMessage("Счетчик подключен №: " +  IntToStr(Factory_Number) + "  "+ InfoMeters );
    Label1->Caption= "Подключен счетчик  №: " +  IntToStr(Factory_Number) + "  "+ InfoMeters;
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
Button2->Caption="Подключить";
ShowMessage("Cчетчик не отвечает!  Проверьте настройки порта и подключение к счетчику!");

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
 Button1->Enabled=false;
 Form1->StringGrid2->Cells[0][0]= "Дата/время";
 Form1->StringGrid2->Cells[1][0]= "Энергия поТ1";
 Form1->StringGrid2->Cells[2][0]= "Энергия поТ2";
 Form1->StringGrid2->Cells[3][0]= "Энергия поТ3";
 Form1->StringGrid2->Cells[4][0]= "Энергия поТ4" ;
 Form1->StringGrid2->Cells[5][0]= "Энергия по сумме тарифов";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
 //Чтение мгновенной энергии
 SendData(0x08,0x03,0x01);
}
//---------------------------------------------------------------------------


