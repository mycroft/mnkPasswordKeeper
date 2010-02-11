TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

ICON = mnkPasswordKeeper.icns
RC_FILE = mnkPasswordKeeper.rc
QT += xml

HEADERS += mnkEncryptionEngine.H \
 mnkFileFactory.H \
 mnkPasswordKeeperMainWindow.H \
 mnkPasswordMakerDialog.H \
 mnkPassword.H \
 mnkCodingTools.H \
 Whirlpool.H \
 Camellia.H \
 mnkCrypt.H \
 mnkPlatform.H \
 LineEdit.H
FORMS += mnkPasswordKeeper.ui mnkPasswordMaker.ui
TRANSLATIONS += mnkPasswordKeeper_french.ts
SOURCES += mnkPasswordKeeperMain.cxx \
 mnkEncryptionEngine.cxx \
 mnkFileFactory.cxx \
 mnkPasswordKeeperMainWindow.cxx \
 mnkPasswordMakerDialog.cxx \
 mnkPassword.cxx \
 mnkCodingTools.cxx \
 Whirlpool.cxx \
 Camellia.cxx \
 mnkCrypt.cxx \
 LineEdit.cxx
