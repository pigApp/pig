#include "installer.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include <QDebug>

Installer::Installer(QWidget *parent) : QWidget(parent)
{
    setFixedSize(320, 180);
    setStyleSheet("color: black; background: white; border: none");

    layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);

    font.setFamily(":/resources/font/pig.ttf");
    font.setPixelSize(20);
    font.setBold(true);

    button = new QPushButton(this);
    button->setFont(font);
    button->setText("INSTALL PIG");
    layout->addWidget(button);

    status = new QLabel(this);
    status->setFont(font);
    status->hide();
    layout->addWidget(status);

    connect(button, SIGNAL(clicked()), this, SLOT(install()));
}

Installer::~Installer()
{
}

void Installer::install()
{
    const int arch = sizeof(void*);
    const QString path = QDir::currentPath();//
    QString originBinary;
    QString targetBinary;
    QString originLib;
    QString targetLib;
    QString originConfig;
    QString targetConfig;

    button->setEnabled(false);
    button->hide();
    status->setText("INSTALLING PIG");
    status->show();

#ifdef _WIN32
    QDir pig;
    pig.mkdir("C:/pig");
    originBinary = path+"/bin/"; TODO: Ver las barras en windows.
    targetBinary = "C:/pig/";
    originLib = path+"/lib/";
    targetLib = "C:/pig/lib";
    originConfig = path+"/.pig/";
    targetConfig = "C:/pig/.pig";
#else
    originBinary = path+"/bin/";
    targetBinary = "/usr/bin/";
    originLib = path+"/lib/";
    if (arch == 4)
        targetLib = "/usr/lib/pig";
    else
        targetLib = "/usr/lib64/pig";
    originConfig = path+"/.pig/";
    targetConfig = "/home/lxfb/.pig";//
#endif

    if (moveFiles(false, originBinary, targetBinary) &&
        moveFiles(true, originLib, targetLib) &&              // TODO: QDir::homePath() devuelve /root.
        moveFiles(true, originConfig, targetConfig))
        status->setText("INSTALLED");
        // TODO: Label 'Usar accesso direco para iniciar pig'
    else
        status->setText("INSTALL FAIL");
}

bool Installer::moveFiles(bool dir, QString origin, QString target)
{
    if (dir) {
        QDir targetDir;
        if (!targetDir.mkdir(target))
            return false;
    }

    QDir originDir(origin);
    foreach (QString files, originDir.entryList(QDir::Files))
        if (!QFile::copy(origin+QDir::separator()+files, target+QDir::separator()+files))
            return false;
    foreach (QString files, originDir.entryList(QDir::Files)) {
        QFile f(target+QDir::separator()+files);
        if (!f.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner))
            return false;
    }

    if (target.contains(".pig")) {
#ifdef _WIN32
        const QString targetInit = "C:/pig/.pig/.init";
        const QStringList desktop = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
        if(!QFile::link("C:/pig/pig.exe", desktop[0]+"/ pig.lnk"))
            return false;
#else
        const QString targetInit = "/home/lxfb/.pig/.init"; //
#endif
        QFile init(targetInit);
        init.open(QIODevice::WriteOnly);
        init.close();
        init.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner); // TODO: Cambiar propietario a User.
    }
    return true;
}
