Unicode true

!addplugindir F:\Source\nsLuaDuiSkin\Debug

; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines

!define PRODUCT_NAME "My application"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "My company, Inc."
!define PRODUCT_WEB_SITE "http://www.mycompany.com"

!define APP_32 "app.7z"
!define COMPRESSION_METHOD "7z"

RequestExecutionLevel user

; MUI 1.67 compatible ------
;!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"

Page custom customPage
;Page instfiles preShowInstall showInstall

; Welcome page
;!insertmacro MUI_PAGE_WELCOME
; Instfiles page
;!insertmacro MUI_PAGE_INSTFILES
; Finish page
;!insertmacro MUI_PAGE_FINISH

; Language files
;!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup.exe"
InstallDir "$AppData\nsLuaDuiSkinTest"

Function .onGUIInit
    ; move window off screen
FunctionEnd

Function .onInit
    nsLuaDuiSkin::myFunction
    nsLuaDuiSkin::Initialize "F:\Source\nsLuaDuiSkin\sample\skin\360Demo"
    nsLuaDuiSkin::AddSearchPath "F:\Source\nsLuaDuiSkin\sample\"

FunctionEnd

Function customPage
    GetFunctionAddress $0 extractUsing7za
    nsLuaDuiSkin::InstallFunction $0

    nsLuaDuiSkin::Require "main"
FunctionEnd

Function preShowInstall
  
FunctionEnd

Function showInstall
    
FunctionEnd

Function extractUsing7za
    MessageBox MB_OK "extractUsing7za"
    ;nsLuaDuiSkin::myFunction
    File /oname=$PLUGINSDIR\app-32.${COMPRESSION_METHOD} "${APP_32}"

    SetOutPath "$INSTDIR"
    ;File "/oname=${UNINSTALL_FILENAME}" "${UNINSTALLER_OUT_FILE}"
    GetFunctionAddress $0 ExtractCallback
    Nsis7z::ExtractWithCallback "$PLUGINSDIR\app-32.${COMPRESSION_METHOD}" $0
    Delete "$PLUGINSDIR\app-32.${COMPRESSION_METHOD}"

FunctionEnd

Function ExtractCallback
    ;MessageBox MB_OK "ExtractCallback"
    Pop $1
    Pop $2
    nsLuaDuiSkin::ProgressCallBack $1 $2

FunctionEnd

Section "MainSection" SEC01
    ;File "main.lua"
SectionEnd