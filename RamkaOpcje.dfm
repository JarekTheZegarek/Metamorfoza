object OknoOpcje: TOknoOpcje
  Left = 239
  Top = 103
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'W'#322'a'#347'ciwo'#347'ci'
  ClientHeight = 365
  ClientWidth = 536
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    536
    365)
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 7
    Top = 8
    Width = 537
    Height = 313
    ActivePage = Zapisywanie
    Anchors = []
    TabIndex = 0
    TabOrder = 0
    object Zapisywanie: TTabSheet
      Caption = 'Zapisywanie'
      object LiczDanychCheckBox: TCheckBox
        Left = 8
        Top = 8
        Width = 161
        Height = 17
        Hint = 
          'Je'#380'eli ta opcja jest ustawiona to zapisuje maksimum 2048 danych ' +
          'w plikach dat z kropk'#261
        Caption = 'Zredukowana Liczba Danych'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
      end
      object GroupAlternatywne: TGroupBox
        Left = 208
        Top = 0
        Width = 177
        Height = 121
        Caption = 'Separatory Zapisywanych Kolumn'
        TabOrder = 1
        object LabelAlternatywa: TLabel
          Left = 114
          Top = 64
          Width = 39
          Height = 19
          Caption = 'Inne:'
          Color = clBtnFace
          Font.Charset = EASTEUROPE_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
        end
        object SeparatoryKolumn: TRadioGroup
          Left = 8
          Top = 16
          Width = 89
          Height = 97
          ItemIndex = 0
          Items.Strings = (
            'Przecinek'
            'Tabulacja'
            'Spacja'
            'Inne znaki ')
          TabOrder = 0
          OnClick = SeparatoryKolumnClick
        end
        object EditInneZnaki: TEdit
          Left = 112
          Top = 88
          Width = 49
          Height = 21
          Color = clInactiveCaptionText
          Enabled = False
          MaxLength = 20
          TabOrder = 1
        end
      end
      object OpDodatDanych: TRadioGroup
        Left = 8
        Top = 40
        Width = 185
        Height = 105
        Hint = 'Opcje Interpretacji Liczb Zawartych w Nazwach Plik'#243'w'
        Caption = 'Odczyt Dodatkowych Danych'
        ItemIndex = 3
        Items.Strings = (
          'Zapis Temperatury [K]'
          'Zapis Temperatury [C]'
          'Zapis  K'#261't'#243'w (anizotropia)'
          'Inne ')
        TabOrder = 2
      end
      object Jednostki: TRadioGroup
        Left = 8
        Top = 152
        Width = 73
        Height = 73
        Caption = 'Jednostki'
        ItemIndex = 0
        Items.Strings = (
          'mT'
          'Gs')
        TabOrder = 3
      end
    end
    object Wykres: TTabSheet
      Caption = 'Wykres'
      ImageIndex = 1
      object Label1: TLabel
        Left = 85
        Top = 20
        Width = 116
        Height = 13
        Caption = 'Numer Pliku na wykresie'
      end
      object EditNumerPliku: TEdit
        Left = 8
        Top = 16
        Width = 57
        Height = 21
        TabOrder = 0
        Text = '1'
      end
      object UpDown1: TUpDown
        Left = 65
        Top = 16
        Width = 15
        Height = 21
        Associate = EditNumerPliku
        Min = 1
        Position = 1
        TabOrder = 1
        Wrap = True
      end
      object GroupBox1: TGroupBox
        Left = 8
        Top = 56
        Width = 113
        Height = 57
        Caption = 'Prezentacja danych'
        TabOrder = 2
        object PokazPunkty: TCheckBox
          Left = 16
          Top = 24
          Width = 97
          Height = 17
          Caption = 'Poka'#380' Punkty'
          TabOrder = 0
        end
      end
      object GroupBox2: TGroupBox
        Left = 136
        Top = 56
        Width = 129
        Height = 81
        Caption = 'Punkty Do'#347'wiadczalne'
        TabOrder = 3
        object SymbDoswiadczalny: TComboBox
          Left = 6
          Top = 18
          Width = 117
          Height = 21
          ItemHeight = 13
          TabOrder = 0
          Text = 'Ko'#322'o'
          Items.Strings = (
            'Ko'#322'o'
            'Kwadrat')
        end
        object RozmDoswiadczalny: TComboBox
          Left = 6
          Top = 50
          Width = 117
          Height = 21
          ItemHeight = 13
          TabOrder = 1
          Text = '2'
          Items.Strings = (
            '2'
            '4'
            '6'
            '8'
            '9'
            '10'
            '11'
            '12'
            '13'
            '14'
            '15'
            '16'
            '17'
            '18'
            '19'
            '20')
        end
      end
      object GroupBox3: TGroupBox
        Left = 280
        Top = 56
        Width = 129
        Height = 81
        Caption = 'Punkty Zaznaczone'
        TabOrder = 4
        object SymbZaznaczony: TComboBox
          Left = 6
          Top = 18
          Width = 117
          Height = 21
          ItemHeight = 13
          TabOrder = 0
          Text = 'Kwadrat'
          Items.Strings = (
            'Ko'#322'o'
            'Kwadrat')
        end
        object RozmZaznaczony: TComboBox
          Left = 6
          Top = 50
          Width = 117
          Height = 21
          ItemHeight = 13
          TabOrder = 1
          Text = '6'
          Items.Strings = (
            '4'
            '6'
            '8'
            '9'
            '10'
            '11'
            '12'
            '13'
            '14'
            '15'
            '16'
            '17'
            '18'
            '19'
            '20')
        end
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'Wykres Punkt'#243'w'
      ImageIndex = 2
      object Label2: TLabel
        Left = 96
        Top = 30
        Width = 103
        Height = 13
        Caption = 'Czu'#322'o'#347#263' Zaznaczania'
      end
      object Czulosc: TEdit
        Left = 19
        Top = 24
        Width = 38
        Height = 21
        TabOrder = 0
        Text = '100'
      end
      object UpDown2: TUpDown
        Left = 57
        Top = 24
        Width = 16
        Height = 21
        Associate = Czulosc
        Min = 5
        Position = 100
        TabOrder = 1
        Wrap = True
      end
    end
  end
  object ButtonOpcje_Ok: TButton
    Left = 336
    Top = 328
    Width = 65
    Height = 25
    Caption = '&Ok'
    Default = True
    TabOrder = 1
    OnClick = ButtonOpcje_OkClick
  end
  object ButtonAnuluj_Opcje: TButton
    Left = 408
    Top = 328
    Width = 51
    Height = 25
    Cancel = True
    Caption = '&Anuluj'
    TabOrder = 2
    OnClick = ButtonAnuluj_OpcjeClick
  end
  object ButtonPomoc_Opcje: TButton
    Left = 472
    Top = 328
    Width = 51
    Height = 25
    Hint = 'Opcja Narazie nieaktywna'
    Caption = 'Pomoc'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 3
    TabStop = False
    OnClick = ButtonPomoc_OpcjeClick
  end
end
