#ifndef UI_H
#define UI_H

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
    QPushButton *btn_reset;
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

        btn_reset = new QPushButton("RESET", Auth);
        btn_reset->setFlat(true);

        layout = new QHBoxLayout(Auth);
        layout->addWidget(input);
        layout->addWidget(btn_reset);

        Auth->setPalette(p);
        Auth->setLayout(layout);
    }
};

class Ui_Update
{
public:
    QLabel *label;
    QPushButton *btn_accept;
    QPushButton *btn_cancel;
    QHBoxLayout *layout;

    void setupUi(QWidget *Update)
    {
        label = new QLabel(Update);
        label->setText("UPDATE AVAILABLE");

        btn_accept = new QPushButton("ACCEPT", Update);
        btn_accept->setFlat(true);

        btn_cancel = new QPushButton("CANCEL", Update);
        btn_cancel->setFlat(true);

        layout = new QHBoxLayout(Update);
        layout->addWidget(label);
        layout->addWidget(btn_accept);
        layout->addWidget(btn_cancel);

        Update->setLayout(layout);
    }
};

class Ui_Topbar
{
public:
    QHBoxLayout *layout;

    void setupUi(QWidget *Topbar)
    {
        QBrush b(QColor(30, 30, 30, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);

        layout = new QHBoxLayout(Topbar);

        Topbar->setPalette(p);
        Topbar->setLayout(layout);
    }
};

class Ui_Finder
{
public:
    QLineEdit *input;
    QPushButton *btn_category;
    QPushButton *btn_pornstar;
    QHBoxLayout *layout;

    void setupUi(QWidget *Finder)
    {
        input = new QLineEdit(Finder);
        input->setAlignment(Qt::AlignCenter);

        btn_category = new QPushButton("CATEGORY", Finder);
        btn_category->setFlat(true);
        //btn_category->hide();//

        btn_pornstar = new QPushButton("PORNSTAR", Finder);
        btn_pornstar->setFlat(true);
        btn_pornstar->hide();//

        layout = new QHBoxLayout(Finder);
        layout->addWidget(btn_category);
        layout->addWidget(input);
        //layout->addWidget(btn_pornstar);

        Finder->setLayout(layout);
    }
};

class Ui_View
{
public:
    QVector<QPushButton*> btn_cover_vector;
    QPushButton *btn_hide_data;
    QLabel *label[5];
    QGroupBox *group_cover;
    QGroupBox *group_data;
    QGridLayout *layout_cover;
    QVBoxLayout *layout_data;
    QVBoxLayout *layout_view;

    void setupUi(QWidget *View)
    {
        QFont f(":/font-global");//
        f.setPointSize(24); //TODO: PASAR A PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);//
        f.setBold(true);//

        group_cover = new QGroupBox(View);
        group_cover->setStyleSheet("QGroupBox{ border: 0; }");
        group_cover->setFlat(true);

        layout_cover = new QGridLayout(View);

        group_cover->setLayout(layout_cover);

        group_data = new QGroupBox(View);
        group_data->setStyleSheet("QGroupBox{ border: 0; }");
        group_data->setFlat(true);

        btn_hide_data = new QPushButton("CLOSE", View);
        btn_hide_data->setFlat(true);

        layout_data = new QVBoxLayout(View);
        layout_data->addWidget(btn_hide_data);
        for (int i = 0; i < 5; i++) {
            label[i] = new QLabel(View);
            label[i]->setStyleSheet("QLabel { color : white; }");//
            label[i]->setFont(f);//
            layout_data->addWidget(label[i]);
        }

        group_data->setLayout(layout_data);
        group_data->setDisabled(true);
        group_data->hide();

        layout_view = new QVBoxLayout(View);
        layout_view->addWidget(group_cover);

        View->setLayout(layout_view);
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
