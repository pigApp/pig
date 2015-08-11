#ifndef UI_H
#define UI_H

#include "player.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QPixmap>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QScrollArea>
#include <QScrollBar>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDesktopWidget>//
#include <QDesktopServices>
#include <QCompleter>

#include <QFont>

#include <QDebug>

QT_BEGIN_NAMESPACE

class Ui_PIG
{
public:
    QVBoxLayout *main_layout;

    void setupUi(QWidget *PIG)
    {
        QFont font(":/font-global");
        font.setPointSize(16); // TODO: PORCENTAJE
        font.setCapitalization(QFont::AllUppercase);
        font.setBold(true);

        QBrush brush_base(QColor(10, 10, 10, 255));
        QBrush brush_white(QColor(255, 255, 255, 255));
        QBrush brush_black(QColor(0, 0, 0, 255));

        QPalette palette;
        palette.setBrush(QPalette::Active, QPalette::Base, brush_base);
        palette.setBrush(QPalette::Active, QPalette::Window, brush_base);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush_white);
        palette.setBrush(QPalette::Active, QPalette::Text, brush_white);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush_black);
        palette.setBrush(QPalette::Active, QPalette::Button, brush_base);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush_white);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush_base);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush_base);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush_black);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush_black);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush_base);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush_base);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush_black);

        main_layout = new QVBoxLayout(PIG);

        PIG->setFont(font);
        PIG->setPalette(palette);
        PIG->setLayout(main_layout);
    }
};

class Ui_Authotization
{
public:
    QPalette palette;
    QPalette palette_error;
    QLabel *label;
    QLineEdit *input;
    QPushButton *button_reset;
    QHBoxLayout *layout_input;
    QVBoxLayout *layout;

    void setupUi(bool set, bool setted, QWidget *Authorization)
    {
        QBrush brush_alternate_base(QColor(15, 15, 15, 255));
        QBrush brush_white(QColor(255, 255, 255, 255));
        QBrush brush_black(QColor(0, 0, 0, 255));
        QBrush brush_dark(QColor(40, 40, 40, 255));
        QBrush brush_red(QColor(115, 10, 10, 255));

        palette.setBrush(QPalette::Active, QPalette::Base, brush_alternate_base);
        palette.setBrush(QPalette::Active, QPalette::Window, brush_alternate_base);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush_white);
        palette.setBrush(QPalette::Active, QPalette::Button, brush_white);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush_black);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush_dark);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush_dark);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush_black);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush_dark);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush_black);

        palette_error.setBrush(QPalette::Active, QPalette::Base, brush_red);
        palette_error.setBrush(QPalette::Active, QPalette::Window, brush_red);
        palette_error.setBrush(QPalette::Active, QPalette::Highlight, brush_red);

        QString str;
        if (set)
            str = "SET PASSWORD";
        else
            str = "ENTER PASSWORD";

        label = new QLabel(str, Authorization);
        label->setPalette(palette);
        if (!set) label->setAlignment(Qt::AlignHCenter);

        input = new QLineEdit(Authorization);
        input->setPalette(palette);
        if (set && setted) input->setText("SETTED");
        input->setDisabled(set && setted);
        if (set && setted)
            input->setEchoMode(QLineEdit::Normal);
        else
            input->setEchoMode(QLineEdit::Password);
        input->setAlignment(Qt::AlignCenter);

        button_reset = new QPushButton("RESET", Authorization);
        button_reset->setPalette(palette);
        button_reset->setFlat(true);
        button_reset->setDisabled(!setted);
        button_reset->setAutoFillBackground(true);
        button_reset->setHidden(!set);

        layout_input = new QHBoxLayout;
        layout_input->setSpacing(20); // TODO: PORCENTAJE
        if (set) layout_input->addWidget(label);
        layout_input->addWidget(input);
        layout_input->addWidget(button_reset);

        layout = new QVBoxLayout(Authorization);
        if (!set) layout->addWidget(label);
        layout->addLayout(layout_input);
        layout->setAlignment(Qt::AlignCenter);

        Authorization->setLayout(layout);
    }
};

class Ui_Update
{
public:
    QPalette palette;
    QPalette palette_error;
    QLabel *label;
    QPushButton *button_a;
    QPushButton *button_b;
    QHBoxLayout *layout;

    void setupUi(QWidget *Update)
    {
        QBrush brush_white(QColor(255, 255, 255, 255));
        QBrush brush_black(QColor(0, 0, 0, 255));
        QBrush brush_red(QColor(115, 10, 10, 255));

        palette.setBrush(QPalette::Active, QPalette::Button, brush_white);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush_black);

        palette_error.setBrush(QPalette::Active, QPalette::Button, brush_red);
        palette_error.setBrush(QPalette::Active, QPalette::ButtonText, brush_white);

        label = new QLabel("UPDATE AVAILABLE", Update);

        button_a = new QPushButton(" ACCEPT ", Update);
        button_a->setPalette(palette);
        button_a->setFlat(true);
        button_a->setDefault(true);
        button_a->setAutoFillBackground(true);

        button_b = new QPushButton(" CANCEL ", Update);
        button_b->setPalette(palette);
        button_b->setFlat(true);
        button_b->setAutoFillBackground(true);

        layout = new QHBoxLayout(Update);
        layout->setSpacing(22); // TODO: PORCENTAJE
        layout->setAlignment(Qt::AlignHCenter);
        layout->addWidget(label);
        layout->addWidget(button_a);
        layout->addWidget(button_b);

        Update->setLayout(layout);
    }
};

class Ui_Topbar
{
public:
    QPushButton *button_setup;
    QGridLayout *layout;

    void setupUi(QWidget *Topbar)
    {
        QFont font(":/font-global");
        font.setPointSize(24); // TODO: PORCENTAJE
        font.setCapitalization(QFont::AllUppercase);
        font.setBold(true);

        button_setup = new QPushButton("≡", Topbar);
        button_setup->setFont(font);
        button_setup->setFlat(true);
        button_setup->setDisabled(true);

        layout = new QGridLayout(Topbar);
        layout->setMargin(0);
        layout->setAlignment(Qt::AlignTop);
        layout->addWidget(button_setup, 0, 1);

        Topbar->setLayout(layout);
    }
};

class Ui_Finder
{
public:
    QGridLayout *_layout_topbar;

    QPalette palette;
    QLineEdit *input;
    QPushButton *button_filters;
    QRadioButton *radio_all_quality;
    QRadioButton *radio_720p;
    QRadioButton *radio_1080p;
    QRadioButton *radio_fullMovie;
    QHBoxLayout *layout_radios;
    QHBoxLayout *layout_filters;
    QGridLayout *layout;

    QVector<QPushButton*> buttons_categories_vector;

    void setupUi(QStringList *movies, QGridLayout *layout_topbar, QWidget *Finder)
    {
        _layout_topbar = layout_topbar;

        QFont font(":/font-global");
        font.setPointSize(24); // TODO: PORCENTAJE
        font.setCapitalization(QFont::AllUppercase);
        font.setBold(true);

        QFont font_completer(":/font-global");
        font_completer.setPointSize(16); // TODO: PORCENTAJE
        font_completer.setCapitalization(QFont::AllUppercase);
        font_completer.setBold(true);

        QBrush brush_base(QColor(10, 10, 10, 255));
        QBrush brush_alternate_base(QColor(15, 15, 15, 255));
        QBrush brush_white(QColor(255, 255, 255, 255));
        QBrush brush_black(QColor(0, 0, 0, 255));
        QBrush brush_dark(QColor(40, 40, 40, 255));

        palette.setBrush(QPalette::Active, QPalette::Base, brush_alternate_base);
        palette.setBrush(QPalette::Active, QPalette::Window, brush_alternate_base);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush_white);
        palette.setBrush(QPalette::Active, QPalette::Text, brush_white);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush_black);
        palette.setBrush(QPalette::Active, QPalette::Button, brush_base);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush_white);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush_alternate_base);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush_alternate_base);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush_black);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush_black);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush_alternate_base);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush_alternate_base);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush_black);

        QPalette palette_completer;
        palette_completer.setBrush(QPalette::Active, QPalette::Base, brush_base);
        palette_completer.setBrush(QPalette::Active, QPalette::Window, brush_base);
        palette_completer.setBrush(QPalette::Active, QPalette::Text, brush_white);
        palette_completer.setBrush(QPalette::Active, QPalette::Highlight, brush_dark);

        QCompleter *completer = new QCompleter(*movies, Finder);
        completer->popup()->setFont(font_completer);
        completer->popup()->setPalette(palette_completer);
        completer->setMaxVisibleItems(10);
        completer->setFilterMode(Qt::MatchContains);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setCompletionMode(QCompleter::PopupCompletion);

        input = new QLineEdit(Finder);
        input->setFont(font);
        input->setPalette(palette);
        input->setMaxLength(45);
        input->setCompleter(completer);
        input->setAlignment(Qt::AlignCenter);

        button_filters = new QPushButton("≡", Finder);
        button_filters->setFont(font);
        button_filters->setPalette(palette);
        button_filters->setFlat(true);
        button_filters->setParent(input);

        layout = new QGridLayout;
        layout->setContentsMargins(82, 0, 2, 0);// TODO: PORCENTAJE
        layout->setSpacing(0); // TODO: PORCENTAJE
        layout->addWidget(input, 0, 0);
        layout->addWidget(button_filters, 0, 0, 0, 0, Qt::AlignLeft);

        _layout_topbar->addLayout(layout, 0, 0);

        input->setFocus();
    }

    void setupFilterUi(QWidget *Finder)
    {
        QFont font(":/font-global");
        font.setPointSize(12); // TODO: PORCENTAJE
        font.setCapitalization(QFont::AllUppercase);
        font.setBold(true);

        QBrush brush_white(QColor(255, 255, 255, 255));
        QBrush brush_dark(QColor(40, 40, 40, 255));

        palette.setBrush(QPalette::Active, QPalette::Base, brush_dark);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush_white);

        radio_all_quality = new QRadioButton("ALL", Finder);
        radio_all_quality->setFont(font);
        radio_all_quality->setPalette(palette);
        radio_all_quality->setChecked(true);

        radio_720p = new QRadioButton("720p", Finder);
        radio_720p->setFont(font);
        radio_720p->setPalette(palette);

        radio_1080p = new QRadioButton("1080p", Finder);
        radio_1080p->setFont(font);
        radio_1080p->setPalette(palette);

        radio_fullMovie = new QRadioButton("FULLMOVIE", Finder);
        radio_fullMovie->setFont(font);
        radio_fullMovie->setPalette(palette);
        radio_fullMovie->setAutoExclusive(false);

        layout_radios = new QHBoxLayout;;
        layout_radios->addWidget(radio_all_quality);
        layout_radios->addWidget(radio_720p);
        layout_radios->addWidget(radio_1080p);
        layout_radios->addSpacing(25); // TODO: PORCENTAJE
        layout_radios->addWidget(radio_fullMovie);

        layout_filters = new QHBoxLayout;
        layout_filters->setContentsMargins(115, 0, 10, 0); // TODO: PORCENTAJE
        layout_filters->addLayout(layout_radios);

        _layout_topbar->addLayout(layout_filters, 1, 0);
    }
};

class Ui_Setup
{
public:
    QPalette palette_torrent;
    QPalette palette_torrent_edit;
    QLabel *label_data_binary;
    QLabel *label_data_database;
    QLabel *label_data_license;
    QLabel *label_torrent_ports;
    QLabel *label_contribute_torrents;
    QLabel *label_contribute_code;
    QLabel *label_contribute_bugs;
    QLabel *label_contribute_bitcoins;
    QLabel *label_contribute_bitcoins_wallet;
    QLabel *label_contribute_help;
    QLineEdit *input_torrent_port_1;
    QLineEdit *input_torrent_port_2;
    QRadioButton *radio_folder_covers;
    QRadioButton *radio_folder_torrents;
    QRadioButton *radio_folder_movies;
    QPushButton *button_folder_covers_clean;
    QPushButton *button_folder_torrents_clean;
    QPushButton *button_folder_movies_clean;
    QPushButton *button_torrent_ports_reset;
    QPushButton *button_contribute_torrents;
    QPushButton *button_contribute_code;
    QPushButton *button_contribute_bugs;
    QPushButton *button_contribute_bitcoins;
    QPushButton *button_contribute_help;
    QGroupBox *group_data;
    QGroupBox *group_authorization;
    QGroupBox *group_folder;
    QGroupBox *group_torrent;
    QGroupBox *group_contribute;
    QHBoxLayout *layout_data;
    QHBoxLayout *layout_authorization;
    QHBoxLayout *layout_folder_covers;
    QHBoxLayout *layout_folder_torrents;
    QHBoxLayout *layout_folder_movies;
    QVBoxLayout *layout_folder;
    QHBoxLayout *layout_torrent;
    QHBoxLayout *layout_contribute_torrents;
    QHBoxLayout *layout_contribute_code;
    QHBoxLayout *layout_contribute_bugs;
    QHBoxLayout *layout_contribute_bitcoins;
    QHBoxLayout *layout_contribute_help;
    QVBoxLayout *layout_contribute;
    QGridLayout *layout;

    void setupUi(QStringList *data, bool **keep_covers, bool **keep_torrents,
                 bool **keep_movies, int **torrent_port_1, int **torrent_port_2,
                 QWidget *authorization, QWidget *Setup)
    {
        QBrush brush_base(QColor(10, 10, 10, 255));
        QBrush brush_alternate_base(QColor(15, 15, 15, 255));
        QBrush brush_white(QColor(255, 255, 255, 255));
        QBrush brush_black(QColor(0, 0, 0, 255));
        QBrush brush_dark(QColor(40, 40, 40, 255));

        QPalette palette;
        palette.setBrush(QPalette::Active, QPalette::Base, brush_base);
        palette.setBrush(QPalette::Active, QPalette::Window, brush_base);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush_white);
        palette.setBrush(QPalette::Active, QPalette::Text, brush_white);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush_black);
        palette.setBrush(QPalette::Active, QPalette::Button, brush_white);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush_black);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush_base);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush_base);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush_black);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush_black);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush_base);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush_base);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush_black);

        QPalette palette_dark;
        palette_dark.setBrush(QPalette::Active, QPalette::WindowText, brush_dark);

        palette_torrent.setBrush(QPalette::Active, QPalette::Base, brush_alternate_base);
        palette_torrent.setBrush(QPalette::Active, QPalette::Text, brush_black);
        palette_torrent.setBrush(QPalette::Active, QPalette::Button, brush_white);
        palette_torrent.setBrush(QPalette::Active, QPalette::ButtonText, brush_black);
        palette_torrent.setBrush(QPalette::Disabled, QPalette::Button, brush_dark);
        palette_torrent.setBrush(QPalette::Disabled, QPalette::ButtonText, brush_black);

        palette_torrent_edit.setBrush(QPalette::Active, QPalette::Base, brush_alternate_base);
        palette_torrent_edit.setBrush(QPalette::Active, QPalette::Text, brush_white);

        group_data = new QGroupBox("ABOUT", Setup);
        group_data->setPalette(palette_dark);
        group_data->setFlat(true);

        label_data_binary = new QLabel("PIG "+(*data)[0]+"."+(*data)[1], group_data);
        label_data_binary->setPalette(palette);

        label_data_database = new QLabel("DATABASE "+(*data)[2], group_data);
        label_data_database->setPalette(palette);

        label_data_license = new QLabel("GPL 2.0", group_data);
        label_data_license->setPalette(palette);

        layout_data = new QHBoxLayout(group_data);
        layout_data->setSpacing(20); // TODO: PORCENTAJE
        layout_data->addWidget(label_data_binary);
        layout_data->addWidget(label_data_database);
        layout_data->addWidget(label_data_license);
        layout_data->addStretch();

        group_authorization = new QGroupBox("AUTHORIZATION", Setup);
        group_authorization->setPalette(palette_dark);
        group_authorization->setFlat(true);

        layout_authorization = new QHBoxLayout(group_authorization);
        layout_authorization->setContentsMargins(0, 0, 0, 0);
        layout_authorization->addWidget(authorization);

        group_folder = new QGroupBox("FOLDERS", Setup);
        group_folder->setPalette(palette_dark);
        group_folder->setFlat(true);

        radio_folder_covers = new QRadioButton("KEEP LOCAL COPY OF COVERS", group_folder);
        radio_folder_covers->setPalette(palette);
        radio_folder_covers->setChecked(**keep_covers);
        radio_folder_covers->setAutoExclusive(false);

        button_folder_covers_clean = new QPushButton("CLEAN", group_folder);
        button_folder_covers_clean->setPalette(palette);
        button_folder_covers_clean->setFlat(true);
        button_folder_covers_clean->setAutoFillBackground(true);

        radio_folder_torrents = new QRadioButton("KEEP LOCAL COPY OF TORRENTS", group_folder);
        radio_folder_torrents->setPalette(palette);
        radio_folder_torrents->setChecked(**keep_torrents);
        radio_folder_torrents->setAutoExclusive(false);

        button_folder_torrents_clean = new QPushButton("CLEAN", group_folder);
        button_folder_torrents_clean->setPalette(palette);
        button_folder_torrents_clean->setFlat(true);
        button_folder_torrents_clean->setAutoFillBackground(true);

        radio_folder_movies = new QRadioButton("KEEP LOCAL COPY OF MOVIES", group_folder);
        radio_folder_movies->setPalette(palette);
        radio_folder_movies->setChecked(**keep_movies);
        radio_folder_movies->setAutoExclusive(false);

        button_folder_movies_clean = new QPushButton("CLEAN", group_folder);
        button_folder_movies_clean->setPalette(palette);
        button_folder_movies_clean->setFlat(true);
        button_folder_movies_clean->setAutoFillBackground(true);

        layout_folder_covers = new QHBoxLayout;
        layout_folder_covers->addWidget(radio_folder_covers);
        layout_folder_covers->insertStretch(1);
        layout_folder_covers->addWidget(button_folder_covers_clean);

        layout_folder_torrents = new QHBoxLayout;
        layout_folder_torrents->addWidget(radio_folder_torrents);
        layout_folder_torrents->insertStretch(1);
        layout_folder_torrents->addWidget(button_folder_torrents_clean);

        layout_folder_movies = new QHBoxLayout;
        layout_folder_movies->addWidget(radio_folder_movies);
        layout_folder_movies->insertStretch(1);
        layout_folder_movies->addWidget(button_folder_movies_clean);

        layout_folder = new QVBoxLayout(group_folder);
        layout_folder->addLayout(layout_folder_covers);
        layout_folder->addLayout(layout_folder_torrents);
        layout_folder->addLayout(layout_folder_movies);

        group_torrent = new QGroupBox("TORRENT", Setup);
        group_torrent->setPalette(palette_dark);
        group_torrent->setFlat(true);

        label_torrent_ports = new QLabel("PORTS", group_torrent);
        label_torrent_ports->setPalette(palette);

        QValidator *validPort = new QIntValidator(0, 65000, Setup);

        input_torrent_port_1 = new QLineEdit(QString::number(**torrent_port_1), group_torrent);
        input_torrent_port_1->setPalette(palette_torrent);
        input_torrent_port_1->setValidator(validPort);
        input_torrent_port_1->setAlignment(Qt::AlignCenter);

        input_torrent_port_2 = new QLineEdit(QString::number(**torrent_port_2), group_torrent);
        input_torrent_port_2->setPalette(palette_torrent);
        input_torrent_port_2->setValidator(validPort);
        input_torrent_port_2->setAlignment(Qt::AlignCenter);

        button_torrent_ports_reset = new QPushButton("RESET", group_torrent);
        button_torrent_ports_reset->setPalette(palette_torrent);
        if ((**torrent_port_1 == 6900) && (**torrent_port_2 == 6999))
            button_torrent_ports_reset->setDisabled(true);
        button_torrent_ports_reset->setFlat(true);
        button_torrent_ports_reset->setAutoFillBackground(true);

        layout_torrent = new QHBoxLayout(group_torrent);
        layout_torrent->setSpacing(20); // TODO: PORCENTAJE
        layout_torrent->addWidget(label_torrent_ports);
        layout_torrent->addWidget(input_torrent_port_1);
        layout_torrent->addWidget(input_torrent_port_2);
        layout_torrent->addWidget(button_torrent_ports_reset);

        group_contribute = new QGroupBox("CONTRIBUTE", Setup);
        group_contribute->setPalette(palette_dark);
        group_contribute->setFlat(true);

        label_contribute_torrents = new QLabel("TORRENTS", group_contribute);
        label_contribute_torrents->setPalette(palette);

        button_contribute_torrents = new QPushButton("→", group_contribute);
        button_contribute_torrents->setPalette(palette);
        button_contribute_torrents->setFlat(true);
        button_contribute_torrents->setAutoFillBackground(true);

        layout_contribute_torrents = new QHBoxLayout;
        layout_contribute_torrents->addWidget(label_contribute_torrents);
        layout_contribute_torrents->insertStretch(1);
        layout_contribute_torrents->addWidget(button_contribute_torrents);

        label_contribute_code = new QLabel("CODE", group_contribute);
        label_contribute_code->setPalette(palette);

        button_contribute_code = new QPushButton("→", group_contribute);
        button_contribute_code->setPalette(palette);
        button_contribute_code->setFlat(true);
        button_contribute_code->setAutoFillBackground(true);

        layout_contribute_code = new QHBoxLayout;
        layout_contribute_code->addWidget(label_contribute_code);
        layout_contribute_code->insertStretch(1);
        layout_contribute_code->addWidget(button_contribute_code);

        label_contribute_bugs = new QLabel("BUGS", group_contribute);
        label_contribute_bugs->setPalette(palette);

        button_contribute_bugs = new QPushButton("→", group_contribute);
        button_contribute_bugs->setPalette(palette);
        button_contribute_bugs->setFlat(true);
        button_contribute_bugs->setAutoFillBackground(true);

        layout_contribute_bugs = new QHBoxLayout;
        layout_contribute_bugs->addWidget(label_contribute_bugs);
        layout_contribute_bugs->insertStretch(1);
        layout_contribute_bugs->addWidget(button_contribute_bugs);

        label_contribute_bitcoins = new QLabel("BITCOINS", group_contribute);
        label_contribute_bitcoins->setPalette(palette);

        label_contribute_bitcoins_wallet = new QLabel(" ACDF723JKD8239OEWMBN7692BH23489BHJK", group_contribute);
        label_contribute_bitcoins_wallet->setPalette(palette_dark);
        label_contribute_bitcoins_wallet->setVisible(false);

        button_contribute_bitcoins = new QPushButton("+", group_contribute);
        button_contribute_bitcoins->setPalette(palette);
        button_contribute_bitcoins->setFlat(true);
        button_contribute_bitcoins->setAutoFillBackground(true);

        layout_contribute_bitcoins = new QHBoxLayout;
        layout_contribute_bitcoins->addWidget(label_contribute_bitcoins);
        layout_contribute_bitcoins->addWidget(label_contribute_bitcoins_wallet);
        layout_contribute_bitcoins->insertStretch(2);
        layout_contribute_bitcoins->addWidget(button_contribute_bitcoins);

        label_contribute_help = new QLabel("HELP", group_contribute);
        label_contribute_help->setPalette(palette);

        button_contribute_help = new QPushButton("→", group_contribute);
        button_contribute_help->setPalette(palette);
        button_contribute_help->setFlat(true);
        button_contribute_help->setAutoFillBackground(true);

        layout_contribute_help = new QHBoxLayout;
        layout_contribute_help->addWidget(label_contribute_help);
        layout_contribute_help->insertStretch(1);
        layout_contribute_help->addWidget(button_contribute_help);

        layout_contribute = new QVBoxLayout(group_contribute);
        layout_contribute->addLayout(layout_contribute_torrents);
        layout_contribute->addLayout(layout_contribute_code);
        layout_contribute->addLayout(layout_contribute_bugs);
        layout_contribute->addLayout(layout_contribute_bitcoins);
        layout_contribute->addLayout(layout_contribute_help);

        layout = new QGridLayout(Setup);
        layout->setSpacing(20); //TODO: PORCENTAJE
        layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        layout->addWidget(group_data, 0, 0);
        layout->addWidget(group_authorization, 1, 0);
        layout->addWidget(group_folder, 2, 0);
        layout->addWidget(group_torrent, 3, 0);
        layout->addWidget(group_contribute, 4, 0);

        Setup->setLayout(layout);
    }
};

class Ui_View
{
public:
    Player *player;

    QPalette palette;
    QLabel *labels_meta[5];
    QLabel *cover;
    QLabel *backCover;
    QPushButton *button_clear;
    QPushButton *button_hideInfo;
    QScrollArea *scrollArea_covers;
    QGroupBox *group_covers;
    QGroupBox *group_info;
    QGridLayout *layout_covers;
    QVBoxLayout *layout_meta;
    QHBoxLayout *layout_multi;
    QVBoxLayout *layout_info;
    QVBoxLayout *layout;

    QVector<QPushButton*> button_vector_covers;

    void setupUi(QWidget *View)
    {
        QBrush brush_base(QColor(10, 10, 10, 255));
        QBrush brush_white(QColor(255, 255, 255, 255));
        QBrush brush_black(QColor(0, 0, 0, 255));

        palette.setBrush(QPalette::Active, QPalette::Base, brush_base);
        palette.setBrush(QPalette::Active, QPalette::Window, brush_base);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush_white);
        palette.setBrush(QPalette::Active, QPalette::Text, brush_white);
        palette.setBrush(QPalette::Active, QPalette::Button, brush_base);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush_white);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush_base);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush_base);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush_black);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush_black);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush_base);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush_black);

        scrollArea_covers = new QScrollArea(View);
        scrollArea_covers->setFrameStyle(QFrame::NoFrame);
        scrollArea_covers->setWidgetResizable(true);
        scrollArea_covers->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_covers->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        group_covers = new QGroupBox(scrollArea_covers);
        group_covers->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                    "border: 0; margin: 0; }"
                                    "QGroupBox::title { border: 0; margin: 0; }");
        group_covers->setFlat(true);

        layout_covers = new QGridLayout(group_covers);
        layout_covers->setContentsMargins(70, 0, 0, 20); // TODO: PORCENTAJE
        layout_covers->setHorizontalSpacing(0);
        layout_covers->setVerticalSpacing(5); // TODO: PORCENTAJE
        layout_covers->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        group_covers->setLayout(layout_covers);

        scrollArea_covers->setWidget(group_covers);

        layout = new QVBoxLayout(View);
        layout->addWidget(scrollArea_covers);

        View->setLayout(layout);

        //        button_clear = new QPushButton("x", group_covers);
        //        button_clear->setFont(font);
        //        button_clear->setPalette(palette);
        //        button_clear->setFlat(true);
        //layout_covers->addWidget(button_clear);
    }

    void setupInfoUi(const int &index, const QString &path, const QStringList **data, QWidget *View)
    {
        const QStringList **_data = data;

        QFont font(":/font-global");
        font.setPointSize(24); //TODO: PORCENTAJE
        font.setCapitalization(QFont::AllUppercase);
        font.setBold(true);

        group_info = new QGroupBox(View);
        group_info->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                  "border: 0; margin: 0; }"
                                  "QGroupBox::title { border: 0;margin: 0; }");
        group_info->setFlat(true);

        layout_meta = new QVBoxLayout;
        layout_meta->setSpacing(10); // TODO: PORCENTAJE
        layout_meta->setSizeConstraint(QLayout::SetMaximumSize);

        for (int i = 0; i < 5; i++) {
            labels_meta[i] = new QLabel(group_info);
            labels_meta[i]->setStyleSheet("QLabel{ background-color: rgba(10, 10, 10, 255);"
                                          "border: 0; margin: 0; }");
            labels_meta[i]->setFont(font);
            labels_meta[i]->setPalette(palette);
            labels_meta[i]->setText(" "+(**_data)[((index * 17) + i)]);
            layout_meta->addWidget(labels_meta[i]);
        }

        QPixmap pixmap_cover(path);
        cover = new QLabel(group_info);
        cover->setPixmap(pixmap_cover.scaled(335, 480, Qt::KeepAspectRatio));//

        QPixmap pixmap_backCover(":/img-cover-back");
        backCover = new QLabel(group_info);
        backCover->setPixmap(pixmap_backCover);//

        QString preview = "http://"+((**_data)[((index * 17) + 11)])+((**_data)[((index * 17) + 12)]);
        player = new Player(&preview, group_info);
        player->setFixedSize(QSize(640, 480)); // TODO: PORCENTAJE

        layout_multi = new QHBoxLayout;
        layout_multi->setSpacing(0);
        layout_multi->setAlignment(Qt::AlignLeft);
        layout_multi->addWidget(cover);
        layout_multi->insertSpacing(1, 20); // TODO: PORCENTAJE
        layout_multi->addWidget(backCover);
        layout_multi->insertSpacing(3, 20); // TODO: PORCENTAJE
        layout_multi->addWidget(player);

        button_hideInfo = new QPushButton("", group_info);
        button_hideInfo->setFont(font);
        button_hideInfo->setPalette(palette);
        button_hideInfo->setFlat(true);

        layout_info = new QVBoxLayout(group_info);
        layout_info->setSpacing(110); // TODO: PORCENTAJE
        layout_info->setAlignment(Qt::AlignVCenter);
        layout_info->addWidget(button_hideInfo);
        layout_info->addLayout(layout_meta);
        layout_info->addLayout(layout_multi);

        group_info->setLayout(layout_info);

        scrollArea_covers->setDisabled(true);
        scrollArea_covers->hide();

        layout->addWidget(group_info);
    }
};

namespace Ui {
    class PIG: public Ui_PIG {};
    class Authorization: public Ui_Authotization {};
    class Update: public Ui_Update {};
    class TopBar: public Ui_Topbar {};
    class Finder: public Ui_Finder {};
    class Setup: public Ui_Setup {};
    class View: public Ui_View {};
}

QT_END_NAMESPACE

#endif
