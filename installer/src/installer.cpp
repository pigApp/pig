#include "installer.h"

#include <QFile>
#include <QDir>
#include <QDebug>

Installer::Installer(QWidget *parent) : QWidget(parent)
{
    setFixedSize(320, 180);
    setStyleSheet("color: black; background: white; border: none");

    layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);

    font.setFamily(":/font/pig.ttf");
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

    connect(button, SIGNAL(clicked()), this, SLOT(get()));
}

Installer::~Installer()
{
}

void Installer::get()
{
    QString host = "dl.bintray.com";
    QString url;
    path = QDir::tempPath()+"/pig";

    const int byte = sizeof(void*);
    if (byte == 4) {
        arch = "32";
    } else {
        arch = "64";
    }

#ifdef _WIN32
    path = "C:/tmp/pig";
    if (arch == "32") {
        url = "windows32-url"; //
    } else {
        url = "windows64-url"; //
    }
#else
    if (arch == "32") {
        url = "linusx86-url"; //
    } else {
        url = "/test%20bintray/testApp/pig?direct";
    }
#endif

    button->setEnabled(false);
    button->hide();

    status->setText("RECOVERING PIG");
    status->show();

    mSocket = new TcpSocket();
    mSocket->host = host;
    mSocket->url = url;
    mSocket->path = path;
    mSocket->doConnect();

    connect (mSocket, SIGNAL(file_ready(const QString)), this, SLOT(check(const QString)));
    connect (mSocket, SIGNAL(error_socket()), this, SLOT(error()));
}

void Installer::check(const QString path)
{
    // TODO: Checkear suma MD5.
    install(path);
}

void Installer::install(const QString path)
{
    status->setText("INSTALLING PIG");

    bool installed;
#ifdef _WIN32
    // TODO: COPIAR pig A C:
    // TODO: Link.
#else
    if (moveFiles(false, path+"/bin/", "/usr/bin/") &&
        moveFiles(true, path+"/.pig/", "/home/lxfb/.pig")) { // TODO: QDir::homePath() devuelve /root.
        if (arch == "32") {
            if (moveFiles(true, path+"/lib/", "/usr/lib/pig"))
                installed = true;
        } else {
            if (moveFiles(true, path+"/lib/", "/usr/lib64/pig"))
                installed = true;
        }
        if (installed) {
            QFile init("/home/lxfb/.pig/.init");
            init.open(QIODevice::WriteOnly);
            init.close();

            // TODO: Permisos.

            status->setText("INSTALLED");
        } else {
            status->setText("INSTALL FAIL");
        }
    } else {
        status->setText("INSTALL FAIL");
    }
#endif
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
    return true;
}

void Installer::error()
{
    qDebug() << "ERROR";
}
