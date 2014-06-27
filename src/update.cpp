#include "update.h"

#include <QCryptographicHash>
#ifdef _WIN32
    #include <windows.h>
#endif

Update::Update(QObject *parent) : QObject(parent)
{
}

Update::~Update()
{
    delete _root;
}

void Update::doCheck()
{
    _root->setProperty("status", "SEEKING UPDATE");
    _root->setProperty("showSpinner", true);

    if (db.open()) {
        QSqlQuery qry;
        qry.prepare("SELECT DatabaseVersion, BinaryVersion, Release, Host, Url FROM PigData");
        if (!qry.exec()) {
            db.close();
            emit updateErrorDb();
        } else {
            qry.next();
            currentDatabaseVersion = qry.value(0).toInt();
            currentBinaryVersion = qry.value(1).toInt();
            currentRelease = qry.value(2).toInt();
            QString hosts = qry.value(3).toString();
            QString urls = qry.value(4).toString();
            QStringList hostList = hosts.split(",");
            QStringList urlList = urls.split(",");
#ifdef _WIN32
    QString host = hostList[1];
    QString url = urlList[1];
#else
    QString host = hostList[0];
    QString url = urlList[0];
#endif
            db.close();
            getVersion(host, url);
        }
    } else {
        emit updateErrorDb();
    }
}

void Update::getVersion(QString host, QString url)
{
    mSocket.host = host;
    mSocket.url = url;
    mSocket.file = '.';
    mSocket.order = "getUpdateVersion";
    mSocket.doConnect();

    connect(&mSocket, SIGNAL(versionReady(QString)), this, SLOT(evaluate(QString)));
}

void Update::evaluate(QString version)
{
    QStringList last = version.split(",");

    if (last[3].toInt() > currentDatabaseVersion) {
        databaseHash = last[5];
        newDatabaseAvailable = true;
    }
    if (last[6].toInt()+last[9].toInt() > currentBinaryVersion+currentRelease) {
        binaryHash = last[8];
        newBinaryAvailable = true;
        currentBinaryVersion = last[6].toInt();
        currentRelease = last[9].toInt();
    }

    //newDatabaseAvailable = true;//tmp

    if (newDatabaseAvailable || newBinaryAvailable) {
        newsAvailable = true;
        _root->setProperty("status", "UPDATE AVAILABLE");
        _root->setProperty("showSpinner", false);
        _root->setProperty("requireAccept", true);
#ifdef _WIN32
    hostFiles = last[0];
    databaseUrl = last[3];
    binaryUrl = last[6];
    newsUrl = last[1];
    newsHash = last [2];
#else
    //hostFiles = last[0];
    hostFiles = "dl.shared.com";//tmp
    databaseUrl = last[3];
    binaryUrl = last[6];
    newsUrl = last[1];
    newsHash = last[2];
#endif
    } else {
        _root->setProperty("status", "");
        _root->setProperty("showSpinner", false);
        emit updateCallFinder();
    }
}

void Update::getFiles()
{
    qDebug() << "UPDATE AVAILABLE";
    _root->setProperty("status", "GETTING UPDATE");
    _root->setProperty("showSpinner", true);
    _root->setProperty("requireAccept", false);

    if (newsAvailable) {
        mSocket.host = hostFiles;
        mSocket.url = newsUrl;
        mSocket.file = "news.txt";
        mSocket.order = "getUpdateFiles";
        mSocket.doConnect();
    } else if (newDatabaseAvailable) {
        mSocket.host = hostFiles;
        mSocket.url = databaseUrl;
        mSocket.file = "db.sqlite";
        mSocket.order = "getUpdateFiles";
        mSocket.doConnect();
    } else {
        mSocket.host = hostFiles;
        mSocket.url = binaryUrl;
#ifdef _WIN32
    mSocket.file = "pig.exe";
#else
    mSocket.file = "pig";
#endif
        mSocket.order = "getUpdateFiles";
        mSocket.doConnect();
    }

    connect(&mSocket, SIGNAL(fileReady(QString, QString)), this, SLOT(integrityFile(QString, QString)));
}

void Update::integrityFile(QString path, QString file)
{
    QFile newFile(path+file);
    newFile.open(QIODevice::ReadOnly);
    QByteArray fileHash = QCryptographicHash::hash(newFile.readAll(), QCryptographicHash::Md5);
    newFile.close();

    if (file == "news.txt") {
        if (fileHash == newsHash)
            replace(path, file);
        else
           emit updateFail();
    } if (file == "db.sqlite") {
        if (fileHash == databaseHash)
            replace(path, file);
        else
            emit updateFail();
    } else {
        if (fileHash == binaryHash)
            replace(path, file);
        else
            emit updateFail();
    }
}

void Update::replace(QString path, QString file)
{
    if (newBinaryAvailable && !newDatabaseAvailable && !newsAvailable) {
        binaryAbsolutePath = path+file;
#ifdef _WIN32
    _root->setProperty("os", "windows");
#else
    _root->setProperty("os", "unix");
#endif
        _root->setProperty("requireRestart", true);
    } else if (newDatabaseAvailable && !newsAvailable) { //Hecho hasta aca. falta la parte de reemplazar el binario.
#ifdef _WIN32
    QString target = "C:/PIG/.pig/db.sqlite";
#else
    QString target = QDir::homePath()+"/.pig/db.sqliteX"; //X tmp
#endif
           QFile::copy(path+file, target);
           newDatabaseAvailable = false;
           databaseUpdated = true;
           if (newBinaryAvailable) {
               getFiles();
           } else {
               _root->setProperty("status", "");
               _root->setProperty("showSpinner", false);
               emit updateCallFinder();
           }
    } else if (newsAvailable) {
#ifdef _WIN32
    QString target = "C:/PIG/.pig/news.txt";
#else
    QString target = QDir::homePath()+"/.pig/news";
#endif
        QFile::copy(path+file, target);
        newsAvailable = false;
        getFiles();
    }
}

void Update::replaceBinaryAndRestart()
{
#ifdef _WIN32
    QString target = "C:/PIG/bin/pig.exe";
    QString updater = "C:/bin/updater.exe";
    Qstring updaterAguments = " "+binaryAbsolutePath+" "+target
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess((TCHAR*)(updater.utf16()), (TCHAR*)(updaterArguments.utf16()), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        _root->setProperty("status", "PERMISSION DENIED");
        _root->setProperty("statusInformation", "RESTART THE APPLICATION WITH ADMINITRATOR RIGHTS");
    } else {
        replaceBinaryReady();
    }
#else
    QString userName = getenv("USER");
    QString updaterArguments = "gksu pigUpdater "+userName+" &";
    runUpdater = new QProcess(this);
    connect(runUpdater, SIGNAL(started()), this, SLOT(replaceBinaryReady()));
    runUpdater->start("/bin/bash", QStringList() << "-c" << updaterArguments);
#endif
}

void Update::replaceBinaryReady()
{
#ifdef _WIN32
    if (!databaseUpdated) {
        if (db.open()) {
            QSqlQuery qry;
            qry.prepare("UPDATE PigData SET BinaryVersion='"+QString::number(currentBinaryVersion)+"'");
            qry.prepare("UPDATE PigData SET Release='"+QString::number(currentRelease)+"'");
            qry.exec();
            db.close();
        }
    }
    exit(0);
#else
    if (runUpdater->state()==QProcess::Running && !runUpdater->error()==QProcess::FailedToStart) {
        if (!databaseUpdated) {
            if (db.open()) {
                QSqlQuery qry;
                qry.prepare("UPDATE PigData SET BinaryVersion='"+QString::number(currentBinaryVersion)+"'");
                qry.prepare("UPDATE PigData SET Release='"+QString::number(currentRelease)+"'");
                qry.exec();
                db.close();
            }
        }
        exit(0);
    } else {
        _root->setProperty("status", "UPDATE FAILED");
        _root->setProperty("statusInformation", "TRY LATER");
        emit updateCallFinder();
    }
#endif
}

//host,newsUrl,hash,dbVersion,url,hash,binVersion,url,hash,release,

//1,https://dl.shared.com/g8cj8cnsxk?s=ld,c19e7dbafca6f26c5bafec07907df361,1,https://dl.shared.com/m9bspu79nd?s=ld,e2462c1f38063a8b14ce102b9a6722e6,1,https://dl.shared.com/gufqgd56p9?s=ld,2fa1055185197a3a6fd10532401ecbea,

//https://dl.shared.com,/g8cj8cnsxk?s=ld,c19e7dbafca6f26c5bafec07907df361,1,/g8cj8cnsxk?s=ld,c19e7dbafca6f26c5bafec07907df361,1,/m9bspu79nd?s=ld,e2462c1f38063a8b14ce102b9a6722e6,1,

//mSocket.host = "gamenetworkmanager.herokuapp.com"; //host // TODO: Falta el host verdadero.
