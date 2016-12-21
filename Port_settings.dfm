object Form2: TForm2
  Left = 432
  Top = 263
  BorderStyle = bsSingle
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1089#1074#1103#1079#1080
  ClientHeight = 281
  ClientWidth = 585
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 116
    Height = 16
    Caption = #1057#1077#1090#1077#1074#1086#1081' '#1072#1076#1088#1077#1089':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 319
    Top = 5
    Width = 164
    Height = 32
    Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1087#1086#1074#1090#1086#1088#1086#1074#13' '#1087#1088#1080' '#1086#1096#1080#1073#1082#1077' :'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 40
    Width = 585
    Height = 241
    ActivePage = TabSheet1
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #1054#1087#1090#1086#1087#1086#1088#1090
      object Button1: TButton
        Left = 40
        Top = 24
        Width = 75
        Height = 25
        Caption = 'Button1'
        TabOrder = 0
        OnClick = Button1Click
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'RS-485'
      ImageIndex = 1
    end
    object TabSheet3: TTabSheet
      Caption = 'PLC'
      ImageIndex = 2
    end
  end
  object Edit1: TEdit
    Left = 139
    Top = 15
    Width = 121
    Height = 21
    TabOrder = 1
    Text = '1'
  end
  object Edit2: TEdit
    Left = 496
    Top = 8
    Width = 65
    Height = 21
    TabOrder = 2
    Text = '3'
  end
end
