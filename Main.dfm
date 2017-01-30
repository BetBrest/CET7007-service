object Form1: TForm1
  Left = 209
  Top = 114
  Width = 937
  Height = 716
  Caption = #1063#1090#1077#1085#1080#1077' '#1072#1088#1093#1080#1074#1072' '#1057#1069#1058'7007 '#1095#1077#1088#1077#1079' '#1086#1087#1090#1086#1087#1086#1088#1090
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 8
    Width = 5
    Height = 16
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object PageControl1: TPageControl
    Left = 16
    Top = 40
    Width = 905
    Height = 601
    ActivePage = TabSheet1
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #1040#1088#1093#1080#1074' '#1101#1085#1077#1088#1075#1080#1080
      DesignSize = (
        897
        573)
      object ComboBox1: TComboBox
        Left = 8
        Top = 8
        Width = 145
        Height = 21
        AutoComplete = False
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        Items.Strings = (
          #1058#1077#1082#1091#1097#1080#1077' '#1087#1086#1082#1072#1079#1072#1085#1080#1103
          #1053#1072' '#1085#1072#1095#1072#1083#1086' '#1089#1091#1090#1086#1082
          #1053#1072' '#1085#1072#1095#1072#1083#1086' '#1084#1077#1089#1103#1094#1072
          #1053#1072' '#1085#1072#1095#1072#1083#1086' '#1075#1086#1076#1072)
      end
      object StringGrid2: TStringGrid
        Left = 16
        Top = 48
        Width = 865
        Height = 505
        Anchors = [akLeft, akTop, akBottom]
        BiDiMode = bdLeftToRight
        ColCount = 6
        FixedCols = 0
        RowCount = 97
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentBiDiMode = False
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 1
        ColWidths = (
          108
          137
          148
          141
          133
          168)
      end
      object CheckBox1: TCheckBox
        Left = 232
        Top = 8
        Width = 297
        Height = 17
        Caption = #1059#1095#1080#1090#1099#1074#1072#1090#1100' '#1074#1074#1077#1076#1077#1085#1085#1099#1077' '#1082#1086#1101#1092#1092#1080#1094#1080#1077#1085#1090#1099' '#1090#1088#1072#1085#1089#1092#1086#1088#1084#1072#1094#1080#1080
        TabOrder = 2
      end
    end
    object TabSheet2: TTabSheet
      Caption = #1040#1088#1093#1080#1074' '#1084#1086#1097#1085#1086#1089#1090#1080
      ImageIndex = 1
      object ComboBox2: TComboBox
        Left = 8
        Top = 8
        Width = 145
        Height = 21
        ItemHeight = 13
        TabOrder = 0
        Text = #1053#1072' '#1085#1072#1095#1072#1083#1086' '#1089#1091#1090#1086#1082
        Items.Strings = (
          #1053#1072' '#1085#1072#1095#1072#1083#1086' '#1089#1091#1090#1086#1082
          #1053#1072' '#1085#1072#1095#1072#1083#1086' '#1084#1077#1089#1103#1094#1072
          #1085#1072' '#1085#1072#1095#1072#1083#1086' '#1075#1086#1076#1072)
      end
      object StringGrid1: TStringGrid
        Left = 24
        Top = 56
        Width = 849
        Height = 297
        TabOrder = 1
      end
      object Button3: TButton
        Left = 768
        Top = 16
        Width = 97
        Height = 25
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1072#1088#1093#1080#1074
        TabOrder = 2
      end
    end
  end
  object Button1: TButton
    Left = 689
    Top = 82
    Width = 91
    Height = 25
    Caption = #1057#1095#1080#1090#1072#1090#1100' '#1072#1088#1093#1080#1074
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 824
    Top = 8
    Width = 75
    Height = 25
    Caption = #1055#1086#1076#1082#1083#1102#1095#1080#1090#1100
    TabOrder = 2
    OnClick = Button2Click
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 643
    Width = 929
    Height = 19
    Panels = <>
    SimplePanel = False
  end
  object MainMenu1: TMainMenu
    Left = 736
    object N3: TMenuItem
      Caption = #1060#1072#1081#1083
      object N4: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = N4Click
      end
    end
    object N1: TMenuItem
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '
      object N2: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1087#1086#1088#1090#1072
        OnClick = N2Click
      end
    end
  end
  object ComPort1: TComPort
    BaudRate = br9600
    Port = 'COM1'
    Parity.Bits = prNone
    StopBits = sbOneStopBit
    DataBits = dbEight
    Events = [evRxChar, evTxEmpty, evRxFlag, evRing, evBreak, evCTS, evDSR, evError, evRLSD, evRx80Full]
    FlowControl.OutCTSFlow = False
    FlowControl.OutDSRFlow = False
    FlowControl.ControlDTR = dtrDisable
    FlowControl.ControlRTS = rtsDisable
    FlowControl.XonXoffOut = False
    FlowControl.XonXoffIn = False
    StoredProps = [spBasic]
    TriggersOnRxChar = True
    OnRxChar = ComPort1RxChar
    Left = 776
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 680
    Top = 8
  end
end
