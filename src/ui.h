#ifndef UI_H
#define UI_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDesktopWidget>//
#include <QScrollArea>//

#include <QFont>

QT_BEGIN_NAMESPACE

class Ui_PIG
{
public:
    QVBoxLayout *mainLayout;

    void setupUi(QWidget *PIG)
    {
        QFont f(":/font-global");
        f.setPointSize(24); //TODO: PASAR A PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush b(QColor(10, 10, 10, 255));
        QBrush b1(QColor(60, 60, 60, 255));
        QBrush b2(QColor(255, 255, 255, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b1);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Active, QPalette::Text, b2);
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b2);
        p.setBrush(QPalette::Active, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b1);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Text, b2);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        mainLayout = new QVBoxLayout(PIG);

        PIG->setFont(f);
        PIG->setPalette(p);
        PIG->setLayout(mainLayout);
    }
};

class Ui_Auth
{
public:
    QLineEdit *input;
    QPushButton *btnReset;
    QHBoxLayout *layout;

    void setupUi(QWidget *Auth)
    {
        QBrush b(QColor(30, 30, 30, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);

        input = new QLineEdit(Auth);

        btnReset = new QPushButton("RESET", Auth);
        btnReset->setFlat(true);

        layout = new QHBoxLayout(Auth);
        layout->addWidget(input);
        layout->addWidget(btnReset);

        Auth->setPalette(p);
        Auth->setLayout(layout);
    }
};

class Ui_Update
{
public:
    QLabel *label;
    QPushButton *btnAccept;
    QPushButton *btnCancel;
    QHBoxLayout *layout;

    void setupUi(QWidget *Update)
    {
        label = new QLabel(Update);
        label->setText("UPDATE AVAILABLE");

        btnAccept = new QPushButton("ACCEPT", Update);
        btnAccept->setFlat(true);

        btnCancel = new QPushButton("CANCEL", Update);
        btnCancel->setFlat(true);

        layout = new QHBoxLayout(Update);
        layout->addWidget(label);
        layout->addWidget(btnAccept);
        layout->addWidget(btnCancel);

        Update->setLayout(layout);
    }
};

class Ui_Topbar
{
public:
    QPushButton *btnSetup;
    QHBoxLayout *layout;

    void setupUi(QWidget *Topbar)
    {
        QFont f(":/font-global");
        f.setPointSize(24); //TODO: PASAR A PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush b(QColor(30, 30, 30, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);

        btnSetup = new QPushButton("/", Topbar);
        btnSetup->setFont(f);
        btnSetup->setPalette(p);
        btnSetup->setFlat(true);

        layout = new QHBoxLayout(Topbar);

        Topbar->setPalette(p);
        Topbar->setLayout(layout);
    }
};

class Ui_Finder
{
public:
    QPushButton *btnFilter;
    QLineEdit *input;
    QGroupBox *groupTop;
    QGroupBox *groupFilter;
    QHBoxLayout *layoutTop;
    QHBoxLayout *layoutFilter;
    QVBoxLayout *layout;

    QVector<QPushButton*> btn_categories_vector;

    void setupUi(QWidget *Finder)
    {
        QFont f(":/font-global");
        f.setPointSize(24); //TODO: PASAR A PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush b(QColor(30, 30, 30, 255));
        QBrush b1(QColor(60, 60, 60, 255));
        QBrush b2(QColor(255, 255, 255, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b1);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Active, QPalette::Text, b2);
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b2);
        p.setBrush(QPalette::Active, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b1);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Text, b2);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        groupTop = new QGroupBox(Finder);
        groupTop->setStyleSheet("QGroupBox { background-color: rgba(10, 10, 10, 255);"
                                "border: 0; margin: 0; }"
                                "QGroupBox::title { border: 0;margin: 0; }");
        groupTop->setFlat(true);

        layoutTop = new QHBoxLayout(groupTop);
        groupTop->setLayout(layoutTop);

        btnFilter = new QPushButton("↓", groupTop);
        btnFilter->setFont(f);
        btnFilter->setPalette(p);
        btnFilter->setFlat(true);

        input = new QLineEdit(groupTop);
        input->setFont(f);
        input->setPalette(p);
        input->setAlignment(Qt::AlignCenter);

        layoutTop->addWidget(btnFilter);
        layoutTop->addWidget(input);

        layout = new QVBoxLayout(Finder);
        layout->setSpacing(0);
        layout->setMargin(0);
        layout->addWidget(groupTop);

        Finder->setLayout(layout);
    }

    void setupFilterUi(QWidget *Finder)
    {
        groupFilter = new QGroupBox(Finder);
        groupFilter->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                   "border: 0; margin: 0; }"
                                   "QGroupBox::title { border: 0;margin: 0; }");
        groupFilter->setFlat(true);

        layoutFilter = new QHBoxLayout(groupFilter);
        layoutFilter->setContentsMargins(0, 0, 0, 0);
        layoutFilter->setSpacing(0);
        layoutFilter->setMargin(0);
        groupFilter->setLayout(layoutFilter);

        layout->addWidget(groupFilter);
    }
};

class Ui_View
{
public:
    QLabel *label_meta[5];
    QPushButton *btnClear;
    QPushButton *btnHideInfo;
    QWidget *player;//
    QGroupBox *groupCovers;
    QGroupBox *groupInfo;
    QGroupBox *groupMeta;
    QGroupBox *groupMulti;
    QGridLayout *layoutCovers;
    QVBoxLayout *layoutInfo;
    QVBoxLayout *layoutMeta;
    QHBoxLayout *layoutMulti;
    QVBoxLayout *layout;

    QVector<QPushButton*> btn_covers_vector;

    void setupUi(QWidget *View)
    {
        QFont f(":/font-global");
        f.setPointSize(24); //TODO: PASAR A PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush b(QColor(30, 30, 30, 255));
        QBrush b1(QColor(60, 60, 60, 255));
        QBrush b2(QColor(255, 255, 255, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b1);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Active, QPalette::Text, b2);
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b2);
        p.setBrush(QPalette::Active, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b1);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Text, b2);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        groupCovers = new QGroupBox(View);
        groupCovers->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                   "border: 0; margin: 0; }"
                                   "QGroupBox::title { border: 0;margin: 0; }");
        groupCovers->setFlat(true);

        layoutCovers = new QGridLayout(View);
        groupCovers->setLayout(layoutCovers);

        btnClear = new QPushButton("x", View);//
        btnClear->setFont(f);//
        btnClear->setPalette(p);//
        btnClear->setFlat(true);//

        layout = new QVBoxLayout(View);
        layout->addWidget(btnClear);
        layout->addWidget(groupCovers);

        View->setLayout(layout);
    }

    void setupInfoUi(const int &index,  const QStringList **data, QWidget *View)
    {
        const QStringList **_data = data;

        QFont f(":/font-global");
        f.setPointSize(24); //TODO: PASAR A PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);

        QBrush b(QColor(30, 30, 30, 255));
        QBrush b1(QColor(60, 60, 60, 255));
        QBrush b2(QColor(255, 255, 255, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b1);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Active, QPalette::Text, b2);
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b2);
        p.setBrush(QPalette::Active, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b1);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Text, b2);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        groupInfo = new QGroupBox(View);
        groupInfo->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                 "border: 0; margin: 0; }"
                                 "QGroupBox::title { border: 0;margin: 0; }");
        groupInfo->setFlat(true);

        groupMeta = new QGroupBox(View);
        groupMeta->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                 "border: 0; margin: 0; }"
                                 "QGroupBox::title { border: 0;margin: 0; }");
        groupMeta->setFlat(true);

        groupMulti = new QGroupBox(View);
        groupMulti->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                  "border: 0; margin: 0; }"
                                  "QGroupBox::title { border: 0;margin: 0; }");
        groupMulti->setFlat(true);

        layoutInfo = new QVBoxLayout(groupInfo);
        layoutInfo->setSpacing(20);
        layoutInfo->setAlignment(Qt::AlignVCenter);
        groupInfo->setLayout(layoutInfo);

        layoutMeta = new QVBoxLayout(groupMeta);
        layoutMeta->setSpacing(20);
        layoutMeta->setSizeConstraint(QLayout::SetMaximumSize);
        groupMeta->setLayout(layoutMeta);

        layoutMulti = new QHBoxLayout(groupMulti);
        layoutMulti->setSpacing(0);
        layoutMulti->setAlignment(Qt::AlignLeft);
        groupMulti->setLayout(layoutMulti);

        btnHideInfo = new QPushButton("←", groupMulti);
        btnHideInfo->setFont(f);
        btnHideInfo->setPalette(p);
        btnHideInfo->setFlat(true);

        QStringList labels;
        labels << "TITLE " << "CASTING " << "CATEGORY " << "QUALITY " << "TIME ";
        for (int i = 0; i < 5; i++) {
            label_meta[i] = new QLabel(groupMeta);
            label_meta[i]->setStyleSheet("QLabel{ background-color: rgba(110, 10, 10, 255);"
                                         "border: 0; margin: 0; }");
            label_meta[i]->setFont(f);
            label_meta[i]->setPalette(p);
            label_meta[i]->setText((**_data)[((index * 15) + (i + 1))]);
            label_meta[i]->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
            layoutMeta->addWidget(label_meta[i]);
        }

        player = new QWidget(groupMulti);
        player->setStyleSheet("QWidget { background-color: rgba(30, 30, 30, 255); }");
        player->setPalette(p);
        player->setFixedSize(QSize(640, 480));

        layoutMulti->addWidget(btn_covers_vector.at(index));
        layoutMulti->addWidget(btn_covers_vector.at(index+1));
        layoutMulti->insertSpacing(2, 20);
        layoutMulti->addWidget(player);

        layoutInfo->addWidget(btnHideInfo);
        layoutInfo->addWidget(groupMeta);
        layoutInfo->addWidget(groupMulti);

        groupCovers->setDisabled(true);
        groupCovers->hide();

        layout->addWidget(groupInfo);
    }
};

namespace Ui {
    class PIG: public Ui_PIG {};
    class Auth: public Ui_Auth {};
    class Update: public Ui_Update {};
    class TopBar: public Ui_Topbar {};
    class Finder: public Ui_Finder {};
    class View: public Ui_View {};
}

QT_END_NAMESPACE

#endif
