object FormularzOpcjiPrezentacji: TFormularzOpcjiPrezentacji
  Left = 287
  Top = 149
  Width = 696
  Height = 480
  Caption = 'FormularzOpcjiPrezentacji'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 120
  TextHeight = 16
  object Label1: TLabel
    Left = 16
    Top = 24
    Width = 45
    Height = 16
    Caption = 'Interwa≥'
  end
  object Edit1: TEdit
    Left = 72
    Top = 20
    Width = 73
    Height = 24
    TabOrder = 0
    Text = '10'
  end
  object UpDown1: TUpDown
    Left = 145
    Top = 20
    Width = 19
    Height = 24
    Associate = Edit1
    Min = 1
    Max = 1000
    Increment = 10
    Position = 10
    TabOrder = 1
    Wrap = False
  end
  object CheckBox1: TCheckBox
    Left = 48
    Top = 88
    Width = 137
    Height = 17
    Caption = 'Widoczny napis'
    TabOrder = 2
  end
  object Button1: TButton
    Left = 592
    Top = 384
    Width = 75
    Height = 25
    Caption = 'Zatwierdü'
    TabOrder = 3
    OnClick = Button1Click
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 328
    Top = 144
  end
end
