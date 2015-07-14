#ifndef UI_H
#define UI_H

#include <QWidget>
#include <QDesktopWidget>//
#include <QScrollArea>//
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include <QFont>

QT_BEGIN_NAMESPACE

class Ui_Topbar
{
public:
    QHBoxLayout *layout;

    void setupUi(QWidget *Topbar)
    {
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

        Topbar->setPalette(p);

        layout = new QHBoxLayout(Topbar);

        Topbar->setLayout(layout);
    }
};

class Ui_Finder
{
public:

    QLineEdit *input;
    QPushButton *btnCategory;
    QPushButton *btnPornstar;
    QHBoxLayout *layout;

    void setupUi(QWidget *Finder)
    {
        QFont f(":/font-global");
        f.setPointSize(24); //TODO: PASAR A PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        input = new QLineEdit(Finder);
        input->setFont(f);
        input->setAlignment(Qt::AlignCenter);

        btnCategory = new QPushButton("CATEGORY", Finder);
        btnCategory->setFont(f);
        btnCategory->setFlat(true);

        btnPornstar = new QPushButton("PORNSTAR", Finder);
        btnPornstar->setFont(f);
        btnPornstar->setFlat(true);

        layout = new QHBoxLayout(Finder);
        layout->addWidget(btnCategory);
        layout->addWidget(btnPornstar);
        layout->addWidget(input);

        Finder->setLayout(layout);
    }
};

namespace Ui {
    class TopBar: public Ui_Topbar {};
    class Finder: public Ui_Finder {};
}

QT_END_NAMESPACE

#endif
